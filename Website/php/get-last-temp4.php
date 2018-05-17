<?php

	require('include.php');
	require ('ifttt-php.class.php');

	$db_handler  = DBconnect();

	
	
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
		  echo 'maker!';		
		$maker = new Maker('temperature_requested', 'iEdUUT2AS3_NTHnHjjPScs6R9cZhaVKh1ObEAxLP2nV');

		// set values to send, optional
		$values = array(
		  'value1' => $outRet,
		  'value2' => 'my second value',
		  'value3' => 'my third value'
		);
		 echo 'set_values!';	
		$maker->set_values($values);

		
		 echo 'trigger!';	
		// trigger the ifttt event
		if($maker->trigger()){
		  echo 'event triggered!';
		}
	
	} catch (PDOException $e) {
		echo $e;
		//$db_handler = false;
	}
	
	
?>