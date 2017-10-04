<?php

	require('include.php');

	logMessage("purgeOldEvents - started");

	$db_handler  = DBconnect();
	$sql = "DELETE FROM LOG WHERE DATE <= date('now','-" . RETENTION_DAYS . " day')";
	$count = DBexec($db_handler, $sql);
	
	logMessage("purgeOldEvents - completed, deleted $count old events");
	
	logMessage("purgeOldLogTemp - started");

	$db_handler  = DBconnect();
	$sql = "DELETE FROM TEMP_LOG WHERE DATE <= date('now','-" . RETENTION_DAYS_LOG_TEMP . " day')";
	$count = DBexec($db_handler, $sql);
	
	logMessage("purgeOldLogTemp - completed, deleted $count old events");
	
?>