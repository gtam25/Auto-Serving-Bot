<?php
	/*
	* Author: L.Goutam
	* Project name: Auto-Serving Bot (E-yantra Specific)
	* Description: This code receives a JSON object as data from the Android Application and stores it in the database by making a connection to the database after parsing the JSON object.
	*/
	
	#import other file for database connection details
	require "db_config2.php";
	require "db_config.php";

	$con = mysqli_connect("localhost",DB_USER1,DB_PASSWORD1,DB_DATABASE1);
	if(!$con){
		echo "Connection Error...".mysqli_connect_error();		
	}
	else{		
		
		$jsondata = file_get_contents("php://input");		
		
		$objToArr = json_decode($jsondata,true);
		
		
		$table = $objToArr['json'][0]['tableNo'];
		$qntyArr = $objToArr['json'][1]['qty'];
		$pidArr = $objToArr['json'][2]['pid'];
		
		$pidArr = str_replace("i","",$pidArr);	// replace the "i" in IDs
		
		
#get item names and costs for pids
		$db1 = mysqli_connect(DB_SERVER, DB_USER, DB_PASSWORD, DB_DATABASE);
		if (!$db1) {
			die("Connection failed: " . mysqli_connect_error());
		}
		 
		$itemlist = array();
		$costlist = array();
		
		foreach($pidArr as $pids){
			$sqlAll="SELECT item,cost from products where pid = $pids";
			$result = mysqli_query($db1,$sqlAll);
			// check for ZERO results
			if (mysqli_num_rows($result) > 0) {
				while ($row = mysqli_fetch_assoc($result)) {
					$itemlist[] = $row["item"];
					$costlist[] = $row["cost"];				
				}
			}
			mysqli_free_result($result);
		}
		
				 
		
		
#create desc array		
		$items_selected = array();
		$price = array();
		
		for($i=0; $i<sizeof($qntyArr); $i++){			
			if($qntyArr[$i] !== ''){
				$price[] = $costlist[$i] * $qntyArr[$i];				
				array_push($items_selected, $itemlist[$i]."-".$qntyArr[$i]);
			}
		}
		$totalprice=array_sum($price);		
		$desc=implode(",", $items_selected);
		
# Check for ZERO orders		
		if(empty($desc)){
			echo "Please select atleast one item";
			die();
		}
		
# Insert into database
		$sql_qry = "insert into orderstatus(tablenumber,orderdescr,totalcost) values('$table','$desc','$totalprice');";
		if(mysqli_query($con, $sql_qry)){
			
			echo "Order placed successfully";
		}
		else{
			
			echo "Cant Place your Order due to :".mysqli_error($con);
		}					
		
		
	}
	
	
?>