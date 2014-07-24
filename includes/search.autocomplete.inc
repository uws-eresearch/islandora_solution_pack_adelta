<?php
/**
 * Menu callback; Retrieve a JSON object containing autocomplete suggestion for search.
 */
function adelta_people_lookup($string) {
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
		$match = array();
		$metadata_array = json_decode(json_encode($person), true);
		
		$given_name = trim($metadata_array['structured_val']['given_name']);
		$family_name = trim($metadata_array['structured_val']['family_name']);
		$full_name = "";
		
		if ($given_name)
			$full_name = $full_name.$given_name.' ';
		if ($family_name)
			$full_name = $full_name.$family_name;
		
		$match['name'] = $full_name;
		
		$uri = trim($metadata_array['id']);
		$uri_pattern = "/^http:\/\//";
		if($uri){
			$match['uri'] = $uri;
		}
			
		if(!preg_match($uri_pattern, $match['uri'])){
			$match['uri'] = "http://".$match['uri'];
		}
		
		array_push($matches, $match);
	}
	
	
	drupal_json_output($matches);
}

function lookUpPeople($keyword) {
	try {
		//$config = $this->getConfig();
		//$url = $config['mint']['url'] . '/Parties_People/opensearch/lookup?searchTerms=' . urlencode($keyword);
		//$url = 'http://research-data.uws.edu.au:9001/mint/Parties_People/opensearch/lookup?searchTerms=' . urlencode($keyword);
		
		//Tokenize the string by comma
		$names = explode(',', $keyword);
		
		if(count($names) > 1){
			$params = 'fname=' . $names[1] . '&gname=' . $names[0];
		}
		else{
			$params = 'fname=&gname=' . $names[0];
		}
		
		//FML integration. 
		$url = 'http://fml.data.ac.nz/fillmylist?entity_type=person&output=json&' . $params . '&email=&orcid=&equipment=&institution=';
		//$url = 'http://localhost/OR2014-chalege/web_root/fillmylist?method=generic&entity_type=person&output=json&name=' . urlencode($keyword);
		
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
			$results = $content_array->items;
			return $results;
		}
	}
	catch (Exception $e) {
		header('HTTP/1.1 400 ' . $e->getMessage());
	}
}