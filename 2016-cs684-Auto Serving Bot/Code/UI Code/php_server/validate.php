<?php
/*
//****************************************************/
// Filename: validate.php
// Created By: Amit Pathania,Manjunath K, Goutam
// Creation Date: 23-10-2016
// Purpose/Description: To validate username and password entered by user with values in database
// Theme: Auto Serving bot
// Functions: None
//Global Variables: None
/****************************************************/

if (isset($_POST['userID']) && isset($_POST['password']) ) 
{
  //get data from user and store in variables 
    $userID = $_POST['userID'];
    $password1 = $_POST['password'];


// server info


   	$servername = "localhost";
	$username = "root";
    $password = "";
   	$dbname = "login_admin";

// connect to the database
$db = new mysqli($servername, $username, $password, $dbname);

// Check connection to database
if (!$db) {
    die("Connection failed: " . mysqli_connect_error());
}
 
 // mysql selecting row from database as per username and password
    $sql="SELECT * from login WHERE loginID ='$userID' AND password='$password1'  ";
   $result = mysqli_query($db,$sql);
//if result is zero, no user exits
	$row=mysqli_fetch_row($result);
   // to view result page on screen
  	if($row)
  	{
  		header("Location: index2.html"); // redirect to new page on correct login
  	
  	}

  	else
  {
  	echo "Oops.. wrong login"; // wrong login message
  }

}

//close the connection
mysqli_close($db);

?>