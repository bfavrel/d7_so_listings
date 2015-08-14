
WARNING : module's template mustn't be edited. The only purpose of these files is to serve as base to overrides.

--- Theming :

* field--listing_type.tpl.php : implementation of field--[field type].tpl.php
Can be overrided by field--[field name].tpl.php

* listing.tpl.php : element's template for nodes list with display type 'default'
Each new display type must match a template like listing--[display type system name].tpl.php

* so_listing.tpl.php : default template for 'so_listings_build()'

* In each rendered node object listed : property 'node_url' has a GET URL param 'back' which contains the path to go back to the calling listing.

* For each field template : the 'back' URL param string can be found in "$element['#object']->listing_back_params"
* For each field template : the 'back' URL param array can be found in "$element['#object']->listing_back_params_array"