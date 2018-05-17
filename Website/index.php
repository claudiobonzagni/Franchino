<?php
//require_once('authenticate.php');
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<head>
  <meta http-equiv="content-type" content="text/html; charset=UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Franchino - Irrigazione automatica</title>
  <link rel='stylesheet' type='text/css' href='css/jquery-ui.css' />
  <link rel='stylesheet' type='text/css' href='css/jquery-ui-slider-pips.css' />
  <link rel="stylesheet" type="text/css" href="css/jquery.dataTables.css" />
  <link rel='stylesheet' type='text/css' href='css/jquery.weekcalendar.css' />

  <link rel='stylesheet' type='text/css' href='css/bootstrap.min.css' />
  <link rel='stylesheet' type='text/css' href='css/main.css' />
  <link rel='stylesheet' type='text/css' href='css/switches.css' />
  <link rel='stylesheet' type='text/css' href='css/buttons.css' />
  <link rel='stylesheet' type='text/css' href='css/fieldsets.css' />

  <script type='text/javascript' src='js/jquery-1.11.2.min.js'></script>
  <script type='text/javascript' src='js/jquery-migrate-1.2.1.min.js'></script>
  <script type='text/javascript' src='js/jquery-ui-1.12.1.js'></script>
  <script type='text/javascript' src='js/bootstrap.min.js'></script>

  <script type="text/javascript" src="js/jquery-ui-slider-pips.js"></script>
  <script type="text/javascript" src="js/date.js"></script>
  <script type='text/javascript' src='js/jquery.weekcalendar.js'></script>
  <script type="text/javascript" src="js/jquery.dataTables.js"></script>
 
  <script type='text/javascript' src='php/irrighino-js.php'></script>
  
  <script type='text/javascript'>

    $(document).ready(function () {
      init();
    });

  </script>

</head>
<body>
  <nav class="navbar navbar-default navbar-fixed-top">
    <div class="container-fluid">
      <div class="navbar-header">
        <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false" aria-controls="navbar">
          <span class="sr-only">Toggle navigation</span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
        </button>
        <div>  <a class="navbar-brand" href="index.html"><span class="glyphicon glyphicon-tint " aria-hidden="true"></span>  Franchino ®</a></div>

      </div>
      <div id="navbar" class="navbar-collapse collapse">
        <!-- Nav tabs -->
        <ul class="nav nav-pills nav-fill franchino-nav">
          <li class="active"><a href="#panel_1" data-toggle="tab">Valvole</a></li>
          <li><a href="#panel_2" data-toggle="tab">Programmazione <span class="badge" id="franchino-prog-badge"></span></a></li>
          <li><a href="#panel_3" data-toggle="tab">Eventi</a></li>
        </ul>



      </div>

    </div>
  </nav>
  <!-- <img src='images/logo.jpg' /> -->
  <div class="container-fluid">
    <!--<div class="row">
      <div class="col-md-1 col-sm-1 col-xs-0 hidden-xs franchino-logo"><div class="pull-left"><img src='images/logo.png' /></div></div>
      <div class="col-md-9 col-sm-9 col-xs-12 "><h1 class="text-center">Franchino - Irrigazione</h1></div>
      <div class="col-md-2 col-sm-2 col-xs-0 hidden-xs"><h1 class="text-right">v.1.1</h1></div>
    </div>-->
    <!-- Tab panes -->
    <div class="tab-content">
      <div class="tab-pane active" id="panel_1">
        <input type="hidden" id="franchino-prog-hidden" value="Sistema di irrigazione automatica tramite Arduino" />
        <div class=" bg-primary franchino-info" id="franchino-prog">Sistema di irrigazione automatica tramite Arduino.</div>


        <div class="panel-group franchino-panel">
          <div class="panel panel-default">
            <div class="panel-heading"><strong>Gestione delle valvole</strong></div>

            <div class="panel-body">

              <div class="row">
                <div class="col-md-12">
                  <div id="runtime-fragment">
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>

      </div>
      <div class="tab-pane" id="panel_2">
        <div id="setup-fragment">
          <div id="setup-fragment-button">
            <button class="btn btn-warning" onclick="selectAllEvents();">Seleziona tutto</button>
            <button class="btn btn-warning" onclick="delSelectedEvents();">Elimina</button>
          </div>
          <div id='calendar'></div>
        </div>
      </div>
      <div class="tab-pane" id="panel_3">
        <div id="events-fragment">

          <div class="panel-group franchino-panel">
            <div class="panel panel-default">
              <!-- <div class="panel-heading">Gestione delle valvole</div>-->
              <div class="panel-body">
                <table id="events-table" class="display" data-order='[[ 1, "desc" ]]'>
                  <thead>
                    <tr>
                      <th>Tipo</th>
                      <th>Data</th>
                      <th>Descrizione</th>
                    </tr>
                  </thead>
                  <tbody>
                    <tr>
                      <td>A</td>
                      <td>Row 1 Data 2</td>
                      <td>Row 1 Data 3</td>
                    </tr>
                    <tr>
                      <td>B</td>
                      <td>Row 2 Data 2</td>
                      <td>Row 2 Data 3</td>
                    </tr>
                  </tbody>
                </table>
              </div>
            </div>
          </div>

        </div>
      </div>
    </div>
  </div>
  <div id="dialog" title="Warning"></div>

</body>
</html>