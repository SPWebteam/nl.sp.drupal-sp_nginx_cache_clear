<?php

function ngcc_www_default_rules_configuration() {
  $configs = array();

  $rule = '{ "rules_nginx_clear_nu_item" : {
    "LABEL" : "Nginx clear nu item",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "TAGS" : [ "www.sp.nl" ],
    "REQUIRES" : [ "rules", "sp_nginx_cache_clear" ],
    "ON" : { "node_update" : [], "node_insert" : [] },
    "IF" : [
      { "node_is_of_type" : {
          "node" : [ "node" ],
          "type" : { "value" : {
              "achtergrond" : "achtergrond",
              "column" : "column",
              "nieuws" : "nieuws",
              "opinie" : "opinie",
              "publicatie" : "publicatie",
              "rapport" : "rapport",
              "spvimeo" : "spvimeo"
          }}
        }}
    ],
    "DO" : [
      { "sp_nginx_cache_clear_regex" : { "regex" : "[node:url]\r\n(nu|nu\/js|nu\/filter)(\\?page=\\d+)?\r\n(nu|nu\/js|nu\/filter)\\?(page=\\d+)?.*(\u0026?type=[node:content-type]).*\r\nonze-mensen\/.*" } },
      { "LOOP" : {
          "USING" : { "list" : [ "node:thema" ] },
          "ITEM" : { "thema_item" : "Thema" },
          "DO" : [
            { "sp_nginx_cache_clear_regex" : { "regex" : "(nu|nu\/js|nu\/filter)\\?(page=\\d+)?.*thema=[thema-item:nid].*" } }
          ]
      }}
    ]
  }}';
  $configs['rules_nginx_clear_nu_item'] = rules_import($rule);

  $rule = '{ "rules_nginx_clear_thema" : {
    "LABEL" : "Nginx clear thema",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "TAGS" : [ "www.sp.nl" ],
    "REQUIRES" : [ "rules", "sp_nginx_cache_clear" ],
    "ON" : {
      "node_update--thema" : { "bundle" : "thema" },
      "node_insert--thema" : { "bundle" : "thema" }
    },
    "IF" : [
      { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "thema" : "thema" } } } }
    ],
    "DO" : [
      { "sp_nginx_cache_clear_regex" : { "regex" : "standpunten\r\n[node:url]" } },
      { "entity_query" : {
          "USING" : {
            "type" : "node",
            "property" : "thema",
            "value" : [ "node" ],
            "limit" : "100000"
          },
          "PROVIDE" : { "entity_fetched" : { "nodes_with_thema" : "Nodes with thema" } }
      }},
      { "LOOP" : {
          "USING" : { "list" : [ "nodes-with-thema" ] },
          "ITEM" : { "node_with_thema" : "Node with thema" },
          "DO" : [
            { "component_rules_filter_onderwerp_nginx_cc" : { "content" : [ "node-with-thema" ] } }
          ]
      }}
    ]
  }}';
  $configs['rules_nginx_clear_thema'] = rules_import($rule);

  $rule = '{ "rules_nginx_clear_action" : {
    "LABEL" : "Nginx clear action",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "TAGS" : [ "www.sp.nl" ],
    "REQUIRES" : [ "rules", "sp_nginx_cache_clear" ],
    "ON" : {
      "node_update--activiteit" : { "bundle" : "activiteit" },
      "node_insert--activiteit" : { "bundle" : "activiteit" }
    },
    "DO" : [ { "sp_nginx_cache_clear_regex" : { "regex" : "[node:url]" } } ]
  }}';
  $configs['rules_nginx_clear_action'] = rules_import($rule);

  $rule = '{ "rules_nginx_clear_geschiedenis" : {
    "LABEL" : "Nginx clear geschiedenis",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "TAGS" : [ "www.sp.nl" ],
    "REQUIRES" : [ "rules", "sp_nginx_cache_clear" ],
    "ON" : {
      "node_update--geschiedenis" : { "bundle" : "geschiedenis" },
      "node_insert--geschiedenis" : { "bundle" : "geschiedenis" }
    },
    "DO" : [
      { "sp_nginx_cache_clear_regex" : { "regex" : "[node:url]\r\ngeschiedenis" } }
    ]
  }}';
  $configs['rules_nginx_clear_geschiedenis'] = rules_import($rule);

  $rule = '{ "rules_nginx_clear_onderwerp" : {
    "LABEL" : "Nginx clear onderwerp",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "TAGS" : [ "www.sp.nl" ],
    "REQUIRES" : [ "rules", "sp_nginx_cache_clear" ],
    "ON" : {
      "node_update--onderwerp" : { "bundle" : "onderwerp" },
      "node_insert--onderwerp" : { "bundle" : "onderwerp" }
    },
    "DO" : [
      { "sp_nginx_cache_clear_regex" : { "regex" : "[node:url]" } },
      { "LOOP" : {
          "USING" : { "list" : [ "node:thema" ] },
          "ITEM" : { "thema_item" : "Thema" },
          "DO" : [ { "sp_nginx_cache_clear_regex" : { "regex" : "[thema-item:url]" } } ]
        }
      }
    ]
  }}';
  $configs['rules_nginx_clear_onderwerp'] = rules_import($rule);

  $rule = '{ "rules_nginx_clear_pagina" : {
    "LABEL" : "Nginx clear pagina",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "TAGS" : [ "www.sp.nl" ],
    "REQUIRES" : [ "rules", "sp_nginx_cache_clear" ],
    "ON" : { "node_update--page" : { "bundle" : "page" } },
    "DO" : [ { "sp_nginx_cache_clear_regex" : { "regex" : "[node:url]" } } ]
  }}';
  $configs['rules_nginx_clear_pagina'] = rules_import($rule);

  $rule = '{ "rules_nginx_clear_persoon" : {
    "LABEL" : "Nginx clear persoon",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "TAGS" : [ "www.sp.nl" ],
    "REQUIRES" : [ "rules", "sp_nginx_cache_clear" ],
    "ON" : {
      "node_update--persoon" : { "bundle" : "persoon" },
      "node_insert--persoon" : { "bundle" : "persoon" }
    },
    "DO" : [
      { "sp_nginx_cache_clear_regex" : { "regex" : "[node:url]\r\n\u003Cfront\u003E\r\nonze-mensen\/gekozen\r\n" } },
      { "entity_query" : {
          "USING" : {
            "type" : "node",
            "property" : "persoon",
            "value" : [ "node" ],
            "limit" : "10000"
          },
          "PROVIDE" : { "entity_fetched" : { "persoon_entity_fetched" : "Entity with persoon field" } }
        }
      },
      { "LOOP" : {
          "USING" : { "list" : [ "persoon-entity-fetched" ] },
          "ITEM" : { "persoon_relation_node" : "Persoon relatie node" },
          "DO" : [
            { "sp_nginx_cache_clear_regex" : { "regex" : "[persoon-relation-node:url]" } }
          ]
        }
      }
    ]
  }}';
  $configs['rules_nginx_clear_pagina'] = rules_import($rule);

  $rule = '{ "rules_nginx_clear_webform" : {
    "LABEL" : "Nginx clear webform",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "TAGS" : [ "www.sp.nl" ],
    "REQUIRES" : [ "rules", "sp_nginx_cache_clear" ],
    "ON" : {
      "node_update--webform" : { "bundle" : "webform" },
      "node_insert--webform" : { "bundle" : "webform" }
    },
    "DO" : [ { "sp_nginx_cache_clear_regex" : { "regex" : "[node:url]" } } ]
  }}';
  $configs['rules_nginx_clear_webform'] = rules_import($rule);

  $rule = '{ "rules_filter_onderwerp_nginx_cc" : {
    "LABEL" : "Filter nodes on type \u0027onderwerp\u0027 and clear node page in nginx",
    "PLUGIN" : "rule",
    "OWNER" : "rules",
    "TAGS" : [ "www.sp.nl" ],
    "REQUIRES" : [ "rules", "sp_nginx_cache_clear" ],
    "USES VARIABLES" : { "content" : { "label" : "Content", "type" : "node" } },
    "IF" : [
      { "node_is_of_type" : {
          "node" : [ "content" ],
          "type" : { "value" : { "onderwerp" : "onderwerp" } }
        }
      }
    ],
    "DO" : [ { "sp_nginx_cache_clear_regex" : { "regex" : "[content:url]" } } ]
  }}';
  $configs['rules_filter_onderwerp_nginx_cc'] = rules_import($rule);

  return $configs;
}
