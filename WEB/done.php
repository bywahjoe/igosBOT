<?php

	if($_POST['timer'] and $_POST['song'] and $_POST['speed']) {
		$servername = "localhost";
		$database   = "bywahjoe_igos";
		$userID     = "bywahjoe_igos";
		$pass       = "iGos@123-";

		$d1=$_POST['timer'];
		$d2=$_POST['song'];
		$d3=$_POST['speed'];
		// Create connection
        $conn = new mysqli($servername, $userID, $pass, $database);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
        $sql = "INSERT INTO bot (TIMER,SONG,SPEED) VALUES ('" . $d1 . "','" . $d2 . "','" . $d3 . "') ";
        if ($conn->query($sql) === TRUE) {
           // echo "New record created successfully";
            //echo "200 OK";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    
        $conn->close();

	}else{
		header('location: index.html');	
	};

	echo '

<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8">
	<link rel="stylesheet" href="style.css">

<meta name="viewport" content="width=device-width, initial-scale=1.0">

	<title>IGos Bot</title>
</head>
<body>
<div class="back">
	<h1>~ IGos BOT ~<?php echo"ppp";?></h1>
	<hr>
	<br>
	<div class="btncenter">
		<img src="media/centang.png" width="90px">
		<br>
		<br>
		<h2> Your Setup Has Done !</h2>
		<br>
		<br>		
		<a href="index.html">
		<button class="btn">BACK TO HOME</button></a>
	</div>
	
</div>
</body>
</html>





	';

?>