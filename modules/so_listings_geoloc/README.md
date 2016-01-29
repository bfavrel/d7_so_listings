# ADMINISTRATION
1. Configure the module
2. In SOL sorting settings, add the module sorting in default sorting.
3. Activate sorting in listing parameters
4. In listing template (ex : `field--listing_type.tpl.php`), insert `$item['sol_geoloc_switcher']` in order to display the switcher.
5. In nodes types VM, activate the extra field labelled `Module SOL Geoloc : distance in listing`.

# USAGE WITH SOL API

- With 'so_listing()' and 'so_listings_compile()' the following options must be passed :  
`$options['so_listings_geoloc'] = array('activate_geolocation_sorting' => true, 'sort' => true);`  
First parameter activates the sorting system (and allows to the switcher to be displayed), and the second force the sorting itself.

- In URL : just append '?so_listings_geoloc' parameter set to '1' (works only if geolocation has been activated in the listing params) :  
`?so_listings_geoloc=1' or '...&so_listings_geoloc=1`
