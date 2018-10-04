     
<?php
    // Connect to database

   // IMPORTANT: If you are using XAMPP you will have to enter your computer IP address here, if you are using webpage enter webpage address (ie. "www.yourwebpage.com")
    $con=mysqli_connect("localhost","root","password","test");
       
    // Retrieve all records and display them   
    $result = mysqli_query($con,'SELECT * FROM sensor ORDER BY id DESC LIMIT 1');
      
    // Process every record
	//$lenght =   mysqli_num_rows($result);
	//echo  $lenght;
	//echo mysqli_fetch_array($result);
	//echo mysqli_fetch_array($result)[$lenght-1];
    while($row = mysqli_fetch_array($result))
    {      
        
	echo nl2br("\r\n".$row['co2']);
	echo nl2br("\r\n".$row['co']);
		echo nl2br("\r\n".$row['nh4']);

    }
        
    // Close the connection   
    mysqli_close($con);
?>