<?php
/*
 * variables:
 *  - subscriptions : array of subscriptions render arrays
 *  - management_href
 */
?>
<?php foreach($subscriptions as $html): ?>
    <?php print($html); ?>
<?php endforeach; ?>

<div><a href="<?php print($management_href); ?>">Cliquez ici pour gérer vos abonnements</a></div>