(function ($) {

Drupal.behaviors.mailjetFieldsetSummaries = {
  attach: function (context) {
    $('fieldset.mailjet-simple-node-campaigns-settings', context).drupalSetSummary(function (context) {
      if ($('.form-item-create-and-send input', context).is(':checked') && $('.form-item-node-campaign-settings-overrides-node-campaign-send-on-publish input', context).is(':checked')) {
        return Drupal.t('Campaign will be sent on node save and on scheduled publish date');
      }
      else if ($('.form-item-create-and-send input', context).is(':checked')) {
        return Drupal.t('Campaign will be sent on node save');
      }
      else if ($('.form-item-node-campaign-settings-overrides-node-campaign-send-on-publish input', context).is(':checked')) {
        return Drupal.t('Campaign will be sent on scheduled publish date');
      }
      else {
        return Drupal.t('Disabled');
      }
    });
  }
};

})(jQuery);
