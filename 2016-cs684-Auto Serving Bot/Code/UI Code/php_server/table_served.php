<!--
 /*
//****************************************************/
// Filename: table_served.php
// Created By: Amit Pathania,Manjunath K, Goutam
// Creation Date: 23-10-2016
// Purpose/Description: Following code is used to dispaly order delivery status.Once order is delivered, its details can be viewed from here.All product details are read from HTTP Post Request
// Theme: Auto Serving bot
// Functions: None
//Global Variables: None
/****************************************************/
-->

<!DOCTYPE html>
<html lang="en">
<head>
<title>AUTO SERVING BOT | ORDERS SERVED</title>
<meta charset="utf-8">
<meta http-equiv="refresh" content="10" > 
<link rel="icon" href="images/favicon.ico">
<link rel="shortcut icon" href="images/favicon.ico">
<link rel="stylesheet" href="css/style.css">
<link rel="stylesheet" href="css/form.css">
<style>
.error {color: #FF0000;}

table{
    border: 1px solid black;
    text-align: center;
 
}

tr {
	border: 1px solid black;
    background-color: white;
    color: black;
}
th {
	border: 1px solid black;
    background-color: red;
    color: white;
}
tr {
	border: 1px solid black;
   }
</style>
<!--[if lt IE 9]>
<script src="js/html5shiv.js"></script>
<link rel="stylesheet" media="screen" href="css/ie.css">
<![endif]-->
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
              <li ><a href="get_menu_3.php">Menu</a></li>
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

    echo "<table border=1px>"; 
    echo "<tr><th>ORDERID</th><th>TABLE NUMBER</th><th>TIME</th><th>SERVING BOT ID</th></tr>";
    $f = fopen("table_served.txt", "r") or die("Unable to open file!"); 

	while (!feof($f)){   
    $data = fgets($f); 
    echo "<tr><td>" . str_replace(',','</td><td>',$data) . '</td></tr>';
}
echo '</table>';
fclose($f);
?>
<FORM><INPUT Type="button" VALUE="Back" onClick="history.go(-1);return true;"></FORM>
</center>	
</body>

	</html>
	

	
