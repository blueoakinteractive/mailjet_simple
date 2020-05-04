(function ($) {

Drupal.behaviors.mailjetFieldsetSummaries = {
  attach: function (context) {
    $('fieldset.mailjet-simple-node-campaigns-settings', context).drupalSetSummary(function (context) {
      if ($('.form-item-create-and-send input', context).is(':checked')) {
        return Drupal.t('Campaign will be sent on node save');
      }
      else {
        return Drupal.t('Disabled');
      }
    });
  }
};

})(jQuery);
