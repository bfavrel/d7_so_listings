<?php

/**
 * Theme the 'listing' type field.
 *
 * Available variables:
 * - $items: An array of field values. Use render() to output them.
 * - $label: The item label.
 * - $label_hidden: Whether the label display is set to 'hidden'.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - field: The current template type, i.e., "theming hook".
 *   - field-name-[field_name]: The current field name. For example, if the
 *     field name is "field_description" it would result in
 *     "field-name-field-description".
 *   - field-type-[field_type]: The current field type. For example, if the
 *     field type is "text" it would result in "field-type-text".
 *   - field-label-[label_display]: The current label position. For example, if
 *     the label position is "above" it would result in "field-label-above".
 *
 * Other variables:
 * - $element['#object']: The entity to which the field is attached.
 * - $element['#view_mode']: View mode, e.g. 'full', 'teaser'...
 * - $element['#field_name']: The field name.
 * - $element['#field_type']: The field type.
 * - $element['#field_language']: The field language.
 * - $element['#field_translatable']: Whether the field is translatable or not.
 * - $element['#label_display']: Position of label display, inline, above, or
 *   hidden.
 * - $field_name_css: The css-compatible field name.
 * - $field_type_css: The css-compatible field type.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 *
 * SO Listings items variables:
 * - 'listing_infos'
 * - 'title'
 * - 'search_form',
 * - 'switcher'
 * - 'count'
 * - 'listing'
 * - 'pager'
 *
 * @see template_preprocess_field()
 * @see theme_field()
 *
 * @ingroup themeable
 */

/* Préparation de quelques données pour l'ajax */
$field_listing_classes = array();
foreach ($items as $delta => $item){
    
    $field_listing_classes[$delta] = ' listing-type-' . $element['#entity_type'] . ' ';
    $field_listing_classes[$delta] .= ' listing-index-' . $item['listing_infos']['#value']['listing_index'] . ' ';
    
    if ($item['listing_infos']['#value']['options']['so_listings']['ajax'] > 0) {
        global $language;
        
        $field_listing_classes[$delta] .='listing-ajax ';

        $lid = $item['listing_infos']['#value']['lid'];
        $js_data = array();
        $js_data[$lid] = array();
        
        $js_data[$lid]['type'] = $element['#entity_type'];
        $js_data[$lid]['listing_index'] = $item['listing_infos']['#value']['listing_index'];
        $js_data[$lid]['nid'] = $element['#object']->nid;
        $js_data[$lid]['path'] = url("node/".$element['#object']->nid);
        $js_data[$lid]['lid'] = $lid;

        drupal_add_js(array('so_listings_ajax' => $js_data), 'setting');
    }
}
?>

<div class="<?php print $classes; ?>"<?php print $attributes; ?>>
    <div class="field-items"<?php print $content_attributes; ?>>
        <?php foreach ($items as $delta => $item): ?>
            <div class="field-item <?php print $delta % 2 ? 'odd' : 'even'; ?> <?php print($field_listing_classes[$delta]); ?>"<?php print $item_attributes[$delta]; ?>>

                <div class="listing_title">
                    <?php print(render($item['title'])); ?>
                </div>

                <div class="listing_search_form">
                    <?php print(render($item['search_form'])); ?>
                </div>

                <div class="listing_switcher">
                    <?php print(render($item['switcher'])); ?>
                </div>

                <?php if($item['count']['#value'] !== null): ?>
                    <div class="listing_num_results">
                        <?php print(format_plural((int)$item['count']['#value'], "@count result", "@count results")); ?>
                    </div>
                <?php endif; ?>

                <div class="listing_content">
                    <?php print(render($item['listing'])); ?>
                </div>

                <div class="listing_pager">
                    <?php print(render($item['pager'])); ?>
                </div>

            </div>
        <?php endforeach; ?>
    </div>
</div>
