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
		$match['name'] = trim($person->given_names . ' ' . $person->family_name);
		
		$uri = trim($person->id);
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
		
		//DAAO integration
		$keyword = urlencode($keyword);
		$url = 'http://api-daao.hol.ly/v1/person/?name=' . $keyword . '&dob=';
		
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
			return json_decode($content);
		}
	}
	catch (Exception $e) {
		header('HTTP/1.1 400 ' . $e->getMessage());
	}
}