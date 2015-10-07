<?php

/**
 * @param string $condition
 * @return array
 */
function _so_listings_filters_conditions($condition = '') {

    $conditions = array(
        'equal' => array(                   // ="arg"
            'label' => t("must have the value"),
            'arg_element_callback' => '_so_listings_filters_arguments_single_element',
            'query_callback' => '_so_listings_filters_query_single_argument',
            'description' => t("Same case, same accents, etc."),
        ),
        'different' => array(               // !="arg"
            'label' => t("must be different from"),
            'arg_element_callback' => '_so_listings_filters_arguments_single_element',
            'query_callback' => '_so_listings_filters_query_single_argument',
            'description' => "Case and accents are considered.",
        ),
        'like' => array(                    // LIKE "arg"
            'label' => t("must look like"),
            'arg_element_callback' => '_so_listings_filters_arguments_single_element',
            'query_callback' => '_so_listings_filters_query_single_argument',
            'description' => t("Case and accents aren't considered."),
        ),
        'not_like' => array(                // NOT LIKE "arg"
            'label' => t("mustn't look like"),
            'arg_element_callback' => '_so_listings_filters_arguments_single_element',
            'query_callback' => '_so_listings_filters_query_single_argument',
            'description' => t("Case and accents aren't considered."),
        ),
        'starts_by' => array(               // LIKE "%arg"
            'label' => t("must start by the text"),
            'arg_element_callback' => '_so_listings_filters_arguments_single_element',
            'query_callback' => '_so_listings_filters_query_single_argument',
            'description' => null,
        ),
        'not_starts_by' => array(               // NOT LIKE "%arg"
            'label' => t("mustn't start by the text"),
            'arg_element_callback' => '_so_listings_filters_arguments_single_element',
            'query_callback' => '_so_listings_filters_query_single_argument',
            'description' => null,
        ),
        'ends_by' => array(                 // LIKE "arg%"
            'label' => t("must end by the text"),
            'arg_element_callback' => '_so_listings_filters_arguments_single_element',
            'query_callback' => '_so_listings_filters_query_single_argument',
            'description' => null,
        ),
        'not_ends_by' => array(                 // LIKE "arg%"
            'label' => t("mustn't end by the text"),
            'arg_element_callback' => '_so_listings_filters_arguments_single_element',
            'query_callback' => '_so_listings_filters_query_single_argument',
            'description' => null,
        ),
        'contains' => array(                // LIKE "%arg%"
            'label' => t("must contain the text"),
            'arg_element_callback' => '_so_listings_filters_arguments_single_element',
            'query_callback' => '_so_listings_filters_query_single_argument',
            'description' => null,
        ),
        'not_contains' => array(                // LIKE "%arg%"
            'label' => t("mustn't contain the text"),
            'arg_element_callback' => '_so_listings_filters_arguments_single_element',
            'query_callback' => '_so_listings_filters_query_single_argument',
            'description' => null,
        ),
        'in_single' => array(               // IN([args])
            'label' => t("must have at least one of theses values"),
            'arg_element_callback' => '_so_listings_filters_arguments_multiple_element',
            'query_callback' => '_so_listings_filters_query_multiple_arguments',
            'description' => t("Separated by comma."),
        ),
        'not_in_single' => array(           // NOT IN([args])
            'label' => t("mustn't have any of theses values"),
            'arg_element_callback' => '_so_listings_filters_arguments_multiple_element',
            'query_callback' => '_so_listings_filters_query_multiple_arguments',
            'description' => t("Separated by comma."),
        ),
        'in_multiple' => array(             // IN(subquery)
            'label' => t("must have at least all of theses values"),
            'arg_element_callback' => '_so_listings_filters_arguments_multiple_element',
            'query_callback' => '_so_listings_filters_query_multiple_arguments',
            'description' => t("Separated by comma."),
        ),
        'not_in_multiple' => array(         // NOT IN(subquery)
            'label' => t("mustn't have this set of values"),
            'arg_element_callback' => '_so_listings_filters_arguments_multiple_element',
            'query_callback' => '_so_listings_filters_query_multiple_arguments',
            'description' => t("Separated by comma."),
        ),
        'is_null' => array(                 // IS NULL
            'label' => t("must be empty"),
            'arg_element_callback' => '_so_listings_filters_arguments_null_element',
            'query_callback' => '_so_listings_filters_query_null_argument',
            'description' => null,
        ),
        'is_not_null' => array(             // IS NOT NULL
            'label' => t("mustn't be empty"),
            'arg_element_callback' => '_so_listings_filters_arguments_null_element',
            'query_callback' => '_so_listings_filters_query_null_argument',
            'description' => null,
        ),
        'greater' => array(                 // >"args"
            'label' => t("must be greather than"),
            'arg_element_callback' => '_so_listings_filters_arguments_single_element',
            'query_callback' => '_so_listings_filters_query_single_argument',
            'description' => null,
        ),
        'greater_equal' => array(           // >="args"
            'label' => t("must be greather or equal than"),
            'arg_element_callback' => '_so_listings_filters_arguments_single_element',
            'query_callback' => '_so_listings_filters_query_single_argument',
            'description' => null,
        ),
        'lower' => array(                   // <"args"
            'label' => t("must be lower than"),
            'arg_element_callback' => '_so_listings_filters_arguments_single_element',
            'query_callback' => '_so_listings_filters_query_single_argument',
            'description' => null,
        ),
        'lower_equal' => array(             // <="args"
            'label' => t("must be lower or equal than"),
            'arg_element_callback' => '_so_listings_filters_arguments_single_element',
            'query_callback' => '_so_listings_filters_query_single_argument',
            'description' => null,
        ),
    );

    return !empty($condition) ? $conditions[$condition] : $conditions;
}

/**
 * @param string $type
 * @return array : about 'conditions' : first array is for cardinality '1' (unique) and second one for '-1' (multiple)
 */
function _so_listings_fields_columns_mapping($type = '') {

    $mappings = array(
        'text' => array(
            'value' => array(
                'label' => null,
                'conditions' => array(
                    array('equal', 'different', 'like', 'not_like', 'starts_by', 'not_starts_by', 'ends_by', 'not_ends_by', 'contains', 'not_contains', 'in_single', 'not_in_single', 'is_null', 'is_not_null'),
                    array('equal', 'different', 'like', 'not_like', 'starts_by', 'not_starts_by', 'ends_by', 'not_ends_by', 'contains', 'not_contains', 'in_single', 'in_multiple', 'not_in_single', 'not_in_multiple', 'is_null', 'is_not_null'),
                ),
            ),
        ),

        'text_long' => array(
            'value' => array(
                'label' => null,
                'conditions' => array(
                    array('equal', 'different', 'like', 'not_like', 'starts_by', 'not_starts_by', 'ends_by', 'not_ends_by', 'contains', 'not_contains', 'in_single', 'not_in_single', 'is_null', 'is_not_null'),
                    array('equal', 'different', 'like', 'not_like', 'starts_by', 'not_starts_by', 'ends_by', 'not_ends_by', 'contains', 'not_contains', 'in_single', 'in_multiple', 'not_in_single', 'not_in_multiple', 'is_null', 'is_not_null'),
                ),
            ),
        ),

        'list_text' => array(
            'value' => array(
                'label' => null,
                'conditions' => array(
                    array('equal', 'in_single', 'not_in_single', 'is_null', 'is_not_null'),
                    array('in_single', 'in_multiple', 'not_in_single', 'not_in_multiple', 'is_null', 'is_not_null'),
                ),
            ),
        ),

        'list_boolean' => array( // widget : options_onoff
            'value' => array(
                'label' => null,
                'conditions' => array(
                    array('is_null', 'is_not_null'),
                    array('is_null', 'is_not_null'),
                ),
            ),
        ),

        'number_integer' => array(
            'value' => array(
                'label' => null,
                'conditions' => array(
                    array('equal', 'greater', 'greater_equal', 'lower', 'lower_equal', 'in_single', 'not_in_single', 'is_null', 'is_not_null'),
                    array('equal', 'greater', 'greater_equal', 'lower', 'lower_equal', 'in_single', 'not_in_single', 'is_null', 'is_not_null'),
                ),
            ),
        ),

        'number_decimal' => array(
            'value' => array(
                'label' => null,
                'conditions' => array(
                    array('equal', 'greater', 'greater_equal', 'lower', 'lower_equal', 'in_single', 'not_in_single', 'is_null', 'is_not_null'),
                    array('equal', 'greater', 'greater_equal', 'lower', 'lower_equal', 'in_single', 'not_in_single', 'is_null', 'is_not_null'),
                ),
            ),
        ),

        'datetime' => array(
            'value' => array(
                'label' => t("start date"),
                'conditions' => array(
                    array('equal', 'greater', 'greater_equal', 'lower', 'lower_equal'),
                    array('equal', 'greater', 'greater_equal', 'lower', 'lower_equal'),
                ),
            ),
            'value2' => array(
                'label' => t("end date"),
                'conditions' => array(
                    array('equal', 'greater', 'greater_equal', 'lower', 'lower_equal'),
                    array('equal', 'greater', 'greater_equal', 'lower', 'lower_equal'),
                ),
            ),
        ),

        'image' => array(
            'fid' => array(
                'label' => null,
                'conditions' => array(
                    array('is_null', 'is_not_null'),
                    array('is_null', 'is_not_null'),
                ),
            ),
            'width' => array(
                'label' => t("width"),
                'conditions' => array(
                    array('equal', 'greater', 'greater_equal', 'lower', 'lower_equal'),
                    array('equal', 'greater', 'greater_equal', 'lower', 'lower_equal'),
                ),
            ),
            'height' => array(
                'label' => t("height"),
                'conditions' => array(
                    array('equal', 'greater', 'greater_equal', 'lower', 'lower_equal'),
                    array('equal', 'greater', 'greater_equal', 'lower', 'lower_equal'),
                ),
            ),
        ),

        'file' => array(
            'fid' => array(
                'label' => null,
                'conditions' => array(
                    array('is_null', 'is_not_null'),
                    array('is_null', 'is_not_null'),
                ),
            ),
        ),
    );

    return !empty($type) ? $mappings[$type] : $mappings;
}

function so_listings_filters_form($form, &$form_state, $lid) {

    $definition = so_listings_get_listings_definitions($lid);

    drupal_set_title(t("Edit listing '@listing'", array('@listing' => $definition->name)));

    $sources = $definition->params['sources'];
    $filters = $definition->filters['filters'];
    $sources_pool = so_listings_load_sources_pool(false);

    if($definition->filters['filters']['#published'] == false && $form_state['rebuild'] != true) {
        drupal_set_message(t("The current set of filters has changed. It needs to be published to take effect."), 'warning');
    }

    $type_fragment = so_listings_prepare_query_fragment_data('type@node');

    $fields_pool = array();
    $sources_fields_pool = array();
    $common_fields_pool = array();

    foreach($sources as $source) {

        $options = array(
            'filters' => false,
            'sorting' => false,
        );

        $query = so_listings_init_query();
        $source_infos = explode(':', $source); // 0:module, 1:group, 2:id
        $callback = $sources_pool[$source_infos[0]][$source_infos[1]]['callback'];
        $args = $sources_pool[$source_infos[0]][$source_infos[1]]['sources'][$source_infos[2]]['args'];
        $callback($query, $args, array());

        $query['fields'] = $type_fragment->fields;
        $query['group_by'] = $type_fragment->fields;

        $results = so_listings_compile($lid, $options, $query);

        $sources_fields_pool[$source] = array();

        foreach($results['nodes'] as $node) {

            $fields_instances = field_info_instances('node', $node['type']);

            foreach($fields_instances as $instance) {

                $field_definition = field_info_field($instance['field_name']);

                $mapped_columns = _so_listings_fields_columns_mapping($field_definition['type']);

                if(empty($mapped_columns)) {continue;}

                foreach($field_definition['columns'] as $column => $infos) {

                    if(!array_key_exists($column, $mapped_columns)) {continue;}

                    $sources_fields_pool[$source][$instance['field_name'] . ':' . $column] = array(
                        'label' => $instance['label'] . (!empty($mapped_columns[$column]['label']) ? " (" . $mapped_columns[$column]['label'] . ")" : ""),
                        'field_name' => $instance['field_name'],
                        'fragment_type' => 'field',
                        'column' => $column,
                        'conditions' => $mapped_columns[$column]['conditions'][(int)($field_definition['cardinality'] != 1)],
                        'widget' => $instance['widget']['type'],
                        'cardinality' => $field_definition['cardinality'],
                    );

                    $fields_pool[$instance['field_name'] . ':' . $column] = $sources_fields_pool[$source][$instance['field_name'] . ':' . $column];
                }
            }
        }

        $common_fields_pool = !empty($common_fields_pool) ? array_intersect_key($common_fields_pool, $sources_fields_pool[$source]) : $sources_fields_pool[$source];
    }

    $form = array(
        '#tree' => true,
        '#attached' => array(
            'css' => array(drupal_get_path('module', 'so_listings') . '/css/so_listings_admin.css')
        ),
        '#attributes' => array(
            'class' => 'so_listing_filters',
        ),

        'fields_pool' => array('#type' => 'value', '#value' => $fields_pool),

        'global_filters' => array(
            '#type' => 'fieldset',
            '#title' => t("Global filters"),
            '#collapsible' => true,
            '#collapsed' => false,
        ),

        'sources_filters' => array(
            '#type' => 'fieldset',
            '#title' => t("Filters by data sources"),
        ),
    );

    $filters_index = 0;

    if(!empty($common_fields_pool)) {
        so_listings_filters_filters_element($form['global_filters'], $form_state['values']['global_filters'], $filters_index++, $filters['global_filters'], $common_fields_pool);
    } else {
        $form['global_filters']['message'] = array('#markup' => "- " . t("These sources have no common fields to filter") . " -");
    }

    foreach($sources as $source) {
        $source_infos = explode(':', $source); // 0:module, 1:group, 2:id

        $form['sources_filters'][$source] = array(
            '#type' => 'fieldset',
            '#title' => $sources_pool[$source_infos[0]][$source_infos[1]]['label'] . " : " . $sources_pool[$source_infos[0]][$source_infos[1]]['sources'][$source_infos[2]]['label'],
            '#collapsible' => true,
            '#collapsed' => true,
        );

        so_listings_filters_filters_element($form['sources_filters'][$source], $form_state['values']['sources_filters'][$source], $filters_index++, $filters[$source], $sources_fields_pool[$source]);

        $form['sources_filters'][$source]['language']['#states'] = array(
            'visible' => array(
                'select#edit-global-filters-language' => array('value' => ''),
            ),
        );
    }

    $form['actions'] = array(
        '#type' => 'actions',
        '#weight' => 100,

        'publish' => array(
            '#type' => 'submit',
            '#value' => t("Publish filters"),
            '#submit' => array('so_listings_filters_form_publish_submit'),
        ),

        'back' => array(
            '#markup' => l(t("Back"), 'admin/structure/so_listings'),
        ),
    );

    return $form;
}

/**
 * One source's set of filters
 *
 * @param array &$element : the form's part dedicated to the source : &$form['filters'][$source].
 * @param array $values
 * @param int $filters_index
 * @param array $filter
 * @param array $source_fields_pool
 */
function so_listings_filters_filters_element(&$element, $values, $filters_index, $filter, $source_fields_pool) {

    $language_list = array('' => t("All")) + so_listings_language_list();

    $fields_options = array(
        '' => "- " . t("Field") . " -",
    );

    foreach($source_fields_pool as $field => $infos) {
        $fields_options[$field] = $infos['label'];
    }

    asort($fields_options);

    $sql_generator_id = 'generator_' . $filters_index;

    $modules = module_implements('so_listings_filter');

    $predefined_options = array('' => t("- Clause -"));

    foreach($modules as $module) {

        $function = $module . '_so_listings_filter';
        $clauses = $function();

        foreach($clauses as $clause => $infos) {
            $predefined_options[$module . ':' . $clause] = $infos['label'];
        }
    }

    $element += array(
        'language' => array(
            '#type' => 'select',
            '#title' => t("Nodes' language"),
            '#options' => $language_list,
            '#default_value' => $filter['language'],
        ),

        'sql_generator' => array(
            '#type' => 'container',
            '#prefix' => "<div id='" . $sql_generator_id . "'>",
            '#suffix' => "</div>",

            'clauses' => array(
                '#type' => 'container',
                '#theme' => 'so_listings_filters_clauses_overview',
                '##id' => 'clauses_overview_' . $filters_index,
            ),

            'save' => array(),

            'add_clause' => array(
                '#type' => 'container',
                '#theme' => 'so_listings_filters_add_clause',
                '#title' => t("Add a custom condition"),

                'type' => array('#type' => 'value', '#value' => 'clause'),

                'clause' => array(
                    '#type' => 'container',
                    '#theme' => 'so_listings_filters_clause',
                ),

                'drag_id' => array(
                    '#type' => 'value',
                ),

                'drag_pid' => array(
                    '#type' => 'value',
                    '#value' => 0,
                ),

                'add' => array(
                    '#name' => 'add_clause_' . $filters_index,
                    '#type' => 'submit',
                    '#value' => t("Add"),
                    '#submit' => array('so_listings_filters_form_add_clause_submit'),
                    '#validate' => array('so_listings_filters_form_add_clause_validate'),
                    '##container_level' => -2,
                    '#ajax' => array(
                        'callback' => 'so_listings_ajax_filters_clause',
                        'wrapper' => $sql_generator_id,
                        'effect' => 'none',
                        'event' => 'click',
                        'method' => 'replace',
                        'progress' => array(
                            'type' => 'throbber',
                            'message' => null,
                        ),
                    ),
                ),
            ),

            'add_predefined' => array(
                '#type' => 'container',
                '#theme' => 'so_listings_filters_add_clause',
                '#title' => t("Add a predefined condition"),

                'type' => array('#type' => 'value', '#value' => 'predefined'),

                'clause' => array(
                    '#type' => 'container',

                    'type' => array('#type' => 'value', '#value' => 'predefined'),

                    'predefined' => array(
                        '#type' => 'select',
                        '#options' => $predefined_options,
                    ),
                ),

                'drag_id' => array(
                    '#type' => 'value',
                ),

                'drag_pid' => array(
                    '#type' => 'value',
                    '#value' => 0,
                ),

                'add' => array(
                    '#name' => 'add_predefined_' . $filters_index,
                    '#type' => 'submit',
                    '#value' => t("Add"),
                    '#submit' => array('so_listings_filters_form_add_clause_submit'),
                    '#validate' => array('so_listings_filters_form_add_clause_validate'),
                    '##container_level' => -2,
                    '#ajax' => array(
                        'callback' => 'so_listings_ajax_filters_clause',
                        'wrapper' => $sql_generator_id,
                        'effect' => 'none',
                        'event' => 'click',
                        'method' => 'replace',
                        'progress' => array(
                            'type' => 'throbber',
                            'message' => null,
                        ),
                    ),
                ),
            ),

            'add_group' => array(
                '#type' => 'container',
                '#theme' => 'so_listings_filters_add_group',

                'type' => array('#type' => 'value', '#value' => 'group'),

                'drag_id' => array(
                    '#type' => 'value',
                ),

                'drag_pid' => array(
                    '#type' => 'value',
                    '#value' => 0,
                ),

                'add' => array(
                    '#name' => 'add_group_' . $filters_index,
                    '#type' => 'submit',
                    '#value' => t("Add"),
                    '#submit' => array('so_listings_filters_form_add_clause_submit'),
                    '##container_level' => -2,
                    '#ajax' => array(
                        'callback' => 'so_listings_ajax_filters_clause',
                        'wrapper' => $sql_generator_id,
                        'effect' => 'none',
                        'event' => 'click',
                        'method' => 'replace',
                        'progress' => array(
                            'type' => 'throbber',
                            'message' => null,
                        ),
                    ),
                ),
            ),
        ),
    );

    if(count($predefined_options) == 1) {
        unset($element['sql_generator']['add_predefined']);
    }

    $clause_index = 0;
    $clause_values = array();
    $max_drag_id = 0;

    _so_listings_filters_clauses_tree($filter['clauses']);

    foreach((array)$filter['clauses'] as $id => $clause) {

        $max_drag_id = max($max_drag_id, $clause['drag_id']);

        if($clause['type'] == 'clause') {

            $element['sql_generator']['clauses'][$id] = array(
                '#type' => 'container',

                'clause' => array(
                    '#type' => 'container',
                    '#theme' => 'so_listings_filters_clause',
                ),

                '##drag_depth' => $clause['depth'],

                'drag_id' => array(
                    '#type' => 'hidden',
                    '#default_value' => $clause['drag_id'],
                    '#attributes' => array('class' => array('clause_drag_id')),
                ),

                'drag_pid' => array(
                    '#type' => 'textfield',
                    '#title' => "pid",
                    '#size' => 3,
                    '#default_value' => $clause['drag_pid'],
                    '#attributes' => array('class' => array('clause_drag_pid')),
                ),

                'weight' => array(
                    '#type' => 'weight',
                    '#title' => t("Weight"),
                    '#default_value' => $clause['weight'],
                    '#delta' => 50,
                    '#attributes' => array('class' => array('clause_weight')),
                ),

                'delete' => array(
                    '#name' => 'delete_' . $filters_index . '_' . $id,
                    '#type' => 'submit',
                    '#value' => t("Delete"),
                    '#submit' => array('so_listings_filters_form_delete_clause_submit'),
                    '##container_level' => -3,
                    '#ajax' => array(
                        'callback' => 'so_listings_ajax_filters_clause',
                        'wrapper' => $sql_generator_id,
                        'effect' => 'none',
                        'event' => 'click',
                        'method' => 'replace',
                        'progress' => array(
                            'type' => 'throbber',
                            'message' => null,
                        ),
                    ),
                ),
            );

            $clause_values = array_merge((array)$clause, (array)$values['sql_generator']['clauses'][$id]['clause']);

            so_listings_filters_clause_element(
                $element['sql_generator']['clauses'][$id]['clause'],
                $clause_values,
                $source_fields_pool,
                $fields_options,
                $filters_index,
                $clause_index++
            );

            // @see so_listings_filters_clause_element()
            if(!empty($element['sql_generator']['clauses'][$id]['clause'])) {
                // DEV : la fonction de thème doit être un theme_wrapper
                /*$element['sql_generator']['clauses'][$id]['clause']['args']['#theme'] = 'so_listings_filters_clause_args';*/
                $element['sql_generator']['clauses'][$id]['clause']['field']['#disabled'] = true;
                array_shift($element['sql_generator']['clauses'][$id]['clause']['condition']['#options']);
            } else {
                unset($element['sql_generator']['clauses'][$id]);
            }

        } elseif($clause['type'] == 'predefined') {

            $clause_values = array_merge((array)$clause, (array)$values['sql_generator']['clauses'][$id]);
            $clause_id = 'predefined_' . $filters_index . '_' . $clause_index++;

            $element['sql_generator']['clauses'][$id] = array(
                '#type' => 'container',

                'clause' => array(
                    '#type' => 'container',
                    '#theme' => 'so_listings_filters_clause',
                    '#prefix' => "<div id='" . $clause_id . "' class='filters_clause'>",
                    '#suffix' => "</div>",

                    'type' => array('#type' => 'value', '#value' => 'predefined'),

                    'mode' => array(
                        '#type' => 'select',
                        '#options' => array(
                            'and' => t("AND"),
                            'or' => t("OR"),
                        ),
                        '#default_value' => $clause_values['mode'],
                        '#attributes' => array('class' => array('filters_clause_mode')),
                    ),

                    'predefined' => array(
                        '#type' => 'select',
                        '#options' => $predefined_options,
                        '#default_value' => $clause['predefined'],
                        '#disabled' => true,
                    ),

                    'settings' => array(
                        '#type' => 'value',
                        '#value' => $clause['settings'],
                    ),

                    'settings_wrapper' => array(
                        '#type' => 'container',
                    ),
                ),

                '##drag_depth' => $clause['depth'],

                'drag_id' => array(
                    '#type' => 'hidden',
                    '#default_value' => $clause['drag_id'],
                    '#attributes' => array('class' => array('clause_drag_id')),
                ),

                'drag_pid' => array(
                    '#type' => 'textfield',
                    '#title' => "pid",
                    '#size' => 3,
                    '#default_value' => $clause['drag_pid'],
                    '#attributes' => array('class' => array('clause_drag_pid')),
                ),

                'weight' => array(
                    '#type' => 'weight',
                    '#title' => t("Weight"),
                    '#default_value' => $clause['weight'],
                    '#delta' => 50,
                    '#attributes' => array('class' => array('clause_weight')),
                ),

                'delete' => array(
                    '#name' => 'delete_' . $filters_index . '_' . $id,
                    '#type' => 'submit',
                    '#value' => t("Delete"),
                    '#submit' => array('so_listings_filters_form_delete_clause_submit'),
                    '##container_level' => -3,
                    '#ajax' => array(
                        'callback' => 'so_listings_ajax_filters_clause',
                        'wrapper' => $sql_generator_id,
                        'effect' => 'none',
                        'event' => 'click',
                        'method' => 'replace',
                        'progress' => array(
                            'type' => 'throbber',
                            'message' => null,
                        ),
                    ),
                ),
            );

            $clause_infos = explode(':', $clause['predefined']);
            $module_clauses = module_invoke($clause_infos[0], 'so_listings_filter');
            $module_clause = $module_clauses[$clause_infos[1]];

            if(array_key_exists('form', $module_clause)) {
                $element['sql_generator']['clauses'][$id]['clause']['settings_wrapper'] = $module_clause['form']($clause['settings']);
            }

        } elseif($clause['type'] == 'group') {

            $clause_values = array_merge((array)$clause, (array)$values['sql_generator']['clauses'][$id]);

            $element['sql_generator']['clauses'][$id] = array(
                '#type' => 'container',

                'clause' => array(
                    'type' => array('#type' => 'value', '#value' => 'group'),

                    'mode' => array(
                        '#type' => 'select',
                        '#options' => array(
                            'and' => t("AND"),
                            'or' => t("OR"),
                        ),
                        '#default_value' => $clause_values['mode'],
                        '#attributes' => array('class' => array('filters_clause_mode')),
                    ),
                ),

                '##drag_depth' => $clause['depth'],

                'drag_id' => array(
                    '#type' => 'hidden',
                    '#default_value' => $clause['drag_id'],
                    '#attributes' => array('class' => array('clause_drag_id')),
                ),

                'drag_pid' => array(
                    '#type' => 'textfield',
                    '#title' => "pid",
                    '#size' => 3,
                    '#default_value' => $clause['drag_pid'],
                    '#attributes' => array('class' => array('clause_drag_pid')),
                ),

                'weight' => array(
                    '#type' => 'weight',
                    '#title' => t("Weight"),
                    '#default_value' => $clause['weight'],
                    '#delta' => 50,
                    '#attributes' => array('class' => array('clause_weight')),
                ),

                'delete' => array(
                    '#name' => 'delete_' . $filters_index . '_' . $id,
                    '#type' => 'submit',
                    '#value' => t("Delete"),
                    '#submit' => array('so_listings_filters_form_delete_clause_submit'),
                    '##container_level' => -3,
                    '#ajax' => array(
                        'callback' => 'so_listings_ajax_filters_clause',
                        'wrapper' => $sql_generator_id,
                        'effect' => 'none',
                        'event' => 'click',
                        'method' => 'replace',
                        'progress' => array(
                            'type' => 'throbber',
                            'message' => null,
                        ),
                    ),
                ),
            );
        }
    }

    so_listings_filters_clause_element(
        $element['sql_generator']['add_clause']['clause'],
        $values['sql_generator']['add_clause']['clause'],
        $source_fields_pool,
        $fields_options,
        $filters_index,
        $clause_index++
    );

    $element['sql_generator']['add_clause']['clause']['mode'] = array();

    $element['sql_generator']['add_clause']['drag_id']['#value'] = $max_drag_id +1;
    $element['sql_generator']['add_callback']['drag_id']['#value'] = $max_drag_id +1;
    $element['sql_generator']['add_group']['drag_id']['#value'] = $max_drag_id +1;

    if(count(element_children($element['sql_generator']['clauses'])) > 0) {
        $element['sql_generator']['save'] = array(
            '#type' => 'submit',
            '#value' => t("Save parameters"),
            '#submit' => array('so_listings_filters_form_save_submit'),
        );
    }
}

function so_listings_filters_clause_element(&$element, $values, $source_fields_pool, $fields_options, $filters_index, $clause_index) {

    $condition_options = array(
        '' => "- " . t("Condition") . " -",
    );

    if(!empty($values) && !empty($values['field'])) {
        $selected_field = $values['field'];

        // si les sources ont changé, on peut se retrouver avec des clauses portant sur des champs qui n'existent pas dans ces sources.
        if(!array_key_exists($selected_field, $source_fields_pool)) {
            $element = array();
            return;
        }

        if(!empty($selected_field)) {
            $field_conditions = array_combine($source_fields_pool[$selected_field]['conditions'], $source_fields_pool[$selected_field]['conditions']);
            $conditions = array_intersect_key(_so_listings_filters_conditions(), $field_conditions);

            foreach($conditions as $condition => $infos) {
                $condition_options[$condition] = $infos['label'];
            }
        }
    }

    $clause_id = 'clause_' . $filters_index . '_' . $clause_index;

    $element += array(
        '#prefix' => "<div id='" . $clause_id . "' class='filters_clause'>",
        '#suffix' => "</div>",

        'type' => array('#type' => 'value', '#value' => 'clause'),

        'mode' => array(
            '#type' => 'select',
            '#options' => array(
                'and' => t("AND"),
                'or' => t("OR"),
            ),
            '#default_value' => $values['mode'],
            '#attributes' => array('class' => array('filters_clause_mode')),
        ),

        'field' => array(
            '#type' => 'select',
            '#options' => $fields_options,
            '#default_value' => $values['field'],
            '##container_level' => -1,
            '#ajax' => array(
                'callback' => 'so_listings_ajax_filters_clause',
                'wrapper' => $clause_id,
                'effect' => 'none',
                'event' => 'change',
                'method' => 'replace',
                'progress' => array(
                    'type' => 'throbber',
                    'message' => null,
                ),
            ),
        ),

        'condition' => array(
            '#type' => 'select',
            '#options' => $condition_options,
            '#disabled' => count($condition_options) < 2,
            '#default_value' => $values['condition'],
            '##container_level' => -1,
            '#ajax' => array(
                'callback' => 'so_listings_ajax_filters_clause',
                'wrapper' => $clause_id,
                'effect' => 'none',
                'event' => 'change',
                'method' => 'replace',
                'progress' => array(
                    'type' => 'throbber',
                    'message' => null,
                ),
            ),
        ),

        'args' => array(),
    );

    $selected_condition = $values['condition'];

    $function = $conditions[$selected_condition]['arg_element_callback'];

    $args_default_value = "- " . t("Arguments") . " -";

    if(!empty($values) && function_exists($function)) {
        $values['args'] = $values['args'] == $args_default_value ? '' : $values['args'];

        $function($element['args'], $values['args'], $source_fields_pool[$selected_field], $conditions[$selected_condition], $clause_id);
    } else {
        _so_listings_filters_arguments_default_element($element['args'], $args_default_value);
    }
}

function so_listings_ajax_filters_clause($form, $form_state) {

    $path = $form_state['triggering_element']['#parents'];
    array_splice($path, $form_state['triggering_element']['##container_level']);

    return drupal_array_get_nested_value($form, $path);
}

function so_listings_filters_form_add_clause_validate($form, &$form_state) {

    $path = $form_state['triggering_element']['#parents'];
    array_pop($path);
    $clause = drupal_array_get_nested_value($form_state['values'], $path);

    if($clause['type'] == 'clause') {
        if(empty($clause['clause']['field'])) {
            form_set_error(implode('][', $path) . '][clause][field', t("You must choose a field"));
        }
    } elseif($clause['type'] == 'predefined') {
        if(empty($clause['clause']['predefined'])) {
            form_set_error(implode('][', $path) . '][clause][predefined', t("You must choose a clause"));
        }
    }
}

function so_listings_filters_form_add_clause_submit($form, &$form_state) {

    $definition = so_listings_get_listings_definitions($form_state['build_info']['args'][0]);

    $definition->filters['filters']['#published'] = false;

    $data_path = $form_state['triggering_element']['#parents'];
    array_splice($data_path, -1);

    $filters_path = $data_path;
    array_splice($filters_path, -2);

    $base_target_path = $filters_path;
    $filters_path = array_pop($filters_path);

    $target_path = so_listings_filters_clauses_order($form_state, $base_target_path);

    $clauses = (array)drupal_array_get_nested_value($form_state['values'], $target_path[0]);

    foreach($clauses as $id => $clause) {
        unset($clause['weight']);
        if(array_key_exists('settings_wrapper', $clause['clause'])) {unset($clause['clause']['settings_wrapper']);}

        $clauses[$id] = array_merge($clause, $clause['clause']);
        unset($clauses[$id]['clause']);
    }

    $added = drupal_array_get_nested_value($form_state['values'], $data_path);
    $added = array_merge($added, (array)$added['clause']);
    unset($added['clause']);

    if($added['type'] == 'predefined') {

        $clause_infos = explode(':', $added['predefined']);
        $module_clauses = module_invoke($clause_infos[0], 'so_listings_filter');
        $module_clause = $module_clauses[$clause_infos[1]];

        if(array_key_exists('callback', $module_clause) && !empty($module_clause['callback'])
                && array_key_exists('settings', $module_clause) && !empty($module_clause['settings'])) {
            $added['settings'] = $module_clause['settings'];
        }
    }

    $definition->filters['filters'][$filters_path]['clauses'] = (array)$clauses;
    $definition->filters['filters'][$filters_path]['clauses'][] = $added;

    $definition = array(
        'id' => $form_state['build_info']['args'][0],
        'filters' => $definition->filters,
    );

    so_listings_save_listing_definition($definition);

    drupal_array_set_nested_value($form_state['values'], $base_target_path, array());
    drupal_array_set_nested_value($form_state['input'], $base_target_path, array());

    $form_state['rebuild'] = true;
}

function so_listings_filters_form_delete_clause_submit($form, &$form_state) {

    $definition = so_listings_get_listings_definitions($form_state['build_info']['args'][0]);

    $definition->filters['filters']['#published'] = false;

    $data_path = $form_state['triggering_element']['#parents'];
    array_splice($data_path, -1, 1, 'clause');

    $filters_path = $data_path;
    array_splice($filters_path, -4);

    $base_target_path = $filters_path;
    $filters_path = array_pop($filters_path);

    $clause_path = $data_path;
    array_pop($clause_path);
    drupal_array_set_nested_value($form_state['values'], $clause_path, null);

    $target_path = so_listings_filters_clauses_order($form_state, $base_target_path);

    $clauses = (array)drupal_array_get_nested_value($form_state['values'], $target_path[0]);

    foreach($clauses as $id => $clause) {
        unset($clause['weight']);

        $clauses[$id] = array_merge($clause, $clause['clause']);
        unset($clauses[$id]['clause']);
    }

    $definition->filters['filters'][$filters_path]['clauses'] = $clauses;

    $definition = array(
        'id' => $form_state['build_info']['args'][0],
        'filters' => $definition->filters,
    );

    so_listings_save_listing_definition($definition);

    $form_state['rebuild'] = true;
}

function so_listings_filters_form_save_submit($form, &$form_state) {

    $definition = so_listings_get_listings_definitions($form_state['build_info']['args'][0]);

    $definition->filters['filters']['#published'] = false;

    $targets_paths = so_listings_filters_clauses_order($form_state);

    $global_language = $form_state['values']['global_filters']['language'];

    foreach($targets_paths as $path) {
        $clauses = (array)drupal_array_get_nested_value($form_state['values'], $path);
        array_splice($path, -2);

        $language_path = $path;
        $language_path[] = 'language';
        $source = array_pop($path);

        foreach($clauses as $id => $clause) {
            unset($clause['weight']);

            if(array_key_exists('settings_wrapper', $clause['clause'])) {

                $clause['clause']['settings'] = $clause['clause']['settings_wrapper'];
                unset($clause['clause']['settings_wrapper']);
            }

            $clauses[$id] = array_merge($clause, $clause['clause']);
            unset($clauses[$id]['clause']);
        }

        $definition->filters['filters'][$source]['language'] = $global_language != '' ? $global_language : drupal_array_get_nested_value($form_state['values'], $language_path);
        $definition->filters['filters'][$source]['clauses'] = $clauses;
    }

    $definition = array(
        'id' => $form_state['build_info']['args'][0],
        'filters' => $definition->filters,
    );

    so_listings_save_listing_definition($definition);

    if(empty($form_state['triggering_element']['#ajax'])) {
        drupal_set_message(t("Configuration has been saved."));
    }
}

function so_listings_filters_form_publish_submit($form, &$form_state) {

    $definition = so_listings_get_listings_definitions($form_state['build_info']['args'][0]);
    $fields_pool = $form_state['values']['fields_pool'];
    $conditions_pool = _so_listings_filters_conditions();
    $sources_pool = so_listings_load_sources_pool(false);

    $definition->filters['filters']['#published'] = true;
    $definition->cache['filters']['filters'] = array();

    $targets_paths = so_listings_filters_clauses_order($form_state);

    $global_language = $form_state['values']['global_filters']['language'];

    foreach($targets_paths as $path) {
        $query = so_listings_init_query();

        $clauses = (array)drupal_array_get_nested_value($form_state['values'], $path);
        array_splice($path, -2);

        $language_path = $path;
        $language_path[] = 'language';
        $source = array_pop($path);

        foreach($clauses as $id => $clause) {
            unset($clause['weight']);

            $clauses[$id] = array_merge($clause, $clause['clause']);
            unset($clauses[$id]['clause']);
        }

        $language = $global_language != '' ? $global_language : drupal_array_get_nested_value($form_state['values'], $language_path);

        $definition->filters['filters'][$source]['language'] = $language;
        $definition->filters['filters'][$source]['clauses'] = $clauses;

        _so_listings_filters_parse_clauses($definition, $query, $source, $clauses, $language, $fields_pool, $conditions_pool, $sources_pool);

        $definition->cache['filters']['filters'][$source] = $query;
    }

    $definition = array(
        'id' => $form_state['build_info']['args'][0],
        'filters' => $definition->filters,
        'cache' => $definition->cache,
    );

    so_listings_save_listing_definition($definition);

    drupal_set_message(t("Filters have been applied on listing."));
}

function _so_listings_filters_parse_clauses($definition, &$query, $source, $clauses, $language, $fields_pool, $conditions_pool, $sources_pool) {

    $where = array();

    $source_infos = explode(':', $source); // 0:module, 1:group, 2:id
    $source_query = so_listings_init_query(false);

    if(count($source_infos) == 3) {
        $callback = $sources_pool[$source_infos[0]][$source_infos[1]]['callback'];
        $args = $sources_pool[$source_infos[0]][$source_infos[1]]['sources'][$source_infos[2]]['args'];

        $callback($source_query, $args, array());
        $query['join'] = array_merge($query['join'], $source_query['join']);
        $query['args'] = array_merge($query['args'], $source_query['args']);
    }

    $where = array_merge($where, $source_query['where']);

    $where[] = $language != '' ? "(n.language='" . $language . "' OR n.language='und')" : "";

    _so_listings_filters_clauses_tree($clauses);

    $filter = "";
    $cur_depth = 0;
    $no_mode = true;
    $opened = 0;
    $callbacks_placeholders = array();
    $callback_placeholders_idx = 0;

    foreach($clauses as $id => $clause) {

        if($clause['type'] == 'clause') {

            $tmp_query = so_listings_init_query(false);

            $function = $conditions_pool[$clause['condition']]['query_callback'];
            $function($tmp_query, $fields_pool[$clause['field']], $clause['condition'], $clause['args']);

            $query['join'] = array_merge($query['join'], $tmp_query['join']);
            $query['args'] = array_merge($query['args'], $tmp_query['args']);
            $where_clause = array_shift($tmp_query['where']);

        } elseif($clause['type'] == 'predefined') {

            $clause_infos = explode(':', $clause['predefined']);
            $module_clauses = module_invoke($clause_infos[0], 'so_listings_filter');
            $module_clause = $module_clauses[$clause_infos[1]];

            if(!array_key_exists('callback', $module_clause) || empty($module_clause['callback'])) { // filtre statique

                $query['#context'] = array_merge($query['#context'], $module_clause['filter']['#context']);
                $query['fields'] = array_merge($query['fields'], $module_clause['filter']['fields']);
                $query['join'] = array_merge($query['join'], $module_clause['filter']['join']);
                $where_clause = array_shift($module_clause['filter']['where']);
                $query['args'] = array_merge($query['args'], $module_clause['filter']['args']);

            } elseif(array_key_exists('callback', $module_clause) && $module_clause['cache'] == true) { // callback statique

                $function = $module_clause['callback'];
                $tmp_query = so_listings_init_query(false);
                $function(
                    $tmp_query,
                    $definition->id,
                    array_key_exists($clause_infos[0], $definition->params['modules_settings']) ?
                        $definition->params['modules_settings'][$clause_infos[0]] :
                        array()
                );

                $query['#context'] = array_merge($query['#context'], $tmp_query['#context']);
                $query['fields'] = array_merge($query['fields'], $tmp_query['fields']);
                $query['join'] = array_merge($query['join'], $tmp_query['join']);
                $where_clause = array_shift($tmp_query['where']);
                $query['args'] = array_merge($query['args'], $tmp_query['args']);

            } else { // callback dynamique

                $placeholder = '[[callback_' . $callback_placeholders_idx++ . ']]';
                $callbacks_placeholders[$placeholder] = array(
                    'module' => $clause_infos[0],
                    'callback' => $module_clause['callback'],
                    'settings' => $clause['settings'],
                );
                $where_clause = $placeholder;
                $query['args'][$placeholder] = count($query['args']);
            }
        }

        $parenthesis = "";

        if($clause['depth'] != $cur_depth) {
            if($clause['depth'] > $cur_depth) {
                $parenthesis = "(";
                $opened++;
                $no_mode = true;
            } else {
                $to_close = abs($cur_depth - $clause['depth']);
                $parenthesis = str_repeat(")", $to_close);
                $opened -= $to_close;
            }

            $cur_depth = $clause['depth'];
        }

        $filter .= $parenthesis;
        $filter .= $no_mode == false ? " "  . strtoupper($clause['mode']) . " " : "";
        $no_mode = false;

        $filter .= $clause['type'] == 'clause' || $clause['type'] == 'predefined' ? $where_clause : "";
    }

    $filter .= str_repeat(")", $opened);

    $where[] = $filter;

    $where = array_filter($where);

    if(!empty($where)) {
        $final_where_clause = "(" . implode(" AND ", $where) . ")";

        if(empty($callbacks_placeholders)) {
            $query['where'][] = $final_where_clause;
        } else {
            $query['where'][] = array(
                'clause' => $final_where_clause,
                'callbacks' => $callbacks_placeholders
            );
        }
    }

    return $filter;
}

function _so_listings_filters_query_single_argument(&$query, $field, $condition, $args) {

    $fragment = so_listings_prepare_query_fragment_data($field['field_name'] . '@' . $field['fragment_type']);

    $argument = '';
    $placeholder = '?';

    switch($field['widget']) {

        case 'date_popup':
            if($args['select'] == 'fixed') {
                $argument = $args['input'];
            } else {
                $argument = null;
                $placeholder = "CURDATE()";
            }
            break;

        case 'text_textfield':
        case 'options_buttons':
        case 'image_image':
        case 'number':
            $argument = trim($args);
    }

    switch($condition) {
        case 'equal':
            $operator = '=' . $placeholder;
            break;

        case 'like':
            $operator = ' LIKE ' . $placeholder;
            $argument = db_like($argument);
            break;

        case 'different':
            $operator = '!=' . $placeholder;
            break;

        case 'not_like':
            $operator = ' NOT LIKE ' . $placeholder;
            $argument = db_like($argument);
            break;

        case 'starts_by':
            $operator = ' LIKE ' . $placeholder;
            $argument = db_like($argument) . '%';
            break;

        case 'not_starts_by':
            $operator = ' NOT LIKE ' . $placeholder;
            $argument = db_like($argument) . '%';
            break;

        case 'ends_by':
            $operator = ' LIKE ' . $placeholder;
            $argument = '%' . db_like($argument);
            break;

        case 'not_ends_by':
            $operator = ' NOT LIKE ' . $placeholder;
            $argument = '%' . db_like($argument);
            break;

        case 'contains':
            $operator = ' LIKE ' . $placeholder;
            $argument = '%' . db_like($argument) . '%';
            break;

        case 'not_contains':
            $operator = ' NOT LIKE ' . $placeholder;
            $argument = '%' . db_like($argument) . '%';
            break;

        case 'greater':
            $operator = '>' . $placeholder;
            break;

        case 'greater_equal':
            $operator = '>=' . $placeholder;
            break;

        case 'lower':
            $operator = '<' . $placeholder;
            break;

        case 'lower_equal':
            $operator = '<=' . $placeholder;
            break;
    }

    $query['join'] = $fragment->joins;
    $query['where'][] = $fragment->fields[$field['column']] . $operator;

    if(!empty($argument)) {
        $query['args'][] = $argument;
    }
}

function _so_listings_filters_query_multiple_arguments(&$query, $field, $condition, $args) {

    $fragment = so_listings_prepare_query_fragment_data($field['field_name'] . '@' . $field['fragment_type']);

    $db_field = $fragment->fields[$field['column']];

    switch($condition) {

        case 'in_single':
            switch($field['widget']) {
                case 'options_select':
                case 'options_buttons':
                    $arguments = "'" . implode("','", array_filter($args)) . "'";
                    break;

                default:
                    $arguments = explode(",", $args);

                    array_walk($arguments, function(&$val){
                        $val = trim($val);
                    });

                    $arguments = "'" . implode("','", $arguments) . "'";
                    break;
            }

            $operator = " IN(" . $arguments . ")";

            break;

        case 'not_in_single':
            switch($field['widget']) {
                case 'options_select':
                case 'options_buttons':
                    $arguments = "'" . implode("','", array_filter($args)) . "'";
                    break;

                default:
                    $arguments = explode(",", $args);

                    array_walk($arguments, function(&$val){
                        $val = trim($val);
                    });

                    $arguments = "'" . implode("','", $arguments) . "'";
                    break;
            }

            $operator = " NOT IN(" . $arguments . ")";

            break;

        case 'in_multiple':
            $args = array_filter($args);
            $arguments = "'" . implode("','", $args) . "'";
            $num_arguments = count($args);

            $operator = " IN (" .
                        "SELECT entity_id" .
                        " FROM " . $fragment->table .
                        " WHERE " . $db_field .
                        " IN(" . $arguments . ")" .
                        " GROUP BY entity_id" .
                        " HAVING COUNT(" . $db_field . ")=" . $num_arguments .
                        ")";

            $db_field = 'n.nid';

            break;

        case 'not_in_multiple':
            $args = array_filter($args);
            $arguments = "'" . implode("','", $args) . "'";
            $num_arguments = count($args);

            $operator = " NOT IN (" .
                        "SELECT entity_id" .
                        " FROM " . $fragment->table .
                        " WHERE " . $db_field .
                        " IN(" . $arguments . ")" .
                        " GROUP BY entity_id" .
                        " HAVING COUNT(" . $db_field . ")=" . $num_arguments .
                        ")";

            $db_field = 'n.nid';

            break;
    }

    $query['join'] = $fragment->joins;
    $query['where'][] = $db_field . $operator;
}

function _so_listings_filters_query_null_argument(&$query, $field, $condition, $args) {

    $fragment = so_listings_prepare_query_fragment_data($field['field_name'] . '@' . $field['fragment_type']);

    if($field['widget'] != 'options_onoff') {
        switch($condition) {
            case 'is_null':
                $operator = " IS NULL";
                break;

            case 'is_not_null':
                $operator = " IS NOT NULL";
                break;
        }

        $query['where'][] = $fragment->fields[$field['column']] . $operator;
    } else {
            switch($condition) {
            case 'is_null':
                $operator = "(" . $fragment->fields[$field['column']] . " IS NULL OR " . $fragment->fields[$field['column']] . "=0)";
                break;

            case 'is_not_null':
                $operator = "(" . $fragment->fields[$field['column']] . " IS NOT NULL AND " . $fragment->fields[$field['column']] . "!=0)";
                break;
        }

        $query['where'][] = $operator;
    }

    $query['join'] = $fragment->joins;

}

/**
 * Sorts the clauses in $form_state['values'] and return an array targeting clauses of each source.
 *
 * @param array $form_state
 * @param array $base_target_path : if provided, only this target will be processed.
 *
 * @return array
 */
function so_listings_filters_clauses_order(&$form_state, $base_target_path = array()) {

    if(empty($base_target_path)) {
        $targets_paths = array(
            array(
                'global_filters',
                'sql_generator',
                'clauses',
            ),
        );

        foreach($form_state['values']['sources_filters'] as $source => $value) {
            $targets_paths[] = array(
                'sources_filters',
                $source,
                'sql_generator',
                'clauses',
            );
        }
    } else {
        $targets_paths = array(array_merge($base_target_path, array(
            'sql_generator',
            'clauses',
        )));
    }

    foreach($targets_paths as $path) {
        $clauses = (array)drupal_array_get_nested_value($form_state['input'], $path);
        $values = (array)drupal_array_get_nested_value($form_state['values'], $path);

        array_walk($clauses, function(&$input, $index) use ($values) {
            $input = $values[$index];
        });

        $clauses = array_filter($clauses);
        $clauses = array_values($clauses);
        drupal_array_set_nested_value($form_state['values'], $path, $clauses);
    }

    return $targets_paths;
}

function _so_listings_filters_clauses_tree(&$clauses) {

    if(empty($clauses)) {return;}

    $depths = array(0 => 0);
    $cur_depth = 0;
    $cur_pid = 0;
    $cur_weight = -50;

    foreach($clauses as &$clause) {
        if($clause['drag_pid'] == $cur_pid) {
            $clause['depth'] = $cur_depth;
        } else {
            $cur_pid = $clause['drag_pid'];

            if(!array_key_exists($clause['drag_pid'], $depths)) {
                $cur_pid = $clause['drag_pid'];
                $depths[$cur_pid] = ++$cur_depth;
            }

            $cur_depth = $depths[$cur_pid];
            $clause['depth'] = $cur_depth;
        }

        $clause['weight'] = $cur_weight++;
    }
}

function _so_listings_filters_arguments_default_element(&$element, $arg) {

    $element = array(
        '#type' => 'textfield',
        '#default_value' => $arg,
        '#disabled' => true,
        '#size' => 30,
    );
}

function _so_listings_filters_arguments_single_element(&$element, $args, $field_infos, $condition_infos, $clause_id) {

    switch($field_infos['widget']) {

        case 'text_textfield':
            $element = array(
                '#type' => 'textfield',
                '#description' => $condition_infos['description'],
                '#size' => 30,
                '#default_value' => $args,
                '#maxlength' => 512,
            );
            break;

        case 'options_buttons':

            $element = array(
                '#type' => 'radios',
                '#options' => list_allowed_values(field_info_field($field_infos['field_name'])),
                '#default_value' => $args,
            );
            break;

        case 'date_popup':

            $element = array(
                '#type' => 'container',

                'select' => array(
                    '#id' => 'date_select_' . $clause_id,
                    '#type' => 'select',
                    '#options' => array(
                        'now' => t("when the user views the page"),
                        'fixed' => t("a fixed date"),
                    ),
                    '#default_value' => (array)$args['select'],
                ),

                'input' => array(
                    '#type' => 'date_popup',
                    '#title' => null,
                    '#date_format' => 'd/m/Y',
                    '#date_year_range' => '-3:+3',
                    '#date_label_position' => 'none',
                    '#default_value' => $args['input'],
                    '#states' => array(
                        'visible' => array(
                            'select#date_select_' . $clause_id => array('value' => 'fixed'),
                        ),
                    ),
                ),
            );
            break;

        case 'number':
            $element = array(
                '#type' => 'textfield',
                '#size' => 10,
                '#default_value' => $args,
            );
            break;

        case 'image_image':
            $element = array(
                '#type' => 'textfield',
                '#field_suffix' => "pixels",
                '#size' => 5,
                '#default_value' => $args,
            );
            break;
    }
}

function _so_listings_filters_arguments_multiple_element(&$element, $args, $field_infos, $condition_infos, $clause_id) {

    switch($field_infos['widget']) {

        case 'text_textfield':
            $element = array(
                '#type' => 'textfield',
                '#description' => $condition_infos['description'],
                '#size' => 70,
                '#default_value' => $args,
                '#maxlength' => 512,
            );
            break;

        case 'options_select':
        case 'options_buttons':

            $element = array(
                '#type' => 'checkboxes',
                '#options' => list_allowed_values(field_info_field($field_infos['field_name'])),
                '#default_value' => (array)$args,
            );
            break;

        case 'number':
            $element = array(
                '#type' => 'textfield',
                '#description' => $condition_infos['description'],
                '#size' => 30,
                '#default_value' => $args,
            );
            break;
    }
}

function _so_listings_filters_arguments_null_element(&$element, $args, $field_infos, $condition_infos, $clause_id) {

    $element = array(
        '#markup' => "&nbsp;",
    );
}