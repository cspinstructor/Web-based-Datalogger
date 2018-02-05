<?php
	include("dbconnect.php"); 	
	$dblink=Connection();
	$query="SELECT * FROM dht11_logs ORDER BY timestamp ASC";
	
	if ($result=mysqli_query($dblink, $query)) {
		echo "reading records successfully from dht11_logs <br>";
	} else {
		echo "Error: " . $query . "<br>" . mysqli_error($dblink);
	}
?>
<html>
<body>
   <h1>Arduino Sensors Data</h1>
   <h3>Temperature and Humidity Reading</h3>
   <table border="1" cellspacing="1" cellpadding="1">
		<tr>
			<td>&nbsp;Timestamp&nbsp;</td>
			<td>&nbsp;Temperature &nbsp;</td>
			<td>&nbsp;Humidity &nbsp;</td>
			
		</tr>
      <?php 
		  if(mysqli_num_rows($result) > 0){
		     while($row = mysqli_fetch_assoc($result)) {
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
