<?php // $Id: imagefield.inc,v 1.3 2009/03/02 18:14:07 vauxia Exp $


if (function_exists('exif_read_data')) {
	$plugin = array(
	  'title' => t("Image/exif"),
	  'description' => t('Get a location from an image that was taken with a GPS enabled phone or camera'),
	  'callback' => 'geocode_exif',
	  'field_types' => array('file'),
	  'field_callback' => 'geocode_exif_field',
	);
}

class geocode_image {
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
