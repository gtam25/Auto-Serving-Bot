<!--
 /*
//****************************************************/
// Filename: place_order_handle.php
// Created By: Amit Pathania,Manjunath K, Goutam
// Creation Date: 23-10-2016
// Purpose/Description:  Following code will be used to read customer order and create the entry in current order table in database.It will create connection with the order_details database and create entry for new order in orderstatus table
// Theme: Auto Serving bot
// Functions: None
//Global Variables: None
/****************************************************/
-->



<html>
<head>
<title>AUTO SERVING BOT</title>

</head>
<body>
<?php


 
// array for JSON response
$response = array();

   
 
 if(!empty($_POST['pid']) && !empty($_POST['table'])) {

 //Loop through pid array to fetch individual item IDs so that we can use them
   $table = $_POST['table'];
     
    // import database connection variables
    require_once __DIR__ . '/db_config.php';

   
 $qtylist = array(); //array to store quantity

foreach($_POST['qty'] as $qty) {
      if($qty != '')
      {
       $qtylist[]=$qty;
   }
     }

// Create connection
$db = mysqli_connect(DB_SERVER, DB_USER, DB_PASSWORD, DB_DATABASE);
// Check connection
// Check connection
if (!$db) {

    die("Connection failed: " . mysqli_connect_error());
}


    require_once __DIR__ . '/db_config2.php';


// Create connection to order database
$db1 = mysqli_connect(DB_SERVER1, DB_USER1, DB_PASSWORD1, DB_DATABASE1);
// Check connection
// Check connection
if (!$db1) {

    die("Connection failed: " . mysqli_connect_error());
}

    $itemlist = array();
    $costlist = array();
    $totalprice=0;
    foreach($_POST['pid'] as $pids) {
    //echo "<p>".$pids ."</p>";
    $sql="SELECT item,cost from products where pid = $pids";
    $result = mysqli_query($db,$sql);

    // check for empty result
if (mysqli_num_rows($result) > 0) {
    // looping through all results
    // products node
 
    while ($row = mysqli_fetch_assoc($result)) {
        // temp user array to store itemlist and their cost
        
        $itemlist[] = $row["item"];
        $costlist[] = $row["cost"];
     
    }
    mysqli_free_result($result);
}

      }


    $order_list = array();
    //implode(",", $itemlist); //convert array to string
    //$ordered_cost = implode(",", $costlist);
   
   $price = array();
   //calculating total cost
    for($i = 0; $i < count($costlist); $i++) {
    $price[] = $costlist[$i] * $qtylist[$i];
    $order_list[]=$itemlist[$i]."-".$qtylist[$i];
    }

    $totalprice=array_sum($price);
    $ordered_items=implode(",", $order_list);
    // mysql inserting a new row
    $sql1="INSERT INTO orderstatus(orderdescr,tablenumber,totalcost) VALUES('$ordered_items','$table','$totalprice')";
   $result1 = mysqli_query($db1,$sql1);
   $last_id = mysqli_insert_id($db1);
   
          
    // check if row inserted or not
    if ($result1) {
        // successfully inserted into database
        $response["success"] = 1;
        $response["message"] = "Order successfully placed.";
 
        // echoing JSON response
        //echo json_encode($response);
    } else {
        // failed to insert row
        $response["success"] = 0;
        $response["message"] = "Oops! An error occurred.";
 
        // echoing JSON response
       // echo json_encode($response);
    }

     



mysqli_close($db1);
} else {
    // required field is missing
    $response["success"] = 0;
    $response["message"] = "Required field(s) is missing";
 
    // echoing JSON response
   // echo json_encode($response);
}

mysqli_close($db);
 
?>
<center>
<h2>ORDER PLACED<br> </h2>
<table align = 'center' border='1' bgcolor='Lightgreen'>
<tr><h3><td> ORDER ID </td><td><?php echo $last_id ?> </td></h3></tr>
<tr><h3><td>ITEMS ORDERED </td><td> <?php echo $ordered_items ?> <tr></h3><td>
<tr><h3><td>TOTAL COST </td><td><?php echo $totalprice  ?> <tr></h3><td>
</table>
<h4>THANX.. YOUR ORDER WILL BE SERVED SHORTLY</h4>
<h3><a href="index.html">GO BACK TO MAIN MENU </h3>
<form>
<br> <input  type = 'button' value='HOME' >
   </form> </a>
</center>
</body>
</html>
