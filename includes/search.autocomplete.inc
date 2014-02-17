<?php
/**
 * Menu callback; Retrieve a JSON object containing autocomplete suggestion for search.
 */
function islandora_search_autocomplete($string) {
	//$module_path = drupal_get_path('module', 'islandora_adelta');
	//drupal_add_js($module_path . '/js/test_me.js');
	$matches = array();
	/*$people = variable_get('input_search_people', array());
	 foreach($people as $phrase){
	if(stristr($phrase, $string)){
	$matches[$phrase] = $phrase;
	}
	}*/
	$results = lookUpPeople($string);
	
	foreach($results as $person){
		//$result_metadata = $person->result-metadata;
		//$data = json_encode($person);
		$match = array();
		
		$metadata_array = json_decode(json_encode($person), true);
		$all_data = $metadata_array['result-metadata']['all'];
		
		$id = $all_data['id'];
		$honorific = trim($all_data['Honorific'][0]);
		$given_name = trim($all_data['Given_Name'][0]);
		$family_name = trim($all_data['Family_Name'][0]);
		$email = trim($all_data['Email'][0]);
		$uri = trim($all_data['URI'][0]);
		$personal_uri = trim($all_data['Personal_uri'][0]);
		$dc_identifier = $metadata_array['dc:identifier'];
		$full_name = "";
		if ($honorific)
			$full_name = $full_name.$honorific.' ';
		if ($given_name)
			$full_name = $full_name.$given_name.' ';
		if ($family_name)
			$full_name = $full_name.$family_name;
		/*if ($email)
			$full_name = $full_name.' '.$email;*/
		
		$match['name'] = $full_name;
		
		$key = "";
		$uri_pattern = "/^http:\/\//";
		if($uri){
			$match['uri'] = $uri;
			//$key = $full_name.' <'.$uri.'>';
		}
		elseif($personal_uri){
			$match['uri'] = $personal_uri;
			//$key = $full_name.' <'.$personal_uri.'>';
		}
		else 
			$match['uri'] = $dc_identifier;
			//$key = $full_name.' <'.$dc_identifier.'>';
			
		if(!preg_match($uri_pattern, $match['uri'])){
			$match['uri'] = "http://".$match['uri'];
		}
		
		//$matches[$key] = $full_name; //this can be an array
		
		array_push($matches, $match);
	}
	
	drupal_json_output($matches);
}

function lookUpPeople($keyword) {
	try {
		//$config = $this->getConfig();
		//$url = $config['mint']['url'] . '/Parties_People/opensearch/lookup?searchTerms=' . urlencode($keyword);
		$url = 'http://basset.uws.edu.au:9001/mint/Parties_People/opensearch/lookup?searchTerms=' . urlencode($keyword);
		//\OCP\Util::writeLog("crate_it::lookUpPeople", $url, \OCP\Util::DEBUG);

		// Now call the mint
		$ch = curl_init();
		curl_setopt($ch, CURLOPT_URL, $url);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		$content = curl_exec($ch);
		$result = curl_getinfo($ch);
		curl_close($ch);
			
		if(empty($content))
		{
			return array();
		}
		else {
			$content_array = json_decode($content);
			$results = $content_array->results;
			return $results;
		}
	}
	catch (Exception $e) {
		header('HTTP/1.1 400 ' . $e->getMessage());
	}
}