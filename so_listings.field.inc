<?php

/*
 * Implementation of hook_field_info()
 * Field type 'listing_type' declaration.
 */
function so_listings_field_info() {
    return array(
        'listing_type' => array(
            'label' => "Listing",
            'description' => t("Display a predefined listing of nodes."),
            'default_widget' => 'listing_widget',
            'default_formatter' => 'listing_formatter',
            'instance_settings' => array(),
        ),
    );
}

/*
 * Implementation of hook_field_instance_settings_form()
 * Widget 'listing_widget' configuration.
 */
function so_listings_field_instance_settings_form($field, $instance) {

    if ($instance['widget']['type'] == 'listing_widget') {

        $presets = so_listings_get_presets();
        $presets_options = array();

        foreach ($presets as $preset => $infos) {
            $presets_options[$preset] = t($infos->label);
        }

        $form = array(
            'presets' => array(
                '#type' => 'checkboxes',
                '#title' => t("Allowed presets"),
                '#options' => $presets_options,
                '#default_value' => isset($instance['settings']['presets']) ? $instance['settings']['presets'] : array($instance['widget']['settings']['presets']),
            ),

            'tags' => array(
                '#type' => 'textfield',
                '#title' => t("Filter listings by tags"),
                '#autocomplete_path' => 'admin/structure/so_listings/autocomplete/tags',
                '#description' => t("Keywords separated by commas."),
                '#default_value' => isset($instance['settings']['tags']) ? $instance['settings']['tags'] : '',
                '#size' => 40,
            ),
        );

        return $form;
    }
}

//------------------------------------------------------------------------------

/*
 * Implementation of hook_field_widget_info()
 * Widget 'listing_widget' declaration.
 */
function so_listings_field_widget_info() {
    return array(
        'listing_widget' => array(
            'label' => "Listing",
            'field types' => array('listing_type'),
            'behaviors' => array(
                'multiple values' => FIELD_BEHAVIOR_DEFAULT,
                'default value' => FIELD_BEHAVIOR_NONE,
            ),
            'settings' => array(
                'presets' => 'default',
            ),
        ),
    );
}

/*
 * Implementation of hook_field_widget_form()
 * Widget 'listing_widget' edition.
 */
function so_listings_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

    if ($instance['widget']['type'] == 'listing_widget') {

        $default_values = unserialize($items[$delta]['presets']);

        $listings = so_listings_get_listings_definitions(null, true);

        $listings_options = array(
            0 => "- " . t("Select") . " -",
        );

        if(!empty($instance['settings']['tags'])) {
            $tags = explode(',', $instance['settings']['tags']);
            $tags = array_map('trim', $tags);
        } else {
            $tags = '';
        }

        foreach ($listings as $listing) {
            if(!empty($tags)) {
                $listing_tags = explode(', ', $listing->tags);
                if(count(array_intersect($listing_tags, $tags)) > 0) {
                    $listings_options[$listing->id] = $listing->name;
                }
            } else {
                $listings_options[$listing->id] = $listing->name;
            }
        }

        $available_presets = so_listings_get_presets();
        $allowed_presets = array_filter($instance['settings']['presets']);

        $presets_options = array();

        foreach ($allowed_presets as $allowed_preset => $weight) {
            $presets_options[$allowed_preset] = t($available_presets[$allowed_preset]->label) . " : " . t($available_presets[$allowed_preset]->description);
        }

        $presets_options = array_merge(array_filter((array)$default_values), $presets_options);

        $element += array(
            '#type' => 'fieldset',
            '#title' => "Listing",

            'lid' => array(
                '#type' => 'select',
                '#title' => "",
                '#options' => $listings_options,
                '#default_value' => $items[$delta]['lid'],
            ),

            'admin_link' => array(),

            'presets_table' => array(
                '#type' => 'container',

                'presets' => array(),

                'weight' => array(),
            ),
        );

        if(count($allowed_presets) > 1) {

            $element['presets_table']['#theme'] = 'so_listings_widget_presets';

            $element['presets_table']['presets'] = array(
                '#type' => 'checkboxes',
                '#title' => t("Available display modes"),
                '#description' => t("If several display modes are selected, the first one will be used by default.<br />Users will be able to switch between them."),
                '#default_value' => array_keys((array)$default_values),
            );

            $weight = -50;

            foreach ($presets_options as $preset => $label) {

                $presets_options[$preset] = '';

                $element['presets_table']['label'][$preset] = array(
                    '#markup' => $label,
                );

                $element['presets_table']['weight'][$preset] = array(
                    '#type' => 'weight',
                    '#delta' => 50,
                    '#default_value' => $weight,
                );

                $weight++;
            }

            $element['presets_table']['presets']['#options'] = $presets_options;

        } else {

            $preset = reset($allowed_presets);

            $element['presets_table']['presets'][$preset] = array(
                '#type' => 'value',
                '#value' => array($preset),
            );

            $element['presets_table']['weight'][$preset] = array(
                '#type' => 'value',
                '#value' => 0,
            );
        }

        if(user_access('administer so_listings advanced') && !empty($items[$delta])) {
            $element['admin_link'] = array(
                '#markup' => l(t("Edit listing"), '/admin/structure/so_listings/edit/' . $items[$delta]['lid'], array('attributes' => array('target' => '_blank'))),
            );
        }

        return $element;
    }
}

//------------------------------------------------------------------------------

/*
 * Implementation of hook_field_formatter_info()
 */
function so_listings_field_formatter_info() {
    return array(
        'listing_formatter' => array(
            'label' => "Listing",
            'field types' => array('listing_type'),
        ),
    );
}

/*
 * Implementation of hook_field_formatter_view()
 * Widget 'listing_widget' display.
 */
function so_listings_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {

    if ($display['type'] == 'listing_formatter') {

        $elements = array();
        $settings = variable_get('sol_settings', array());
        $q = $_GET['q'];

        foreach ($items as $delta => $item) {

            $switcher_idx = &drupal_static(__FUNCTION__);

            if (!isset($switcher_idx)) {
                $switcher_idx = 0;
            }

            $lid = $item['lid'];
            $presets = unserialize($item['presets']);

            if(empty($presets) || empty($lid)) {
                continue;
            }

            if($settings['global_switcher'] == true && isset($presets[$_SESSION['listings_global_switcher']])) {

                $current_preset = $_SESSION['listings_global_switcher'];

            } elseif($settings['global_switcher'] == false && isset($presets[$_SESSION['listings_local_switcher'][$q][$switcher_idx]])) {

                $current_preset = $_SESSION['listings_local_switcher'][$q][$switcher_idx];

            } else {
                $current_preset = key($presets);
            }

            if(isset($_GET['preset']) && array_key_exists($_GET['preset'], $presets)) {
                $current_preset = $_GET['preset'];
            }

            // on offre aux modules la possibilité de court-circuiter la constitution du listing
            $modules = module_implements('so_listings_formatter_view_alter');

            $element = array();

            foreach($modules as $module) {
                $function = $module . '_so_listings_formatter_view_alter';

                $element = $function($current_preset, $delta, $entity_type, $entity, $field, $instance, $langcode, $items, $display);

                if(!empty($element)) {
                    break;
                }
            }

            if(empty($element)) {
                $options = array(
                    'count' => true, // TODO : ça, ça devrait se trouver dans les presets, non ?
                    'wrapping_queries' => true,
                    'debug_infos' => __FUNCTION__ . "()",
                );

                $listing = so_listings_listing($lid, $current_preset, $options);
                $element = so_listings_build($listing);//modifié le 16/02/2016 - le second param était 'null'
            }

            $element['switcher'] = $listing['nodes']['count'] !== '0' ?
                                   drupal_get_form('so_listings_preset_switcher_form_' . $switcher_idx, $switcher_idx, $presets, $current_preset) :
                                   array();

            $switcher_idx++;

            $elements[] = $element;
        }

        return $elements;
    }
}

//------------------------------------------------------------------------------

/*
 * Implementation of hook_field_presave()
 * Serialize the 'presets' param.
 */
function so_listings_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {

    if ($instance['widget']['type'] == 'listing_widget') {

        foreach ($items as $delta => $item) {

            $presets = array_filter($item['presets_table']['presets']);
            $weights = array_intersect_key($item['presets_table']['weight'], $presets);
            unset($items[$delta]['presets_table']);

            asort($weights);

            $items[$delta]['presets'] = serialize($weights);
        }
    }
}

/*
 * Implementation of hook_field_is_empty()
 */
function so_listings_field_is_empty($item, $field) {

    if ($field['type'] == 'listing_type') {
        $presets = array_filter($item['presets_table']['presets']);

        if(empty($presets)) {
            return true;
        }

        return false;
    }
}