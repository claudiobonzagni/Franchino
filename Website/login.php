<?php
$username = null;
$password = null;

if ($_SERVER['REQUEST_METHOD'] == 'POST') {

	if(!empty($_POST["username"]) && !empty($_POST["password"])) {
		$username = $_POST["username"];
		$password = $_POST["password"];
		
		if($username == 'admin' && $password == 'Banano73') {
			session_start();
			$_SESSION["authenticated"] = 'true';
			header('Location: index.php');
		}
		else {
			header('Location: login.php');
		}
		
	} else {
		header('Location: login.php');
	}
} else {
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<head>
  <meta http-equiv="content-type" content="text/html; charset=UTF-8" /
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Franchino - Irrigazione automatica</title>
  <link rel='stylesheet' type='text/css' href='css/jquery-ui.css' />


  <link rel='stylesheet' type='text/css' href='css/bootstrap.min.css' />
  <link rel='stylesheet' type='text/css' href='css/main.css' />
  <link rel='stylesheet' type='text/css' href='css/switches.css' />
  <link rel='stylesheet' type='text/css' href='css/buttons.css' />
  <link rel='stylesheet' type='text/css' href='css/fieldsets.css' />

  <script type='text/javascript' src='js/jquery-1.11.2.min.js'></script>
  <script type='text/javascript' src='js/jquery-migrate-1.2.1.min.js'></script>
  <script type='text/javascript' src='js/jquery-ui-1.12.1.js'></script>
  <script type='text/javascript' src='js/bootstrap.min.js'></script>

</head>
<body>
<div id="page">
<!-- Nav Bar -->
  <nav class="navbar navbar-default navbar-fixed-top">
    <div class="container-fluid">
      <div class="navbar-header">
        <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false" aria-controls="navbar">
          <span class="sr-only">Toggle navigation</span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
        </button>
        <div>  <a class="navbar-brand" href="index.php"><span class="glyphicon glyphicon-tint " aria-hidden="true"></span>  Franchino ®</a></div>

      </div>
      <div id="navbar" class="navbar-collapse collapse">
        <!-- Nav tabs -->
        <ul class="nav nav-pills nav-fill franchino-nav">
          <li class="disabled"><a href="#panel_1" data-toggle="tab">Valvole</a></li>
          <li class="disabled"><a href="#panel_2" data-toggle="tab">Programmazione <span class="badge" id="franchino-prog-badge"></span></a></li>
          <li class="disabled"><a href="#panel_3" data-toggle="tab">Eventi</a></li>
        </ul>



      </div>

    </div>
  </nav>
  
    <!-- <img src='images/logo.jpg' /> -->
  <div class="container-fluid">
    <div class="tab-content">
   <div class=" franchino-panel">
   
   
   <div class="panel-group franchino-panel">
          <div class="panel panel-default">
            <div class="panel-heading"><h1>Login</h1></div>

            <div class="panel-body">

              <div class="row">
                <div class="col-sm-offset-2 col-sm-10">
                 <form id="login" method="post">
					<label for="username">Username:</label>
					<input id="username" name="username" type="text" class="form-control" style="width:400px;" required>
					<label for="password">Password:</label>
					<input id="password" name="password" type="password" class="form-control" style="width:400px;" required>					
					<br />
					<input type="submit" class="btn btn-primary" value="Login">
				</form>
                </div>
              </div>
            </div>
          </div>
        </div>
		
		
	
		</div>
	</div>
	</div>
</div>
<!-- [/page] -->
</body>
</html>
<?php } ?>
