<!--
 /*
//****************************************************/
// Filename: remove_order.php
// Created By: Amit Pathania,Manjunath K, Goutam
// Creation Date: 23-10-2016
// Purpose/Description:  Following code will  will delete order from the database once order is delivered by bot. All product details are read from HTTP Post Request
// Theme: Auto Serving bot
// Functions: None
//Global Variables: None
/****************************************************/
-->


<?php

 
// array for JSON response
$response = array();

// check for required fields
if (isset($_POST['order'])) {
 
    $item = $_POST['order'];
   
     
    // import database connection variables
    require_once __DIR__ . '/db_config2.php';

   // $servername = "localhost";
   // $username = "root";
   // $password = "";
   // $dbname = "android_app";

// Create connection
$db = mysqli_connect(DB_SERVER1, DB_USER1, DB_PASSWORD1,DB_DATABASE1);
// Check connection
// Check connection
if (!$db) {
    die("Connection failed: " . mysqli_connect_error());
}

 
    // mysql inserting a new row
    $sql="DELETE FROM orderstatus WHERE orderID ='$item' ";
   $result = mysqli_query($db,$sql);
   
    // check if row inserted or not
    if (mysqli_affected_rows($db)) {
        // successfully inserted into database
        $response["success"] = 1;
        $response["message"] = "Item successfully deleted.";
 
        // echoing JSON response
        echo json_encode($response);
    } else {
        // failed to insert row
        $response["success"] = 0;
        $response["message"] = "No such item exist.";
 
        // echoing JSON response
        echo json_encode($response);
    }
} else {
    // required field is missing
    $response["success"] = 0;
    $response["message"] = "Required field(s) is missing";
 
    // echoing JSON response
    echo json_encode($response);
}
mysqli_close($db);

?>
