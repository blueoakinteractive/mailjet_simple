<?php

/**
 * implementation of hook_rules_condition_info()
 */
function mailjet_simple_node_campaigns_scheduler_rules_condition_info() {
  return array(
    'mailjet_simple_node_campaigns_scheduler_condition_send_on_publish' => array(
      'group' => t('MailJet Node Campaigns'),
      'label' => t('Node Campaign is scheduled to be sent'),
      'arguments' => array(
        'node' => array('type' => 'node',
        'label' => t('Node'))
      ),
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function mailjet_simple_node_campaigns_scheduler_rules_action_info() {
  return array(
    'mailjet_simple_node_campaigns_scheduler_send_campaign' => array(
      'label' => t('Send node campaign'),
      'group' => t('MailJet Node Campaigns'),
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Node'),
        ),
      ),
    ),
  );
}

/**
 * Determine if the node campaign should be sent on publish.
 */
function mailjet_simple_node_campaigns_scheduler_condition_send_on_publish($node) {
  return variable_get('node_campaign_send_on_publish_' . $node->nid);
}

/**
 * Send a node campaign.
 */
function mailjet_simple_node_campaigns_scheduler_send_campaign($node) {
  if (!empty($node)) {
    // Prepare Mailjet campaign (ie: create and send).
    $list_id = variable_get('node_campaign_list_' . $node->nid);
    $template_id = variable_get('node_campaign_template_' . $node->nid);
    $email = variable_get('node_campaign_email_' . $node->nid);
    $title = $node->title;
    $message = !empty($node->body[LANGUAGE_NONE][0]['value']) ? $node->body[LANGUAGE_NONE][0]['value'] : NULL;

    // Additional "Optional" arguments.
    $arguments = array();

    // Specify From/Reply address if one is specified.
    if (!empty($email)) {
      $arguments['SenderEmail'] = check_plain($email);
    }

    // Specify the text format to be used when sanitizing the message.
    $advanced_settings = mailjet_simple_node_campaigns_get_setting('advanced_settings');
    $text_format = !empty($advanced_settings['text_format']) ? $advanced_settings['text_format'] : NULL;

    // Send the node campaign.
    mailjet_simple_node_campaigns_prepare_campaign_draft($list_id, $title, $message, $template_id, $text_format, $node, $arguments);

    // Create log entry.
    watchdog('mailjet_simple_node_campaigns_scheduler', 'scheduled node campaign sent: %title (@type).', array('@type' => $node->type, '%title' => $node->title), WATCHDOG_NOTICE, l(t('view'), 'node/' . $node->nid, array('alias' => TRUE)));
  }
}
