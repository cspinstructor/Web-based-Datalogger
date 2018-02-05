<?php
	include("dbconnect.php"); 	
	$dblink=Connection();
	$query_1="SELECT * FROM `ldr_logs` ORDER BY `timestamp` ASC LIMIT 10";
	$query_2="SELECT * FROM `ultrasonic_logs` ORDER BY `timestamp` ASC LIMIT 10";
	$query_3="SELECT * FROM `dht11_logs` ORDER BY `timestamp` ASC LIMIT 10";
	
	if ($result_1=mysqli_query($dblink, $query_1)) {
		echo "reading records successfully from ldr_logs <br>";
	} else {
		echo "Error: " . $query_1 . "<br>" . mysqli_error($dblink);
	}
	if ($result_2=mysqli_query($dblink, $query_2)) {
		echo "reading records successfully from ultrasonic_logs <br>";
	} else {
		echo "Error: " . $query_2 . "<br>" . mysqli_error($dblink);
	}if ($result_3=mysqli_query($dblink, $query_3)) {
		echo "reading records successfully from dht11_logs <br>";
	} else {
		echo "Error: " . $query_3 . "<br>" . mysqli_error($dblink);
	}
	
?>
<html>
<body>
   <h1>Arduino Sensors Data Events</h1>
   <h3>Light Levels - Thresholds Events</h3>
   <table border="1" cellspacing="1" cellpadding="1">
		<tr>
			<td style="background-color:green;color:white;font-weight:bold">&nbsp;Timestamp&nbsp;</td> 
			<td style="background-color:blue;color:white;font-weight:bold">&nbsp;Light Level&nbsp;</td>
		</tr>
	<?php 
		  if(mysqli_num_rows($result_1) > 0){
		     while($row = mysqli_fetch_assoc($result_1)) {
		        printf("<tr>
					<td> &nbsp;%s </td>
					<td> &nbsp;%s&nbsp; </td>
					</tr>", 
		           $row["timestamp"], $row["lightlevel"]);
		     }
		  }
	?>	
   </table>	
  <h3>Distance - Thresholds Events</h3>
   <table border="1" cellspacing="1" cellpadding="1">
		<tr>
			<td style="background-color:green;color:white;font-weight:bold">&nbsp;Timestamp&nbsp;</td>
			<td style="background-color:blue;color:white;font-weight:bold">&nbsp;Distance &nbsp;</td>
		</tr>
      <?php 
		  if(mysqli_num_rows($result_2) > 0){
		     while($row = mysqli_fetch_assoc($result_2)) {
		        printf("<tr>
					<td> &nbsp;%s </td>
					<td> &nbsp;%s&nbsp; </td>
					</tr>", 
		           $row["timestamp"], $row["distance_cm"]);
		     }
		  }	  
      ?>
   </table>
   <h3>Temperature and Humidity Reading</h3>
   <table border="1" cellspacing="1" cellpadding="1">
		<tr>
			<td style="background-color:green;color:white;font-weight:bold">&nbsp;Timestamp&nbsp;</td>
			<td style="background-color:blue;color:white;font-weight:bold">&nbsp;Temperature &nbsp;</td>
			<td style="background-color:blue;color:white;font-weight:bold">&nbsp;Humidity &nbsp;</td>
			
		</tr>
      <?php 
		  if(mysqli_num_rows($result_3) > 0){
		     while($row = mysqli_fetch_assoc($result_3)) {
		        printf("<tr>
					<td> &nbsp;%s </td>
					<td> &nbsp;%s&nbsp; </td>
					<td> &nbsp;%s&nbsp; </td>
					</tr>", 
		           $row["timestamp"], $row["temperature"], $row["humidity"]);
		     }
		  }
		  mysqli_close($dblink);
      ?>
   </table>
</body>
</html>
