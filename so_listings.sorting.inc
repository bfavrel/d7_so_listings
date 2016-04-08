<?php

/*
 * Implementation of hook_so_listings_sorting()
 */
function so_listings_so_listings_sorting() {

    return array(
        'sol_random_daily' => array(
            'label' => t("Random daily changing order"),
            'clause' => "RAND(CURDATE() + 0)",
            'fields' => array(),
            'join' => null,
            'contexts' => array(),
        ),
        'sol_view_modes' => array(
            'label' => t("Sorting by fiche type (view modes)"),
            'callback' => 'so_listings_so_listings_sorting_vm_callback',
            'cache' => true,
            'contexts' => array(),
        ),
    );
}

function so_listings_so_listings_sorting_vm_callback($lid, $context) {

    $sorting = array(
        'order_by' => "",
        'join' => array(),
    );

    $view_modes = so_listings_get_view_modes_settings('configs');

    array_pop($view_modes);

    if(empty($view_modes)) {return false;}

    so_listings_prepare_query_fragment_data(); // reset du pool de tables

    $query_fragment_data = so_listings_prepare_query_fragment_data('config@so_listings_view_modes');

    $sorting['join'] = $query_fragment_data->joins;

    $clauses = array(
        // vm par défaut - NOTA : array_pop() effectue un reset() sur le tableau, donc key() peut être utilisé pour l'élément suivant.
        "(" . $query_fragment_data->fields[0] . " IS NULL OR " . $query_fragment_data->fields[0] . "='" . key($view_modes) . "') DESC",
    );

    array_shift($view_modes);

    while($key = key($view_modes)) {

        $clauses[] = $query_fragment_data->fields[0] . "='" . $key . "' DESC";

        array_shift($view_modes);
    }

    $sorting['order_by'] = implode(', ', $clauses);

    return $sorting;
}

function so_listings_sorting_settings() {

    $output = array();

    drupal_set_title(t("Sorting"));

    $output[] = drupal_get_form('so_listings_sorting_settings_form');

    $output[] = drupal_get_form('so_listings_sorting_settings_recompile_form');

    return $output;
}

function so_listings_sorting_settings_form($form, &$form_state) {

    $modules_clauses = module_invoke_all('so_listings_sorting');
    $default_clauses = variable_get('sol_sorting_settings', array());

    $clauses_options = array();

    foreach($modules_clauses as $clause_id => $infos) {
        // on ne conserve que les clauses universelles ou limitées au contexte par défaut
        if(array_key_exists('contexts', $infos)
                && !empty($infos['contexts'])
                && !in_array('so_listings_default', $infos['contexts'], true)) {continue;}
        $clauses_options[$clause_id] = $infos['label'];
    }

    asort($clauses_options);

    $form = array(
        '#tree' => true,

        '#theme_wrappers' => array('fieldset', 'form'),

        '#title' => t("Sorting by default"),

        'overview' => array(
            '#type' => 'container',
            '#theme' => 'so_listings_sorting_context_clauses',
            '#id' => 'settings_clauses_overview',

            'clauses' => array(
                '#type' => '#container',
            ),

            'add_clause' => array(
                '#type' => 'container',

                'clause' => array(
                    '#type' => 'select',
                    '#title' => t("Available clauses"),
                ),

                'add' => array(
                    '#type' => 'submit',
                    '#value' => t("Add"),
                    '#submit' => array('so_listings_sorting_settings_form_add_submit'),
                    '#ajax' => array(
                        'callback' => 'so_listings_sorting_context_form_ajax_callback',
                        'wrapper' => 'settings_clauses_overview',
                        'effect' => 'fade',
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

        'save' => array(
            '#type' => 'submit',
            '#value' => t("Save"),
            '#ajax' => array(
                'callback' => 'so_listings_sorting_context_form_ajax_callback',
                'wrapper' => 'settings_clauses_overview',
                'effect' => 'fade',
                'event' => 'click',
                'method' => 'replace',
                'progress' => array(
                    'type' => 'throbber',
                    'message' => null,
                ),
            ),
        ),
    );

    $delta = 50;
    $weight = -$delta;

    foreach($default_clauses as $index => $clause_id) {

        $form['overview']['clauses'][$clause_id] = array(
            'label' => array(
                '#markup' => $modules_clauses[$clause_id]['label'],
            ),

            'weight' => array(
                '#type' => 'weight',
                '#default_value' => $weight++,
                '#delta' => $delta,
            ),

            'delete' => array(
                '#type' => 'submit',
                '#value' => t("Delete"),
                '#submit' => array('so_listings_sorting_settings_form_delete_submit'),
                '#ajax' => array(
                    'callback' => 'so_listings_sorting_context_form_ajax_callback',
                    'wrapper' => 'settings_clauses_overview',
                    'effect' => 'fade',
                    'event' => 'click',
                    'method' => 'replace',
                    'progress' => array(
                        'type' => 'throbber',
                        'message' => null,
                    ),
                ),
            ),
        );

        unset($clauses_options[$clause_id]);
    }

    $form['overview']['add_clause']['clause']['#options'] = $clauses_options;

    return $form;
}

function so_listings_sorting_settings_form_add_submit($form, &$form_state) {

    $default_clauses = variable_get('sol_sorting_settings', array());

    $default_clauses[] = $form_state['values']['overview']['add_clause']['clause'];

    variable_set('sol_sorting_settings', $default_clauses);

    drupal_set_message(t("Clause has been added."));
    $form_state['rebuild'] = true;
}

function so_listings_sorting_settings_form_delete_submit($form, &$form_state) {

    $clause_to_delete = $form_state['triggering_element']['#parents'][2];
    $default_clauses = variable_get('sol_sorting_settings', array());

    unset($default_clauses[array_search($clause_to_delete, $default_clauses)]);

    variable_set('sol_sorting_settings', $default_clauses);

    drupal_set_message(t("Clause has been removed."));
    $form_state['rebuild'] = true;
}

function so_listings_sorting_settings_form_submit($form, &$form_state) {

    $modules_clauses = module_invoke_all('so_listings_sorting');
    $default_clauses = variable_get('sol_sorting_settings', array());

    // on en profite pour vérifier que les clauses précédemment entrées existent toujours
    foreach($default_clauses as $index => $clause_id) {
        if(!array_key_exists($clause_id, $modules_clauses)) {unset($default_clauses[$index]);}
    }

    // ordre des clauses
    $sort = array();
    foreach ((array)$form_state['values']['overview']['clauses'] as $key => $infos) {
        $sort[$key] = $infos['weight'];
    }

    // array_multisort() détruit les indexes numériques
    asort($sort);
    $clauses_sorted = array_values(array_flip($sort)); // réindexation

    variable_set('sol_sorting_settings', $clauses_sorted);

    $listings = so_listings_get_listings_definitions();

    foreach($listings as $definition) {

        // on en profite pour aller formater les stockages antérieurs à l'ajout de la fonctionnalité de tri
        if(!array_key_exists('sorting', $definition->sorting)) {
            $definition->sorting['sorting'] = array_fill_keys(array_keys(so_listings_get_contexts()), 'so_listings_default');
            $definition->sorting['sorting']['so_listings_default'] = 'default';

             $definition->cache['sorting'] = array();
        }

        if(!array_key_exists('so_listings_default', $definition->sorting['sorting']) || $definition->sorting['sorting']['so_listings_default'] == 'default') {

            $modules_clauses = module_invoke_all('so_listings_sorting');

            $definition->cache['sorting']['so_listings_default'] = so_listings_sorting_compile_clauses(
                array('so_listings_default' => $clauses_sorted), // on fabrique un faux tableau
                'so_listings_default',
                array(
                    'custom_clauses' => array(),
                    'modules_clauses' => $modules_clauses,
                ),
                $definition->id
            );
        }

        so_listings_save_listing_definition($definition);
    }

    drupal_set_message(t("Settings has been saved.<br />Listings have been updated."));
    $form_state['rebuild'] = true;
}

function so_listings_sorting_settings_recompile_form($form, &$form_state) {

    $form = array(
        '#theme_wrappers' => array('fieldset', 'form'),

        '#title' => t("Recompile static clauses/callbacks cache"),
        '#description' => t("Clauses and static callbacks results clauses need to be recompiled in cache when params change (ie : view modes).") . "<br />" .
                          t("When a new context is added/activated cache must also be rebuilt."),

        'recompile' => array(
            '#type' => 'submit',
            '#value' => t("Recompile"),
        ),
    );

    return $form;
}

function so_listings_sorting_settings_recompile_form_submit($form, &$form_state) {

    $listings = so_listings_get_listings_definitions();
    $contexts = so_listings_get_contexts(true);

    foreach($listings as $definition) {

        $clauses_pool = so_listings_sorting_load_clauses_pool($definition->id);

        $missing_contexts = array_keys(array_diff_key($contexts, $definition->sorting['sorting']));

        foreach($missing_contexts as $missing_context) {
            $definition->sorting['sorting'][$missing_context] = 'so_listings_default';
        }

        foreach($definition->sorting['sorting'] as $context => $clauses) {

            if(!is_array($clauses)) {

                if($clauses != 'default') {
                    $definition->cache['sorting'][$context] = $clauses;
                }

                continue;
            }

            if(count(array_intersect_key($clauses_pool['modules_clauses'], array_flip($clauses))) == 0) {continue;}

            $definition->cache['sorting'][$context] = so_listings_sorting_compile_clauses(
               $definition->sorting['sorting'],
               $context,
               $clauses_pool,
               $definition->id // pour les callbacks statiques
            );
        }

        so_listings_save_listing_definition($definition);
    }

    drupal_set_message(t("Cache has been rebuilt"));
}

function so_listings_sorting_edit($lid) {

    $output = array();

    $definition = so_listings_get_listings_definitions($lid);
    drupal_set_title(t("Edit listing '@listing'", array('@listing' => $definition->name)));

    $contexts = array_reverse(so_listings_get_contexts(), true); // les plus "spécialisés" à la fin

    $clauses_pool = so_listings_sorting_load_clauses_pool($definition->id);

    if(user_access('administer so_listings advanced')) {
        $output[] = so_listings_sorting_custom_clauses_overview($definition->id, $clauses_pool);
    }

    $contexts_list = array();

    foreach($contexts as $context => $context_infos) {
        $contexts_list[$context] = $context_infos['label'];
    }

    foreach($contexts as $context => $context_infos) {
        $context_infos['context'] = $context;
        $output[] = drupal_get_form('so_listings_sorting_context_form', $lid, $context_infos, $contexts_list, $clauses_pool);
    }

    return $output;
}

function so_listings_sorting_load_clauses_pool($lid) {

    $listings = so_listings_get_listings_definitions();
    $fields_pool = so_listings_get_fields_list($lid);

    $custom_clauses = array();

    foreach($listings as $listing) {

        if(!array_key_exists('custom_clauses', $listing->sorting)) {continue;}

        foreach($listing->sorting['custom_clauses'] as $clause_id => $clause_infos) {

            if(count(array_intersect_key(array_flip($clause_infos['fields']), $fields_pool)) != count($clause_infos['fields'])) {continue;}

            if(!array_key_exists($clause_id, $custom_clauses)) {

                $custom_clauses[$clause_id] = $clause_infos;
                $custom_clauses[$clause_id]['clause_id'] = $clause_id;
                $custom_clauses[$clause_id]['lid'] = $listing->id;
                $custom_clauses[$clause_id]['defined_in'] = array($listing->name);

            } elseif($listing->id == $lid) {

                $custom_clauses[$clause_id]['lid'] = $lid;

            } else {

                $custom_clauses[$clause_id]['defined_in'][] = $listing->name;
            }
        }
    }

    $labels = array();
    foreach ($custom_clauses as $key => $clause) {
        $labels[$key] = $clause['label'];
    }
    natcasesort($labels);

    $custom_clauses_sorted = array();

    // array_multisort() détruit les indexes numériques
    foreach($labels as $key => $value) {
        $custom_clauses_sorted[$key] = $custom_clauses[$key];
    }

    $modules_clauses = module_invoke_all('so_listings_sorting');

    return array(
        'custom_clauses' => $custom_clauses_sorted,
        'modules_clauses' => $modules_clauses,
    );
}

function so_listings_sorting_custom_clauses_overview($lid, $clauses_pool) {

    $destination = drupal_get_destination();

    $output = array(
        '#type' => 'container',
        '#theme_wrappers' => array('fieldset'),
        '#attributes' => array('class' => array('collapsible')),
        '#attached' => array('library' => array('drupal.collapse')),

        '#title' => "- " . t("Custom clauses") . " -",

        'add' => array(
            '#markup' => l("+ " . t("Create a new clause"), 'admin/structure/so_listings/edit/' . $lid . '/sorting/edit_clause/0/0', array('query' => $destination)),
        ),

        'clauses' => array(
            '#type' => 'container',
            '#theme' => 'so_listings_sorting_custom_clauses',
            '#attributes' => array(), // bug D7
        ),
    );

    foreach($clauses_pool['custom_clauses'] as $clause_id => $custom_clause) {

        $output['clauses'][$clause_id] = array(

            'label' => array(
                '#markup' => $custom_clause['label'],
                '#suffix' => " " . theme('image', array(
                    'path' => drupal_get_path('module', 'so_listings') . '/images/help.png',
                    'attributes' => array('title' => $custom_clause['clause']),
                )),
            ),

            'operations' => array(
                '#type' => 'container', // bug D7
                '#attributes' => array(),

                'edit' => array(),

                'delete' => array(),
            ),

            'scope' => array(),
        );

        // la clause existe localement
        if($custom_clause['lid'] == $lid) {

            $output['clauses'][$clause_id]['operations']['edit'] = array(
                '#markup' => l(
                    t("Edit"),
                    'admin/structure/so_listings/edit/' . $custom_clause['lid'] . '/sorting/edit_clause/' . $clause_id . '/0',
                    array('query' => $destination)
                ),
            );

            $output['clauses'][$clause_id]['operations']['delete'] = array(
                '#markup' => l(
                    t("Delete"),

                    'admin/structure/so_listings/edit/' . $custom_clause['lid'] . '/sorting/delete_clause/' . $clause_id,
                    array('query' => $destination)
                ),
            );

            $output['clauses'][$clause_id]['scope'] = array(
                '#markup' => t("Local"),
            );

        } else {
            $output['clauses'][$clause_id]['operations']['delete'] = array(
                '#markup' => l(
                    t("Import"),
                    'admin/structure/so_listings/edit/' . $custom_clause['lid'] . '/sorting/edit_clause/' . $clause_id . '/' . $lid,
                    array('query' => $destination)
                ),
            );

            $output['clauses'][$clause_id]['scope'] = array(
                '#markup' => t("Global"),
                '#suffix' => " " . theme('image', array(
                    'path' => drupal_get_path('module', 'so_listings') . '/images/help.png',
                    'attributes' => array('title' => t("Defined in") . " : " . PHP_EOL . implode(PHP_EOL, $custom_clause['defined_in'])),
                )),
            );
        }
    }

    return $output;
}

function so_listings_sorting_delete_clause($lid, $clause_id) {

    $definition = so_listings_get_listings_definitions($lid);
    $clauses_pool = so_listings_sorting_load_clauses_pool($lid);

    unset($definition->sorting['custom_clauses'][$clause_id]);

    foreach($definition->sorting['sorting'] as $context => $clauses) {

        if(!is_array($clauses)) {continue;}

        $index = array_search($clause_id, $clauses);

        if($index !== false) {
            unset($definition->sorting['sorting'][$context][$index]);
            $definition->cache['sorting'][$context] = so_listings_sorting_compile_clauses($definition->sorting['sorting'], $context, $clauses_pool, $lid);
        }
    }

    so_listings_save_listing_definition($definition);

    drupal_set_message(t("Clause has been locally deleted, and its local usages have been removed"));
    drupal_goto();
}

function so_listings_sorting_context_form($form, &$form_state, $lid, $context_infos, $contexts_list, $clauses_pool) {

    unset($contexts_list[$context_infos['context']]);

    $definition = so_listings_get_listings_definitions($lid);
    $clauses_pool_flat = $clauses_pool['custom_clauses'] + $clauses_pool['modules_clauses'];

    $clauses_options = array();

    foreach($clauses_pool['custom_clauses'] as $clause => $infos) {
        if($infos['lid'] != $lid) {continue;}
        $clauses_options[$clause] = $infos['label'];
    }

    foreach($clauses_pool['modules_clauses'] as $clause => $infos) {

        if(array_key_exists('contexts', $infos) && !empty($infos['contexts']) && !in_array($context_infos['context'], $infos['contexts'], true)) {continue;}

        $clauses_options[$clause] = $infos['label'];
    }

    natcasesort($clauses_options);

    $form = array(
        '#tree' => true,
        '#theme_wrappers' => array('fieldset', 'form'),

        '#title' => t("Sorting in the '@context' context", array('@context' => $context_infos['label'])),

        '#collapsible' => true,
        '#collapsed' => false,
        '#process' => array('form_process_fieldset'),

        'context_op' => array(
            'op' => array(),
            'source' => array(),
        ),

        'overview' => array(
            '#type' => 'container',
            '#theme' => 'so_listings_sorting_context_clauses',
            '#id' => $context_infos['context'] . '_clauses_overview',

            '#states' => array(
                'visible' => array(
                    'select#' . ('op_' . $context_infos['context']) => array('value' => 'custom'),
                ),
            ),

            'clauses' => array(
                '#type' => '#container',
            ),

            'add_clause' => array(
                '#type' => 'container',

                'clause' => array(
                    '#type' => 'select',
                    '#title' => t("Available clauses"),
                ),

                'add' => array(
                    '#type' => 'submit',
                    '#value' => t("Add"),
                    '#submit' => array('so_listings_sorting_context_form_add_submit'),
                    '#ajax' => array(
                        'callback' => 'so_listings_sorting_context_form_ajax_callback',
                        'wrapper' => $context_infos['context'] . '_clauses_overview',
                        'effect' => 'fade',
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

        'save' => array(
            '#type' => 'submit',
            '#value' => t("Publish sorting"),
            '#ajax' => array(
                'callback' => 'so_listings_sorting_context_form_ajax_callback',
                'wrapper' => $context_infos['context'] . '_clauses_overview',
                'effect' => 'fade',
                'event' => 'click',
                'method' => 'replace',
                'progress' => array(
                    'type' => 'throbber',
                    'message' => null,
                ),
            ),
        ),
    );

    $default_values = array(
        'op' => 'instance',
        'source' => 'so_listings_default',
    );

    if(array_key_exists($context_infos['context'], $definition->sorting['sorting'])) {

        if(is_string($definition->sorting['sorting'][$context_infos['context']])) {

            $default_values['source'] = $definition->sorting['sorting'][$context_infos['context']];

        } elseif(is_array($definition->sorting['sorting'][$context_infos['context']])) {

            if(!empty($definition->sorting['sorting'][$context_infos['context']]) || !empty($definition->cache['sorting'][$context_infos['context']])) {

                $default_values['op'] = 'custom';

            } else {

                $default_values['op'] = 'none';
            }
        }
    }

    $form['context_op']['op'] = array(
        '#type' => 'select',
        '#title' => t("Operation"),
        '#options' => array(
            'instance' => t("Use the instance of an existing set"),
            'duplicate' => t("Duplicate an existing set"),
            'custom' => t("Use a custom set"),
            'none' => t("No sorting"),
        ),
        '#default_value' => $default_values['op'],
        '#id' => 'op_' . $context_infos['context'],
    );

    $form['context_op']['source'] = array(
        '#type' => 'select',
        '#title' => t("Context sorting to use"),
        '#options' => $contexts_list,
        '#default_value' => $default_values['source'],
        '#states' => array(
            // workaround bug JQuery 1.7
            'visible' => array(
                'select#' . ('op_' . $context_infos['context']) . ',condition_1' => array('!value' => 'custom'),
                'select#' . ('op_' . $context_infos['context']) . ',condition_2' => array('!value' => 'none'),
                'select#' . ('op_' . $context_infos['context']) . ',condition_3' => array('!value' => 'default'),
            ),
        ),
    );

    if($context_infos['context'] == 'so_listings_default') {

        $form['context_op']['op']['#options'] = array(
            'default' => t("Use default settings"),
            'custom' => t("Use a custom set"),
            'none' => t("No sorting"),
        );

        if(!array_key_exists('so_listings_default', $definition->sorting['sorting'])
                || $definition->sorting['sorting']['so_listings_default'] == 'default') {

            $form['context_op']['op']['#default_value'] = 'default';
        }
    }

    if($form['context_op']['op']['#default_value'] == 'custom') {
        $delta = 50;
        $weight = -$delta;

        foreach((array)$definition->sorting['sorting'][$context_infos['context']] as $index => $clause_id) {

            $form['overview']['clauses'][$clause_id] = array(
                'label' => array(
                    '#markup' => $clauses_pool_flat[$clause_id]['label'],
                ),

                'weight' => array(
                    '#type' => 'weight',
                    '#default_value' => $weight++,
                    '#delta' => $delta,
                ),

                'delete' => array(
                    '#type' => 'submit',
                    '#value' => t("Delete"),
                    '#submit' => array('so_listings_sorting_context_form_delete_submit'),
                    '#ajax' => array(
                        'callback' => 'so_listings_sorting_context_form_ajax_callback',
                        'wrapper' => $context_infos['context'] . '_clauses_overview',
                        'effect' => 'fade',
                        'event' => 'click',
                        'method' => 'replace',
                        'progress' => array(
                            'type' => 'throbber',
                            'message' => null,
                        ),
                    ),
                ),
            );

            unset($clauses_options[$clause_id]);
        }

        $form['overview']['add_clause']['clause']['#options'] = $clauses_options;

    } else {
        $form['overview']['clauses'] = array();
        $form['overview']['add_clause'] = array();
    }

    return $form;
}

function so_listings_sorting_context_form_ajax_callback($form, &$form_state) {
    return $form['overview'];
}

function so_listings_sorting_context_form_add_submit($form, &$form_state) {

    $lid = $form_state['build_info']['args'][0];
    $context = $form_state['build_info']['args'][1]['context'];
    $clauses_pool = $form_state['build_info']['args'][3];
    $clauses_pool_flat = $clauses_pool['custom_clauses'] + $clauses_pool['modules_clauses'];

    $definition = so_listings_get_listings_definitions($lid);

    $added_clause = $form_state['values']['overview']['add_clause']['clause'];

    // on en profite pour vérifier que les clauses précédemment entrées existent toujours
    foreach((array)$definition->sorting['sorting'][$context] as $index => $clause_id) {
        if(!array_key_exists($clause_id, $clauses_pool_flat)) {unset($definition->sorting['sorting'][$context][$index]);}
    }

    $definition->sorting['sorting'][$context][] = $added_clause;

    so_listings_save_listing_definition($definition);

    drupal_set_message(t("Context's sorting has been modified. Modifications will only take effect when published."));

    $form_state['rebuild'] = true;
}

function so_listings_sorting_context_form_delete_submit($form, &$form_state) {

    $lid = $form_state['build_info']['args'][0];
    $context = $form_state['build_info']['args'][1]['context'];
    $definition = so_listings_get_listings_definitions($lid);
    $clause_to_delete = $form_state['triggering_element']['#parents'][2];

    unset($definition->sorting['sorting'][$context][array_search($clause_to_delete, $definition->sorting['sorting'][$context])]);

    so_listings_save_listing_definition($definition);

    drupal_set_message(t("Context's sorting has been modified. Modifications will only take effect when published."));

    $form_state['rebuild'] = true;
}

function so_listings_sorting_context_form_submit($form, &$form_state) {

    $form_state['rebuild'] = true;

    $lid = $form_state['build_info']['args'][0];
    $context = $form_state['build_info']['args'][1]['context'];
    $clauses_pool = $form_state['build_info']['args'][3];

    $definition = so_listings_get_listings_definitions($lid);

    // compilation...
    if($form_state['values']['context_op']['op'] == 'custom') {

        if(!empty($form_state['values']['overview']['clauses'])) {

            // ordre des clauses
            $sort = array();
            foreach ($form_state['values']['overview']['clauses'] as $key => $infos) {
                $sort[$key] = $infos['weight'];
            }

            // array_multisort() détruit les indexes numériques
            asort($sort);
            $clauses_sorted = array_values(array_flip($sort)); // réindexation

            $definition->sorting['sorting'][$context] = $clauses_sorted;

            $sorting_cache = so_listings_sorting_compile_clauses($definition->sorting['sorting'], $context, $clauses_pool, $lid);

        } else {

            $definition->sorting['sorting'][$context] = array();

            $sorting_cache = array(array(
                'order_by' => '',
                'join' => array(),
            ));
        }

    // ...ou duplication...
    } elseif($form_state['values']['context_op']['op'] == 'duplicate') {

        $definition->sorting['sorting'][$context] = $definition->sorting['sorting'][$form_state['values']['context_op']['source']];

        // TODO : Filtrer les clauses qui sont limitées au niveau des contextes

        if(!is_array($definition->sorting['sorting'][$context])) {
            drupal_set_message(t("Nothing to duplicate."), 'warning');
            return;
        }

        $sorting_cache = so_listings_sorting_compile_clauses($definition->sorting['sorting'], $context, $clauses_pool, $lid);

    // ...ou instanciation...
    } elseif($form_state['values']['context_op']['op'] == 'instance') {

        $definition->sorting['sorting'][$context] = $form_state['values']['context_op']['source'];
        $sorting_cache = $form_state['values']['context_op']['source'];

    // ...ou les settings par defaut...
    } elseif($form_state['values']['context_op']['op'] == 'default') {
        $definition->sorting['sorting'][$context] = 'default';

        $default_clauses = variable_get('sol_sorting_settings', array());

        $sorting_cache = so_listings_sorting_compile_clauses(
            array('so_listings_default' => $default_clauses), // on fabrique un faux tableau
            'so_listings_default',
            $clauses_pool,
            $lid // pour les callbacks statiques
        );

    // ...ou enfin pas de tri.
    } elseif($form_state['values']['context_op']['op'] == 'none') {
        $definition->sorting['sorting'][$context] = array();
        $sorting_cache = array(); // array() sinon boucle infinie dans le compilateur de requêtes
    }

    $definition->cache['sorting'][$context] = $sorting_cache;

    so_listings_save_listing_definition($definition);
}


function so_listings_sorting_compile_clauses($raw_clauses, $context, $clauses_pool, $lid = null) {

    $sorting_cache = array();

    $clauses_pool_flat = $clauses_pool['custom_clauses'] + $clauses_pool['modules_clauses'];

    foreach($raw_clauses[$context] as $clause_id) {

        if(array_key_exists('clause', $clauses_pool_flat[$clause_id])) {

            $compiled_clause = array(
                'order_by' => '',
                'join' => array(),
            );

            // la clause a besoin d'être parsée
            if(array_key_exists('fields', $clauses_pool_flat[$clause_id]) && !empty($clauses_pool_flat[$clause_id]['fields'])) {

                $order_by = $clauses_pool_flat[$clause_id]['clause'];

                $matches = array();
                preg_match_all("#(field_[a-z0-9_]+)_([a-z0-9]+)#", $clauses_pool_flat[$clause_id]['clause'], $matches, PREG_SET_ORDER);

                foreach($matches as $field_infos) {

                    $query_fragment_data = so_listings_prepare_query_fragment_data($field_infos[1] . '@field');

                    $order_by = str_replace($field_infos[0], $query_fragment_data->fields[$field_infos[2]], $order_by);

                    $compiled_clause['join'][] = $query_fragment_data->joins[0];
                }
                $compiled_clause['order_by'] = $order_by;

            } else {

                $compiled_clause['order_by'] = $clauses_pool_flat[$clause_id]['clause'];

                if(array_key_exists('join', $clauses_pool_flat[$clause_id]) && !empty($clauses_pool_flat[$clause_id]['join'])) {
                    $compiled_clause['join'] = $clauses_pool_flat[$clause_id]['join'];
                }
            }

            $sorting_cache[] = $compiled_clause;

        // callback cachable
        } elseif(array_key_exists('cache', $clauses_pool_flat[$clause_id]) && $clauses_pool_flat[$clause_id]['cache'] == true) {

            $function = $clauses_pool_flat[$clause_id]['callback'];
            $return = $function($lid, $context);

            if(!empty($return)) {
                $sorting_cache[] = $return;
            }

        // callback non cachable
        } else {
            $sorting_cache[] = $clauses_pool_flat[$clause_id]['callback'];
        }
    }

    return $sorting_cache;
}

function so_listings_sorting_edit_clause_form($form, &$form_state, $lid, $clause_id = null, $target_lid = null) {// $target_lid est traité dans le submit

    if(!empty($clause_id)) {
        $definition = so_listings_get_listings_definitions($lid);

        $defaults_values = array(
            'label' => $definition->sorting['custom_clauses'][$clause_id]['label'],
            'clause' => $definition->sorting['custom_clauses'][$clause_id]['clause'],
        );

    } else {

        $defaults_values = array(
            'label' => '',
            'clause' => '',
        );
    }

    drupal_set_title(t("Edit a sorting clause"));

    $num_sources = 0;
    $fields_pool = so_listings_get_fields_list($lid, "(*) ", $num_sources);

    $fields_helper_options = array(0 => "- " . t("Select a field") . " -") + $fields_pool;

    $form = array(
        '#tree' => true,
        '#attached' => array(
            'css' => array(drupal_get_path('module', 'so_listings') . '/css/so_listings_admin.css'),
            'js' => array(
                'misc/ajax.js', // @see 'save'.#ajax
                drupal_get_path('module', 'so_listings') . '/js/so_listings_admin.js',
            ),
        ),

        'label' => array(
            '#type' => 'textfield',
            '#title' => t("Clause name"),
            '#description' => t("Clause direction (ascendant/descendant) should be explicitly mentioned"),
            '#required' => true,
            '#default_value' => $defaults_values['label'],
        ),

        'helper' => array(
            '#type' => 'container',
            '#id' => 'sol_sorting_fields_helper_wrapper',

            'fields_pool' => array(
                '#type' => 'select',
                '#id' => 'sol_sorting_fields_helper_pool',
                '#title' => t("Fields helper"),
                '#options' => $fields_helper_options,
            ),

            'lid' => array(
                '#type' => 'hidden',
                '#attributes' => array('id' => 'so_listings_id'),
                '#value' => $lid,
            ),

            'fields_helper' => array(
                '#type' => 'container',
                '#id' => 'sol_sorting_fields_helper',
                '#attributes' => array('style' => 'display: none;'),
            ),
        ),

        'clause' => array(
            '#type' => 'textarea',
            '#title' => t("Clause"),
            '#description' => t("No 'ORDER BY' prefix. Several clauses can be chained, as in a normal SQL 'ORDER BY' section."),
            '#wysiwyg' => 'false',
            '#default_value' => $defaults_values['clause'],
            '#required' => true,
        ),

        'save' => array(
            '#type' => 'submit',
            '#value' => t("Save"),
// BUG : ne m'affiche pas les drupal_message() (@see #attached)
//            '#ajax' => array(
//                'callback' => 'so_listings_sorting_edit_clause_form_ajax_callback',
//                'wrapper' => 'so-listings-sorting-add-clause-form',
//                'effect' => 'fade',
//                'event' => 'click',
//                'method' => 'replace',
//                'progress' => array(
//                    'type' => 'throbber',
//                    'message' => null,
//                ),
//            ),
        ),

        'back' => array(
            '#markup' => l(t("Back"), $_GET['destination']),
        ),
    );

    if($num_sources > 1) {
        $form['helper']['fields_pool']['#description'] = t("(*) : field shared by whole sources");
    }

    return $form;
}

function so_listings_sorting_edit_clause_form_ajax_callback($form, &$form_state) {
    return $form;
}

function so_listings_sorting_edit_clause_form_submit($form, &$form_state) {

    // target lid
    $lid = !empty($form_state['build_info']['args'][2]) ? $form_state['build_info']['args'][2] : $form_state['build_info']['args'][0];

    $definition = so_listings_get_listings_definitions($lid);

    $clause = $form_state['values']['clause'];
    $old_clause_id = $form_state['build_info']['args'][1];
    $new_clause_id = substr(md5($clause), 0, 8);

    $matches = array();
    preg_match_all("#(field_[a-z0-9_]+)_[a-z0-9_]+#", $clause, $matches, PREG_SET_ORDER);

    $fields = array();

    foreach($matches as $match) {
        $fields[] = $match[1];
    }

    unset($definition->sorting['custom_clauses'][$old_clause_id]);

    $definition->sorting['custom_clauses'][$new_clause_id] = array(
        'label' => $form_state['values']['label'],
        'clause' => $clause,
        'fields' => array_unique($fields),
    );

    $contexts_infos = so_listings_get_contexts();

    foreach($definition->sorting['sorting'] as $context => $clauses) {

        if(!is_array($clauses)) {continue;}

        $index = array_search($old_clause_id, $clauses);
        if($index === false) {continue;}

        $definition->sorting['sorting'][$context][$index] = $new_clause_id;

        $definition->cache['sorting'][$context] = so_listings_sorting_compile_clauses(
            $definition->sorting['sorting'],
            $context,
            array(
                'custom_clauses' => $definition->sorting['custom_clauses'],
                'modules_clauses' => array(),
            ),
            $lid
        );

        drupal_set_message(t("Sorting in the '@context' context has been updated", array('@context' => $contexts_infos[$context]['label'])));
    }

    so_listings_save_listing_definition($definition);

    drupal_set_message(t("Clause has been saved"));// BUG : marche pas avec #ajax (???)
}

function so_listings_sorting_ajax_field($lid, $field_name) {

    module_load_include('inc', 'so_listings', 'so_listings.search');

    $output = "";

    $field_definition = field_info_field($field_name);
    $mapping = so_listings_search_map_fields_types($field_definition['type']);

    $output .= "<span>" . t("Column(s)") . " : </span><br />";

    if(!empty($mapping)) {

        $columns = array_intersect_key($field_definition['storage']['details']['sql']['FIELD_LOAD_CURRENT']['field_data_' . $field_name], array_flip($mapping['columns']));

    } else {

        $columns = $field_definition['storage']['details']['sql']['FIELD_LOAD_CURRENT']['field_data_' . $field_name];
    }

    $output .= implode('<br />', $columns);

    $values = array();

    if(array_key_exists('allowed_values', $field_definition['settings'])) {

        foreach($field_definition['settings']['allowed_values'] as $val => $label) {
            $values[] = $val . " => " . $label;
        }

    } elseif($field_definition['type'] == 'text') {

        // on ré-utilise la mécanique so_listings qui fournit les valeurs à so_forms
        $values = array_filter(so_listings_search_field_values(
            array(
                'field_name' => $field_name,
                'field_context' => $lid,
                'data' => array('columns' => $mapping['columns']),
            ),
            null,
            null
        ));

    }

    if(!empty($values)) {
        $output .= "<br /><span>" . t("Values(s)") . " : </span>";
        $output .= "<div id='fields_helper_values_wrapper'>" . implode("<br />", $values) . "</div>";
    }

    return drupal_json_output($output);
}