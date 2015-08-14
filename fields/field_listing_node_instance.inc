<?php

function get_field_listing_node_instance() {

    $instance = array(
        'label' => 'Listing',
        'widget' => array(
            'weight' => '-4',
            'type' => 'listing_widget',
            'module' => 'so_listings',
            'active' => 0,
            'settings' => array(
            'presets' => 'default',
            ),
        ),
        'settings' => array(
            'presets' => array(
                'default' => 'default',
            ),
            'user_register_form' => FALSE,
        ),
        'display' => array(
            'default' => array(
                'label' => 'hidden',
                'type' => 'listing_formatter',
                'weight' => '0',
                'settings' => array(),
                'module' => 'so_listings',
            ),
            'full' => array(
                'label' => 'hidden',
                'type' => 'listing_formatter',
                'weight' => '0',
                'settings' => array(),
                'module' => 'so_listings',
            ),
            'teaser' => array(
                'label' => 'hidden',
                'type' => 'hidden',
                'weight' => '0',
                'settings' => array(),
            ),
        ),
        'required' => 0,
        'description' => '',
        'field_name' => 'field_listing_node',
        'entity_type' => 'node',
        'bundle' => 'listing',
        'deleted' => '0',
    );

    return $instance;
}
