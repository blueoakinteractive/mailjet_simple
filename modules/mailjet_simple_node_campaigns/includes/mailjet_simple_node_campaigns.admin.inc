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
  $node_campaign_settings = variable_get('mailjet_simple_node_campaigns_settings');

  if (!empty($mailjet_simple_settings['public_key'])) {
    $form['mailjet_simple_node_campaigns_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Mailjet Simple Node Campaign Settings'),
      '#description' => t('Node Campaigns allow nodes to be converted into Mailjet campaigns and be sent to all subscribers of the associated Mailjet List.  Campaigns can be configured on a per content type basis.  Expand each content type listed below to enable/configure it\'s node campaign settings.'),
      '#tree' => TRUE,
    );

    // Create a list of mailjet lists.
    $lists = array(0 => t('- Disabled -')) + mailjet_simple_options_list();

    // Create a list of user created mailjet templates.
    $templates = array(0 => t('- None -')) + mailjet_simple_node_campaigns_template_options_list();

    // Create a list of node types.
    $content_types = node_type_get_types();

    foreach ($content_types as $key => $type) {
      $content_types[$key] = t($type->name);

      $form['mailjet_simple_node_campaigns_settings'][$key] = array(
        '#type' => 'fieldset',
        '#title' => $content_types[$key],
        '#collapsible' => TRUE,
        '#collapsed' => empty($node_campaign_settings[$key]['node_campaign_enabled']),
      );

      $form['mailjet_simple_node_campaigns_settings'][$key]['node_campaign_enabled'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable Node Campaigns for \'@type\'?', array('@type' => $content_types[$key])),
        '#default_value' => !empty($node_campaign_settings[$key]['node_campaign_enabled']),
      );

      $form['mailjet_simple_node_campaigns_settings'][$key]['node_campaign_list'] = array(
        '#type' => 'select',
        '#title' => t('Select a List'),
        '#options' => $lists,
        '#default_value' => !empty($node_campaign_settings[$key]['node_campaign_list']) ? $node_campaign_settings[$key]['node_campaign_list'] : 0,
        '#description' => t('Select the list that you would like to tie to the \'@type\' content type. If you do not see any lists, please <a href="@url">update your Mailjet Settings</a>.', array('@url' => url('admin/config/services/mailjet-simple'), '@type' => $content_types[$key])),
        '#states' => array(
          // Hide the field when node_campaign_enabled has not been checked.
          'invisible' => array(
            ':input[name="mailjet_simple_node_campaigns_settings[' . $key . '][node_campaign_enabled]"]' => array('checked' => FALSE),
          ),
          // Make the field required when node_campaign_enabled has been checked.
          'required' => array(
            ':input[name="mailjet_simple_node_campaigns_settings[' . $key . '][node_campaign_enabled]"]' => array('checked' => TRUE),
          ),
        ),
      );

      $form['mailjet_simple_node_campaigns_settings'][$key]['node_campaign_template'] = array(
        '#type' => 'select',
        '#title' => t('Select a Template'),
        '#options' => $templates,
        '#default_value' => !empty($node_campaign_settings[$key]['node_campaign_template']) ? $node_campaign_settings[$key]['node_campaign_template'] : 0,
        '#description' => t('Select a template that will be associated with \'@type\' campaigns if you would like your emails styled. If no templates are listed you can <a href="@url" target="_blank">create one</a> within your Mailjet account.', array('@url' => 'https://app.mailjet.com/support/how-to-create-my-own-campaign-template,594.htm', '@type' => $content_types[$key])),
        '#states' => array(
          // Hide the field when node_campaign_enabled has not been checked.
          'invisible' => array(
            ':input[name="mailjet_simple_node_campaigns_settings[' . $key . '][node_campaign_enabled]"]' => array('checked' => FALSE),
          ),
        ),
      );

    }

  }
  else {
    drupal_set_message(t('You must configure a <a href="@url">Mailjet API Key</a> before managing this feature.', array('@url' => url('admin/config/services/mailjet-simple'))), 'error');
  }

  return system_settings_form($form);
}

/**
 * Validation handler for mailjet_simple_node_campaigns_admin_settings_form().
 */
function mailjet_simple_node_campaigns_admin_settings_form_validate($form, &$form_state) {
  $content_types = node_type_get_types();

  foreach ($content_types as $key => $type) {
    $content_types[$key] = t($type->name);

    if ($form_state['values']['mailjet_simple_node_campaigns_settings'][$key]['node_campaign_enabled'] == FALSE) {
      return;
    }
    else if (empty($form_state['values']['mailjet_simple_node_campaigns_settings'][$key]['node_campaign_list'])) {
      form_set_error('mailjet_simple_node_campaigns_settings][' . $key . '][node_campaign_list', t('A Mailjet list must be selected to enable this feature for \'@type\'.', array('@type' => $content_types[$key])));
    }

  }
}
