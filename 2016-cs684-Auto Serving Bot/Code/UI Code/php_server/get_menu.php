<!--
 /*
//****************************************************/
// Filename: get_menu.php
// Created By: Amit Pathania,Manjunath K, Goutam
// Creation Date: 23-10-2016
// Purpose/Description:  Following code will list/display all menu items after reading from database
// Theme: Auto Serving bot
// Functions: None
//Global Variables: None
/****************************************************/
-->


<?php

/*
 * Following code will list all menu items
 */
 
// array for JSON response
$response = array();
 
require_once __DIR__ . '/db_config.php';

   

// Create connection
$db = mysqli_connect(DB_SERVER, DB_USER, DB_PASSWORD,DB_DATABASE);
// Check connection
// Check connection
if (!$db) {
    die("Connection failed: " . mysqli_connect_error());
}
 
// get all products from products table
$sql="SELECT *FROM products";
$result = mysqli_query($db,$sql);

 
// check for empty result
if (mysqli_num_rows($result) > 0) {
    // looping through all results
    // products node
    $response["products"] = array();
    $itemlist = array();
    while ($row = mysqli_fetch_assoc($result)) {
        // temp user array
        
        $itemlist["pid"] = $row["pid"];
        $itemlist["item"] = $row["item"];
        $itemlist["cost"] = $row["cost"];
 
        // push single product into final response array
        array_push($response["products"], $itemlist);
    }
    mysqli_free_result($result);
    // success
    $response["success"] = 1;
 
    // echoing JSON response
    echo json_encode($response);
} else {
    // no products found
    $response["success"] = 0;
    $response["message"] = "No products found";
 
    // echo no users JSON
    echo json_encode($response);
}

mysqli_close($db);

?>