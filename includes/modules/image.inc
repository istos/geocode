<?php // $Id: imagefield.inc,v 1.3 2009/03/02 18:14:07 vauxia Exp $

/**
 * Implementation of hook_geocode_handler_info() on behalf of imagefield.
 */
function image_geocode_handler_info() {
  $handlers = array();
  // This functionality requires PHP's exif module
  if (function_exists('exif_read_data')) {
    $handlers['geocode_image'] = array(
      'title' => t('Image field'),
      'module' => 'geocode',
      'file' => 'imagefield.inc',
      'file path' => drupal_get_path('module', 'geocode') .'/includes/modules',
      'return types' => array(
        'geo' => array('point'),
      ),
      'field types' => array('image'),
    );
  }
  return $handlers;
}

class geocode_image extends geocode {
  function geocode($file) {
    if ($file['filepath'] && $data = exif_read_data($file['filepath'])) {
      if (!isset($data['GPSLatitudeRef'])) return FALSE;
      $this->set_result($data);
      return TRUE;
    }
    return FALSE;
  }

  function set_result($data) {
    
    $this->result = $data;
    $this->point = array(
      'lat' => $this->from_exif($data['GPSLatitudeRef'], $data['GPSLatitude']),
      'lon' => $this->from_exif($data['GPSLongitudeRef'], $data['GPSLongitude']),
    );
    if (isset($data['GPSAltitude'])) {
      list($alt, $pct) = explode('/', $item);
      if ($pct) $alt = $alt / $pct;
      $this->point['ele'] = $alt;
    }
  }

  function from_exif($dir, $data) {
    foreach ($data as $k => $item) {
      list($deg, $pct) = explode('/', $item);
      if ($pct) $data[$k] = $deg / $pct;
    }
    $point = (float) $data[0] + ($data[1] / 60) + ($data[2] / 3600);
    if (in_array($dir, array('S', 'W'))) $point = $point * -1;
    return $point;
  }
}
