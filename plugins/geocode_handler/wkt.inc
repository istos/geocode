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
  'callback' => 'geocode_geocode_wkt',
);

/**
 * Process Markup
 */
function geocode_geocode_wkt($wkt) {
  geocode_load_geophp();
  return geoPHP::load($wkt,'wkt');
}
