<?php

/**
 * Gathers available modules' sources of nodes.
 *
 * @return array : array of arrays (optionally indexed)
 *                      - 'label' : string : human readable name of the group of sources (must be sufficiently explicit for large usages)
 *                      - 'callback' : string : name of the function which process the query, according of selected source(s).
 *                                              This function will be called with these arguments : $query, $args (see below), $options (compilator options).
 *                                              @see so_listings_sources_callback() for an example.
 *                      - 'sources' : indexed by unique (in module's context) identifier of the source :
 *                              - 'label' : string : human readable name of the source.
 *                              - 'args' : array : callback args.
 */
function hook_so_listings() {}

/**
 * Offers oportunity to modules to provide configuration elements.
 * Modules' own settings will get their settings passed back in hooks, in '$data' parameter.
 *
 * @param array $values : stored settings. Will be empty at listing creation.
 *
 * @return array : an array of FAPI elements. (#tree is set to 'true' for the form)
 */
function hook_so_listings_settings($values = array()) {}

/**
 * Informs modules that a listing has changed : sources or filters have been modified so the results aren't the same.
 *
 * @param stdClass $listing  : the new database listing object
 */
function hook_so_listings_changed($listing) {}

/**
 * Informs modules that a listing is about to be deleted
 *
 * @param stdClass $listing  : the database listing object
 */
function hook_so_listings_delete($listing) {}

/**
 * Provides form elements for edit preset form.
 * Stored values are passed to the compilator, and are available for so_listing's hooks.
 *
 * @param array &$form : the module's fieldset children elements.
 * @param array $default_values : database values previously stored for the calling module.
 */
function hook_so_listings_preset(&$form, $default_values) {}

/**
 * Informs modules that a preset has been edited.
 *
 * @param string $preset_id
 * @param array $preset_data : preset's infos or null if preset has been deleted.
 */
function hook_so_listings_preset_changed($preset_id, $preset_data = null) {}

/**
 * Modules can define listing contexts which will impact the compilation.
 * Currently, contexts are used by sorting process only.
 *
 * @return array : indexed by context system name :
 *                      - 'label' : localised name of the context provided
 */
function hook_so_listings_contexts() {}

/**
 * Modules can provide predefined clauses to listings' filters configuration form.
 *
 * @return array : indexed by filter machine name :
 *                      - 'label' : string : localized human readable clause's name
 *
 *                      - 'callback' : string : name of a callback function.
 *                                              If 'cache' is set to 'true', the callback will be called once
 *                                              only when filters set will be published.
 *                                              Callback query fragments will be stored in cache.
 *
 *                                              If 'cache' is set to 'false' the callback will be called for each
 *                                              listing's query.
 *
 *                                              Both types of callback will be called with &$query, $lid, &$data params and
 *                                              $sources (only if module provides data sources).
 *                                              At this state the $data parameter contains only possibly module's settings
 *                                              (@see hook_so_listings_settings() for details on $data param)
 *                                              (@see hook_so_listings_query() for details on $sources param).
 *
 *                                              The dynamic callback can return a message string which will be displayed in
 *                                              Drupal's message zone (ie : for debugging purpose).
 *
 *                      - 'form' : string : name of the settings form function. This function is called with an array of saved values as param.
 *                                          (@see 'settings' right after). Function must return a mono-dimensional array of elements.
 *
 *                      - 'settings' : array : default settings values indexed by settings' names.
 *
 *                      - 'cache' : boolean : determine if the callback is a dynamic or static one. If 'callback' key
 *                                            doesn't used, this directive will be ignored.
 *
 *                      - 'filter' : array : a query array (@see so_listings_init_query()). Only '#context', 'fields', 'join',
 *                                           'where' and 'args' will be used. Be careful ! Only the first entry of 'where' is used !
 *                                           If module needs a complex clause, it has to build it by its own.
 *                                           If a 'callback' has been set, the 'filter' will be ignored.
 */
function hook_so_listings_filter() {}

/**
 * Modules can provide clauses to listings' sorting process
 *
 * About 'callback' : - static callbacks (cachable ones) will be called with $lid and $context (string) parameters. Since the cache is local for each listing,
 *                    clause can differ from one to another listing. On the other hand, clause musn't differ for any other criteria. Else,
 *                    dynamic callback must be used.
 *
 *                    - dynamic callbacks : will be called twice : once without param : if module wants to get his data parameters it must return his name.
 *                                          The reason is an error on conception : modules' name are not saved with callback name.
 *                                          The second time, function is called with $context (string), &$data (module's own params and settings) and $sources
 *                                          (@see hook_so_listings_query() for details on $sources param).
 *                                          In this case,  it must return an associative array :
 *                                                                             - 'order_by' : string : one or more SQL ORDER BY clauses.
 *                                                                             - 'join' : array : @see so_listings_init_query()
 *
 *          Both callback types have to return an array with 'order_by' and 'join' (@see explanations below) keys, or FALSE for no clause.
 *          Modules have to parse their own data ; So 'fields' array will be discarded and clause will be stored "as it".
 *
 * @return array : indexed by clauses' machine names :
 *                      - 'label' : string : localized human readable clause's name
 *
 *                      - 'clause' or 'callback' : string : one or more well formed ORDER BY clauses or the name of a callback function (static or dynamic one).
 *                                                          Fields syntax depend on which JOIN entry is populated :
 *                                                              - if 'fields' is used : no table name prefix : they will be added by the parser
 *                                                              - if 'join' is used : fields have to be prefixed with table names defined.
 *
 *                      - 'fields' : array : one or more field names used in the clause. If provided, 'join' won't be processed : JOIN clauses will be
 *                                           builded from this fields list.
 *
 *                      - 'join' : array : of well formed JOIN clauses strings. Only processed if 'clause' entry is used and 'fields' not.
 *
 *                      - 'cache' : boolean : can callback result be cached ? (static callback). Only processed if 'callback' entry is used.
 *
 *                      - 'contexts' : array : clause will be allowed for the contexts listed only. If left empty whole contexts will be assumed.
 *
 * @see : so_listings_sorting_compile_clauses()
 */
function hook_so_listings_sorting() {}

/**
 * Modules own provided search field query process
 *
 * User inputs are provided "as it". Modules can choose to use so_listings_prepare_query_fragment_data() or not.
 *
 * @param array $field
 * @param array &$query
 * @param array &$data : $options[module name] : @see so_listings_compile()
 * @param array $sources : the sources provided by the calling module in the form :
 *                              module's source group => (array) module's sources' ids.
 *
 * @return string : an optionnal HTML message wich will be be displayed in Drupal's message zone
 */
function hook_so_listings_search($field, &$query, &$data, $sources = array()) {}

/**
 * Modules can alter the query, just before SQL compilation.
 * Especially, they can add fields to extract, but they should consider the possible wrapping queries
 * when they define fields alias.
 *
 * @param array &$query
 * @param array &$data : $options[module name] : @see so_listings_compile()
 * @param array $sources : the sources provided by the calling module in the form :
 *                              module's source group => (array) module's sources' ids.
 *
 * @return string : an HTML message wich will be be displayed in Drupal's message zone
 */
function hook_so_listings_query(&$query, &$data, $sources = array()) {}

/**
 * Modules can nest the main query in one of their own.
 * Each module who implement this hook will add a nesting level of 'SELECT FROM ()' around the main query.
 * This hook can be used to substitute nodes (ie : translation) or to perform a last minute filtering.
 * It can add additional fields to extract, and define one of them as the nid source for higher
 * wrapping queries (or final one).
 * Each extracted field is transmited to higher wrapping level, and so, will be available in listing.
 *
 * For the moment, only these query's clauses are processed : 'fields', 'join' and 'where'.
 * So 'order_by' clauses and other ones will be ignored.
 *
 * Exemple of use : @see so_feedsagent.module:so_feedsagent_so_listings_query()/so_feedsagent_so_listings_wrapping_query()
 *
 * @param array &$query
 * @param string $nid_alias : nid alias from previous wrapping level.
 * @param array &$data : $options[module name] : @see so_listings_compile()
 * @param array $sources : the sources provided by the calling module in the form :
 *                              module's source group => (array) module's sources' ids.
 *
 * @return string : a column name/alias : define this field as nid one
 *  for current level of wrapping query. If let null, the next level subquery
 *  nid alias will be used instead.
 */
function hook_so_listings_wrapping_query(&$query, $nid_alias, &$data, $sources = array()) {}

/**
 * Modules can alter the raw listing about to be returned by the compilator.
 * Alterations must not modify the number of records, since count has already be performed at this step.
 * In a general maneer, this hook soud be avoided to use.
 *
 * @param array &$listing
 * @param array &$data : $options[module name] : @see so_listings_compile()
 *
 * @return string : an HTML message wich will be be displayed in Drupal's message zone
 */
function hook_so_listings_listing(&$listing, &$data) {}

/**
 * Modules can alter the node's object, just before node_view().
 *
 * @param array $node
 * @param array $node_fields : query extracted fields.
 * @param array $data : = $options[module name] : @see so_listings_compile()
 * @param string $view_mode : contains the current view mode (alterable by module) which will be used by node_view(). *
 */
function hook_so_listings_node_loaded(&$node, $node_fields, $data, &$view_mode) {}

/**
 * Modules can alter the node's render array, just after node_view().
 *
 * @param array $node
 * @param array $node_fields : query extracted fields.
 * @param array $data : $options[module name] : @see so_listings_compile()
 */
function hook_so_listings_node(&$node, $node_fields, $data) {}

/**
 * Modules can provides additional elements to the fully builded listing's render array.
 *
 * @param array &$build : the listing's render array
 * @param array $nodes_fields : query extracted fields.
 * @param array $data : $options[module name] : @see so_listings_compile()
 *
 */
function hook_so_listings_build(&$build, $nodes_fields, $data) {}

/**
 * Modules can bypass the whole process of listing generation and display their own.
 * If a module return an $element, this array will be used in place of the normal one.
 * Modules are processed in install order. If multiple modules return $element, only the last one
 * will be used.
 *
 * @param string $current_preset : preset used to display the field
 * @param int $delta : the index of the current processed item out of $items.
 * @other_params : @see hook_field_formatter_view()
 *
 * @return mixed : either an $element array or null.
 */
function hook_so_listings_formatter_view_alter($current_preset, $delta, $entity_type, $entity, $field, $instance, $langcode, $items, $display) {}