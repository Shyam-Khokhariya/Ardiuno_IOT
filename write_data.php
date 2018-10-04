<?php

    // Prepare variables for database connection
   
    $dbusername = "root";  // enter database username, I used "arduino" in step 2.2
    $dbpassword = "password";  // enter database password, I used "arduinotest" in step 2.2
    $server = "localhost"; // IMPORTANT: if you are using XAMPP enter "localhost", but if you have an online website enter its address, ie."www.yourwebsite.com"

    // Connect to your database

    $dbconnect = mysqli_connect($server, $dbusername, $dbpassword);	
    $dbselect = mysqli_select_db($dbconnect,"test");

    // Prepare the SQL statement
	$request=$_GET['value'];
    $sql = "INSERT INTO test.sensor (value) VALUES ('$request')";    

    // Execute SQL statement

    mysqli_query($dbconnect,$sql);

?>