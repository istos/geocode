<?php // $Id: gpx.inc,v 1.1 2009/03/02 18:14:07 vauxia Exp $

/**
 * @file
 *
 * Parsing utilities for GPX track files.  Originally written to cover a more
 * comprehensive usage than what Geocode is doing, but handy nonetheless.
 *
 */

function gpx_trackpoints($gpx) {
  return gpx_points($gpx->getElementsByTagName('trkpt'), true);
}

function gpx_speed($gpx) {
  $trackpoints = gpx_points($gpx->getElementsByTagName('trkpt'), true);
}

function gpx_elevation($gpx) {
  return gpx_minmax($gpx->getElementsByTagName('ele'), true, 'ele');
}

function gpx_timestamps($gpx) {
  return gpx_minmax($gpx->getElementsByTagName('time'), true, 'time');
}

function gpx_points($items, $return_values = false) {
  $points = array();

  for ($i = 0; $i < $items->length; $i++) {
    $lat = $items->item($i)->getAttribute('lat');
    $lon = $items->item($i)->getAttribute('lon');

    if (!isset($points['min_lat']) || ($lat < $points['min_lat']))
      $points['min_lat'] = $lat;
    if (!isset($values['max_lat']) || ($lat > $points['max_lat']))
      $points['max_lat'] = $lat;
    if (!isset($points['min_lon']) || ($lon < $points['min_lon']))
      $points['min_lon'] = $lon;
    if (!isset($points['max_lon']) || ($lon > $points['max_lon']))
      $points['max_lon'] = $lon;

    if ($return_values) {
      $p = array( 'lat' => $lat, 'lon' => $lon );
      $children = $items->item($i)->getElementsByTagName('*');

      for ($j = 0; $j < $children->length; $j++) {
        $type = $children->item($j)->nodeName;
        $p[$type] = gpx_callbacks($children->item($j), $children, $j);
      }

      // Calculate some extra values
      if (isset($p['time'])) {

        $points['end_time'] = $p['time'];
        if (!isset($points['start_time'])) {
          $points['start_time'] = $p['time'];
        }

        if (isset($prev)) {
          // climb
          if ($p['ele']) {
            $p['climb'] = $p['ele'] - $prev['ele'];
            if ($p['climb'] > 0) $points['climb'] += $p['climb'];
          }

          // distance
          $distance = gpx_distance($prev['lat'], $prev['lon'], $p['lat'], $p['lon']);
          // Add elevation change (in feet) to distance (in miles)
          if ($p['ele']) {
            $distance += abs($p['climb']) * 0.000189393939;
          }

          $points['distance'] += $distance;
          $p['distance'] = $distance;

          // mph
          $mph  = $distance / (($p['time'] - $prev['time']) / 3600 );

          if (!isset($points['min_mph']) || ($mph < $points['min_mph']))
            $points['min_mph'] = $mph;
          if (!isset($points['max_mph']) || ($mph > $points['max_mph']))
            $points['max_mph'] = $mph;

          $p['mph'] = $mph;
        }
      }

      $points['data'][] = $p;
      $prev = $p;
    }
  }

  if (isset($points['start_time'])) {
    if($points['time'] = $points['end_time'] - $points['start_time']) {
      $points['mph']  = $points['distance'] / ($points['time'] / 3600 );
    }
  }

  return $points;
}

function gpx_minmax($items, $return_values = false, $type = null) {
  $values = array();

  for ($i = 0; $i < $items->length; $i++) {
    $x = gpx_callbacks($items->item($i)->nodeValue, $items, $i);
    if (!isset($values['min']) || ($x < $values['min'])) $values['min'] = $x;
    if (!isset($values['max']) || ($x > $values['max'])) $values['max'] = $x;
    if ($return_values) $values['data'][] = $x;
  }

  return $values;
}

function gpx_distance($lat1, $lon1, $lat2, $lon2) {
  $r = 3958.75; // Earth's radius, in miles
  $d_lat = deg2rad($lat2 - $lat1);
  $d_lon = deg2rad($lon2 - $lon1);

  $a = sin($d_lat/2) * sin($d_lat/2) +
       cos(deg2rad($lat1)) * cos(deg2rad($lat2)) *
       sin($d_lon/2) * sin($d_lon/2);
  $c = 2 * atan2(sqrt($a), sqrt(1-$a));
  return $r * $c;
}

function gpx_callbacks($element, $items, $i, $type=null) {
  if (!$type) $type = $element->nodeName;

  switch ($type) {

    case 'time':
      return strtotime(str_replace('T', ' ', $element->nodeValue));

    case 'ele':
      return $element->nodeValue * 3.2808399;  // Convert meters to feet

    default:
      return $element->nodeValue;
  }
}
