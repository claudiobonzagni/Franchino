<?php

	require('include.php');

	class Response {
		
	
		public $data;
	}
	
	class Data {
		//public $id;
		public $date;
		public $eventId;
		public $eventDesc;
		
	}
	
	$response = new Response();
	
	
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
	$sql .= " UNION ";
	$sql .= "SELECT EVENT_ID, DATE, EVENT_DESC FROM (SELECT EVENT_ID, datetime(DATE, 'localtime') as DATE, EVENT_DESC FROM TEMP_LOG WHERE EVENT_ID=3 ORDER BY DATE DESC LIMIT 1)";
	$sql .= " ORDER BY EVENT_ID ";
		

	//locale_set_default('it-IT');
	//Locale::setDefault('de-DE');
	//setlocale(LC_TIME, 'it_IT');
	//setlocale(LC_TIME, 'ita', 'it_IT');
//setlocale(LC_ALL, 'it_IT.UTF-8'); 
	setlocale(LC_ALL, 'IT'); 
	
	$data = array();
	foreach (DBquery($db_handler, $sql) as $row) {
	
		$event = array();
		array_push($event, $row['EVENT_ID']);
		array_push($event,date("d/m/Y H:i:s", strtotime($row['DATE'])) );
		array_push($event, $row['EVENT_DESC']);
		array_push($event,date("l d F Y H:i:s", strtotime($row['DATE'])) );
		array_push($data, $event);
	}
	
	$response->data = $data;
	$out = json_encode($data);
	header('Content-Type: application/json');
	echo $out;
?>