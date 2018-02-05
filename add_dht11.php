<?php
   	include("dbconnect.php");
   	$dblink=Connection();

	$query = "INSERT INTO dht11_logs (temperature, humidity) 
	          VALUES (".$_GET["sensor1"].",".$_GET["sensor2"].")"; 
	
	if (mysqli_query($dblink, $query)) {
		echo "New record created successfully";
	} else {
		echo "Error: " . $query . "<br>" . mysqli_error($dblink);
	}
	mysqli_close($dblink);
?>
