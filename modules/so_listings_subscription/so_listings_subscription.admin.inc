<?php

function so_listings_subscription_settings_form($form, &$form_state) {

    $settings = variable_get('so_listings_subscription', array());

    $form = array(
        '#tree' => true,

        'settings' => array(
            'config' => array(
                '#type' => 'fieldset',
                '#collapsible' => true,
                '#collapsed' => true,
                '#title' => t("Configuration"),

                'activate_on_search' => array(
                    '#type' => 'checkbox',
                    '#title' => t("Activate subscription form only if a search has been made"),
                    '#default_value' => array_key_exists('config', $settings) ? $settings['config']['activate_on_search'] : false,
                ),
            ),

            'cron' => array(
                '#type' => 'fieldset',
                '#title' => "Cron",
                '#collapsible' => true,
                '#collapsed' => true,

                'active' => array(
                    '#type' => 'checkbox',
                    '#title' => t("Activate cron"),
                    '#default_value' => $settings['cron']['active'],
                ),

                'periodicity' => array(
                    '#type' => 'select',
                    '#title' => t("Periodicity"),
                    '#options' => array(
                        'hourly' => t("Hourly@sols", array('@sols' => null)),
                        'daily' => t("Daily@sols", array('@sols' => null)),
                        'weekly' => t("Weekly@sols", array('@sols' => null)),
                        'monthly' => t("Monthly@sols", array('@sols' => null)),
                    ),
                    '#default_value' => array_key_exists('cron', $settings) ? $settings['cron']['periodicity'] : 'daily',
                ),

                'monthly' => array(
                    '#type' => 'textfield',
                    '#title' => t("Day of month for monthly cron"),
                    '#default_value' => array_key_exists('cron', $settings) ? $settings['cron']['monthly'] : 1,
                    '#size' => 2,
                    '#states' => array(
                        'visible' => array(
                            '#edit-settings-cron-periodicity' => array('value' => 'monthly'),
                        ),
                    ),
                ),

                'weekly' => array(
                    '#type' => 'textfield',
                    '#title' => t("Day of week for weekly cron"),
                    '#description' => t("From 1 (monday) to 7 (sunday)"),
                    '#default_value' => array_key_exists('cron', $settings) ? $settings['cron']['weekly'] : 7,
                    '#size' => 2,
                    '#states' => array(
                        'visible' => array(
                            '#edit-settings-cron-periodicity' => array('value' => 'weekly'),
                        ),
                    ),
                ),

                'time_windows' => array(
                    '#type' => 'textfield',
                    '#title' => t("Time window(s) for cron"),
                    '#description' => t("Two hours (separated by '-') periods, separated by '|'.<br />Exemple : '0-6|13-14' = \"one time between 00h00 (included) and 06h00 (not included) and one time between 13h (included) and 14h (not included)\"."),
                    '#default_value' => array_key_exists('cron', $settings) ? $settings['cron']['time_windows'] : '0-6',
                    '#size' => 25,
                    '#required' => true,
                    '#states' => array(
                        'invisible' => array(
                            '#edit-settings-cron-periodicity' => array('value' => 'hourly'),
                        ),
                    ),
                ),
            ),

            'strings' => array(
                '#type' => 'fieldset',
                '#collapsible' => true,
                '#collapsed' => true,
                '#title' => t("Strings"),
                '#description' => t("In default language"),

                'head' => array(
                    '#type' => 'textfield',
                    '#title' => t("Head sentence"),
                    '#default_value' => array_key_exists('strings', $settings) ? $settings['strings']['head'] : "Abonnez-vous à ce listing et recevez les nouveautés chaque jour par email",
                ),

                'field_label' => array(
                    '#type' => 'textfield',
                    '#title' => t("Field email label"),
                    '#default_value' => array_key_exists('strings', $settings) ? $settings['strings']['field_label'] : "Votre adresse email",
                ),

                'button_label' => array(
                    '#type' => 'textfield',
                    '#title' => t("Submit button label"),
                    '#default_value' => array_key_exists('strings', $settings) ? $settings['strings']['button_label'] : "Ok",
                ),

                'success' => array(
                    '#type' => 'textfield',
                    '#title' => t("Subscription success message"),
                    '#default_value' => array_key_exists('strings', $settings) ? $settings['strings']['success'] : "Votre abonnement a bien été pris en compte",
                ),

                'manage_link' => array(
                    '#type' => 'textfield',
                    '#title' => t("Manage subscriptions link"),
                    '#default_value' => array_key_exists('strings', $settings) ? $settings['strings']['manage_link'] : "Gérer vos abonnements",
                ),

                'manage_head' => array(
                    '#type' => 'textfield',
                    '#title' => t("Manage subscriptions head"),
                    '#default_value' => array_key_exists('strings', $settings) ? $settings['strings']['manage_head'] : "En vous désabonnant des recherches ci-dessous, vous ne recevrez plus les nouveautés par mail",
                ),

                'unsubscribe_button_label' => array(
                    '#type' => 'textfield',
                    '#title' => t("Unsubscribe button label"),
                    '#default_value' => array_key_exists('strings', $settings) ? $settings['strings']['unsubscribe_button_label'] : "Me désabonner des recherches sélectionnées",
                ),

                'unsubscribe_confirmation' => array(
                    '#type' => 'textfield',
                    '#title' => t("Unsubscribe confirmation"),
                    '#default_value' => array_key_exists('strings', $settings) ? $settings['strings']['unsubscribe_confirmation'] : "Votre désabonnement a bien été pris en compte",
                ),

                'mail_subject' => array(
                    '#type' => 'textfield',
                    '#title' => t("Email subject"),
                    '#default_value' => array_key_exists('strings', $settings) ? $settings['strings']['mail_subject'] : "Vos recherches sur le site",
                ),
            ),
        ),

        'submit' => array(
            '#type' => 'submit',
            '#value' => t("Save"),
        ),
    );

    return $form;
}

function so_listings_subscription_settings_form_submit($form, &$form_state) {

    $settings = $form_state['values']['settings'];

    foreach($settings['strings'] as $name => $string) {
        i18n_string('module:so_listings_subscription:' . $name, $string, array('update' => true));
    }

    variable_set('so_listings_subscription', $settings);
}