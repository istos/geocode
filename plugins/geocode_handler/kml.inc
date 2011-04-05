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
  'title' => t("KML Geocoder"),
  'description' => t('Get the geometry out of a KML string, file, or URL'),
  'callback' => 'geocode_kml',
  'field_types' => array('text','text_long','file','link'),
  'field_callback' => 'geocode_kml_field',
);

/**
 * Process Markup
 */
function geocode_kml($kml_string) {
  geocode_load_geophp();
  return geoPHP::load($kml_string,'kml_string');
}

function geocode_kml_field($field, $field_item) {
  if ($field['type'] == 'text' || $field['type'] == 'text_long') {
  	return geocode_kml($field_item['value']);
  }
  else {
  	dpm($field);
  	dpm($field_item);
  }
}