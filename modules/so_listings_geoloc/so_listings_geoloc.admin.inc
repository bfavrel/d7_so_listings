<?php

function so_listings_geoloc_settings_form($form, &$form_state) {

    $settings = variable_get('so_listings_geoloc_settings', array(
        'fields' => array(
            'lng' => null,
            'lat' => null,
        ),
        'debug_mode' => false,
    ));

    $options = array();

    $result = db_query("SELECT field_name FROM {field_config} ORDER BY field_name ASC");

    while($row = $result->fetchColumn()) {
        $options[$row] = $row;
    }

    $form = array(
        '#tree' => true,

        'fields' => array(
            '#type' => 'fieldset',
            '#title' => t("Coordinates fields"),
            '#collapsible' => true,
            '#description' => t("NOTE : when adding theses two field to a node type, you must clear the menu cache in order to display extra fields in VM forms settings."),

            'lng' => array(
                '#type' => 'select',
                '#title' => t("Longitude"),
                '#options' => $options,
                '#default_value' => $settings['fields']['lng'],
            ),

            'lat' => array(
                '#type' => 'select',
                '#title' => t("Latitude"),
                '#options' => $options,
                '#default_value' => $settings['fields']['lat'],
            ),
        ),

        'debug_mode' => array(
            '#type' => 'checkbox',
            '#title' => t("Debug mode"),
            '#description' => t("Provide geoloc functionalities to admin, even for desktop"),
            '#default_value' => $settings['debug_mode'],
        ),

        'save' => array(
            '#type' => 'submit',
            '#value' => t("Save"),
        ),
    );

    return $form;
}

function so_listings_geoloc_settings_form_submit($form, &$form_state) {

    $settings = variable_get('so_listings_geoloc_settings');

    $settings['fields'] = $form_state['values']['fields'];
    $settings['debug_mode'] = $form_state['values']['debug_mode'];

    variable_set('so_listings_geoloc_settings', $settings);
}