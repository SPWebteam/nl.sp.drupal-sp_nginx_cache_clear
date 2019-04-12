<?php

/**
 * @file
 * Settings form for SP nginx cache clear.
 */

/**
 * Callback from sp_nginx_cache_clear_menu().
 */
function sp_nginx_cache_clear_admin_settings() {
  $form['server_cache_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('SP Nginx Cache Clear Configuration'),
  );

  $form['server_cache_config']['sp_nginx_cache_clear_auto_delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto clear Nginx cache when content edit'),
    '#required' => FALSE,
    '#default_value' => variable_get('sp_nginx_cache_clear_auto_delete', ''),
    '#description' => t('If this option is enabled, then the Nginx cache file of content(URL alias) will be deleted when the user update the content.'),
  );

  return system_settings_form($form);
}
