<?php
require('include.php');

//session_start();
//if(empty($_SESSION["authenticated"]) || $_SESSION["authenticated"] != 'true') {
//	header('Location: login.php');
//	exit();
//}

?>

var table;
var previousValue;
var noFireEvent = false;

function init() {
	
	// initialize warning dialog
	$("#dialog").dialog({ 
		autoOpen: false,
		buttons: [{
      text: "Ok",
      click: function() {$( this ).dialog( "close" );}
		}],
	});
	
	// initialize tabs (jQueryUI)
	// i$("#tabs").tabs(
	/*{
		activate: function( event, ui ) {
			table.ajax.reload();
		}
	}*/
	// i);
	// Inizializzo temp gauge
	//$(".tempGauge0").tempGauge({width:50, borderWidth:4, borderColor:"#adadad", fillColor:"red", showLabel:true});
	//$(".tempGauge1").tempGauge({width:50, borderWidth:4, borderColor:"#adadad", fillColor:"green", showLabel:true});
	//$(".tempGauge2").tempGauge({width:50, borderWidth:4, borderColor:"#adadad", fillColor:"#dcdcdc", showLabel:true});
	
	// draw the runtime tab
	$("#runtime-fragment").append("<?php
		for($i = 0; $i < OUTPUTS_NUMBER; $i++) {
			echo '<div class=\"out-line row\">';
			echo '<div class=\"out-label col-md-2 col-xs-2\">' . $outputs[$i]["name"] . '</div>';
			echo '<div id=\"slider' . $i . '\" class=\"out-slider col-md-10 col-xs-10\"></div>';
			echo '</div>';
		}
	?>
	");
	
	// initialize the sliders
	<?php
		for($i = 0; $i < OUTPUTS_NUMBER; $i++) {
			echo '$( "#slider' . $i . '" ).slider({';
	?>
				animate: true,
				orientation: "horizontal",
				min: 0,
				max: 2,
				step: 1,
				start: function (event, ui) {previousValue = ui.value;},
				//change: function (event, ui) {sliderChangeHandler(<?php echo $i ?>, ui, event);}
			}).slider("pips", {
				rest: "label",
				labels: ["OFF", "AUTO", "ON"],
			});
	<?php
		}
	?>
	
	// default the sliders to AUTO (1)
	<?php
		for($i = 0; $i < OUTPUTS_NUMBER; $i++) 
			echo '$( "#slider' . $i . '" ).slider( "value", 1 );';
	?>
	
	// attach change event handler
	<?php
		for($i = 0; $i < OUTPUTS_NUMBER; $i++) 
			echo '$( "#slider' . $i . '" ).on( "slidechange", function( event, ui ) {sliderChangeHandler(' . $i . ', ui, event);} );';
	?>
	
		
	$.getJSON("php/get-last-temp.php", function(data) {  

	//alert(data);
	var tempGauge;
	var tempGaugeValue;
	var tempColor;
	
	$.each(data, function(key, value){		
		
		tempGauge = ".tempGauge" + value[0] ;
		tempGaugeValue = value[2] + "&deg;C " ;
		console.log(tempGauge);
		console.log(tempGaugeValue);		
		//debugger;
		tempColor ="#5DB85C";
		if (value[0]=="0") {tempColor="#D9534F"};
		if (value[0]=="1") {tempColor="#E89821"};
		if (value[0]=="2") {tempColor="#337AB7"};
		
		$(tempGauge).html(tempGaugeValue);
		$(tempGauge).tempGauge({width:100, borderWidth:4, labelSize:18, borderColor:"#adadad", fillColor:tempColor ,showScale:false, showLabel:true, minTemp: -30, maxTemp: 50, defaultTemp: value[2]});
		$(tempGauge + "Date").html(value[1].replace(" ", "<br />"));
		
		<?php
			//echo '$(".tempGauge' . value[0] . '").html("31&deg;C")';
		?>
	});
	
	/*
	var dataPoints = [];
	$.each(data, function(key, value){
		dataPoints.push({x: value[0], y: parseInt(value[1])});
	});

	  var chart = new CanvasJS.Chart("chartContainer",{
			title:{
				text:"Rendering Chart with dataPoints from External JSON"
			},
			data: [{
			type: "line",
				dataPoints : dataPoints,
			}]
		});
		chart.render();
		*/
	});



	// initialize the WeekCalendar object
	initWeekCalendar();
	
	// initialize the DataTable object
	var maxHeight = $(window).height() - $('h1').outerHeight(true) - $('#tabs').outerHeight(true) - 50;
	table = $('#events-table').DataTable({
		pageLength: 10,
		lengthMenu: [5, 10, 25],
		serverSide: true,
		ajax: {
			url: 'php/log-data.php',
			type: 'POST'
		},
		"language": {
                "url": "js/dataTables.italian.lang"
            },
		"autoWidth": false,
		"columns": [
			{ "width": "40px",
			  "class": "dt-body-center",
			  "render": function ( data, type, full, meta ) {
				return '<img src=images/' + data + 'n.png />'; }},
			{ "width": "170px",
			  "type": "date" },
			null
		]
	});	
	
	// initialize the DataTable object	
	table = $('#temp-table').DataTable({
		pageLength: 10,
		lengthMenu: [5, 10, 25],
		serverSide: false,
		ajax: {
			url: 'php/get-log-temp.php',
			type: 'POST'
		},
		"language": {
                "url": "js/dataTables.italian.lang"
            },
		"autoWidth": false,
		"columns": [
			{ "width": "200px",			  
			   "render": function ( data, type, full, meta ) {
				   var descMeteo='';
				   
				   if (data=='0') {descMeteo='Casetta'};
				   if (data=='1') {descMeteo='Esterno'};
				   if (data=='2') {descMeteo='Int. inferiore'};
				   if (data=='3') {descMeteo='Int. superiore'};
				   if (data=='4') {descMeteo='CED'};
				
				return descMeteo  + ' (' + data + ')'; }},			  
			{ "width": "170px",
			  "type": "date" },
			null
		]
	});	
	
	// a key was pressed on the keyboard
	$(document).keyup(function(event) {
	
		// DEL key, delete all the selected events
		if(event.keyCode == 46) delSelectedEvents();
		
		// CTRL+A shortcut, select all the events
		else if((event.ctrlKey) && (event.keyCode == 65)) {
			selectAllEvents();
			event.preventDefault();
			event.stopPropagation();
			event.returnValue = false;
		}
	});

	// auto-update the GUI every 15 seconds
	updateGUI();
	setInterval(updateGUI, 15000);
}

function initWeekCalendar() {
	
    $('#calendar').weekCalendar({
		
		// basic configuration
		date: '2000/01/07',
		hourLine: true,
		timeslotsPerHour: 12,
		timeslotHeight: 12,
		businessHours: {
			start: 0,
			end: 0,
			limitDisplay: false
		},
		use24Hour: true,
		showAsSeparateUser: true,
		showHeader: false,
		buttons: false,
		//height: function($calendar) {
		//	return $(window).height() - $('h1').outerHeight(true) - $('#tabs').outerHeight(true) - 20;
		//},
		
		// configure the number of outputs ("users")
		users: [
		<?php
			for($i = 1; $i < OUTPUTS_NUMBER; $i++) echo "'V" . $i . "',";
			echo "'V" . $i . "'";
		?>],
		
		// fetch data from a service.php page
		data: function(start, end, callback) {
			$.ajax({
				url: 'php/service.php?action=get_events',
				dataType: 'json',
				success: function(result) {
										
					callback(result);
					// auto-update the GUI every 15 seconds
					updateGUI();
				},
				error: function( data ) {
					alert( "ERROR:  " + data.responseText );
				}
			});
		},
		
		// before adding a new event, store it in the DB
		beforeEventNew: function($event, ui) {

			var start_ms, end_ms, user_id;
			start_ms = ui.calEvent.start.getTime();
			end_ms = ui.calEvent.end.getTime();
			user_id = ui.calEvent.userId;

			$.ajax({		
				url: 'php/service.php?action=insert_event&start=' + start_ms + '&end=' + end_ms + '&user_id=' + user_id,
				dataType: 'json',
				async: false,
				success: function(result) {
					ui.calEvent.id = result.event_id;
					table.ajax.reload();
					// auto-update the GUI every 15 seconds
					updateGUI();
				},
				error: function( data ) {
					alert( "ERROR:  " + data );
				}
			});
		},
		
		eventResize: function(calEvent, element) {
			updateEvent(calEvent);
        },
		
		eventDrop: function(calEvent, element) {
			updateEvent(calEvent);		
        },
		
		// a click on an event select/deselect it
		eventClick: function(calEvent, element, dayFreeBusyManager, calendar, clickEvent) {
			
			if(typeof element.data('selected') === 'undefined') element.data('selected', false);
			if(element.data('selected') == false) element.data('selected', true);
			else element.data('selected', false);
			renderEvent(element.data('calEvent'), element);
        },
		
		// callback when an event must be rendered
		eventRender: function(calEvent, $event) {
			
			calEvent.title = "";
			renderEvent(calEvent, $event);
			
			
        },
	});	
}


// -------------------- GUI FUNCTIONS --------------------
function updateGUI() {
	
	// update the tabe
	table.ajax.reload();
	
	var eventCount = $(".wc-cal-event").size();
	$("#franchino-prog-badge").html(eventCount)
	;
	if (eventCount >0){
		$("#franchino-prog").html($("#franchino-prog-hidden").val() + "; sono state trovate: " + eventCount + " programmazioni.");
	}
	else
	{
		$("#franchino-prog").html($("#franchino-prog-hidden").val() + "; nessuna programmazione inserita");
	}
	
	// update the sliders
	<?php
		for($i = 0; $i < OUTPUTS_NUMBER; $i++) { 
		?>
		$("#slider<?php echo $i ?>").slider( "value", 1);		
		
		
		<?php } ?>
}
function updateGUI_Old() {
	
	// update the tabe
	table.ajax.reload();
	
	var eventCount = $(".wc-cal-event").size();
	$("#franchino-prog-badge").html(eventCount)
	
	if (eventCount >0){
		$("#franchino-prog").html($("#franchino-prog-hidden").val() + "; sono state trovate: " + eventCount + " programmazioni.");
	}
	else
	{
		$("#franchino-prog").html($("#franchino-prog-hidden").val() + "; nessuna programmazione inserita");
	}
	
	// update the sliders
	<?php
		for($i = 0; $i < OUTPUTS_NUMBER; $i++) { 
		?>
				
		$.ajax({		
			url: 'php/service.php?action=get_out_status&output_id=<?php echo $i ?>',
			dataType: 'json',
			async: false,
			success: function(result) {
				noFireEvent = true;
				
				// managed_by AUTO
				if(result.managed_by == 2) $("#slider<?php echo $i ?>").slider( "value", 1);
								
				// managed by WEB or SWITCH, configure the actual status
				else {
					if(result.status == 0) $("#slider<?php echo $i ?>").slider( "value", 0);
					else $("#slider<?php echo $i ?>").slider( "value", 2);
					
					// managed by SWITCH, disable the slider
					if(result.managed_by == 0) $("#slider<?php echo $i ?>").slider("option", "disabled", true);
				}
			}});
		
		<?php } ?>
}

// -------------------- SLIDER FUNCTIONS --------------------
function sliderChangeHandler(id, ui, event) {

	if(!noFireEvent) {
		
	}
	else noFireEvent = false;
}
function sliderChangeHandler_Old(id, ui, event) {

	if(!noFireEvent) {
		$.ajax({		
			url: 'php/service.php?action=change_out&output_id=' + id + '&status_id=' + ui.value,
			dataType: 'json',
			success: function(result) {
				if(result.code != 0) {
					$("#dialog").text("Unable to change output status: " + result.message);
					$("#dialog").dialog("open");
					noFireEvent = true;
					$('#' + event.target.id).slider( "value", previousValue);
				}
			},
			error: function(data) {
				
				$("#dialog").text("Unable to call irrighino");
				$("#dialog").dialog("open");
				noFireEvent = true;
				$('#' + event.target.id).slider( "value", previousValue);
			}
		});	
	}
	else noFireEvent = false;
}

// -------------------- CALENDAR FUNCTIONS --------------------

// render an event
function renderEvent(calEvent, element) {
	
	selected = element.data('selected');
	position_top = element.position().top;
	baseColor = getEventColor(calEvent);
	borderColor = getSelectedEventColor(calEvent);
	
	element.css('backgroundColor', baseColor);
	element.find('.wc-time').css({'backgroundColor': baseColor, 'border':'0px', 'font-size':'0px'});
	
	if(selected == true) {
		position_top = position_top - 2;
		element.css({'top' : position_top, 'left' : '-2px', 'border-style' : 'solid', 'border-color' : borderColor, 'border-width' : '2px'});
	}		
	else {
		position_top = position_top + 2;
		element.css({'top' : position_top, 'left' : '0px', 'border-style' : 'hidden'});
	}
}

// select all the events (CTRL+A)
function selectAllEvents() {
	
	$('#calendar').find('.wc-cal-event').each(function() {
		$(this).data('selected', true);
		renderEvent($(this).data('calEvent'), $(this));
	});
}	

// update an event when moved/resized
function updateEvent(calEvent) {
	
	var start_ms, end_ms, user_id, event_id;
	start_ms = calEvent.start.getTime();
	end_ms = calEvent.end.getTime();
	user_id = calEvent.userId;		
	event_id = calEvent.id;
	
	$.ajax({		
		url: 'php/service.php?action=update_event&event_id=' + event_id + '&start=' + start_ms + '&end=' + end_ms + '&user_id=' + user_id,
		dataType: 'json',
		async: false,
		success: function(result) {
			table.ajax.reload();
		},
		error: function( data ) {
			alert( "ERROR:  " + data );
		}
	});		
}

// delete an event
function delSelectedEvents() {
	
	$('#calendar').find('.wc-cal-event').each(function() {
		calEventData = $(this).data('calEvent')
		console.log("Analyzing event with id: " + calEventData.id);
		if($(this).data('selected') == true) {
			console.log("Event selected");
			$.ajax({		
				url: 'php/service.php?action=delete_event&event_id=' + calEventData.id,
				dataType: 'json',
				async: false,
				success: function(result) {
					$('#calendar').weekCalendar("removeEvent", calEventData.id);
					table.ajax.reload();
				},
				error: function( data ) {
					alert( "ERROR:  " + data );
				}
			});				
		}
		else console.log("Event not selected");
	});
	// auto-update the GUI every 15 seconds
	updateGUI();
}

// return the color based on the output number
function getEventColor(calEvent)  {

	switch(calEvent.userId) {
	<?php
		for($i = 0; $i < OUTPUTS_NUMBER; $i++) {
			echo "case $i: return '" . $outputs[$i]["baseColor"] . "'; break;";
		}
	?>
	}
}

// return the border color based on the output number
function getSelectedEventColor(calEvent)  {

	switch(calEvent.userId) {
	<?php
		for($i = 0; $i < OUTPUTS_NUMBER; $i++) {
			echo "case $i: return '" . $outputs[$i]["borderColor"] . "'; break;";
		}
	?>
	}
}

