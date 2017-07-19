<?php
/*
 * variables :
 *  - label
 *  - nids
 *  - listing_href
 */
?>

<div>Votre recherche : "<?php print($label); ?>" : </div>
<br />
<div>
  <ul>
    <?php
        foreach($nids as $nid) {
            $node = node_load($nid);
            $build = node_view($node, 'sol_subscription');
            $href = url('node/' . $nid, array('absolute' => true));

            print("<li>" . $node->title . "<br />" . render($build) . "<a href='" . $href . "'>" . $href . "</a></li>");
        }
    ?>
  </ul>
</div>
<div><a href="<?php print($listing_href); ?>">Voir tous les résultats en ligne</a></div>
<br /><br />