<?php

	require('include.php');

	
	
	$db_handler  = DBconnect();

	// count the number of records in the DB
	//$sql = "SELECT COUNT(ID) AS NUMRECORDS FROM TEMP_LOG";
//	if(!empty($search_value)) $sql .= " WHERE EVENT_DESC LIKE '%" . 	$search_value ."%'";
	
//	foreach (DBquery($db_handler, $sql) as $row) {
	//	$response->recordsTotal = $row['NUMRECORDS'];
	//	$response->recordsFiltered = $row['NUMRECORDS'];
	//}
	
	$sql = "SELECT EVENT_ID, DATE, EVENT_DESC FROM (SELECT EVENT_ID, datetime(DATE, 'localtime') as DATE, EVENT_DESC FROM TEMP_LOG WHERE EVENT_ID=0 ORDER BY DATE DESC LIMIT 1)";
	$sql .= " UNION ";
	$sql .= "SELECT EVENT_ID, DATE, EVENT_DESC FROM (SELECT EVENT_ID, datetime(DATE, 'localtime') as DATE, EVENT_DESC FROM TEMP_LOG WHERE EVENT_ID=1 ORDER BY DATE DESC LIMIT 1)";
	$sql .= " UNION ";
	$sql .= "SELECT EVENT_ID, DATE, EVENT_DESC FROM (SELECT EVENT_ID, datetime(DATE, 'localtime') as DATE, EVENT_DESC FROM TEMP_LOG WHERE EVENT_ID=2 ORDER BY DATE DESC LIMIT 1)";
	$sql .= " ORDER BY EVENT_ID ";
		
	$outRet = "";
	$stazione="";
	
	foreach (DBquery($db_handler, $sql) as $row) {
			
		if($row['EVENT_ID']=="0") $stazione="Casetta";
		if($row['EVENT_ID']=="1") $stazione="Esterno";
		if($row['EVENT_ID']=="2") $stazione="Interno";			
		
		$outRet .= $stazione . ' T:' . $row['EVENT_DESC'] . ' &deg;C   ';
		$outRet .= date("d/m/Y H:i:s", strtotime($row['DATE'])) . '<br />';
				
	}
	
	
	try {
		$fullURL = "https://maker.ifttt.com/trigger/temperature_requested/with/key/iEdUUT2AS3_NTHnHjjPScs6R9cZhaVKh1ObEAxLP2nV";
	
	// inizialize the cURL library
	$ch = curl_init();
	curl_setopt($ch,CURLOPT_CONNECTTIMEOUT, 10);
	curl_setopt($ch,CURLOPT_RETURNTRANSFER, true);
	curl_setopt($ch,CURLOPT_TIMEOUT, 60);
	//curl_setopt($ch, CURLOPT_USERPWD, ARDUINO_USERNAME . ":" . ARDUINO_PASSWORD);
	
	// set gateway URL and parameters
	curl_setopt($ch,CURLOPT_URL, $fullURL);
	
	// make the request and return the response (or the error message if failed)
	$response = curl_exec($ch);
	if($response == false) $response = curl_error($ch);
	
	curl_close($ch);
	//return $response;
	//https://maker.ifttt.com/trigger/temperature_requested/with/key/iEdUUT2AS3_NTHnHjjPScs6R9cZhaVKh1ObEAxLP2nV
	//header('Content-Type: application/json');
	echo $outRet ;
	} catch (PDOException $e) {
		echo $e;
		//$db_handler = false;
	}
	
	
?>