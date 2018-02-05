<?php
	function Connection(){
		$servername="localhost";
		$username="myuser";
		$password="mypassword";
		$dbname="arduino";
	
		// Create connection   	
		$dblink = mysqli_connect($servername, $username, $password,$dbname);
		// Check connection
		if (!$dblink) {
			die("connection failed!: " . mysqli_connect_error());
		}
		echo "connected successfully to the database <br>";
	return $dblink;
	}
?>
