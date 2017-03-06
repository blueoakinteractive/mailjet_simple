<?php
/**
 * @file
 * Provides API implementation of MailJet.
 */

use \Mailjet\Resources;

class MailJetSimple {
  private $api;

  /**
   * Constructor method to bootstrap the MailJet API.
   *
   * @param $public_key
   * @param $private_key
   *
   * @throws \Exception
   */
  public function __construct($public_key, $private_key) {
    $this->api = new \Mailjet\Client($public_key, $private_key);
    if (!(is_a($this->api, '\MailJet\Client'))) {
      throw new Exception('Unable to invoke the MailJet Client API');
    }
  }

  /**
   * Fetches the list information for a contact by their email.
   *
   * @param $email string
   *
   * @return array
   *   An array of lists if any were loaded.
   */
  public function getListsByEmail($email) {
    $contact = $this->contactByEmail($email);
    if (!empty($contact)) {
      return $this->getListsByContact($contact['ID']);
    }
    return [];
  }

  /**
   * Fetches the list subscription records for a contact.
   *
   * @param $id integer.
   *   The MailJet Id of the contact object.
   *
   * @return array
   *   An array of list data
   *   http://dev.mailjet.com/email-api/v3/contact-getcontactslists.
   */
  public function getListsByContact($id) {
    $response = $this->api->get(Resources::$ContactGetcontactslists, ['id' => $id]);
    if ($response->success()) {
      return $response->getData();
    }
    return [];
  }

  /**
   * Fetches a MailJet Contact record by email address.
   * @param $email
   * @return mixed|null
   */
  public function contactByEmail($email) {
    // Load the recipient record by email.
    $filters = [
      'ContactEmail' => $email
    ];
    $response = $this->api->get(Resources::$Listrecipient, ['filters' => $filters]);

    // Fetch the contact record from the recipient response.
    if ($response->success()) {
      // One record per subscription is returned, we only need the first item to get the ID.
      $recipients = $response->getData();
      $recipient = reset($recipients);
      if (!empty($recipient)) {
        $contact_response = $this->api->get(Resources::$Contact, ['id' => $recipient['ContactID']]);
        if ($contact_response->success()) {
          // Only one contact is returned, but it's an array of results.
          $contacts = $contact_response->getData();
          return reset($contacts);
        }
      }
    }

    return NULL;
  }


  /**
   * Updates a contacts subscription to a particular list.
   *
   * @param $id integer
   *   The MailJet contact object id.
   * @param $list integer
   *   The MailJet list object id.
   * @param $action string
   *   The action to take on the list.
   *   Options:
   *     - addforce
   *     - addnoforce
   *     - remove
   *     - unsub
   *
   * @return bool
   */
  public function updateListsByContact($id, $list, $action) {
    $body = [
      'ContactsLists' => [
        [
          'ListID' => $list,
          'Action' => $action
        ]
      ]
    ];
    $response = $this->api->post(Resources::$ContactManagecontactslists, ['id' => $id, 'body' => $body]);
    return $response->success();
  }

  /**
   * Creates a new MailJet contact and subscribes to lists.
   *
   * @param $values
   *   The values for the contact.
   *   Options:
   *     - Email
   *     - Name
   *     - Properties
   *         - Property 1
   *         - Property 2
   * @param array $lists
   *   An optional array of list ids and actions.
   *   Options:
   *     - ListId
   *     - Action (addforce, addnoforce, remove, unsub).
   *
   * @return boolean
   *
   */
  public function updateContact($values, $lists = []) {
    $body = [
      'Contacts' => [
        $values
      ]
    ];

    if (!empty($lists)) {
      $body['ContactsLists'] = $lists;
    }

    $response = $this->api->post(Resources::$ContactManagemanycontacts, ['body' => $body]);

    return $response->success();
  }

  /**
   * Loads the available Mailjet lists.
   *
   * @return array
   */
  public function getContactLists() {
    $response = $this->api->get(Resources::$Contactslist);
    if ($response->success()) {
      return (array) $response->getData();
    }
    return [];
  }

  /**
   * Creates a Mailjet campaign.
   *
   * @return array
   *    Campaign response.
   */
  public function createCampaign($list_id, $body) {
    $response = $this->api->post(Resources::$Campaigndraft, ['body' => $body]);
    if ($response->success()) {
      return($response->getData());
    }
    else {
      drupal_set_message(t('There was an error creating your campaign.'), 'error');
    }

    return [];
  }

  /**
   * Posts campaign detail content (ie: message) to a Mailjet campaign.
   *
   * @return boolean
   */
  public function createCampaignContent($campaign_id, $body) {
    $response = $this->api->post(Resources::$CampaigndraftDetailcontent, ['id' => $campaign_id, 'body' => $body]);
    if ($response->success()) {
      return $response->getData();
    }
    else {
      drupal_set_message(t('There was an error setting your campaigns body text.'), 'error');
    }
    return [];
  }

  /**
   * Sends a Mailjet campaign.
   *
   * @return boolean
   */
  public function sendCampaign($campaign_id) {
    $response = $this->api->post(Resources::$CampaigndraftSend, ['id' => $campaign_id]);
    if ($response->success()) {
      drupal_set_message(t('You campaign was sent successfully!'), 'status');
      return $response->getData();
    }
    else {
      drupal_set_message(t('There was an error sending your campaign.  This is usually because the sending email address (ie: site_mail) does not match a valid sending address in your Mailjet account.'), 'error');
      return $response->getData();
    }
    return [];
  }
}
