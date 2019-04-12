<?php
/**
 * @file
 * Rules integration.
 */

/**
 * Implements hook_rules_action_info().
 *
 * Provides an action sync civicrm data with local user.
 */
function sp_nginx_cache_clear_rules_action_info() {
  $actions = array();

  $actions['sp_nginx_cache_clear_regex'] = array(
    'label' => t('Nginx regex cache clear.'),
    'parameter' => array(
      'regex' => array(
        'type' => 'text',
        'label' => t('Path regex'),
        'description' => t('One regex per line. Do not add ^ and $ to the regex, they are automaticly appended in the server side Lua purge script. A valid example: .* on the www.sp.nl domain will match ^www.sp.nl/.*$ Use <front> as regex for the front page. A full url can also be used. For example https://www.sp.nl/nieuws/SP-wint-verkiezingen'),
        'multiple' => TRUE,
      ),
    ),
  );

  return $actions;
}

function sp_nginx_cache_clear_regex($regex) {
  $regexes = explode("\n", $regex);
  if (!empty($regexes)) {
    foreach ($regexes as $regex) {
      sp_nginx_cache_clear_clear_regex($regex);
    }
  }
}
