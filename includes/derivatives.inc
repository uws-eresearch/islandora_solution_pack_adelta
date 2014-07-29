<?php

/**
 * @file
 * Functions deriving images for objects.
 */

/**
 * Creates the derivatives for this content model type.
 *
 * @param FedoraObject $object
 *   The object to add derivatives to.
 */
function islandora_adelta_create_all_derivatives(FedoraObject $object) {
  $mime_detect = new MimeDetect();
  if (!isset($object['OBJ'])) {
    drupal_set_message(t('Could not create image derivatives for %s.  No image file was uploaded.', array('%s' => $object->id)), 'error');
    return "";
  }
  $ext = $mime_detect->getExtension($object['OBJ']->mimeType);
  $file_name = str_replace(':', '-', $object->id);

  // Create a file object we can save.
  $file_uri = file_create_filename("{$file_name}OBJ.{$ext}", 'temporary://');
  $file = new stdClass();
  $file->uri = $file_uri;
  $file->filename = $file_name;
  $file->filemime = $object['OBJ']->mimeType;
  // Temporary.
  $file->status = 0;
  $object['OBJ']->getContent($file_uri);
  $original_file = file_save($file);

  $tn_file = file_copy($original_file, 'temporary://' . $file_name . 'TN.' . $ext);
  
  // Now create thumbnail
  $args = array();
  $args[] = '-thumbnail ' . escapeshellarg('150x150^'); 
  $args[] = '-background indigo';
  $args[] = '-gravity center';
  $args[] = '-extent ' . escapeshellarg('150x150'); 
  
  $tn_path = islandora_adelta_imagemagick_convert('temporary://' . $file_name . 'TN.' . $ext, 
    'temporary://' . $file_name . 'TN.' . $ext, $args);
  
  if($tn_path) {
  	islandora_adelta_add_datastream($object, 'TN', $tn_path);
  }
  
  $medium_file = file_copy($original_file, 'temporary://' . $file_name . 'MEDIUM.' . $ext);
  
  // Now create medium image
  unset($args);
  $args = array('-resize ' .  escapeshellarg('460x345>'));
  
  $medium_path = islandora_adelta_imagemagick_convert('temporary://' . $file_name . 'MEDIUM.' . $ext,
  		'temporary://' . $file_name . 'MEDIUM.' . $ext, $args);
  
  if($medium_path) {
  	islandora_adelta_add_datastream($object, 'MEDIUM_SIZE', $medium_path);
  }
  
  file_delete($original_file);
}


/**
 * Calls imagemagick's convert command with the given arguments.
 *
 * @param string $src
 *   The source file uri to convert from.
 * @param string $dest
 *   The destination file uri to convert into.
 *   
 * @return string
 *   The destination file path if successful otherwise FALSE.
 */
function islandora_adelta_imagemagick_convert($src, $dest, $args) {
	$src = drupal_realpath($src);
	$dest = drupal_realpath($dest);
	$context = array(
			'source' => $src,
			'destination' => $dest,
	);
	drupal_alter('imagemagick_arguments', $args, $context);
	/**
	 * To make use of ImageMagick 6's parenthetical command grouping we need to make the $source image the first parameter and $dest the last.
	 * See http://www.imagemagick.org/Usage/basics/#cmdline
	*/
	$command = escapeshellarg($src) . ' ' . implode(' ', $args) . ' ' . escapeshellarg($dest);
	$output = '';
	$ret = -1;
	if (_imagemagick_convert_exec($command, $output, $ret) !== TRUE) {
		$message = 'imagemagick convert failed to create derivative<br/>Error: @ret<br/>Command: @command<br/>Output: @output';
		$variables = array('@ret' => $ret, '@command' => $command, '@output' => $output);
		watchdog('islandora_large_image', $message, $variables, WATCHDOG_ERROR);
		return FALSE;
	}
	return $dest;
}

/**
 * Scales the given image.
 *
 * @param object $file
 *   The image file to scale.
 * @param int $width
 *   The width to scale the derived image to.
 * @param int $height
 *   The height to scale the derived image to.
 *
 * @return bool
 *   TRUE if successful, FALSE otherwise.
 */
function islandora_adelta_scale_image($file, $width, $height) {
  $real_path = drupal_realpath($file->uri);
  $image = image_load($real_path);
  if (!empty($image)) {
    $scale = image_scale($image, $width, $height, TRUE);
    if ($scale) {
      return image_save($image);
    }
  }
  return FALSE;
}

/**
 * Adds a datastream to an object.
 *
 * @param FedoraObject $object
 *   The object to add a datastream to.
 * @param string $dsid
 *   The datastream ID to be added.
 * @param string $file
 *   The URI of the file to be added as the datastream content.
 */
function islandora_adelta_add_datastream(FedoraObject $object, $dsid, $file) {
  try {
    $ds = $object->constructDatastream($dsid, 'M');
    $ds->label = $dsid;
    $ds->mimeType = $object['OBJ']->mimeType;
    $ds->setContentFromFile(drupal_realpath($file));
    $object->ingestDatastream($ds);
    drupal_unlink($file);
  }
  catch (exception $e) {
    drupal_set_message(t('@message', array('@message' => check_plain($e->getMessage()))), 'error');
  }
}
