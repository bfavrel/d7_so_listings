(function($) {
    $(document).ready(function(){

        //----- VIEW MODES CONFIG

        function updateVMPictoPath() {
            $('select.so_listings_vm_picto_path option').not('.option_iconized').each(function(){

                $(this).html("<img src='" + Drupal.settings.basePath + $(this).val() + "' /> " + $(this).html());
                $(this).addClass('option_iconized');
            });
        }

        function initSolVmConfigSelectAll() {
            $('a.sol_vm_config_select_all').click(function(evt){
                evt.preventDefault();
                $('input.form-radio[value="' + $(this).attr('rel') + '"]').attr('checked', true);
            });
        }

        function initUpdateVMPictoPathDisplay() {
            $('select.so_listings_vm_picto_path').change(function(){
                updateVMPictoPathDisplays();
            });
        }

        function updateVMPictoPathDisplays() {
            $('select.so_listings_vm_picto_path').each(function(){
                $(this).siblings('span.field-suffix').html("<img src='" + Drupal.settings.basePath + $(this).val() + "' />");
            });
        }

        Drupal.behaviors.soListingsViewModesFormPictos = {

            attach: function(context, settings) {

                initSolVmConfigSelectAll();
                updateVMPictoPath();
                initUpdateVMPictoPathDisplay();
                updateVMPictoPathDisplays();
            }

        };

        initSolVmConfigSelectAll();
        updateVMPictoPath();
        initUpdateVMPictoPathDisplay();
        updateVMPictoPathDisplays();

        //----- VIEW MODES NODES OVERVIEW

        Drupal.ajax.prototype.commands.nodeAdminContentAjaxReturn = function() {
            $('form#so-listings-view-modes-node-admin-content-form').fadeOut();

            var form = $('form#so-listings-view-modes-node-admin-content-form');
            var currentRowPictoElement = form.data('curent_row_picto_element');
            var newPictoSrc = form.data('new_selected_picto_src');

            $(currentRowPictoElement).attr('src', newPictoSrc);
            $(currentRowPictoElement).siblings('input.vm_vm').val(form.data('new_selected_vm'));
        }

        $('img.node_admin_content_vm_picto').click(function(){

            if($('input#sol_vm_locked').val() == true) {return;}

            $('form#so-listings-view-modes-node-admin-content-form').data('curent_row_picto_element', $(this));

            $('form#so-listings-view-modes-node-admin-content-form input#sol_vm_nid').val($(this).siblings('input.vm_nid').val());
            $('form#so-listings-view-modes-node-admin-content-form input.form-radio[value="' + $(this).siblings('input.vm_vm').val() + '"]').attr('checked', true);

            var position = $(this).position();
            $('form#so-listings-view-modes-node-admin-content-form').css('left', position.left).css('top', position.top);
            $('form#so-listings-view-modes-node-admin-content-form').fadeIn('fast');
        });

        Drupal.behaviors.nodeAdminContentSelectVm = {
            attach: function(context, settings) {
                initNodeAdminContentVmForm();
            }
        };

        function initNodeAdminContentVmForm() {
            $('form#so-listings-view-modes-node-admin-content-form input.form-radio').change(function(){
                $('form#so-listings-view-modes-node-admin-content-form input#sol_vm_locked').val(1);

                $('form#so-listings-view-modes-node-admin-content-form').data('new_selected_picto_src', $(this).siblings('label').find('img').attr('src'));
                $('form#so-listings-view-modes-node-admin-content-form').data('new_selected_vm', $(this).val());
            });
        }
        initNodeAdminContentVmForm();

        $('form#so-listings-view-modes-node-admin-content-form').mouseleave(function(){
            if($('input#sol_vm_locked').val() == true) {return;}
            $(this).fadeOut();
        });

        //----- ADD SORTING CLAUSE

        Drupal.behaviors.solSortingAdmin = {
            attach: function(context, settings) {
                initSorSortingFieldsHelper();
            }
        };

        function initSorSortingFieldsHelper() {
            $('select#sol_sorting_fields_helper_pool').change(function(){

                if($(this).val() == 0) {
                    $('div#sol_sorting_fields_helper').html("").fadeOut();
                    return;
                }

                $.ajax({
                    url: Drupal.settings['basePath'] + 'admin/structure/so_listings/sorting/ajax/field_helper/' + $('input#so_listings_id').val() + '/' + $(this).val(),
                    type: 'POST',
                    success: function(data) {
                        $('div#sol_sorting_fields_helper').html(data).fadeIn();

                    }
                });
            });
        }
        initSorSortingFieldsHelper();

    });
})(jQuery);