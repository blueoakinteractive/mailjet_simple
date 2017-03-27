<?php
/**
 * @file
 * Provides API methods exposed by this module.
 */

/**
 * Allow modules to alter the update contact request in the form
 * submit handler.
 *
 * @see \MailJetSimple->updateContact() for override options.
 *
 * @param $values
 *   The values passed to Mailjet to create the contact
 *
 * @param $lists
 *   An array of lists to subscribe/unsubscribe the user from.
 *
 * @param $form
 *   The form object for altering.
 */
function hook_mailjet_simple_update_contact_submit(&$values, &$lists, &$form, $form_state) {
  $values['Name'] = $form_state['values']['field_first_name'];
}