<!--
 /*
//****************************************************/
// Filename: place_order.php
// Created By: Amit Pathania,Manjunath K, Goutam
// Creation Date: 23-10-2016
// Purpose/Description:  Following code will used by customer to place order.He can select items from the list displaying all menu items after reading from database
// Theme: Auto Serving bot
// Functions: None
//Global Variables: None
/****************************************************/
-->



<!DOCTYPE html>
<html lang="en">
<head>
<title>AUTO SERVING BOT | PLACE ORDER</title>
<meta charset="utf-8">
<link rel="icon" href="images/favicon.ico">
<link rel="shortcut icon" href="images/favicon.ico">
<link rel="stylesheet" href="css/style.css">
<link rel="stylesheet" href="css/slider.css">

</head>

<style>

table, th, td {
    border: 1px solid black;
    border-collapse: collapse;
    text-align: center;
    table-layout:fixed; ;
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
 <script>

function display_alert()
 {
   
    var x = document.forms["form1"]["table"].value;
    if (x == null || x == "") {
        alert("All enteries should be made");
        return false;

    }

  alert("ORDER PLACED");
 }
 </script>
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
              <li><a href="get_menu_3.php">Menu</a></li>
              <li class="current"><a href="place_order.php">Place order</a></li>
               <li class="with_ul" ><a href="admin.html"> Admin Zone</a>      </li>
              
              <li><a href="contacts.html">Contact us</a></li>
            </ul>
          </nav>
          <div class="clear"></div>
        </div>
        <div class="clear"></div>
      </div>
    </div>
  </header>


<h3>
<form id="form1" action="place_order_handle.php" onsubmit="return display_alert()" method="post">
<div align="center"><br>

<?php

/*
 * Following code will list all menu items
 */
 
// array for JSON response
$response = array();
 
 //read connection variables
require_once __DIR__ . '/db_config.php';

// Create connection
$db = mysqli_connect(DB_SERVER, DB_USER, DB_PASSWORD,DB_DATABASE);
// Check connection
// Check connection
if (!$db) {
    die("Connection failed: " . mysqli_connect_error());
}
 
// get all products from products table
$sql="SELECT * FROM products ORDER BY products.pid";
$result = mysqli_query($db,$sql);

 
// check for empty result
if (mysqli_num_rows($result) > 0) {
    // looping through all results
    // products node
   
    echo "<table id=t01>";
    echo "<tr>";
     echo "<th>ITEM ID</th><th>SELECT</th><th>ITEM NAME</th> <th>COST</th><th>QUANTITY</th><th>DESCRIPTION</th>";
     echo "</tr>";
    while ($row = mysqli_fetch_assoc($result)) {
        // temp user array
        echo "<tr>";
        echo "<td>".$row['pid']."</td>";
        echo '<td><input type="checkbox" name="pid[]" value='.$row['pid'].'></td>';
        echo "<td>".$row['item']."</td>";
        echo "<td>".$row['cost']."</td>";
        echo '<td><input  type="text" name="qty[]" ></td>';
        echo "<td class=wrapword; word-wrap:break-word;>".$row['description']."</td>";
        echo "</tr>";
    
    }
    echo "</table>";
    mysqli_free_result($result);
   }
//close the connection
mysqli_close($db);

?>
Enter table Number: <input type="text" name="table"><br><br>
<br>
<input type="submit" value ="CLICK TO PLACE ORDER" ><br>
</div>
</form>
</h3>
</center>
</body>
</html>