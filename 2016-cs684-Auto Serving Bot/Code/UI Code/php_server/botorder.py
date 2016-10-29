#!/usr/bin/python
'''
 /*
//****************************************************/
// Filename: botorder.py
// Created By: Amit Pathania,Manjunath K, Goutam
// Creation Date: 23-10-2016
// Purpose/Description: Following code is used to write the table number whose order is ready and to be delivered by bot.The details of all orders which
are raedy but not delivered maintained in file table_number.txt.The script is called from php page (order_view_counter.php)
// Theme: Auto Serving bot
// Functions: None
//Global Variables: None
/****************************************************/
'''

import sys
#open the file
f = open("table_number.txt", "a")
#write table number to file 
f.write( str(sys.argv[1]) + "\n" )      # str() converts to string
#clsoe the file
f.close()
