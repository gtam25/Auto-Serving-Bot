<!--
 /*
//****************************************************/
// Filename: del_item.php
// Created By: Amit Pathania,Manjunath K, Goutam
// Creation Date: 23-10-2016
// Purpose/Description: Following code will delete from the database.All product details are read from HTTP Post Request
// Theme: Auto Serving bot
// Functions: None
//Global Variables: None
/****************************************************/
-->


<html>
<head>
<title>AUTO SERVING BOT</title>
<center>
<h3>ITEM DELETED.<br>
<a href="index.html">GO BACK TO MAIN MENU.</a></h3>
</center>
</head>
</html>



<?php



/*
 * 
 */
 
// array for JSON response
$response = array();

// check for required fields
if (isset($_POST['item'])) {
 
    $item = $_POST['item'];
   
 
      
    // import database connection variables
    require_once __DIR__ . '/db_config.php';


// Create connection
$db = mysqli_connect(DB_SERVER, DB_USER, DB_PASSWORD,DB_DATABASE);
// Check connection
// Check connection
if (!$db) {
    die("Connection failed: " . mysqli_connect_error());
}

 
    // mysql deleting row
    $sql="DELETE FROM products WHERE item ='$item' ";
   $result = mysqli_query($db,$sql);
   

        
    // check if row inserted or not
    if (mysqli_affected_rows($db)) {
        // successfully removed into database
        $response["success"] = 1;
        $response["message"] = "Item successfully deleted.";
 
       
    } else {
        // failed to remove row
        $response["success"] = 0;
        $response["message"] = "No such item exist.";
 
    }
} else {
    // required field is missing
    $response["success"] = 0;
    $response["message"] = "Required field(s) is missing";
 
    
}
//close connection to database
mysqli_close($db);
 echo "<html> <form>";
    echo "<br> <input  action='action' type = 'button' value='BACK' onClick='history.go(-1);return true;'>";
    echo " </form> </html>";
?>
