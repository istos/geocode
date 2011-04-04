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
  'description' => t('Get the geometry out of a KML string'),
  'callback' => 'geocode_geocode_kml',
);

/**
 * Process Markup
 */
function geocode_geocode_kml($kml_string) {
  geocode_load_geophp();
  return geoPHP::load($kml_string,'kml_string');
}
