<?php

function get_field_listing_block_definition() {
    
    $definition = array(
        'translatable' => '0',
        'entity_types' => array(),
        'settings' => array(),
        'storage' => array(
            'type' => 'field_sql_storage',
            'settings' => array(),
            'module' => 'field_sql_storage',
            'active' => '1',
            'details' => array(
                'sql' => array(
                    'FIELD_LOAD_CURRENT' => array(
                        'field_data_field_listing_block' => array(
                            'lid' => 'field_listing_block_lid',
                            'presets' => 'field_listing_block_presets',
                        ),
                    ),
                    'FIELD_LOAD_REVISION' => array(
                        'field_revision_field_listing_block' => array(
                            'lid' => 'field_listing_block_lid',
                            'presets' => 'field_listing_block_presets',
                        ),
                    ),
                ),
            ),
        ),
        'foreign keys' => array(),
        'indexes' => array(),
        'field_name' => 'field_listing_block',
        'type' => 'listing_type',
        'module' => 'so_listings',
        'active' => '1',
        'locked' => '0',
        'cardinality' => '1',
        'deleted' => '0',
        'columns' => array(
                'lid' => array(
                'type' => 'varchar',
                'length' => '255',
                'not null' => FALSE,
                'description' => "The listing's id.",
            ),
            'presets' => array(
                'type' => 'varchar',
                'length' => '255',
                'not null' => TRUE,
                'description' => 'Available presets for this listing.',
            ),
        ),
        'bundles' => array(
            'node' => array(), // on propose le champ dans tous les content types
        ),
    );

    return $definition;
}

