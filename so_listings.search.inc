<?php

/**
 * Implementation of hook_so_listings_search()
 */
function so_listings_so_listings_search($field, &$query, &$data) {

    if(count(array_filter($field['values'])) == 0) {return;}

    $matching_mode = isset($field['configuration']['matching_mode']) ?
                     $data['matching_modes'][$field['configuration']['matching_mode']] :
                     $data['matching_modes']['contain'];

    $query_fragment_data = so_listings_prepare_query_fragment_data($field['field_name']. '@field');
    $query['join'] = array_merge($query['join'], $query_fragment_data->joins);

    switch($field['field_type']) {

        case 'text':
            switch($field['widget']) {
                case 'textfield':
                    $input_value = preg_replace('#[\s-&()_\+\!,\:;\.\"\']#', '_', trim($field['values'][0]));

                    $query['where'][] = $query_fragment_data->fields['value'] . " LIKE ?";
                    $query['args'][] = str_replace('$', $input_value, $matching_mode);

                    break;

                default:
                    $input_values = $field['values'];

                    if(!is_array($input_values[0])) {   // exemple : cas des checkboxes utilisant les valeurs existantes

                        if($field['configuration']['query_mode'] == 'OR') {

                            $arguments = addslashes(implode("#*#", $input_values));
                            $arguments = str_replace('#*#', "','", $arguments);
                            $query['where'][] = $query_fragment_data->fields['value'] . " IN('" . $arguments . "')";

                        } else {

                            $arguments = addslashes(implode("#*#", $input_values));
                            $arguments = str_replace('#*#', "','", $arguments);
                            $num_arguments = count($input_values);

                            $query['where'][] = "n.nid IN (" .
                                        "SELECT entity_id" .
                                        " FROM " . $query_fragment_data->table .
                                        " WHERE " . $query_fragment_data->fields['value'] .
                                        " IN('" . $arguments . "')" .
                                        " GROUP BY entity_id" .
                                        " HAVING COUNT(" . $query_fragment_data->fields['value'] . ")=" . $num_arguments .
                                        ")";
                        }

                    } else { // exemple : cas des checkboxes utilisant des options custom
                        $where = array();

                        foreach((array)$input_values as $values_set) {

                            if($field['configuration']['values_and_mode'] == 0) {

                                $arguments = addslashes(implode("#*#", $values_set));
                                $arguments = str_replace('#*#', "','", $arguments);
                                $where[] = $query_fragment_data->fields['value'] . " IN('" . $arguments . "')";

                            } else {

                                $arguments = addslashes(implode("#*#", $values_set));
                                $arguments = str_replace('#*#', "','", $arguments);
                                $num_arguments = count($values_set);

                                $where[] = "n.nid IN (" .
                                            "SELECT entity_id" .
                                            " FROM " . $query_fragment_data->table .
                                            " WHERE " . $query_fragment_data->fields['value'] .
                                            " IN('" . $arguments . "')" .
                                            " GROUP BY entity_id" .
                                            " HAVING COUNT(" . $query_fragment_data->fields['value'] . ")=" . $num_arguments .
                                            ")";
                            }
                        }

                        $query['where'][] = "(" . implode(" " . $field['configuration']['query_mode'] . " ", $where) . ")";
                    }

                    break;

            }
            break;

        case 'dates':

            $where = array();

            if($field['values']['from'] == null) {$field['values']['from'] = '1970-01-01';}
            if($field['values']['to'] == null) {$field['values']['to'] = '3000-01-01';}

            if($field['values']['from'] > $field['values']['to']) {
                $temp = $field['values']['from'];
                $field['values']['from'] = $field['values']['to'];
                $field['values']['to'] = $temp;
            }

            if(!empty($field['configuration']['results_constraints']['surrounding'])) {
                $where[] = "(DATE(" . $query_fragment_data->fields['value'] . ")<=? AND DATE(" . $query_fragment_data->fields['value2'] . ")>=?)";
                $query['args'] = array_merge($query['args'], array_values((array)$field['values']));
            }

            if(!empty($field['configuration']['results_constraints']['start_in'])) {
                $where[] = "(DATE(" . $query_fragment_data->fields['value'] . ")>=? AND DATE(" . $query_fragment_data->fields['value'] . ")<=?)";
                $query['args'] = array_merge($query['args'], array_values((array)$field['values']));
            }

            if(!empty($field['configuration']['results_constraints']['end_in'])) {
                $where[] = "(DATE(" . $query_fragment_data->fields['value2'] . ")>=? AND DATE(" . $query_fragment_data->fields['value2'] . ")<=?)";
                $query['args'] = array_merge($query['args'], array_values((array)$field['values']));
            }

            if(!empty($field['configuration']['results_constraints']['surrounded']) && (empty($field['configuration']['results_constraints']['start_in']) && empty($field['configuration']['results_constraints']['end_in']))) {
                $where[] = "(DATE(" . $query_fragment_data->fields['value'] . ")>=? AND DATE(" . $query_fragment_data->fields['value2'] . ")<=?)";
                $query['args'] = array_merge($query['args'], array_values((array)$field['values']));
            }

            if(!empty($where)) {
                $query['where'][] = "(" . implode(' OR ', $where) . ")";
            }

            break;

        case 'numerical':

            switch($field['widget']) {
                case 'textfield':

                    $sql_operator = array(
                        'equal' => '=',
                        'different' => '!=',
                        'lower' => '<',
                        'lower_equal' => '<=',
                        'greater' => '>',
                        'greater_equal' => '>=',
                    );

                    if(empty($field['configuration']['operator'])) {$field['configuration']['operator'] = 'equal';}

                    $where = $query_fragment_data->fields['value'] . $sql_operator[$field['configuration']['operator']] . "?";
                    $query['args'][] = $field['values'][0];

                    if(!empty($where)) {
                        $query['where'][] = $where;
                    }

                    break;

                case 'slider':

                    switch($field['values']['type']) {

                        case 'number':
                            $query['where'][] = $query_fragment_data->fields['value'] . "=?";
                            $query['args'][] = $field['values']['value_1'];
                            break;

                        case 'number_max':
                            $query['where'][] = "(" . $query_fragment_data->fields['value'] . ">=? AND " . $query_fragment_data->fields['value'] . "<=?)";
                            $query['args'][] = $field['values']['min'];
                            $query['args'][] = $field['values']['value_1'];
                            break;

                        case 'absolute_number_max':
                            $query['where'][] = $query_fragment_data->fields['value'] . "<=?";
                            $query['args'][] = $field['values']['value_1'];
                            break;

                        case 'number_min':
                            $query['where'][] = "(" . $query_fragment_data->fields['value'] . ">=? AND " . $query_fragment_data->fields['value'] . "<=?)";
                            $query['args'][] = $field['values']['value_1'];
                            $query['args'][] = $field['values']['max'];
                            break;

                        case 'absolute_number_min':
                            $query['where'][] = $query_fragment_data->fields['value'] . ">=?";
                            $query['args'][] = $field['values']['value_1'];
                            break;

                        case 'range':
                            $query['where'][] = "(" . $query_fragment_data->fields['value'] . ">=? AND " . $query_fragment_data->fields['value'] . "<=?)";
                            $query['args'][] = $field['values']['value_1'];
                            $query['args'][] = $field['values']['value_2'];
                            break;
                    }

                    break;

                case 'values_ranges':

                    switch($field['values']['type']) {

                        case 'number_max_included':
                            $query['where'][] = $query_fragment_data->fields['value'] . "<=?";
                            $query['args'][] = $field['values']['max'];
                            break;

                        case 'number_max':
                            $query['where'][] = $query_fragment_data->fields['value'] . "<?";
                            $query['args'][] = $field['values']['max'];
                            break;

                        case 'number_min_included':
                            $query['where'][] = $query_fragment_data->fields['value'] . ">=?";
                            $query['args'][] = $field['values']['min'];
                            break;

                        case 'number_min':
                            $query['where'][] = $query_fragment_data->fields['value'] . ">?";
                            $query['args'][] = $field['values']['min'];
                            break;

                        case 'range_upper_included':
                            $query['where'][] = "(" . $query_fragment_data->fields['value'] . ">? AND " . $query_fragment_data->fields['value'] . "<=?)";
                            $query['args'][] = $field['values']['min'];
                            $query['args'][] = $field['values']['max'];
                            break;

                        case 'range_lower_included':
                            $query['where'][] = "(" . $query_fragment_data->fields['value'] . ">=? AND " . $query_fragment_data->fields['value'] . "<?)";
                            $query['args'][] = $field['values']['min'];
                            $query['args'][] = $field['values']['max'];
                            break;

                        case 'range_both_included':
                            $query['where'][] = "(" . $query_fragment_data->fields['value'] . ">=? AND " . $query_fragment_data->fields['value'] . "<=?)";
                            $query['args'][] = $field['values']['min'];
                            $query['args'][] = $field['values']['max'];
                            break;

                        case 'range_none_included':
                            $query['where'][] = "(" . $query_fragment_data->fields['value'] . ">? AND " . $query_fragment_data->fields['value'] . "<?)";
                            $query['args'][] = $field['values']['min'];
                            $query['args'][] = $field['values']['max'];
                            break;
                    }

                    break;
            }

            break;
    }
}

/**
 * Helper : maps specific Field API data types on one or more generic so_forms custom ones
 * Without parameter, returns all mappings.
 *
 * @param string $type
 * @return array of array or array
 */
function so_listings_search_map_fields_types($type = null) {
    $types = array(
        'text' => array(
            'type' => 'text',
            'columns' => array('value'),
            'js' => 'so_listings_search_field_js',
        ),
        'text_long' => array(
            'type' => 'text',
            'columns' => array('value'),
        ),
        'list_text' => array(
            'type' => 'text',
            'columns' => array('value'),
        ),
        'list_boolean' => array(
            'type' => 'text',
            'columns' => array('value'),
        ),
        'number_float' => array(
            'type' => 'numerical',
            'columns' => array('value'),
        ),
        'number_decimal' => array(
            'type' => 'numerical',
            'columns' => array('value'),
        ),
        'number_integer' => array(
            'type' => 'numerical',
            'columns' => array('value'),
        ),
        'datetime' => array(
            'type' => 'dates',
            'columns' => array(
                'value',
                'value2',
            ),
        ),
    );

    return $type != null ? $types[$type] : $types;
}

/**
 * Implementation of hook_so_forms()
 */
function so_listings_so_forms($module, $id) {

    if($module != 'so_listings') {return;} // on n'est pas sensé fournir de champs aux autres modules (pour le moment)

    $parsed_id = explode('@', $id);
    $lid = $parsed_id[0];

    if($parsed_id[1] == 'search') {

        $definition = so_listings_get_listings_definitions($lid);
        $sources = $definition->params['sources'];

        $sources_pool = so_listings_load_sources_pool(false);
        $allowed_fields_types = array_keys(so_listings_search_map_fields_types());

        $type_fragment = so_listings_prepare_query_fragment_data('type@node');

        $fields_pool = array();
        $shared_fields = array();
        $num_nodes_types = 0;

        foreach($sources as $source) {

            $options = array(
                'filters' => false,
                'sorting' => false,
                'debug' => false,
            );

            $query = so_listings_init_query();
            $source_infos = explode(':', $source); // 0:module, 1:group, 2:id
            $callback = $sources_pool[$source_infos[0]][$source_infos[1]]['callback'];
            $args = $sources_pool[$source_infos[0]][$source_infos[1]]['sources'][$source_infos[2]]['args'];
            $callback($query, $args, array());

            $query['fields'] = $type_fragment->fields;
            $query['group_by'] = $type_fragment->fields;

            $results = so_listings_compile($lid, $options, $query);

            foreach($results['nodes'] as $node) {

                $num_nodes_types++;

                $fields_instances = field_info_instances('node', $node['type']);

                foreach($fields_instances as $instance) {

                    $field_definition = field_info_field($instance['field_name']);

                    if(in_array($field_definition['type'], $allowed_fields_types)) {

                        $shared_fields[$instance['field_name']] = (int)$shared_fields[$instance['field_name']] + 1;

                        $mapping = so_listings_search_map_fields_types($field_definition['type']);

                        $fields_pool[$instance['field_name']] = array(
                            'label' => $instance['label'],
                            'language' => $definition->filters['filters'][$source]['language'],
                            'type' => $mapping['type'],
                            'context' => $lid,
                            'data' => array('columns' => $mapping['columns']),
                            'include_paths' => array(
                                array(
                                    'module' => 'so_listings',
                                    'script' => 'so_listings.search',
                                ),
                            ),
                            'callbacks' => array(
                                'values' => 'so_listings_search_field_values',
                                'configuration' => 'so_listings_search_field_configuration',
                                'render' => 'so_listings_search_field_render',
                                'js' => $mapping['js'],
                            )
                        );
                    }
                }
            }
        }

        foreach($shared_fields as $shared_field => $occurences) {
            if($num_nodes_types > 1 && $occurences == $num_nodes_types) {
                $fields_pool[$shared_field]['label'] .= " (" . t("shared") . ")";
            }
        }

        $transliterate = module_exists('transliteration') ? 'transliteration_get' : 'return';

        $labels = array();
        foreach ($fields_pool as $key => $field) {
            $labels[$key] = $transliterate($field['label']);
        }
        array_multisort($labels, SORT_ASC, $fields_pool);

        return array(
            'fields_definitions' => $fields_pool,
        );
    }
}

function so_listings_search_form_interface($lid, $form_display = 'default') {

    $definition = so_listings_get_listings_definitions($lid);
    drupal_set_title(t("Edit listing '@listing'", array('@listing' => $definition->name)));

    $form_params = so_forms_infos('so_listings', $lid . '@search');

    if(empty($form_params['secured_sfid'])) {

        $settings = variable_get('sol_settings', array());

        $form_name = db_query("SELECT name FROM {so_listings} WHERE id=?", array($lid))->fetchColumn();

        $default_configuration = array(
            'name' => $form_name,
            'title' => $form_name,
            'memorize' => true,
            'url_key' => false,
            'advanced' => true,
            'api_only' => false,
            'no_groups' => false,
            'classes' => 'so_listings_search_form',
            'buttons' => array(
                'submit' => "Search",
                'reset' => "Reset",
            ),
            'perms' => array(
                'configure' => 'administer so_listings',
                'display' => 'access content',
            ),
            'include_paths' => array(
                array(
                    'module' => 'so_listings',
                    'script' => 'so_listings.search',
                ),
            ),
            'callbacks' => array(
                'process' => null,
                'populate' => 'so_listings_search_populate',
                'redirect' => null
            ),
        );

        $configuration = (array_key_exists('search_forms', $settings) ? $settings['search_forms'] : array()) + $default_configuration;

        so_forms_create('so_listings', $lid . '@search', $configuration);
    }

    return(so_forms_configure('so_listings', $lid . '@search', $form_display));
}

/**
 * Dispatch search fields to modules for processing
 */
function so_listings_search_filter($lid, &$query, &$compiler_options, $listing_definition) {

    // les premiers champs du form de recherchent doivent se trouver en dernier, ainsi, les modules correspondants vont pouvoir
    // supplanter les éléments de requête des champs moins importants.
    $user_inputs = (array)so_forms_get_user_inputs('so_listings', $compiler_options['so_listings']['definition']->id . '@search');
    $user_inputs = array_reverse($user_inputs);

    $compiled_forced_inputs = so_listings_search_process_forced_values($lid, false, $compiler_options['search_values']);
    $user_inputs = array_merge($user_inputs, $compiled_forced_inputs);

    $is_there_values = false;

    foreach($user_inputs as $field) {

        // on sauvegarde les clauses WHERE existantes et on initialise le container
        $backup_where = array_splice($query['where'], 0);
        $search_where = array();

        // on exécute d'abord la requête sur les éventuels champs secondaires, afin que le champ principal conserve sa priorité de supplantation)
        foreach((array)$field['configuration']['secondary_fields'] as $secondary_field_infos) {
            $secondary_field_infos = explode(':', $secondary_field_infos);

            $secondary_field = $field;
            unset($secondary_field['configuration']['secondary_fields']);

            $secondary_field['secondary_field'] = true;
            $secondary_field['module_name'] = $secondary_field_infos[0];
            $secondary_field['field_name'] = $secondary_field_infos[1];
            $secondary_field['field_context'] = $secondary_field_infos[2];

            $function = $secondary_field['module_name'] . '_so_listings_search';

            if(function_exists($function)) {
                $function(
                    $secondary_field,
                    $query,
                    $compiler_options[$secondary_field['module_name']],
                    array_key_exists($secondary_field['module_name'], $compiler_options['so_listings']['sources_by_module']) ?
                        $compiler_options['so_listings']['sources_by_module'][$secondary_field['module_name']] : array()
                );
            }

            // on assemble les clauses WHERE générées
            $search_where[] = implode(' AND ', array_splice($query['where'], 0));
        }

        $function = $field['module_name'] . '_so_listings_search';

        if(function_exists($function)) {
            $function(
                $field,
                $query,
                $compiler_options[$field['module_name']],
                array_key_exists($field['module_name'], $compiler_options['so_listings']['sources_by_module']) ?
                    $compiler_options['so_listings']['sources_by_module'][$field['module_name']] : array()
            );

            // on assemble le paquet de clauses WHERE du champ principal
            $search_where[] = implode(' AND ', array_splice($query['where'], 0));
        }

        // on restaure les clauses WHERE orginales
        $query['where'] = $backup_where;

        $search_where = array_filter($search_where);

        $is_there_values = $is_there_values || !empty($search_where);

        if(!empty($search_where)) {

            // on assemble les clauses WHERE principales et secondaires du champ et on les ajoute aux clauses originales
            $query['where'][] = "(" . implode(' OR ', $search_where) . ")";
        }
    }

    if($is_there_values == true) {
        $query['#context'][] = 'so_listings_search';
    }
}

/*
 * So_forms callback.
 * TODO : documentation
 */
function so_listings_search_populate($id) {

    $infos = explode('@', $id);

    $values = so_listings_search_process_forced_values($infos[0], true);

    return $values != null ? $values : array();
}

/*
 * TODO : documentation
 */
function so_listings_search_process_forced_values($lid, $return_raw = false, $compiler_options_values = array()) {

    $raw_values = &drupal_static(__FUNCTION__);

    if(!isset($raw_values)) {
        $raw_values = array();
    }

    if($return_raw == true && array_key_exists($lid, $raw_values)) {
        return $raw_values[$lid];
    }

    global $language;

    $raw_values[$lid] = array();

    $from_options_compiled = so_forms_compile_user_raw_inputs($compiler_options_values, $language->language, $raw_values[$lid]);

    $url_parameters = drupal_get_query_parameters();
    $url_parameters_values = array();

    foreach($url_parameters as $key => $value) {

        $sub_keys = explode('__', $key);
        $sub_key_pointer = &$url_parameters_values;

        $sub_key = "";

        while(!empty($sub_keys)) {
            $sub_key = array_shift($sub_keys);
            $sub_key_pointer[$sub_key] = (array)$sub_key_pointer[$sub_key];
            $sub_key_pointer = &$sub_key_pointer[$sub_key];
        }

        $sub_key_pointer += explode(',', check_plain(urldecode($value)));
    }

    $from_url_compiled = so_forms_compile_user_raw_inputs($url_parameters_values, $language->language, $raw_values[$lid]);

    return array_merge($from_options_compiled, $from_url_compiled);
}

/**
 * so_forms callback : search fields values.
 * Also used by JS callback (autocompletion).
 *
 * @param array $field_infos
 * @param string $langcode
 * @param array $configuration : stored results of module custom configuration form submission
 * @param string $autocomplete : the term for autocompletion
 *
 * @return array
 */
function so_listings_search_field_values($field_infos, $langcode, $configuration, $autocomplete = null) {

    $values = array();

    $field_definition = field_info_field($field_infos['field_name']);

    if($field_definition['type'] == 'list_text' || $field_definition['type'] == 'list_boolean') {

        //TODO : ça, ça ne fonctionne pas.
        //C'est destiné au champs SIRTAqui, seulement, il n'y a pas de réelle traduction des valeurs dans le connecteur SIRTAqui,
        //puisque rien ne permet d'associer une valeur en langue par défaut à sa traduction.
        //La seule solution, c'est de requêter la base, pour réunir les ID de valeurs utilisées, puis faire un array_intersect keys().
        //C'est envisageable pour le mode 'additif', mais en terme de performances, difficilement possible avec le mode 'temps-réel'.
        //Pour ce dernier, il faudrait un système de cache + cron. Pas le temps pour le moment.

        if(module_exists('i18n_field')) {
            $allowed_values = i18n_field_translate_allowed_values($field_definition, $configuration['options_language']);
        } else {
            $allowed_values = list_allowed_values($field_definition);
        }

        natcasesort($allowed_values);

        foreach($allowed_values as $value => $label) {
            $values[] = array(
                'value' => $value,
                'alias' => $label,
            );
        }

    } else {

        $lid = $field_infos['field_context'];
        $query = so_listings_init_query();

        $query_fragment_data = so_listings_prepare_query_fragment_data($field_infos['field_name'] . '@field');
        $query['join'] = array_merge($query['join'], $query_fragment_data->joins);

        $column = reset($field_infos['data']['columns']); // on ne considère que la première colonne. C'est suffisant pour un affichage de valeurs significatives
        $query['fields'][] = $query_fragment_data->fields[$column] . " AS " . $field_infos['field_name'];

        $options = array(
            'filters' => true,
            'sorting' => false,
            'count' => false,
            'debug_infos' => __FUNCTION__ . "() : " . t("Search field '@field'", array('@field' => $field_infos['label'])),
        );

        if($autocomplete != null) {
            $options['debug'] = false;

            $input_value = preg_replace('#[\s-&()_+\!,\:;\.\"\']#', '_', trim($autocomplete));

            $query['where'][] = $query_fragment_data->fields[$column] . " LIKE ?";
            $query['args'][] = '%' . $input_value . '%';
        }

        $results = so_listings_compile($lid, $options, $query);

        foreach($results['nodes'] as $node) {
            $values[] = $node[$field_infos['field_name']];
        }
        $values = array_unique($values);
        natcasesort($values);
    }

    return $values;
}

/**
 * Configuration form callback for so_forms field config form.
 * Add option 'AND/OR' to widgets with multiple choices.
 *
 * @param array $field_infos
 * @param string $langcode
 * @param array $configuration
 *
 * @return array form elements to add
 */
function so_listings_search_field_configuration($field_infos, $langcode, $configuration) {

    $form = array();
    $form['#weight'] = -2;

    switch($field_infos['widget_name']) {
        case 'textfield':
            module_load_include('inc', 'so_forms', 'so_forms.admin');

            $options = so_forms_modules_fields_list('so_listings:' . $field_infos['field_context'] . '@search', 'so_listings', $field_infos['field_type'], null);

            // On utilise la recherche sur les champs secondaires uniquement pour so_listings.
            if($field_infos['field_module'] == 'so_listings') {
                // on retire le champ actuel de la liste
                unset($options[$field_infos['field_module']][$field_infos['field_module'] . ':' . $field_infos['field_name'] . ':' . $field_infos['field_context']]);

                $form['secondary_fields'] = array(
                    '#type' => 'select',
                    '#title' => t("Search results in this fields also"),
                    '#multiple' => true,
                    '#size' => 4,
                    '#options' => $options,
                    '#default_value' => !empty($configuration['secondary_fields']) ? $configuration['secondary_fields'] : '',
                    '#weight' => -3,
                );
            }

            if($field_infos['field_type'] == 'text') {
                $form['matching_mode'] = array(
                    '#type' => 'radios',
                    '#title' => t("Results matching"),
                    '#default_value' => !empty($configuration['matching_mode']) ? $configuration['matching_mode'] : 'contain',
                    '#options' => array(
                        'contain' => t("Results contain the term"),
                        'start' => t("Results start by the term"),
                        'match' => t("Results match the term"),
                    ),
                    '#required' => true,
                    '#weight' => -2,
                );

                // autocompletion seulement disponible pour so_listings
                if($field_infos['field_module'] == 'so_listings') {
                    $form['autocomplete'] = array(
                        '#type' => 'textfield',
                        '#title' => t("Use autocompletion"),
                        '#default_value' => (int)$configuration['autocomplete'],
                        '#size' => 1,
                        '#description' => t("Minimum number of letters to type before triggering.<br />'0' = autocompletion disabled"),
                        '#weight' => -1,
                    );
                }

            } elseif($field_infos['field_type'] == 'numerical') {

                $form['operator'] = array(
                    '#type' => 'select',
                    '#title' => t("Results have to be..."),
                    '#options' => array(
                        'equal' => t("equal at entered value"),
                        'different' => t("different from entered value"),
                        'lower' => t("lower than entered value"),
                        'lower_equal' => t("lower or equal than entered value"),
                        'greater' => t("greater than entered value"),
                        'greater_equal' => t("greater or equal than entered value"),
                    ),
                    '#default_value' => $configuration['operator'],
                );
            }

            break;

        case 'checkboxes':
        case 'select':
        case 'onoff':
        case 'radios':
            $form['query_mode'] = array(
                '#type' => 'radios',
                '#title' => t("Result mode"),
                '#default_value' => !empty($configuration['query_mode']) ? $configuration['query_mode'] : 'OR',
                '#options' => array(
                    'OR' => t("Results have to match at least one selected option"),
                    'AND' => t("Results have to match all selected options"),
                ),
                '#required' => true,
            );

            $form['values_and_mode'] = array(
                '#type' => 'checkbox',
                '#title' => t("For a given selected option, results have to match its whole set of values"),
                '#default_value' => $configuration['values_and_mode'],
            );
            $form['#weight'] = -1;

            if(module_exists('i18n_field')) {
                $languages = language_list();

                $language_options = array();

                foreach($languages as $language) {
                    $language_options[$language->language] = t($language->name);
                }

                $form['options_language'] = array(
                    '#type' => 'select',
                    '#title' => t("Options language"),
                    '#options' => $language_options,
                    '#default_value' => array_key_exists('options_language', $configuration) ? $configuration['options_language'] : language_default('language'),
                );
            }

            break;

        case 'datefields':
            $form['results_constraints'] = array(
                '#type' => 'checkboxes',
                '#title' => t("Show the results which"),
                '#default_value' => !empty($configuration['results_constraints']) ? $configuration['results_constraints'] : array('surrounding', 'end_in', 'start_in'),
                '#options' => array(
                    'surrounding' => t("have their both dates surrounding the searched period"),
                    'start_in' => t("have at least their start date in the searched period"),
                    'end_in' => t("have at least their end date in the searched period"),
                    'surrounded' => t("have their both dates surrounded by the searched period") . "*",
                ),
                '#required' => true,
                '#description' => "*" . t("last option results are necessarily included in those of its two preceding options.<br />Hence, it is only applicable as a constraint if they aren't checked."),
            );
            $form['#weight'] = 10;

            break;
    }

    return $form;
}

/**
 * so_forms callback : alter form elements before rendering
 *
 * @param array $field_infos
 * @param string $langcode
 * @param array $configuration
 * @param array $element
 *
 * @return array altered element
 */
function so_listings_search_field_render($field_infos, $langcode, $configuration, $element) {

    // adds autocomplete feature to textfields
    if($configuration['autocomplete'] == true) {
        $element['#autocomplete_path'] = $configuration['js_path'] . '/autocomplete';
    }

    // adds date popup to datefields widget
    if($field_infos['widget_name'] == 'datefields' && module_exists('date_popup')) {

        $element['from'] = array_merge($element['from'], array(
            '#type' => 'date_popup',
            '#date_format' => 'd/m/Y',
            '#date_year_range' => '0:+2',
        ));

        $element['to'] = array_merge($element['to'], array(
            '#type' => 'date_popup',
            '#date_format' => 'd/m/Y',
            '#date_year_range' => '0:+2',
        ));
    }

    return $element;
}

/**
 * so_forms callback : JS process
 *
 * @param array $field_infos
 * @param string $langcode
 * @param array $configuration
 * @param string $op
 * @param string $term
 *
 * return array
 */
function so_listings_search_field_js($field_infos, $langcode, $configuration, $op, $term = null) {

    switch($op) {
        case 'autocomplete':
            if(strlen($term) < $configuration['autocomplete']) {break;}

            if(empty($langcode)) {
                global $language;
                $langcode = $language->language;
            }

            $values = so_listings_search_field_values($field_infos, $langcode, $configuration, $term);

            foreach((array)$configuration['secondary_fields'] as $field) {
                if(empty($field)) {continue;}

                $secondary_field_infos = $field_infos;
                $field = explode(':', $field);

                $secondary_field_infos['field_name'] = $field[1];
                $secondary_field_infos['field_context'] = $field[2];
                $secondary_field_infos['field_module'] = $field[0];

                $values = array_merge($values, so_listings_search_field_values($secondary_field_infos, $langcode, $configuration, $term));
            }

            if(empty($values)) {return array();}

            sort($values);

            return array_combine($values, $values);

            break;
    }

    return array();
}