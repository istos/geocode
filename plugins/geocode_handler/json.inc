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
  'title' => t("JSON Geocoder"),
  'description' => t('Get the geometry of a GeoJSON string'),
  'callback' => 'geocode_geocode_json',
);

/**
 * Process Markup
 */
function geocode_geocode_json($json) {
  geocode_load_geophp();
  return geoPHP::load($json,'json');
}
