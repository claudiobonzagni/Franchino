<?php
$username = null;
$password = null;

if ($_SERVER['REQUEST_METHOD'] == 'POST') {

	if(!empty($_POST["username"]) && !empty($_POST["password"])) {
		$username = $_POST["username"];
		$password = $_POST["password"];
		
		if($username == 'user' && $password == 'password') {
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


<div id="page">
	<!-- [banner] -->
	<header id="banner">
		<hgroup>
			<h1>Login</h1>
		</hgroup>		
	</header>
	<!-- [content] -->
	<section id="content">
		<form id="login" method="post">
			<label for="username">Username:</label>
			<input id="username" name="username" type="text" required>
			<label for="password">Password:</label>
			<input id="password" name="password" type="password" required>					
			<br />
			<input type="submit" value="Login">
		</form>
	</section>
	<!-- [/content] -->
	
	<footer id="footer">
		<details>
			<summary>Copyright 2013</summary>
			<p>Jonathan Schnittger. All Rights Reserved.</p>
		</details>
	</footer>
</div>
<!-- [/page] -->

<?php } ?>
