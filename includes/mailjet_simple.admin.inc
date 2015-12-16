<?php

/**
 * @file
 * Provides admin interface for managing Mailjet simple.
 */

/**
 * Admin settings form for Mailjet simple.
 *
 * @param $form array
 *
 * @return array
 */
function mailjet_simple_admin_settings_form($form) {
  $settings = variable_get('mailjet_simple_settings', mailjet_simple_settings_defaults());

  $form['mailjet_simple_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mailjet Simple Settings'),
    '#tree' => TRUE,
  );

  $form['mailjet_simple_settings']['instructions'] = array(
    '#markup' => '<h2>Mailjet Setup Instructions</h2><ul>
        <li>' . t('Visit !link to gather your Mailjet API Settings.', array('!link' => l('https://app.mailjet.com/account/api_keys', 'https://app.mailjet.com/account/api_keys'))). '</li>
        <li>' . t('Enter your private and public keys below.') . '</li>
        <li>' . t('Save the initial configuration.') . '</li>
        <li>' . t('Select the Mailjet lists that become available once you have entered your API credentials.') . '</li>
        <li>' . t('Re-save this settings form with the lists selected') . '</li>
    </ul>',
  );

  $form['mailjet_simple_settings']['public_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Mailjet API Key'),
    '#default_value' => $settings['public_key'],
  );

  $form['mailjet_simple_settings']['private_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Mailjet Secret Key'),
    '#default_value' => $settings['private_key'],
  );

  if (!empty($settings['public_key'])) {
    $lists = mailjet_simple_options_list(TRUE);

    $form['mailjet_simple_settings']['enabled_lists'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Mailjet Lists'),
      '#options' => $lists,
      '#default_value' => $settings['enabled_lists'],
      '#description' => t('Select the lists that you would like be be available for subscription.
        If you do not see any lists, please check your API settings and that the lists have been created at Mailjet.'),
    );
    $form['mailjet_simple_settings']['lists'] = array(
      '#type' => 'value',
      '#value' => $lists,
    );
  }

  return system_settings_form($form);
}
