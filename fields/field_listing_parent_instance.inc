<?php

function get_field_listing_parent_instance() {
    
    $instance = array(
        'label' => 'Listing parent',
        'widget' => array(
            'weight' => '1',
            'type' => 'options_onoff',
            'module' => 'options',
            'active' => 1,
            'settings' => array(
            'display_label' => 0,
            ),
        ),
        'settings' => array(
            'user_register_form' => FALSE,
        ),
        'display' => array(
            'default' => array(
            'label' => 'hidden',
            'type' => 'hidden',
            'weight' => '2',
            'settings' => array(),
            ),
            'full' => array(
            'label' => 'hidden',
            'type' => 'hidden',
            'weight' => '0',
            'settings' => array(),
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
        'default_value' => array(
            array(
            'value' => 0,
            ),
        ),
        'field_name' => 'field_listing_parent',
        'entity_type' => 'node',
        'bundle' => 'listing',
        'deleted' => '0',
    );
    
    return $instance;
    
}