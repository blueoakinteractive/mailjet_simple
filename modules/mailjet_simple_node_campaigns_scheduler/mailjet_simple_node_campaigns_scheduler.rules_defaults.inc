<?php

/**
 * @file
 * mailjet_simple_node_campaigns_scheduler.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function mailjet_simple_node_campaigns_scheduler_default_rules_configuration() {
  $items = array();
  $items['rules_mailjet_simple_node_campaigns_scheduler_component'] = entity_import('rules_config', '{ "rules_mailjet_simple_node_campaigns_scheduler_component" : {
      "LABEL" : "Send node campaign",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "mailjet_simple_node_campaigns_scheduler" ],
      "USES VARIABLES" : { "node" : { "label" : "Node", "type" : "node" } },
      "IF" : [
        { "mailjet_simple_node_campaigns_scheduler_condition_send_on_publish" : { "node" : [ "node" ] } }
      ],
      "DO" : [
        { "mailjet_simple_node_campaigns_scheduler_send_campaign" : { "node" : [ "node" ] } }
      ]
    }
  }');
  $items['rules_mailjet_simple_node_campaigns_scheduler_rule'] = entity_import('rules_config', '{ "rules_mailjet_simple_node_campaigns_scheduler_rule" : {
      "LABEL" : "Send Mailjet Node Campaign when content is published via Scheduler",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "scheduler" ],
      "ON" : { "scheduler_node_has_been_published_event" : [] },
      "DO" : [
        { "component_rules_mailjet_simple_node_campaigns_scheduler_component" : { "node" : [ "node" ] } }
      ]
    }
  }');
  return $items;
}
