/**
 * Ajax behavior for listings.
 */

(function($) {

    Drupal.behaviors.so_listings_ajax = {
        attach: function(context, settings) {

            for (var lid in settings.so_listings_ajax) {
                var item = settings.so_listings_ajax[lid];

                $('.listing-index-' + item.listing_index).each(function(id, listing_e) {
                    var $listing = $(listing_e);

                    /* --------- Pager links ------------- */
                    $listing.find('.pager a:not(.processed)').each(function() {
                        $e = $(this);
                        $e.addClass('processed');

                        var params = so_listings_get_params($(this).attr('href'));
                        var realhref = item['path'] + (params.length > 0 ? '?' + params : '');
                        $(this).attr('href', realhref);

                        // Création du path Ajax
                        // Force la geoloc à cause des listings multiples qui sont forcés en geoloc même s'il n'y a pas l'argument GET (donc on le rajoute)
                        if (item['force_geoloc'] > 0) {
                            if (params.indexOf("&geoloc=") === -1 && params.indexOf("?geoloc=") === -1) {
                                if (params.length > 0) {
                                    params += '&geoloc=1';
                                } else {
                                    params += '?geoloc=1';
                                }
                            }
                        }
                        var ajax_url;
                        if (item['type'] === 'embeded') {
                            ajax_url = Drupal.settings.basePath + Drupal.settings.pathPrefix + "so_listings/ajax/embeded/" + item['nid'] + "/" + item['lid'] + "/" + item['listing_index'] + "/" + item['preset'] + "/" + item['theme_hook'];
                        } else if (item['type'] === 'node') {
                            ajax_url = Drupal.settings.basePath + Drupal.settings.pathPrefix + "so_listings/ajax/node/" + item['nid'] + "/" + item['listing_index'];
                        } else if (item['type'] === 'block') {
                            ajax_url = Drupal.settings.basePath + Drupal.settings.pathPrefix + "so_listings/ajax/block/" + item['nid'] + "/" + item['listing_index'];
                        } else {
                            return;
                        }
                        var ajaxPath = ajax_url;

                        $e.click(function(event) {
                            event.preventDefault();
                            history.pushState(null, null, realhref);
                            window.addEventListener("popstate", function(e) {
                                window.location = location.pathname;
                            });
                            $(this).addClass('throbber');

                            $.ajax({
                                url: ajaxPath,
                                data: params
                            })
                                    .done(function(response) {
                                        $(this).removeClass('throbbing');
                                        if (response) {
                                            var $newContent = $(response);
                                            if (item['type'] === 'node' || item['type'] === 'block') {
                                                // Ca passe dans le template complet, mais avec un seul item. enlever les wrappers en double
                                                $newContent = $newContent.children().children();
                                            }
                                            $listing.replaceWith($newContent);
                                            Drupal.attachBehaviors($newContent.parent());
                                            $('html, body').animate({
                                                scrollTop: $newContent.parent().offset().top - 100
                                            }, 600);
                                        }
                                    })
                                    .fail(function() {
                                        $(this).removeClass('throbber');
                                        alert(Drupal.t("An error occurred at @path.", {
                                            '@path': ajaxPath
                                        }));
                                    });
                            return false;
                        });
                    });
                    
                    /* ---- Modification des href et action des formulaires et du lien de geoloc --- */
                    $listing.find('form').each(function() {
                        var params = so_listings_get_params(window.location.href);
                        $(this).attr('action', item['path'] + (params.length > 0 ? '?' + params : ''));
                    });

                    $listing.find('.listing_geoloc_switcher a').each(function() {
                        var params = so_listings_get_params($(this).attr('href'));
                        $(this).attr('href', item['path'] + (params.length > 0 ? '?' + params : ''));
                    });
                });
            }
        }
    };
    // Renvoie la chaine après le ? de l'url
    function so_listings_get_params(url) {
        var args = url.split("?");
        if (args.length > 1) {
            return args[1];
        }
        return '';
    }
})(jQuery);
