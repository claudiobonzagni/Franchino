<?php
//Es.: http://localhost/irrighino/php/log-temp.php?temp=25,5&v=1
//Es.: http://192.168.178.185/sd/irrighino/php/log-temp.php?temp=25,5&v=1
	require('include.php');
	
	
	if($argv[1]){
		
		echo "prendo argomenti ";
		echo $argv[1];
		
		$temp=$argv[1];
		
		if($argv[2]){
			
			echo "arg2: ";
			echo $argv[2];
			
			$valv=$argv[2];
		}
	}
	else
	{
		echo "prendo query string ";
		$temp= $_GET['temp'];
		$valv= $_GET['v'];
	
	}
echo "temp: ";
echo $temp;
echo "valv: ";
echo $valv;
	logMessage("log-temp - started with argument $argv[1]");

	$db_handler  = DBconnect();
	
	logTemp($db_handler, $valv, $temp);
			
	logMessage("log-temp - completed");
	echo "OK from log-temp.php !!!";
	echo "log-temp - started with argument $argv[1]";
	
?>