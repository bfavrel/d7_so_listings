<?php
/**
 * Templates suggestion : so-listing--[preset].tpl.php
 *
 * Available variables:
 * - 'element' : the listing element, with the following sub-elements :
 *      - 'listing_infos' : type 'value' : use $element['listing_infos']['#value'] to get value
 *      - 'compiler_options' : type 'value' : use $element['compiler_options']['#value'] to get value
 *      - 'title'
 *      - 'search_form'
 *      - 'switcher'
 *      - 'count' : type 'value' : use $element['count']['#value'] to get value.
 *      - 'count_mode' : type 'value' : use $element['count_mode']['#value'] to get value : either 'query' or 'count' (simple 'count()' on results)
 *      - 'listing'
 *      - 'pager' : markup
 */

$listing_classes .= ' listing-index-' . $element['listing_infos']['#value']['listing_index'] . ' ';
if ($element['listing_infos']['#value']['options']['so_listings']['ajax'] > 0) {
    $listing_classes .='listing-ajax ';
}

if ($element['listing_infos']['#value']['options']['so_listings']['ajax'] > 0) {
    global $language;
    $js_data = array();
    $lid = $element['listing_infos']['#value']['lid'];
    $js_data[$lid] = array();

    $js_data[$lid]['nid'] = $element['listing_infos']['#value']['nid'];
    $js_data[$lid]['path'] = url("node/".$element['listing_infos']['#value']['nid']);
    $js_data[$lid]['lid'] = $lid;
    $js_data[$lid]['listing_index'] = $element['listing_infos']['#value']['listing_index'];
    $js_data[$lid]['preset'] = $element['listing_infos']['#value']['preset'];
    $js_data[$lid]['theme_hook'] = $element['#theme'];
    $js_data[$lid]['type'] = 'embeded';
    $js_data[$lid]['force_geoloc'] = $element['listing_infos']['#value']['options']['creuse_geoloc']['sort'];

    drupal_add_js(array('so_listings_ajax' => $js_data), 'setting');
}
?>

<div class="<?php print($listing_classes); ?>">
    <div class="listing_title">
        <?php print(render($element['title'])); ?>
    </div>

    <div class="listing_search_form">
        <?php print(render($element['search_form'])); ?>
    </div>

    <div class="listing_switcher">
        <?php print(render($element['switcher'])); ?>
    </div>

    <?php if ($element['count']['#value'] !== null): ?>
        <div class="listing_num_results">
            <?php print(format_plural((int) $element['count']['#value'], "@count result", "@count results")); ?>
        </div>
    <?php endif; ?>

    <div class="listing_content">
        <?php print(render($element['listing'])); ?>
    </div>

    <div class="listing_pager">
        <?php print(render($element['pager'])); ?>
    </div>
</div>