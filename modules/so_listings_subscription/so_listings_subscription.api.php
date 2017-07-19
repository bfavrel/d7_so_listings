<?php

/**
 * Module can make a custom subscription title made of various listing/form informations.
 * 
 * @param array $search_data : for detail @see so_listings_subscription.module:so_listings_subscription_so_listings_build()
 * @return string
 */
function hook_so_listings_subscription_label($search_data) {
    return "label";
}

