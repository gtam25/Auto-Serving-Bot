<!--
 /*
//****************************************************/
// Filename: order_view_counter.php
// Created By: Amit Pathania,Manjunath K, Goutam
// Creation Date: 23-10-2016
// Purpose/Description: Following code is used to view the current pending order status.Once order is ready,user can click button to run python script to pass the message tpo  bot over Xigbee.All product details are read from HTTP Post Request
// Theme: Auto Serving bot
// Functions: None
//Global Variables: None
/****************************************************/
-->



<!DOCTYPE HTML>  
<html>
<head>
<title>AUTO SERVING BOT|ADMIN VIEW</title>

<meta charset="utf-8">
<meta http-equiv="refresh" content="10" > 

<link rel="icon" href="images/favicon.ico">
<link rel="shortcut icon" href="images/favicon.ico">
<link rel="stylesheet" href="css/style.css">
<link rel="stylesheet" href="css/slider.css">



<style>
.error {color: #FF0000;}

table, th, td {
    border: 1px solid black;
    border-collapse: separate;
    border-spacing: 1em;
    text-align: center;
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

<script>
function display_alert()
 {
 alert("ORDER COMPLETE");
 }



 </script>


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
              <li><a href="get_menu_3.php">Menu</a></li>
              <li ><a href="place_order.php">Place order</a></li>
              <li class="with_ul" ><a href="order_view_counter.php"> Admin Zone</a>
                <ul>
                  <li><a href="add_item.html">Add item</a></li>
                  <li><a href="del_item.html">Delete item</a></li>
                  <li class="current"><a href="order_view_counter.php">Manage orders</a></li>
                </ul>
                </li>
              
              <li><a href="contacts.html">Contact us</a></li>
            </ul>
          </nav>
          <div class="clear"></div>
        </div>
        <div class="clear"></div>
      </div>
    </div>
  </header>
<form id="form1" action="order_view_counter.php" method="post">
<?php


 
// array for JSON response
$response = array();
 
//read values from configuration files
require_once __DIR__ . '/db_config2.php';


// Create connection
$db = mysqli_connect(DB_SERVER1, DB_USER1, DB_PASSWORD1,DB_DATABASE1);
// Check connection

// Check connection
if (!$db) {
    die("Connection failed: " . mysqli_connect_error());
}

//chekc required user inputs fields
if (isset($_POST['order'])) {
 
    $itemdel = $_POST['order'];
     $served_items=implode(",", $itemdel);

//get table number from table orderstatus
$tablesql="SELECT tablenumber,orderID FROM orderstatus WHERE orderID ='$served_items' ";
$botorder = mysqli_query($db,$tablesql);
 while ($row = mysqli_fetch_assoc($botorder)) {
        $tabno=$row['tablenumber'];
        $orderID=$row['orderID'];
}

//delete order from table which is served
$sql="DELETE FROM orderstatus WHERE orderID ='$served_items' ";
   $result = mysqli_query($db,$sql);


//execute pythom scripts
   //run script to store tabnumber to the file
exec("python botorder.py $tabno");
//run script to pass message and order details to bot over Xigbee
exec("python comnwithxbee.py $orderID");



}
// get all products from orderstatus table
$sql="SELECT *FROM orderstatus";
$result = mysqli_query($db,$sql);

 
// check for empty result
if (mysqli_num_rows($result) > 0) {
    // looping through all results
    // products node
    $response["products"] = array();
    $itemlist = array();
    echo "<table id=t01 style='table-layout: fixed; width: 100%''>";
    echo "<tr>";
     echo "<th>ORDER ID</th><th>TABLE NO</th> <th>ORDER DETAILS</th><th>TOTAL COST</th><th>TIME OF ORDER</th><th>IF SERVED</th>";
     echo "</tr>";
    while ($row = mysqli_fetch_assoc($result)) {
        // temp user array
        echo "<tr>";
        echo "<td >  ".$row['orderID']."</td>";
        echo "<td>   ".$row['tablenumber']."</td>";
        echo "<td>  ".$row['orderdescr']."</td>";
        echo "<td>  Rs ".$row['totalcost']."</td>";
        echo "<td>  ".$row['created_at']."</td>";
        echo "<td><input type='submit' name='order[]' value =".$row['orderID']." onclick='display_alert()'><br></td>";
        echo "</tr>";
    
        // push single product into final response array
        array_push($response["products"], $itemlist);
    }
    echo "</table>";
    mysqli_free_result($result);
   }

mysqli_close($db);
//$url = $_GET['url'];

?>
</form>
<a href="table_served.php"><input type='button' value='ORDERS SERVED DETAILS' ></a>
</body>
</center>


</html>
