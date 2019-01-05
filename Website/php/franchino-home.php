<?php
require('include.php');

//session_start();
//if(empty($_SESSION["authenticated"]) || $_SESSION["authenticated"] != 'true') {
//	header('Location: login.php');
//	exit();
//}

?>
  var currentMeteoStation = 0;
  var maxMeteoStation = 5;

  function init() {

    // initialize warning dialog
    $("#dialog").dialog({
      autoOpen: false,
      buttons: [{
        text: "Ok",
        click: function () { $(this).dialog("close"); }
      }],
    });

    //impostazioni gauge
    $('.js-gauge--1').kumaGauge({
      // The radius of the arc
      radius: 100,

      // The padding on the top and bottom of the gauge
      paddingX: 10,

      // The padding on the left and right of the gauge
      paddingY: 10,

      // The width of the gauge itseld
      gaugeWidth: 30,

      // The fill of the gauge, this can be a solid color or a gradient
      fill: '0-#2fbec7:0-#fdbe37:50-#fa4133:100',

      // The fill of the gauge background, this can be a solid color or a gradient
      gaugeBackground: '#f4f4f4',

      // The fill of the canvas, this can be a solid color or a gradient
      background: '#fff',

      // Show or hide the needle, 
      // if true the value label shows half of the range
      // if false the value label shows the value
      showNeedle: true,

      // The speed of the animation in miliseconds
      animationSpeed: 500,

      // The minimum value of the gauge
      min: -20,

      // The maximum value of the gauge
      max: 40,

      title: {

        // show or hide this label
        display: false,

        // String the value of the title
        value: 'titolo',

        // The font family of this label
        fontFamily: 'Verdana',

        // The font color of this label
        fontColor: '#000',

        // Integer of The font size of this label (without px)
        fontSize: 20,

        // The font weight of this label
        fontWeight: 'normal'
      },
      label: {

        // show or hide this label
        display: true,

        // The value of the left (minimum) label
        left: 'Min',

        // The value of the right (maximum) label
        right: 'Max',

        // The font family of this label
        fontFamily: 'Verdana',

        // The font color of this label
        fontColor: '#000',

        // Integer of The font size of this label (without px)
        fontSize: 12,

        // The font weight of this label
        fontWeight: 'normal'
      }
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
      var tempGaugeDate;
      var tempGaugeValue;
      var tempColor;
      var tempGaugeTitle;
      //debugger;
      var value = data[currentMeteoStation];

      tempGauge = ".tempGauge";

      if (value[0] == '-1') {
        //ora corrente
        tempGaugeValue = value[2];
        $('.js-gauge--1').hide();
        tempGaugeDate="";
      }
      else {
        tempGaugeValue = value[2] + "&deg;C ";
        $('.js-gauge--1').show();
        tempGaugeDate= value[3];
      }

      if (value[0] == '-1') { tempGaugeTitle = value[3] };
      if (value[0] == '0') { tempGaugeTitle = 'Casetta' };
      if (value[0] == '1') { tempGaugeTitle = 'Esterno' };
      if (value[0] == '2') { tempGaugeTitle = 'Interno giù' };
      if (value[0] == '3') { tempGaugeTitle = 'Interno su' };
      if (value[0] == '4') { tempGaugeTitle = 'CED' };

      // Imposto i colori
      tempColor = "#5DB85C";
      if (value[0] == "0") { tempColor = "#D9534F" };
      if (value[0] == "1") { tempColor = "#E89821" };
      if (value[0] == "2") { tempColor = "#337AB7" };

      console.log(tempGaugeTitle);
      console.log(tempGauge);
      console.log(tempGaugeValue);
      console.log(tempColor);

      // Aggiorno valori		   
      $(".tempGaugeTitle").html(tempGaugeTitle);
      $(".tempGauge").html(tempGaugeValue);
      $(tempGauge + "Date").html(tempGaugeDate);

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


