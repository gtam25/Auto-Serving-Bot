<!--
 /*
//****************************************************/
// Filename: add_item.php
// Created By: Amit Pathania,Manjunath K, Goutam
// Creation Date: 23-10-2016
// Purpose/Description: Following code is used to add new item to menu list.All product details are read from HTTP Post Request
// Theme: Auto Serving bot
// Functions: None
//Global Variables: None
/****************************************************/
-->

<html>
<head>
<title>AUTO SERVING BOT</title>
<center>
<h3>ITEM ADDED.<br>
<a href="index.html">GO BACK TO MAIN MENU.</a></h3>
</center>
</head>
</html>

<?php
 


/*
 * Following code will add new item to the database
 * All product details are read from HTTP Post Request
 */
 
// array for JSON response
$response = array();

// check for required fields
if (isset($_POST['item']) && isset($_POST['cost']) && isset($_POST['description'])) {
 
    $item = $_POST['item'];
    $cost = $_POST['cost'];
    $description = $_POST['description'];
 
  
 
      
    // import database connection variables
    require_once __DIR__ . '/db_config.php';


// Create connection
$db = mysqli_connect(DB_SERVER, DB_USER, DB_PASSWORD,DB_DATABASE);
// Check connection
// Check connection
if (!$db) {
    die("Connection failed: " . mysqli_connect_error());
}

 
    // mysql inserting a new row
    $sql="INSERT INTO products(item, cost, description) VALUES('$item', '$cost', '$description')";
   $result = mysqli_query($db,$sql);
   // $result = mysql_query("INSERT INTO products(item, cost, description) VALUES('$item', '$cost', '$description')");

        
    // check if row inserted or not
    if ($result) {
        // successfully inserted into database
        $response["success"] = 1;
        $response["message"] = "Product successfully created.";
 
    } else {
        // failed to insert row
        $response["success"] = 0;
        $response["message"] = "Oops! An error occurred.";
 
     
    }
} else {
    // required field is missing
    $response["success"] = 0;
    $response["message"] = "Required field(s) is missing";
 
   
}
//close the connection
mysqli_close($db);

?>
