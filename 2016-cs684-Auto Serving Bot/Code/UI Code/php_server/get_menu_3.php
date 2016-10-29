<!--
 /*
//****************************************************/
// Filename: get_menu3.php
// Created By: Amit Pathania,Manjunath K, Goutam
// Creation Date: 23-10-2016
// Purpose/Description:  Following code will list/display all menu items after reading from database
// Theme: Auto Serving bot
// Functions: None
//Global Variables: None
/****************************************************/
-->


<!DOCTYPE HTML>  
<html>
<html lang="en">
<head>
<title>AUTO SERVING BOT|PLACE ORDER</title>
<meta charset="utf-8">
<link rel="icon" href="images/favicon.ico">
<link rel="shortcut icon" href="images/favicon.ico">
<link rel="stylesheet" href="css/style.css">
<link rel="stylesheet" href="css/slider.css">

<!--[if lt IE 9]>
<script src="js/html5shiv.js"></script>
<link rel="stylesheet" media="screen" href="css/ie.css">
<![endif]-->

<style>
.error {color: #FF0000;}

table, th, td {
    border: 1px solid black;
    text-align: center;
    border-collapse: separate;
    border-spacing: 1em;
    padding-top: .5em;
    padding-bottom: .5em;
}

table#t01 tr:nth-child(even) {
    background-color: #eee;
}
table#t01 tr:nth-child(odd) {
   background-color:#fff;
}
table#t01 th {
    background-color: red;
    color: white;
}
</style>
</head>
<center>
<body> 
<div class="main">
  <header>
    <div class="container_12">
      <div class="grid_12">
        <h1><a href="index.html"><img src="images/logo.png" alt=""></a></h1>
        <div class="menu_block">
          <nav>
            <ul class="sf-menu">
              <li ><a href="index.html">Home</a></li>
              <li class="current"><a href="get_menu_3.php">Menu</a></li>
              <li ><a href="place_order.php">Place order</a></li>
               <li ><a href="admin.html"> Admin Zone</a>      </li>
              
              <li><a href="contacts.html">Contact us</a></li>
            </ul>
          </nav>
          <div class="clear"></div>
        </div>
        <div class="clear"></div>
      </div>
    </div>
  </header>


<?php

/*
 * Following code will list all menu items
 */
 
// array for JSON response
$response = array();
 
require_once __DIR__ . '/db_config.php';

   // $servername = "localhost";
   // $username = "root";
   // $password = "";
   // $dbname = "android_app";

// Create connection
$db = mysqli_connect(DB_SERVER, DB_USER, DB_PASSWORD,DB_DATABASE);
// Check connection
// Check connection
if (!$db) {
    die("Connection failed: " . mysqli_connect_error());
}
 
// get all products from products table
$sql="SELECT * FROM products ORDER BY products.pid ";
$result = mysqli_query($db,$sql);

 
// check for empty result
if (mysqli_num_rows($result) > 0) {
    // looping through all results
    // products node
    $response["products"] = array();
    $itemlist = array();
    echo "<table id=t01>";
    echo "<tr>";
     echo "<th>ITEM ID</th><th>ITEM NAME</th> <th>COST PER ITEM</th><th>DESCRIPTION</th>";
     echo "</tr>";
    while ($row = mysqli_fetch_assoc($result)) {
        // temp user array
        echo "<tr>";
        echo "<td>".$row['pid']."</td>";
        echo "<td>".$row['item']."</td>";
        echo "<td>".$row['cost']."</td>";
        echo "<td text-align:left>".$row['description']."</td>";
        echo "</tr>";
    
        // push single product into final response array
        array_push($response["products"], $itemlist);
    }
    echo "</table>";
    mysqli_free_result($result);
   }

mysqli_close($db);

?>
</body>
</center>
</html>