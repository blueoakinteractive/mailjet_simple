<?php

/**
 * Admin settings form for Mailjet Simple Node Campaigns.
 *
 * @param $form array
 *
 * @return array
 */
function mailjet_simple_node_campaigns_admin_settings_form($form) {
  $mailjet_simple_settings = variable_get('mailjet_simple_settings', mailjet_simple_settings_defaults());
  $node_campaign_settings = variable_get('mailjet_simple_node_campaigns_settings', mailjet_simple_node_campaigns_settings_defaults());

  $form['mailjet_simple_node_campaigns_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mailjet Simple Node Campaign Settings'),
    '#description' => t('Node Campaigns allow nodes to be converted into Mailjet campaigns and be sent to all subscribers of the associated Mailjet List.'),
    '#tree' => TRUE,
  );

  if (!empty($mailjet_simple_settings['public_key'])) {
    $form['mailjet_simple_node_campaigns_settings']['node_campaign_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable Node Campaigns?'),
      '#default_value' => !empty($node_campaign_settings['node_campaign_enabled']) ? $node_campaign_settings['node_campaign_enabled'] : 0,
      '#description' => t('Enable this feature if you would like to enable the node campaigns feature for a content type.'),
    );

    $lists = array(0 => t('- Disabled -')) + mailjet_simple_options_list(TRUE);

    $form['mailjet_simple_node_campaigns_settings']['node_campaign_lists'] = array(
      '#type' => 'select',
      '#title' => t('Select a List'),
      '#options' => $lists,
      '#default_value' => !empty($node_campaign_settings['node_campaign_lists']) ? $node_campaign_settings['node_campaign_lists'] : 0,
      '#description' => t('Select the list that you would like to tie to your content type.
        If you do not see any lists, please check your API settings and that the lists have been created at Mailjet.'),
      '#states' => array(
        // Hide the field when node_campaign_enabled has not been checked.
        'invisible' => array(
          ':input[name="mailjet_simple_node_campaigns_settings[node_campaign_enabled]"]' => array('checked' => FALSE),
        ),
        // Make the field required when node_campaign_enabled has been checked.
        'required' => array(
          ':input[name="mailjet_simple_node_campaigns_settings[node_campaign_enabled]"]' => array('checked' => TRUE),
        ),
      ),
    );

    // Create list of node types.
    $content_types = node_type_get_types();

    foreach ($content_types as $key => $type) {
      $content_types[$key] = t($type->name);
    }

    $content_types = array(0 => t('- None -')) + $content_types;

    $form['mailjet_simple_node_campaigns_settings']['node_campaign_content_types'] = array(
      '#type' => 'select',
      '#title' => t('Select a Node Type'),
      '#options' => $content_types,
      '#default_value' => !empty($node_campaign_settings['node_campaign_content_types']) ? $node_campaign_settings['node_campaign_content_types'] : 0,
      '#description' => t('Select the content type that should create and send emails on save.'),
      '#states' => array(
        // Hide the field when node_campaign_enabled has not been checked.
        'invisible' => array(
          ':input[name="mailjet_simple_node_campaigns_settings[node_campaign_enabled]"]' => array('checked' => FALSE),
        ),
        // Make the field required when node_campaign_enabled has been checked.
        'required' => array(
          ':input[name="mailjet_simple_node_campaigns_settings[node_campaign_enabled]"]' => array('checked' => TRUE),
        ),
      ),
    );

  }

  $form['#validate'][] = 'mailjet_simple_node_campaigns_admin_settings_form_validate';

  return system_settings_form($form);
}

/**
 * Validation handler for mailjet_simple_node_campaigns_admin_settings_form().
 */
function mailjet_simple_node_campaigns_admin_settings_form_validate($form, &$form_state) {
  if ($form_state['values']['mailjet_simple_node_campaigns_settings']['node_campaign_enabled'] == FALSE) {
    return;
  }
  else {
    if (empty($form_state['values']['mailjet_simple_node_campaigns_settings']['node_campaign_lists'])) {
      form_set_error('mailjet_simple_node_campaigns_settings][node_campaign_lists', t('A Mailjet list must be selected to enable this feature.'));
    }
    if (empty($form_state['values']['mailjet_simple_node_campaigns_settings']['node_campaign_content_types'])) {
      form_set_error('mailjet_simple_node_campaigns_settings][node_campaign_content_types', t('A node type must be selected to enable this feature.'));
    }
  }
}
