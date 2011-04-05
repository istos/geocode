<?php
// $Id$

/**
 * @file
 * Plugin to provide a google geocoder.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("WKT Geocoder"),
  'description' => t('Get the geomtry of a WKT string'),
  'callback' => 'geocode_wkt',
  'field_types' => array('text','text_long'),
  'field_callback' => 'geocode_wkt_field',
);

/**
 * Process WKT
 */
function geocode_wkt($wkt) {
  geocode_load_geophp();
  return geoPHP::load($wkt,'wkt');
}

function geocode_wkt_field($field, $field_item) {
  return geocode_wkt($field_item['value']);
}
