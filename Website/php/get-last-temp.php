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
	$sql .= " UNION ";
	$sql .= "SELECT EVENT_ID, DATE, EVENT_DESC FROM (SELECT EVENT_ID, datetime(DATE, 'localtime') as DATE, EVENT_DESC FROM TEMP_LOG WHERE EVENT_ID=4 ORDER BY DATE DESC LIMIT 1)";
	$sql .= " ORDER BY EVENT_ID ";
	
	$data = array();

	// data ora correnti
	$curHour=date("H:i");
	$curDay=date("l");
	$d=date("d");
	$curMonth=date("m");
	$curYear=date("Y");

	$curDay=getWeekDay($curDay);
	$curMonth=getMonth($curMonth);
	$curDayDesc="$curDay $d $curMonth $curYear";

	//Aggiungo ora
	$event = array();
	array_push($event, "-1");
	array_push($event,"2");
	array_push($event, $curHour);
	array_push($event,$curDayDesc  );
	array_push($data, $event);

	foreach (DBquery($db_handler, $sql) as $row) {
	
		$d=date("d", strtotime($row['DATE']));
		$dayVal=date("l", strtotime($row['DATE']));
		$monthVal=date("m", strtotime($row['DATE']));
		
		$day=getWeekDay($dayVal);
		$month=getMonth($monthVal);
		//echo $month;
		
		$dayDesc=date("Y H:i", strtotime($row['DATE']));
		//echo $dayDesc;
	
		$event = array();
		array_push($event, $row['EVENT_ID']);
		array_push($event,date("d/m/Y H:i:s", strtotime($row['DATE'])) );
		array_push($event, $row['EVENT_DESC']);
		array_push($event,"$day $d $month $dayDesc"  );
		array_push($data, $event);
	}
	
	


	$response->data = $data;
	$out = json_encode($data);
	header('Content-Type: application/json');
	echo $out;
	
	
function getWeekDay($fname) {
	
	$retValue=$fname;

    if ($retValue=="Monday") $retValue="lunedì";
    if ($retValue=="Tuesday") $retValue="martedì";
    if ($retValue=="Wednesday") $retValue="mercoledì";
    if ($retValue=="Thursday") $retValue="giovedì";
    if ($retValue=="Friday") $retValue="venerdì";
    if ($retValue=="Saturday") $retValue="sabato";
    if ($retValue=="Sunday") $retValue="domenica";
	
    return $retValue;
}

function getMonth($fname) {
	
	$retValue=$fname;

    if ($retValue=="01") $retValue="gennaio";
    if ($retValue=="02") $retValue="febbraio";
    if ($retValue=="03") $retValue="marzo";
	if ($retValue=="04") $retValue="aprile";
	if ($retValue=="05") $retValue="maggio";
	if ($retValue=="06") $retValue="giugno";
	if ($retValue=="07") $retValue="luglio";
	if ($retValue=="08") $retValue="agosto";
	if ($retValue=="09") $retValue="settembre";
	if ($retValue=="10") $retValue="ottobre";
	if ($retValue=="11") $retValue="novembre";
	if ($retValue=="12") $retValue="dicembre";
	
    return $retValue;
}

?>