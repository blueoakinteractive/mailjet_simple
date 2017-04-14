<?php
/**
 * @file
 * Provides API implementation of MailJet.
 */

use \Mailjet\Resources;

class MailJetSimpleNodeCampaigns extends MailJetSimple {
  /**
   * Constructor method to bootstrap the MailJet API.
   *
   * @param $public_key
   * @param $private_key
   *
   * @throws \Exception
   */
  public function __construct($public_key, $private_key) {
    parent::__construct($public_key, $private_key);
  }

  /**
   * Creates a Mailjet campaign draft.
   * https://dev.mailjet.com/guides/?php#prepare-a-campaign
   * https://dev.mailjet.com/email-api/v3/campaigndraft/
   *
   * @param array $arguments
   *    Properties used for creating the campaign.
   *
   * @return array
   *    API response.
   */
  public function createCampaignDraft($arguments) {
    // Default properties required for creating a campaign draft.
    $properties = array(
      'Locale' => "en_US",
      'Sender' => variable_get('site_name', 'Example Site Name'),
      'SenderName' => variable_get('site_name', 'Example Site Name'),
      'SenderEmail' => variable_get('site_mail', 'example@example.com'),
      'Subject' => NULL,
      'ContactsListID' => NULL,
      'Title' => NULL,
    );

    // Overriding existing properties and adding new ones.
    $properties = array_merge($properties, $arguments);

    $response = $this->api->post(Resources::$Campaigndraft, ['body' => $properties]);
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
   * https://dev.mailjet.com/guides/?php#prepare-a-campaign
   * https://dev.mailjet.com/email-api/v3/campaigndraft-detailcontent/
   *
   * @param integer $campaign_id
   *    The campaign id updated with body text.
   *
   * @param array $arguments
   *    Properties used for updating the campaign.
   *
   * @return array
   *    API response.
   */
  public function createCampaignDraftDetailContent($campaign_id, $arguments) {
    // Default properties required for creating a campaign draft detail content.
    $properties = array(
      'Html-part' => NULL,
      'Text-part' => NULL
    );

    // Overriding existing properties and adding new ones.
    $properties = array_merge($properties, $arguments);

    $response = $this->api->post(Resources::$CampaigndraftDetailcontent, ['id' => $campaign_id, 'body' => $arguments]);
    if ($response->success()) {
      return $response->getData();
    }
    else {
      drupal_set_message(t('There was an error setting your campaigns body text.'), 'error');
    }
    return [];
  }

  /**
   * Sends a Mailjet campaign draft.
   * https://dev.mailjet.com/guides/?php#send-a-campaign
   * https://dev.mailjet.com/email-api/v3/campaigndraft-send/
   *
   * @param integer $campaign_id
   *    The ID associated with the campaign draft to be sent.
   *
   * @return array
   *    API response.
   */
  public function sendCampaignDraft($campaign_id) {
    $response = $this->api->post(Resources::$CampaigndraftSend, ['id' => $campaign_id]);
    if ($response->success()) {
      drupal_set_message(t('You campaign was sent successfully!'), 'status');
      return $response->getData();
    }
    else {
      drupal_set_message(t('There was an error sending your campaign.  This is usually because the sending email address (ie: site_mail) does not match a valid sending address in your Mailjet account or the Mailjet list has 0 subscribed contacts.'), 'error');
      return $response->getData();
    }
    return [];
  }

  /**
   * Loads Mailjet template(s).
   * https://dev.mailjet.com/email-api/v3/template/
   *
   * @param integer $template_id
   *    The id of the template to be loaded.
   *    If no template id is provided load all user templates.
   *
   * @param array $filter_arguments
   *    Properties used for filtering templates.
   *
   * @return array
   *    API response.
   */
  public function getTemplates($template_id = NULL, $filter_arguments = []) {
    // Load templates with the owner type of user.
    $filters = [
      'OwnerType' => 'user'
    ];

    // Overriding existing filters and adding new ones.
    $properties = array_merge($filters, $filter_arguments);

    // If the template id is provided load the template else load all user templates.
    if (!empty($template_id)) {
      $response = $this->api->get(Resources::$Template, ['id' => $template_id], ['filters' => $filters]);
    }
    else {
      $response = $this->api->get(Resources::$Template, ['filters' => $filters]);
    }

    return $response->getData();
  }

  /**
   * Loads Mailjet template detail.
   * https://dev.mailjet.com/email-api/v3/template-detailcontent/
   *
   * @param integer $template_id
   *    The id of the template to be loaded.
   *    If no template id is provided load all user templates.
   *
   * @return array
   *    API response.
   */
  public function getTemplateDetail($template_id) {
    $response = $this->api->get(Resources::$TemplateDetailcontent, ['id' => $template_id]);
    if ($response->success()) {
      return $response->getData();
    }
    else {
      drupal_set_message(t('There was an error fetching the template detail.'), 'error');
    }
  }
}
