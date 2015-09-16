<?php

function theme_so_listings_widget_presets($vars) {
    $output = "";

    $element = $vars['element'];

    $rows = array();

    foreach(element_children($element['presets']) as $preset) {

        $element['weight'][$preset]['#attributes']['class'] = array('draggable_weight');

        $rows[] = array(
            'data' => array(
                render($element['presets'][$preset]),
                render($element['label'][$preset]),
                render($element['weight'][$preset]),
            ),
            'class' => array('draggable'),
        );
    }

    $table_id = 'table_' . $element['#id'];

    $output .= "<label for='" . $table_id . "'>" . $element['presets']['#title'] . "</label>";

    $output .= theme('table', array(
        'header' => array(),
        'rows' => $rows,
        'attributes' => array('id' => $table_id),
    ));

    $output .= "<div class='description'>" . $element['presets']['#description'] . "</div>";

    drupal_add_tabledrag($table_id, 'order', 'sibling', 'draggable_weight');

    return $output;
}

function theme_so_listings_sorting_context_clauses($vars) {

    $output = "";

    $element = $vars['element'];

    $rows = array();

    foreach(element_children($element['clauses']) as $clause) {

        $element['clauses'][$clause]['weight']['#attributes']['class'] = array('draggable_weight');

        $rows[] = array(
            'data' => array(
                render($element['clauses'][$clause]['label']),
                render($element['clauses'][$clause]['weight']),
                render($element['clauses'][$clause]['delete']),
            ),
            'class' => array('draggable'),
        );
    }

    if(!empty($rows)) {
        $output .= theme('table', array(
            'header' => array(
                t("Label"),
                t("Weight"),
                t("Operations"),
            ),
            'rows' => $rows,
            'attributes' => array('id' => 'table-' . $element['#id']),
        ));

        drupal_add_tabledrag('table-' . $element['#id'], 'order', 'sibling', 'draggable_weight');
    }

    if(!empty($element['add_clause']['add']) && !empty($element['add_clause']['clause']['#options'])) {
        $output .= theme('table', array(
            'header' => array(),
            'rows' => array(
                array(
                    render($element['add_clause']['clause']),
                    render($element['add_clause']['add']),
                ),
            ),
        ));
    }

    return $output;
}

function theme_so_listings_sorting_custom_clauses($vars) {

    $output = "";

    $element = $vars['element'];

    $rows = array();

    foreach(element_children($element) as $custom_clause) {

        $operations = array();

        foreach(element_children($element[$custom_clause]['operations']) as $operation) {
            $operations[] = render($element[$custom_clause]['operations'][$operation]);
        }

        $rows[] = array(
            render($element[$custom_clause]['label']),
            implode(' ', $operations),
            render($element[$custom_clause]['scope']),
        );

    }

    if(empty($rows)) {return;}

    $output .= theme('table', array(
        'header' => array(
            t("Label"),
            t("Operations"),
            t("Type"),
        ),
        'rows' => $rows,
    ));

    return $output;
}

function theme_so_listings_filters_add_clause($vars) {
    $output = "";

    $element = $vars['element'];

    $rows = array(
        array(
            'data' => array(
                render($element['clause']),
                render($element['add']),
            ),
            'class' => array('so_listing_filters_clause'),
        ),
    );

    $header = array(
        array(
            'data' => $element['#title'],
            'colspan' => 2,
        ),
    );

    $output .= theme('table', array('header' => $header, 'rows' => $rows));

    return $output;
}

function theme_so_listings_filters_add_group($vars) {
    $output = "";

    $element = $vars['element'];

    $rows = array(
        array(
            'data' => array(
                render($element['add']),
            ),
            'class' => array('so_listing_filters_group'),
        ),
    );

    $header = array(
        array(
            'data' => t("Add a conditions grouper"),
            'colspan' => 1,
        ),
    );

    $output .= theme('table', array('header' => $header, 'rows' => $rows));

    return $output;
}

function theme_so_listings_filters_clause($vars) {
    $output = "";

    $element = $vars['element'];

    if($element['type']['#value'] == 'clause') {

        if($element['field']['#disabled'] == true) {
            $cell_2 = array(
                '#prefix' => "<div class='clause_label'>",
                '#markup' => $element['field']['#options'][$element['field']['#default_value']],
                '#suffix' => "</div>",
            );
        } else {
            $cell_2 = $element['field'];
        }

        $cell_3 = $element['condition'];
        $cell_4 = $element['args'];

    } elseif($element['type']['#value'] == 'predefined') {

        if($element['predefined']['#disabled'] == true) {
            $cell_2 = array(
                '#prefix' => "<div class='clause_label'>",
                '#markup' => $element['predefined']['#options'][$element['predefined']['#default_value']],
                '#suffix' => "</div>",
            );
        } else {
            $cell_2 = $element['predefined'];
        }

        $cell_3 = array();
        $cell_4 = $element['settings_wrapper'];
    }

    $rows = array(
        array(
            render($element['mode']),
            render($cell_2),
            render($cell_3),
            render($cell_4),
        ),
    );

    $output .= theme('table', array('header' => null, 'rows' => $rows));

    return $output;
}

function theme_so_listings_filters_clause_args($vars) {
    $output = "";

    $element = &$vars['element'];

    $args = element_children($element);

    $container = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('clause_args_cols')),
        'col1' => array('#type' => 'markup', '#prefix' => "<div class='args_col'>", "#suffix" => "</div>"),
        'col2' => array('#type' => 'markup', '#prefix' => "<div class='args_col'>", "#suffix" => "</div>"),
        'col3' => array('#type' => 'markup', '#prefix' => "<div class='args_col'>", "#suffix" => "</div>"),
    );

    $num_rows = ceil(count($args) / 3);

    for($i = 0; $i < $num_rows; $i++) {
        $container['col1']['#markup'] .= drupal_render($element[$args[$i]]);
    }

    array_splice($args, 0, $num_rows);

    for($j = 0; $j < $num_rows; $j++) {
        $container['col2']['#markup'] .= drupal_render($element[$args[$j]]);
    }

    foreach($args as $arg) {
        $container['col3']['#markup'] .= drupal_render($element[$arg]);
    }

    $output = render($container);

    return $output;
}

function theme_so_listings_filters_clauses_overview($vars) {
    $output = "";

    $element = $vars['element'];

    $rows = array();

    foreach((array)element_children($element) as $container) {

        if($element[$container]['clause']['type']['#value'] == 'clause' || $element[$container]['clause']['type']['#value'] == 'predefined') {
            $classes = array(
                'so_listing_filters_clause',
                'tabledrag-leaf',
            );

        } else {
            $classes = array(
                'so_listing_filters_group',
            );
        }

        $classes[] = 'draggable';

        $rows[] = array(
            'data' => array(
                theme('indentation', array('size' => $element[$container]['##drag_depth'])) . render($element[$container]['clause']),
                render($element[$container]['drag_id']),
                render($element[$container]['drag_pid']),
                render($element[$container]['weight']),
                render($element[$container]['delete']),
            ),
            'class' => $classes,
        );
    }

    $table_id = $element['##id'];

    $output .= theme('table', array(
        'header' => null,
        'rows' => $rows,
        'attributes' => array(
            'id' => $table_id,
            'class' => array('filters_clauses_overview'),
        ),
    ));

    drupal_add_tabledrag($table_id, 'match', 'parent', 'clause_drag_pid', 'clause_drag_pid', 'clause_drag_id', true);
    drupal_add_tabledrag($table_id, 'order', 'sibling', 'clause_weight', null, null, true);

    return $output;
}

function theme_so_listings_overview($vars) {
    $output = "";

    $element = $vars['element'];

    $rows = array();
    $listings = element_children($element);

    if(empty($listings)) {
        return "<div class='overview_empty_msg'>- " . t("No listing set yet") . " -</div>";
    }

    foreach($listings as $listing) {
        $rows[] = array(
            render($element[$listing]['name']) . render($element[$listing]['tags']),
            render($element[$listing]['description']),
            render($element[$listing]['usages']),
            render($element[$listing]['operations']['edit']) . " " . render($element[$listing]['operations']['delete']),
        );
    }

    $headers = array(
        t("Name") . " (<img class='so_listings_tags' src='" . base_path() . "misc/help.png' /> = " . t("tags list") . ")",
        t("Description"),
        t("Usages"),
        t("Operations"),
    );

    $output .= theme('table', array('header' => $headers, 'rows' => $rows));

    return $output;
}

function theme_so_listings_new_preset($vars) {
    $output = "";

    $element = $vars['element'];

    $rows = array(
        array(
            render($element['label']),
            render($element['id']),
            render($element['pattern']),
            render($element['add']),
        ),
    );

    $output .= theme('table', array('header' => null, 'rows' => $rows));

    return $output;
}

function theme_so_listings_presets_overview($vars) {
    $output = "";

    $element = $vars['element'];

    $rows = array();

    $presets = element_children($element);

    foreach($presets as $preset) {
        $rows[] = array(
            render($element[$preset]['label']),
            render($element[$preset]['description']),
            render($element[$preset]['id']),
            render($element[$preset]['template']),
            render($element[$preset]['operations']['edit']) . " " . render($element[$preset]['operations']['delete']),
        );
    }

    $headers = array(
        t("Name"),
        t("Description"),
        t("Machine name"),
        t("Template"),
        t("Operations"),
    );

    $output .= theme('table', array('header' => $headers, 'rows' => $rows));

    return $output;
}

function theme_so_listings_view_modes_overview($vars) {
    $output = "";

    $element = $vars['element'];

    $default_vm_radios = array();

    foreach(element_children($element['default_view_modes']) as $node_type) {
        $default_vm_radios[] = $element['default_view_modes'][$node_type]['disabled'];
    }

    $select_all = "<a class='sol_vm_config_select_all' href='#' rel='disabled'>" . t("all") . "</a>";

    $disabled_nodes_types = array(
        '#type' => 'container',
        '#attributes' => array(),
        '#title' => t("Disable configuration for") . " : ",
        '#description' => t("'default' config will be used."),
        '#theme_wrappers' => array('form_element'),

        'select_all' => array('#markup' => $select_all),
        'content' => $default_vm_radios,
    );

    $output .= render($disabled_nodes_types);

    $params = element_children($element['configs']);

    if(!empty($params)) {

        $rows = array();

        foreach($params as $listing_view_mode) {

            $default_vm_radios = array();

            foreach(element_children($element['default_view_modes']) as $node_type) {
                $default_vm_radios[] = $element['default_view_modes'][$node_type][$listing_view_mode];
            }

            $select_all = "<a class='sol_vm_config_select_all' href='#' rel='" . $listing_view_mode . "'>" . t("all") . "</a>";

            $element['configs'][$listing_view_mode]['config_label']['#description'] = t("System name : @name", array(
                '@name' => $element['configs'][$listing_view_mode]['id']['#value'],
            ));

            $rows[] = array(
                'data' => array(
                    render($element['configs'][$listing_view_mode]['config_label']) . render($element['configs'][$listing_view_mode]['id']),
                    render($element['configs'][$listing_view_mode]['vm_listing']),
                    render($element['configs'][$listing_view_mode]['vm_fiche']),
                    render($element['configs'][$listing_view_mode]['picto_path']),
                    $select_all . render($default_vm_radios),
                    render($element['configs'][$listing_view_mode]['delete']),
                    render($element['configs'][$listing_view_mode]['weight']),
                ),
                'class' => array('draggable'),
            );

        }

        $headers = array(
            t("Config label"),
            "Listing VM ",
            "Fiche VM",
            t("Pictogram"),
            t("By default for"),
            "",
            t("Weight"),
        );

        $output .= theme('table', array(
            'header' => $headers,
            'rows' => $rows,
            'attributes' => array('id' => 'so-listings-view-modes-overview-table'),
        ));

        drupal_add_tabledrag('so-listings-view-modes-overview-table', 'order', 'sibling', 'draggable_weight');

    } else {
        $output .= "<div class='overview_empty_msg'>- " . t("No custom view modes set. 'teaser' will be used for listings, and 'full' for fiches") . " -</div>";
    }

    $rows = array(
        array(
            render($element['add_listing_config']['new_config_label']),
            render($element['add_listing_config']['new_config_id']),
            render($element['add_listing_config']['add']),
        ),
    );

    $output .= theme('table', array('header' => array(), 'rows' => $rows,));

    return $output;
}

function theme_so_listings_contexts_overview($vars) {

    $output = "";

    $element = $vars['element'];

    $rows = array();

    foreach(element_children($element['contexts']) as $context) {

        $element['contexts'][$context]['weight']['#attributes']['class'] = array('draggable_weight');

        $label = array(
            '#markup' => $element['contexts'][$context]['label']['#value'],
        );

        $rows[] = array(
            'data' => array(
                render($element['active'][$context]),
                render($element['contexts'][$context]['weight']),
            ),
            'class' => array('draggable'),
        );
    }

    $output .= "<label for='so-listings-contexts-overview'>" . $element['#title'] . "</label>";

    $output .= theme('table', array(
        'header' => array(),
        'rows' => $rows,
        'attributes' => array('id' => 'so-listings-contexts-overview'),
    ));

    $output .= "<div class='description'>" . $element['#description'] . "</div>";

    drupal_add_tabledrag('so-listings-contexts-overview', 'order', 'sibling', 'draggable_weight');

    return $output;
}