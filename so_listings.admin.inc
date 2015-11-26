<?php

/*
 * The global module's settings
 */
function so_listings_settings_form($form, &$form_state) {

    drupal_set_title(t("Listings general settings"));

    $settings = variable_get('sol_settings', array());

    $form = array(
        '#tree' => true,
        '#theme_wrappers' => array('fieldset', 'form'),

        'settings' => array(),
    );

    $nodes_types = node_type_get_types();
    $nodes_types_options = array();

    foreach ($nodes_types as $type => $infos) {
        $nodes_types_options[$type] = $infos->name;
    }

    $form['settings']['nodes_types_wrapper'] = array(
        '#type' => 'fieldset',
        '#title' => t("Allowed nodes types in listings"),
        '#collapsible' => true,
        '#collapsed' => true,

        'nodes_types' => array(
            '#type' => 'checkboxes',
            '#title' => null,
            '#options' => $nodes_types_options,
            '#default_value' => (array)$settings['nodes_types'],
            '#parents' => array('settings', 'nodes_types'),
        ),
    );

    $form['settings']['search_forms'] = array(
        '#type' => 'fieldset',
        '#title' => t("Search forms default parameters"),
        '#description' => t("This settings will only affect new listings. For existing ones, settings have to be set manually in each individual form's parameters."),
        '#collapsible' => true,
        '#collapsed' => true,

        'buttons' => array(
            'submit' => array(
                '#type' => 'textfield',
                '#title' => t("Submit buttons' label"),
                '#description' => t("In english (it will be automatically translated at display)"),
                '#default_value' => array_key_exists('search_forms', $settings) ? $settings['search_forms']['buttons']['submit'] : "Search",
                '#size' => 25,
            ),

            'reset' => array(
                '#type' => 'textfield',
                '#title' => t("Reset buttons' label"),
                '#description' => t("In english (it will be automatically translated at display)"),
                '#default_value' => array_key_exists('search_forms', $settings) ? $settings['search_forms']['buttons']['reset'] : "Reset",
                '#size' => 25,
            ),
        ),

        'memorize' => array(
            '#type' => 'checkbox',
            '#title' => t("Store values in user's session ?"),
            '#default_value' => array_key_exists('search_forms', $settings) ? $settings['search_forms']['memorize'] : true,
        ),

        'url_key' => array(
            '#type' => 'checkbox',
            '#title' => t("Store values in database and provide an URL key ?"),
            '#default_value' => array_key_exists('search_forms', $settings) ? $settings['search_forms']['url_key'] : false,
        ),

        'advanced' => array(
            '#type' => 'checkbox',
            '#title' => t("Use the 'advanced' form region ?"),
            '#default_value' => array_key_exists('search_forms', $settings) ? $settings['search_forms']['advanced'] : true,
        ),

        'classes' => array(
            '#type' => 'textfield',
            '#title' => t("Classes to apply to the forms"),
            '#description' => t("Separated by commas."),
            '#default_value' => array_key_exists('search_forms', $settings) ? $settings['search_forms']['classes'] : 'so_listings_search_form',
            '#size' => 60,
        ),

        'overrides' => array(
            '#type' => 'fieldset',
            '#title' => t("Override parameters of existing forms"),
            '#description' => t("Prior to apply overridding, configuration has to be saved first."),
            '#collapsible' => true,
            '#collapsed' => true,

            'params' => array(
                '#type' => 'checkboxes',
                '#title' => t("Parameters to override"),
                '#options' => array(
                    'buttons' => t("Buttons' labels"),
                    'memorize' => t("Users' sessions values storage mode"),
                    'url_key' => t("Database values storage mode and URL key provided"),
                    'advanced' => t("'advanced' form region usage"),
                    'classes' => t("Forms' classes"),
                ),
            ),

            'override' => array(
                '#type' => 'submit',
                '#value' => t("Override"),
                '#submit' => array('so_listings_settings_form_override_submit'),
            ),
        ),
    );

    $form['previous_listing_display_types'] = array(
        '#type' => 'value',
        '#value' => $settings['listing_display_types'],
    );

    $form['settings']['listing_display_types'] = array(
        '#type' => 'textfield',
        '#title' => t("Available display types"),
        '#description' => t("Comma separated. Must only contain : [a-z0-9_-]<br />" .
                            "Used to define templates of type 'listing--[display type].tpl.php'.<br />" .
                            "These values are available for each preset.<br />" .
                            "The default one ('default') match the template 'listing.tpl.php'."),
        '#default_value' => $settings['listing_display_types'],
        '#size' => 100,
        '#maxlength' => 512,
    );

    $form['settings']['global_switcher'] = array(
        '#type' => 'checkbox',
        '#title' => t("Global presets switcher"),
        '#description' => t("If checked user preferences for a listing display will be reflected in all listings all over the site (since the selected preset is available for a given listing)."),
        '#default_value' => $settings['global_switcher'],
    );

    $form['settings']['debug'] = array(
        '#type' => 'checkbox',
        '#title' => t("Debug mode"),
        '#description' => t("Display SOL queries and results in Drupal messages zone."),
        '#default_value' => $settings['debug'],
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t("Save configuration"),
    );

    return $form;
}

function so_listings_settings_form_submit($form, &$form_state) {

    unset($form_state['values']['settings']['search_forms']['overrides']);

    variable_set('sol_settings', $form_state['values']['settings']);

    if($form_state['values']['settings']['listing_display_types'] != $form_state['values']['previous_listing_display_types']) {
        drupal_theme_rebuild();
    }

    drupal_set_message(t("Configuration has been saved."));
}

function so_listings_settings_form_override_submit($form, &$form_state) {

    $overrided_params = array_filter($form_state['values']['settings']['search_forms']['overrides']['params']);

    $results = db_query("SELECT * FROM {so_forms_displays} WHERE sfid LIKE 'so_listings:%@search'");

    while($row = $results->fetchObject()) {

        $row->configuration = unserialize($row->configuration);

        foreach($overrided_params as $param) {
            $row->configuration[$param] = $form_state['values']['settings']['search_forms'][$param];
        }

        $row->configuration = serialize($row->configuration);

        drupal_write_record('so_forms_displays', $row, array('sfid', 'display'));
    }

    drupal_set_message(t("Search forms' parameters have been overrided."));
}

function so_listings_contexts_settings($form, &$form_state) {

    drupal_set_title(t("Contexts"));

    $settings = so_listings_get_contexts(false);

    $modules_contexts = module_invoke_all('so_listings_contexts');

    $contexts = array_intersect_key($settings, $modules_contexts) + $modules_contexts;

    $form = array(
        '#tree' => true,

        'overview' => array(
            '#type' => 'container',

            '#title' => t("Activation and priorities"),
            '#description' => t("Higher priorities on top.<br />IMPORTANT : new contexts are discarded until this form is submitted."),

            '#theme' => 'so_listings_contexts_overview',

            'active' => array(
                '#type' => 'checkboxes',
                '#title' => null,
                '#options' => array(),
                '#default_value' => array('so_listings_default'),
            ),

            'contexts' => array(),
        ),

        'save' => array(
            '#type' => 'submit',
            '#value' => t("Save"),
        ),
    );

    $delta = 50;
    $weight = -$delta;

    foreach($contexts as $context => $infos) {
        $form['overview']['contexts'][$context] = array(
            'weight' => array(
                '#type' => 'weight',
                '#default_value' => $weight++,
                '#delta' => $delta,
            ),

            'label' => array(
                '#type' => 'textfield',
                '#title' => null,
                '#value' => $infos['label'],
            ),
        );

        $form['overview']['active']['#options'][$context] = $infos['label'];

        if($infos['active'] == true) {
            $form['overview']['active']['#default_value'][] = $context;
        }
    }

    return $form;
}

function so_listings_contexts_settings_submit($form, &$form_state) {

    $contexts = $form_state['values']['overview']['contexts'];

    $modules_contexts = module_invoke_all('so_listings_contexts');

    $weights = array();
    foreach ($contexts as $key => $context) {
        $weights[$key] = $context['weight'];
        $contexts[$key]['active'] = !empty($form_state['values']['overview']['active'][$key]);
        $contexts[$key]['label'] = $modules_contexts[$key]['label'];
    }
    array_multisort($weights, SORT_ASC, $contexts);

    variable_set('sol_contexts_settings', $contexts);

    drupal_set_message("Contexts order has been saved");
}

function so_listings_view_modes_form($form, &$form_state) {

    drupal_set_title(t("View modes"));

    $settings = so_listings_get_view_modes_settings();

    $node_entity_infos = entity_get_info('node');
    $node_view_modes = $node_entity_infos['view modes'];

    $view_modes_pool = array();

    foreach($node_view_modes as $view_mode => $infos) {
        $view_modes_pool[$view_mode] = ucfirst($infos['label']);
    }

    asort($view_modes_pool);

    $fiche_vm_options = $view_modes_pool;

    if($form_state['submitted'] == false) {
        $listing_vm_options = array_diff_key($view_modes_pool, $settings['configs']);
    } elseif(array_key_exists('configs', $form_state['input']['view_modes_overview'])) {
        $listing_vm_options = array_diff_key($view_modes_pool, $form_state['input']['view_modes_overview']['configs']); // @see note #limit_validation_errors
    } else {
        $listing_vm_options = $view_modes_pool;
    }

    // le mode 'full' est utilisé pour détecter les node en mode pleine page dans so_listings_entity_view_mode_alter().
    if(array_key_exists('full', $fiche_vm_options)) {
        unset($listing_vm_options['full']);
    }

    $form = array(
        '#tree' => true,
        '#attached' => array(
            'css' => array(drupal_get_path('module', 'so_listings') . '/css/so_listings_admin.css'),
            'js' => array(drupal_get_path('module', 'so_listings') . '/js/so_listings_admin.js'),
        ),

        'view_modes_overview' => array(
            '#type' => 'container',
            '#theme' => 'so_listings_view_modes_overview',
            '#id' => 'so-listings-view-modes-overview',

            'configs' => array(),

            'default_view_modes' => array(),

            'add_listing_config' => array(

                'new_config_label' => array(
                    '#type' => 'textfield',
                    '#title' => t("New config label"),
                    '#size' => 15,
                    '#required' => true,
                ),

                'new_config_id' => array(
                    '#type' => 'machine_name',
                    '#machine_name' => array(
                        'source' => array('view_modes_overview', 'add_listing_config', 'new_config_label'),
                        'exists' => 'so_listings_vm_config_id_exists',
                        'standalone' => true,
                    ),
                    '#required' => true,
                ),

                'add' => array(
                    '#type' => 'submit',
                    '#value' => t("Add"),
                    '#submit' => array('so_listings_view_modes_add_submit'),
                    '#ajax' => array(
                        'callback' => 'so_listings_view_modes_ajax_callback',
                        'wrapper' => 'so-listings-view-modes-overview',
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

        'navigation' => array(
            '#type' => 'actions',

            'save' => array(
                '#type' => 'submit',
                '#value' => t("Save"),
                '#limit_validation_errors' => array(),
                '#submit' => array('so_listings_view_modes_form_submit'),
            ),
        ),
    );

    if($form_state['submitted'] == false) {
        $view_modes_configs = $settings['configs'];
    } elseif(array_key_exists('configs', $form_state['input']['view_modes_overview'])) {
        $view_modes_configs = array_merge(
            $form_state['input']['view_modes_overview']['configs'], // @see note #limit_validation_errors
            (array)$form_state['values']['view_modes_overview']['configs']
        );

    } else {
        $view_modes_configs = array();
    }

    $fiche_vm_options = array('404' => "- " . t("None (404)") . " -") + $fiche_vm_options;

    $pictos_pool = file_scan_directory(drupal_get_path('module', 'so_listings') . '/images/view_modes', '#.*#');

    $vm_pictos_options = array();

    foreach($pictos_pool as $infos) {
        $vm_pictos_options[$infos->uri] = $infos->filename;
    }

    asort($vm_pictos_options);

    $weight = 0;

    $presets_settings = so_listings_get_presets();
    $presets = array();

    foreach($presets_settings as $id => $preset_settings) {
        if(!empty($preset_settings->params['so_listings']['nodes_view_mode'])) {continue;} // "" = "- Use mode defined in each node -" option

        $presets[$id] = t($preset_settings->label);
    }

    foreach ($view_modes_configs as $config => $params) {

        $vm_listing = array();

        foreach($presets as $preset => $preset_label) {
            $vm_listing[$preset] = array(
                '#type' => 'select',
                '#title' => t("Preset : @preset", array('@preset' => $preset_label)),
                '#options' => $listing_vm_options,
                '#default_value' => array_key_exists($preset, (array)$params['vm_listing']) ? $params['vm_listing'][$preset] : 'teaser',
            );
        }

        if(!empty($vm_listing)) {
            $vm_listing['#type'] = 'container';
        } else {
            $vm_listing['#markup'] = "- " . t("Predefined in presets") . " -";
        }

        $form['view_modes_overview']['configs'][$config] = array(

            'config_label' => array(
                '#type' => 'textfield',
                '#title' => null,
                '#size' => 15,
                '#required' => true,
                '#default_value' => $params['config_label'],
            ),

            'id' => array(
                '#type' => 'hidden',
                '#value' => $params['id'],
            ),

            'vm_listing' => $vm_listing,

            'vm_fiche' => array(
                '#type' => 'select',
                '#title' => null,
                '#options' => $fiche_vm_options,
                '#default_value' => array($params['vm_fiche']),
            ),

            'picto_path' => array(
                '#type' => 'select',
                '#title' => null,
                '#options' => $vm_pictos_options,
                '#attributes' => array('class' => array('so_listings_vm_picto_path')),
                '#default_value' => array($params['picto_path']),
                '#field_suffix' => "", // placeholder pour le display du picto
            ),

            'delete' => array(
                '#type' => 'submit',
                '#value' => t("Delete"),
                '#name' => 'delete_' . $config,
                '#submit' => array('so_listings_view_modes_delete_submit'),
                '#limit_validation_errors' => array(),  // la suppression ne passe pas par la validation, mais s'accompagne de la
                                                        // non-transmission des autres configs dans $fs['values'].
                                                        // En revanche, elles sont passées dans $fs['input'].
                '#ajax' => array(
                    'callback' => 'so_listings_view_modes_ajax_callback',
                    'wrapper' => 'so-listings-view-modes-overview',
                    'effect' => 'fade',
                    'event' => 'click',
                    'method' => 'replace',
                    'progress' => array(
                        'type' => 'throbber',
                        'message' => null,
                    ),
                ),
            ),

            'weight' => array(
                '#type' => 'weight',
                '#title' => null,
                '#default_value' => $weight++,
                '#attributes' => array('class' => array('draggable_weight')),
            ),
        );

        if($config == 'default') {
            $form['view_modes_overview']['configs'][$config]['delete'] = array(
                '#markup' => t("Hard coded"),
            );
        }
    }

    // on ne traite que les sources de données de so_listing, qui ne retourne qu'un unique groupe
    // dont les sources sont indexées par nom machine du node type :
    $datasource = so_listings_so_listings();
    $allowed_nodes_types = $datasource[0]['sources'];

    $nodes_types_vm_options_pool = array();

    $view_modes_default_configs = array_merge(array('disabled'), array_keys($view_modes_configs));

    if(!empty($allowed_nodes_types)) {
        foreach($allowed_nodes_types as $node_type => $infos) {

            if($form_state['submitted'] == false) {
                $default = array_key_exists($node_type, $settings['default_view_modes']) ? $settings['default_view_modes'][$node_type] : null;
            } else {
                $default = array_key_exists($node_type, $form_state['input']['view_modes_overview']['default_view_modes']) ? $form_state['input']['view_modes_overview']['default_view_modes'][$node_type] : null;
            }

            $form['view_modes_overview']['default_view_modes'][$node_type] = array(
                '#type' => 'radios',
                '#title' => null,
                '#options' => array(),
                '#default_value' => $default,
            );

            $nodes_types_vm_options_pool[$node_type] = array();

            foreach($view_modes_default_configs as $config) {
                $form['view_modes_overview']['default_view_modes'][$node_type]['#options'][$config] = $infos['label'];
            }
        }
    }

    return $form;
}

function so_listings_vm_config_id_exists($id) {
    $settings = so_listings_get_view_modes_settings('configs');

    if(array_key_exists($id, $settings)) {
        return true;
    } else {
        return false;
    }
}

function so_listings_view_modes_ajax_callback($form, &$form_state) {
    return $form['view_modes_overview'];
}

function so_listings_view_modes_add_submit($form, &$form_state) {

    $new_config_id = $form_state['values']['view_modes_overview']['add_listing_config']['new_config_id'];
    $new_config_label = $form_state['values']['view_modes_overview']['add_listing_config']['new_config_label'];

    $form_state['input']['view_modes_overview']['add_listing_config'] = array();

    $form_state['input']['view_modes_overview']['configs'][$new_config_id] = array(
        'id' => $new_config_id,
        'config_label' => ucfirst($new_config_label),
        'vm_listing' => array(),
        'vm_fiche' => 'full',
        'weight' => count($form_state['input']['view_modes_overview']['configs']),
    );

    $form_state['rebuild'] = true;
}

function so_listings_view_modes_delete_submit($form, &$form_state) {

    array_pop($form_state['triggering_element']['#parents']);
    $param = array_pop($form_state['triggering_element']['#parents']);

    unset($form_state['values']['view_modes_overview']['configs'][$param]);
    unset($form_state['input']['view_modes_overview']['configs'][$param]);

    foreach($form_state['input']['view_modes_overview']['default_view_modes'] as $key => $value) {
        if($value == $param) {
            unset($form_state['input']['view_modes_overview']['default_view_modes'][$key]);
        }
    }

    $form_state['rebuild'] = true;
}

function so_listings_view_modes_form_submit($form, &$form_state) {

    if(!array_key_exists('configs', $form_state['input']['view_modes_overview'])) {
        $configs = array();
    } else {
        $configs = $form_state['input']['view_modes_overview']['configs'];
    }

    $weight = array();
    foreach ($configs as $key => $params) {
        $weight[$key] = $params['weight'];
    }
    array_multisort($weight, SORT_ASC, $configs);

    $variable = array(
        'configs' => $configs,
        'default_view_modes' => array_filter((array)$form_state['input']['view_modes_overview']['default_view_modes']),
    );

    variable_set('so_listings_view_modes', $variable);
}

function so_listings_presets_overview_form($form, &$form_state) {

    drupal_set_title(t("Listings presets"));

    $presets = so_listings_get_presets();

    $form = array(
        '#tree' => true,

        'presets_overview' => array(
            '#type' => 'container',
            '#theme' => 'so_listings_presets_overview',
        ),
    );

    foreach($presets as $preset) {
        $form['presets_overview'][] = array(
            'label' => array('#markup' => t($preset->label)),
            'description' => array('#markup' => t($preset->description)),
            'id' => array('#markup' => $preset->id),
            'template' => array('#markup' => $preset->params['so_listings']['listing_display_type'] == 'default' ? "listing.tpl.php" : "listing--" . $preset->params['so_listings']['listing_display_type'] . ".tpl.php"),
            'operations' => array(
                'edit' => array('#markup' => l(t("configure"), 'admin/config/so_listings/setting/preset/edit/' . $preset->id, array('query' => drupal_get_destination()))),
                'delete' => $preset->id == 'default' ? '' : array('#markup' => l(t("delete"), 'admin/config/so_listings/setting/preset/delete/' . $preset->id, array('query' => drupal_get_destination()))),
            ),
        );
    }

    $pattern_options = array();

    foreach($presets as $preset) {
        $pattern_options[$preset->id] = t($preset->label);
    }

    $form['new_preset'] = array(
        '#type' => 'container',
        '#theme' => 'so_listings_new_preset',

        'label' => array(
            '#type' => 'textfield',
            '#title' => t("Name"),
            '#required' => true,
        ),

        'id' => array(
            '#type' => 'textfield',
            '#title' => t("Machine name"),
            '#required' => true,
        ),

        'pattern' => array(
            '#type' => 'select',
            '#title' => t("Based on preset"),
            '#description' => t("Preset to copy parameters from."),
            '#options' => $pattern_options,
        ),

        'add' => array(
            '#type' => 'submit',
            '#value' => t("Add"),
        ),
    );

    return $form;
}

function so_listings_presets_overview_form_validate($form, &$form_state) {

    $preset = so_listings_get_presets($form_state['values']['new_preset']['id']);

    if(!empty($preset)) {
        form_set_error('new_preset][id', t("Preset id '@id' already exists.", array('@id' => $preset->id)));
    }
}

function so_listings_presets_overview_form_submit($form, &$form_state) {

    $preset = so_listings_get_presets($form_state['values']['new_preset']['pattern']);

    $preset->id = $form_state['values']['new_preset']['id'];
    $preset->label = $form_state['values']['new_preset']['label'];
    $preset->params = serialize($preset->params);

    drupal_write_record('so_listings_presets', $preset);

    drupal_set_message(t("Preset created.<br />It has to be configured now :"), 'status');
    drupal_goto('admin/config/so_listings/setting/preset/edit/' . $preset->id);
}

function so_listings_edit_preset_form($form, &$form_state, $id) {

    $preset = so_listings_get_presets($id);
    drupal_set_title(t("'@preset' preset configuration", array('@preset' => $preset->label)));

    $form = array(
        '#tree' => true,
        '#theme_wrappers' => array('fieldset', 'form'),

        'label' => array(
            '#type' => 'textfield',
            '#title' => t("Name"),
            '#description' => t("Must be in english. Will be processed by t() function later."),
            '#default_value' => $preset->label,
            '#required' => true,
        ),

        'description' => array(
            '#type' => 'textarea',
            '#title' => t("A short description of preset's features. These informations will be displayed as widget's display modes options."),
            '#description' => t("Must be in english. Will be processed by t() function later."),
            '#default_value' => $preset->description,
            '#required' => true,
        ),

        'params' => array(
            '#type' => 'fieldset',
            '#title' => t("Compilator settings"),

            'limit_num' => array(
                '#type' => 'textfield',
                '#title' => t("Number of results by page"),
                '#description' => t("'0' = all"),
                '#size' => 2,
                '#required' => true,
                '#default_value' => $preset->params['limit_num'],
            ),

            'limit_to' => array(
                '#type' => 'textfield',
                '#title' => t("Number maximum of results"),
                '#description' => t("'0' = all"),
                '#size' => 2,
                '#required' => true,
                '#default_value' => $preset->params['limit_to'],
            ),

            'search' => array(
                '#type' => 'checkbox',
                '#title' => t("Listing is filtered by search engine"),
                '#default_value' => $preset->params['search'],
            ),

            'sorting' => array(
                '#type' => 'checkbox',
                '#title' => t("Listing use sorting"),
                '#default_value' => $preset->params['sorting'],
            ),
        ),
    );

    $modules = module_implements('so_listings_preset');

    foreach($modules as $module) {
        $form['params'][$module] = array(
            '#type' => 'fieldset',
            '#title' => $module,
        );

        $function = $module . "_so_listings_preset";
        $function($form['params'][$module], $preset->params[$module]);
    }

    $form['inform_modules'] = array(
        '#type' => 'checkbox',
        '#title' => t("Inform modules that preset has changed"),
        '#default_value' => 0,
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t("Save"),
    );

    $form['back'] = array(
        '#markup' => l(t("Back"), 'admin/config/so_listings/settings/presets'),
    );

    return $form;
}

function so_listings_edit_preset_form_submit($form, &$form_state) {

    $preset = array(
        'id' => $form_state['build_info']['args'][0],
        'label' => $form_state['values']['label'],
        'description' => $form_state['values']['description'],
        'params' => $form_state['values']['params'],
    );

    if($form_state['values']['inform_modules'] == 1) {
        module_invoke_all('so_listings_preset_changed', $preset['id'], $preset);
    }

    $preset['params'] = serialize($preset['params']);

    drupal_write_record('so_listings_presets', $preset, 'id');

    drupal_set_message(t("Preset configuration has been saved."));
    drupal_goto('admin/config/so_listings/settings/presets');
}

function so_listings_delete_preset_confirm($form, &$form_state, $id) {

    $preset = so_listings_get_presets($id);

    return confirm_form(
        array(),
        t("Are you sure you want to delete the preset '@preset' ?", array('@preset' => $preset->label)),
        null,
        null,
        t("Delete")
    );
}

function so_listings_delete_preset_confirm_submit($form, &$form_state) {

    $preset = so_listings_get_presets($form_state['build_info']['args'][0]);

    db_query("DELETE FROM {so_listings_presets} WHERE id=?", array($preset->id));

    module_invoke_all('so_listings_preset_changed', $preset->id, null);

    drupal_set_message(t("Preset '@preset' has been deleted", array('@preset' => $preset->label)), 'status');
    drupal_goto();
}

function so_listings_overview_form($form, &$form_state) {

    if(array_key_exists('values', $form_state)) {

        $default_values = $form_state['values']['filters'];

    } elseif(array_key_exists('so_listings_overview_filters', $_SESSION)) {

        $default_values = $_SESSION['so_listings_overview_filters'];

    } else {
        $default_values = array();
    }

    $names_results = db_query("SELECT DISTINCT name FROM {so_listings} ORDER BY name");

    $prefixes_options = array(
        '' => "- " . t("All") . " -",
        'no_prefix' => "- " . t("Without prefix") . " -",
    );

    while($name = $names_results->fetchColumn()) {

        $matches = array();
        preg_match("#^\[[^\]]+\]#", $name, $matches);

        if(empty($matches[0])) {continue;}

        $prefixes_options[$matches[0]] = $matches[0];
    }

    $form = array(
        '#tree' => true,
        '#attached' => array(
            'css' => array(drupal_get_path('module', 'so_listings') . '/css/so_listings_admin.css')
        ),

        'filters' => array(
            '#type' => 'fieldset',
            '#title' => t("Filters"),
            '#attributes' => array('class' => array('so_listings_overview_filter')),
            '#collapsible' => true,
            '#collapsed' => !empty($default_values) ? false: true,

            'prefix' => array(
                '#type' => 'select',
                '#title' => t("By prefix"),
                '#options' => $prefixes_options,
                '#default_value' => array_key_exists('prefix', $default_values) ? $default_values['prefix'] : '',
            ),

            'tags' => array(
                '#type' => 'textfield',
                '#title' => t("By tags"),
                '#autocomplete_path' => 'admin/structure/so_listings/autocomplete/tags',
                '#description' => t("Keywords separated by commas."),
                '#default_value' => array_key_exists('tags', $default_values) ? $default_values['tags'] : '',
                '#size' => 40,
            ),

            'tags_mode' => array(
                '#type' => 'select',
                '#title' => t("Tag filtering mode"),
                '#options' => array(
                    'AND' => t("All tags"),
                    'OR' => t("At least one tag"),
                ),
                '#default_value' => array_key_exists('tags_mode', $default_values) ? $default_values['tags_mode'] : '',
            ),

            'name' => array(
                '#type' => 'textfield',
                '#title' => t("By name"),
                '#autocomplete_path' => 'admin/structure/so_listings/autocomplete/name',
                '#default_value' => array_key_exists('name', $default_values) ? $default_values['name'] : '',
                '#size' => 40,
            ),

            'persistent' => array(
                '#type' => 'checkbox',
                '#title' => t("Keep these values permanently"),
                '#default_value' => array_key_exists('persistent', $default_values) ? $default_values['persistent'] : '',
            ),

            'actions' => array(
                '#type' => 'actions',
                '#parents' => array(),
                '#id' => 'actions',

                'reset' => !empty($default_values) ?
                    array(
                        '#type' => 'submit',
                        '#value' => t("Reset"),
                    )
                : array(),

                'submit' => array(
                    '#type' => 'submit',
                    '#value' => t("Filter"),
                ),
            ),
        ),

        'overview' => array(
            '#type' => 'container',
            '#attributes' => array(), // bug D7 connu
            '#theme' => 'so_listings_overview',
        ),
    );

    $fields_results = db_query("SELECT data FROM {field_config} WHERE type='listing_type'");

    $usages = array();

    while($field_data = unserialize($fields_results->fetchColumn())) {

        $sql_fields = reset($field_data['storage']['details']['sql']['FIELD_LOAD_CURRENT']);

        $lid_field = $sql_fields['lid'];
        $table = key($field_data['storage']['details']['sql']['FIELD_LOAD_CURRENT']);

        $nodes_results = db_query(
            "SELECT t.entity_id AS nid, t." . $lid_field . " AS lid, n.title, n.language FROM {" . $table . "} AS t
            LEFT JOIN {node} AS n ON n.nid=t.entity_id
            ORDER BY t." . $lid_field . " ASC, n.title ASC"
        );

        while($row = $nodes_results->fetchObject()) {
            $usages[$row->lid][] = l("(" . $row->language . ") " . $row->title, drupal_get_path_alias('node/' . $row->nid), array('attributes' => array('target' => '_blank')));
        }
    }

    $where = array();
    $join = "";
    $group_by = "";
    $args = array();

    if(array_key_exists('prefix', $default_values) && !empty($default_values['prefix'])) {

        if($default_values['prefix'] == 'no_prefix') {
            $where[] = "name NOT LIKE :prefix";
            $args[':prefix'] = "[%]%";
        } else {
            $where[] = "name LIKE :prefix";
            $args[':prefix'] = db_like($default_values['prefix']) . "%";
        }
    }

    if(array_key_exists('tags', $default_values) && !empty($default_values['tags'])) {

        $join = " LEFT JOIN {so_listings_tags AS t ON t.lid=sol.id} ";

        $tags = drupal_explode_tags($default_values['tags']);

        $where[] = "t.tag IN (:tags)";
        $args[':tags'] = $tags;

        if($default_values['tags_mode'] == 'AND') {

            $group_by = " GROUP BY sol.id HAVING COUNT(t.tag)=:count ";
            $args[':count'] = count($tags);
        }
    }

    if(array_key_exists('name', $default_values) && !empty($default_values['name'])) {

        $where[] = "name LIKE :name";
        $args[':name'] = "%" . db_like($default_values['name']) . "%";
    }

    if(!empty($where)) {

        $lid_results = db_query("SELECT sol.id FROM {so_listings} AS sol " . $join . " WHERE " . implode(" AND ", $where) . $group_by . " ORDER BY sol.name ASC", $args);

        $listings = array();

        while($lid = $lid_results->fetchColumn()) {
            $listings[$lid] = so_listings_get_listings_definitions($lid, true);
        }

    } else {
        $listings = so_listings_get_listings_definitions(null, true);
    }

    foreach($listings as $id => $listing) {

        $form['overview'][$id] = array(
            'name' => array('#markup' => $listing->name),
            'tags' => !empty($listing->tags) ? array('#markup' => " <img class='so_listings_tags' src='" . base_path() . "misc/help.png' title='" . t("Tags") . " : " . htmlentities($listing->tags, ENT_QUOTES) . "' />") : "",
            'description' => array('#markup' => $listing->description),
            'usages' => array_key_exists($listing->id, $usages) ? array('#markup' => implode('<br />', $usages[$listing->id])) : array(),
            'operations' => array(
                'edit' => array('#markup' => l(t("edit"), 'admin/structure/so_listings/edit/' . $listing->id)),
                'delete' => array('#markup' => l(t("delete"), 'admin/structure/so_listings/delete/' . $listing->id, array('query' => drupal_get_destination()))),
            ),
        );
    }

    return $form;
}

function so_listings_overview_form_submit($form, &$form_state) {

        if(array_pop($form_state['triggering_element']['#parents']) == 'submit') {

            $filters = $form_state['values']['filters'];

            if($filters['persistent'] == true) {
                $_SESSION['so_listings_overview_filters'] = $filters;
            } elseif(array_key_exists('so_listings_overview_filters', $_SESSION)) {
                unset($_SESSION['so_listings_overview_filters']);
            }

            $form_state['rebuild'] = true;

        } else {

            if(array_key_exists('so_listings_overview_filters', $_SESSION)) {
                unset($_SESSION['so_listings_overview_filters']);
            }
        }
}

function so_listings_delete_confirm($form, &$form_state, $lid) {

    $definition = so_listings_get_listings_definitions($lid);

    return confirm_form(
        $form,
        t("Are you sure you want to delete the listing '@listing' ?", array('@listing' => $definition->name)),
        null,
        null,
        t("Delete")
    );
}

function so_listings_delete_confirm_submit($form, &$form_state) {

    $definition = so_listings_get_listings_definitions($form_state['build_info']['args'][0]);

    // on informe les modules qu'un listing est sur le point d'être supprimé
    module_invoke_all('so_listings_delete', $definition);

    so_forms_destroy('so_listings', $definition->id . '@search');

    $req = "DELETE FROM {so_listings} WHERE id=?";
    db_query($req, array($definition->id));

    drupal_set_message(t("Listing '@listing' has been deleted", array('@listing' => $definition->name)), 'status');
    drupal_goto();
}

/*
 * Listing creation form
 */
function so_listings_create_form($form, &$form_state) {

    $form = array(
        '#tree' => true,
        '#theme_wrappers' => array('fieldset', 'form'), // un peu d'embellissement
        '#title' => t("New listing basic parameters"),
    );

    so_listings_params_form_elements($form, $form_state);

    if(user_access('administer so_listings advanced') == true) {
        $definitions = so_listings_get_listings_definitions();
        $language_list = so_listings_language_list();
        $model_options = array(
            '' => t("None"),
        );

        foreach($definitions as $id => $definition) {
            $model_options[$id] = $definition->name . (!empty($definition->language) ? " (" . $language_list[$definition->language] . ")" : "");
        }

        $form['model'] = array(
            '#type' => 'select',
            '#title' => t("Based on listing"),
            '#description' => t("New listing's parameters will be duplicated from this listing.<br />Be carreful of some of these ones may be incompatible."),
            '#options' => $model_options,
            '#weight' => 0,
        );
    }

    $form['sources']['#states'] = array(
        'visible' => array(
            ':input[name="model"]' => array('value' => ''),
        ),
    );

    $form['actions'] = array(
        '#type' => 'actions',
        '#weight' => 100,

        'create' => array(
            '#type' => 'submit',
            '#value' => t("Create"),
        ),

        'back' => array(
            '#markup' => l(t("Back"), 'admin/structure/so_listings'),
        ),
    );

    return $form;
}

function so_listings_create_form_submit($form, &$form_state) {

    $sources = array();

    foreach($form_state['values']['sources'] as $sources_group) {
        $sources = array_merge($sources, $sources_group);
    }

    $sources = array_values(array_filter($sources));

    $definition = array(
        'name' => $form_state['values']['name'],
        'title' => $form_state['values']['title'],
        'tags' => $form_state['values']['tags'],
        'description' => $form_state['values']['description'],

        'params' => array(
            'sources' => $sources,
            'listing_parent' => $form_state['values']['listing_parent'],
            'modules_settings' => $form_state['values']['modules_settings'],
        ),

        'filters' => array(
            'filters' => array()
        ),
        'sorting' => array(
            'custom_clauses' => array(),
            'sorting' => array_fill_keys(array_keys(so_listings_get_contexts()), 'so_listings_default'),
        ),
        'cache' => array(
            'tags_hash' => "",
            'filters' => array(),
            'sorting' => array_fill_keys(array_keys(so_listings_get_contexts()), 'so_listings_default'),
        ),
    );

    // sorting du contexte par défaut
    $definition['sorting']['sorting']['so_listings_default'] = 'default';

    $definition['model'] = $form_state['values']['model'];

    so_listings_save_listing_definition($definition);

    $default_clauses = variable_get('sol_sorting_settings', array());

    module_load_include('inc', 'so_listings', 'so_listings.sorting');

    $definition['cache']['sorting']['so_listings_default'] = so_listings_sorting_compile_clauses(
        array('so_listings_default' => $default_clauses), // on fabrique un faux tableau
        'so_listings_default',
        array(
            'custom_clauses' => array(),
            'modules_clauses' => module_invoke_all('so_listings_sorting'),
        ),
        $definition['id']
    );

    so_listings_save_listing_definition($definition);

    drupal_set_message(t("Listing '@listing' has been created.", array('@listing' => $definition['name'])));
    drupal_set_message(t("Filters have to be set now, in order to listing can be displayed."), 'warning');
    drupal_goto('admin/structure/so_listings/edit/' . $definition['id'] . '/filters');
}

function so_listings_edit_form($form, &$form_state, $lid) {

    $definition = so_listings_get_listings_definitions($lid, true);

    drupal_set_title(t("Edit listing '@listing'", array('@listing' => $definition->name)));

    $form = array(
        '#tree' => true,
        '#theme_wrappers' => array('fieldset', 'form'), // un peu d'embellissement
        '#title' => t("Listing basic parameters"),
    );

    so_listings_params_form_elements($form, $form_state, (array)$definition);

    $form['actions'] = array(
        '#type' => 'actions',
        '#weight' => 100,

        'save' => array(
            '#type' => 'submit',
            '#value' => t("Save"),
        ),

        'back' => array(
            '#markup' => l(t("Back"), 'admin/structure/so_listings'),
        ),
    );

    return $form;
}

function so_listings_edit_form_submit($form, &$form_state) {

    $sources = array();

    foreach($form_state['values']['sources'] as $sources_group) {
        $sources = array_merge($sources, $sources_group);
    }

    $sources = array_values(array_filter($sources));

    $existing = so_listings_get_listings_definitions($form_state['build_info']['args'][0]);

    $definition = array(
        'id' => $form_state['build_info']['args'][0],
        'name' => $form_state['values']['name'],
        'title' => $form_state['values']['title'],
        'tags' => $form_state['values']['tags'],
        'description' => $form_state['values']['description'],
        'params' => array_merge($existing->params, array(
            'sources' => $sources,
            'listing_parent' => $form_state['values']['listing_parent'],
            'modules_settings' => $form_state['values']['modules_settings'],
        )),
    );

    $sources_have_changed = so_listings_save_listing_definition($definition);

    drupal_set_message(t("Listing '@listing' has been saved.", array('@listing' => $definition['name'])));

    if($sources_have_changed == true) {
        drupal_set_message(t("Data sources have changed. Filters have to be updated, in order to reflect the changes."), 'warning');
        drupal_set_message(t("Sorting should be checked also."), 'warning');
        drupal_goto('admin/structure/so_listings/edit/' . $form_state['build_info']['args'][0] . '/filters');
    }
}

/*
 * Listing's basic parameters form elements
 *
 * @param array $listing_definition
 */
function so_listings_params_form_elements(&$form, &$form_state, $listing_definition = array()) {

    $form['name'] = array(
        '#type' => 'textfield',
        '#title' => t("Name"),
        '#description' => t("Only displayed in listings overviews and in selectbox of widget.") . "<br />" .
                          t("Name can be prefixed with a short value between braces (usually its language), in order to facilitate sorting and filtering in overview. Ex : \"[fr] Hôtels\""),
        '#required' => true,
        '#default_value' => !empty($listing_definition) ? $listing_definition['name'] : "",
        '#weight' => 10,
    );

    $form['title'] = array(
        '#type' => 'textfield',
        '#title' => t("Title"),
        '#description' => t("Optionally displayed in rendered widget."),
        '#required' => true,
        '#default_value' => !empty($listing_definition) ? $listing_definition['title'] : "",
        '#weight' => 11,
    );

    $form['tags'] = array(
        '#type' => 'textfield',
        '#title' => t("Tags"),
        '#description' => t("Keywords separated by commas.") . " " . t("If a keyword contains itself a comma, it must be surrounded by double quotes."),
        '#autocomplete_path' => 'admin/structure/so_listings/autocomplete/tags',
        '#default_value' => !empty($listing_definition) ? $listing_definition['tags'] : "",
        '#weight' => 12,
    );

    $form['description'] = array(
        '#type' => 'textarea',
        '#title' => t("Description"),
        '#default_value' => !empty($listing_definition) ? $listing_definition['description'] : "",
        '#weight' => 40,
    );

    // TODO : remplacer par une implémentation du hook_so_listings_settings()
    $form['listing_parent'] = array(
        '#type' => 'checkbox',
        '#title' => t("Parent listing"),
        '#description' => t("If checked, listing's nodes will be tied with this listing, even if they exist in another one."),
        '#default_value' => !empty($listing_definition) ? $listing_definition['params']['listing_parent'] : 0,
        '#weight' => 45,
    );

    $form['sources'] = array(
        '#type' => 'fieldset',
        '#title' => t("Nodes sources"),
        '#weight' => 50,
    );

    $form['modules_settings'] = array(
        '#weight' => 55,
    );

    $sources_pool = so_listings_load_sources_pool(false);

    foreach($sources_pool as $module => $groups) {

        foreach($groups as $group => $infos) {

            $sources_options = array();

            foreach($infos['sources'] as $id => $source) {
                $sources_options[$module . ':' . $group . ':' . $id] = $source['label'];
            }

            $form['sources'][] = array(
                '#type' => 'checkboxes',
                '#title' => $infos['label'],
                '#options' => $sources_options,
                '#default_value' => (array)$listing_definition['params']['sources'],
            );
        }
    }

    $modules = module_implements('so_listings_settings');

    foreach($modules as $module) {

        if(!empty($listing_definition)
            && array_key_exists('modules_settings', $listing_definition['params']) // for retro-compatibility
            && array_key_exists($module, $listing_definition['params']['modules_settings'])) {

            $module_settings = $listing_definition['params']['modules_settings'][$module];
        } else {
            $module_settings = array();
        }

        $module_infos = system_get_info('module', $module);

        $form['modules_settings'][$module] = array(
            '#type' => 'fieldset',
            '#title' => t("@module settings", array('@module' => $module_infos['name'])),
        );

        $elements = module_invoke($module, 'so_listings_settings', $module_settings);

        $form['modules_settings'][$module] += (array)$elements;
    }
}

function so_listings_tags_autocomplete($string) {

    $output = array();

    $tags = drupal_explode_tags($string);
    $term = trim(array_pop($tags));
    $previous_terms = drupal_implode_tags($tags) . (!empty($tags) ? ", " : "");

    $results = db_query("SELECT tag FROM {so_listings_tags} WHERE tag LIKE ? ORDER BY tag ASC", array("%" . db_like($term) . "%"));

    while($tag = $results->fetchColumn()) {
        $output[$previous_terms . (strpos($tag, ',') === false ? $tag : '"' . $tag . '"')] = $tag;
    }

    drupal_json_output($output);
}

function so_listings_name_autocomplete($string) {

    $output = array();

    $results = db_query("SELECT name FROM {so_listings} WHERE name LIKE ? ORDER BY name ASC", array("%" . db_like($string) . "%"));

    while($name = $results->fetchColumn()) {
        $output[$name] = $name;
    }

    drupal_json_output($output);
}