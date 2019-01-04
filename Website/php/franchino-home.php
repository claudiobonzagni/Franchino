<?php
require('include.php');

//session_start();
//if(empty($_SESSION["authenticated"]) || $_SESSION["authenticated"] != 'true') {
//	header('Location: login.php');
//	exit();
//}

?>
 	var currentMeteoStation = 0;
    var maxMeteoStation = 3;


    function init() {

      // initialize warning dialog
      $("#dialog").dialog({
        autoOpen: false,
        buttons: [{
          text: "Ok",
          click: function () { $(this).dialog("close"); }
        }],
      });
     

      // auto-update the GUI every 5 seconds
      updateGUI();
      setInterval(updateGUI, 5000);


    }


    // -------------------- GUI FUNCTIONS --------------------
    function updateGUI() {

      $.getJSON("php/get-last-temp.php", function (data) {

        //alert(data);
        var tempGauge;
        var tempGaugeValue;
        var tempGaugeTitle;
        //debugger;
        var value = data[currentMeteoStation];

        tempGauge = ".tempGauge";
        tempGaugeValue = value[2] + "&deg;C ";

        if (value[0] == '0') { tempGaugeTitle = 'Casetta' };
        if (value[0] == '1') { tempGaugeTitle = 'Esterno' };
        if (value[0] == '2') { tempGaugeTitle = 'Int. inferiore' };
        if (value[0] == '3') { tempGaugeTitle = 'Int. superiore' };
        if (value[0] == '4') { tempGaugeTitle = 'CED' };

        console.log(tempGaugeTitle);
        console.log(tempGauge);
        console.log(tempGaugeValue);

        // Aggiorno valori		   
        $(".tempGaugeTitle").html(tempGaugeTitle);
        $(".tempGauge").html(tempGaugeValue);
        $(tempGauge + "Date").html(value[1]);

        $('.js-gauge--1').kumaGauge('update', {
          value: value[2]
        });
        // Incremento il contatore della stazione meteo
        currentMeteoStation = currentMeteoStation + 1;

        if (currentMeteoStation > maxMeteoStation) {
          currentMeteoStation = 0;
        }
      });
    }

