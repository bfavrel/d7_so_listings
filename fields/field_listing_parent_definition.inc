<?php

function get_field_listing_parent_definition() {
    
    $definition = array(
        'translatable' => '0',
        'entity_types' => array(),
        'settings' => array(
            'allowed_values' => array(
            '',
            'Listing parent',
            ),
            'allowed_values_function' => '',
        ),
        'storage' => array(
            'type' => 'field_sql_storage',
            'settings' => array(),
            'module' => 'field_sql_storage',
            'active' => '1',
            'details' => array(
            'sql' => array(
                'FIELD_LOAD_CURRENT' => array(
                'field_data_field_listing_parent' => array(
                    'value' => 'field_listing_parent_value',
                ),
                ),
                'FIELD_LOAD_REVISION' => array(
                'field_revision_field_listing_parent' => array(
                    'value' => 'field_listing_parent_value',
                ),
                ),
            ),
            ),
        ),
        'foreign keys' => array(),
        'indexes' => array(
            'value' => array(
            'value',
            ),
        ),
        'field_name' => 'field_listing_parent',
        'type' => 'list_boolean',
        'module' => 'list',
        'active' => '1',
        'locked' => '0',
        'cardinality' => '1',
        'deleted' => '0',
        'columns' => array(
            'value' => array(
            'type' => 'int',
            'not null' => FALSE,
            ),
        ),
        'bundles' => array(
            'node' => array(), // on propose le champ dans tous les content types
        ),
        );
    
    return $definition;
    
}