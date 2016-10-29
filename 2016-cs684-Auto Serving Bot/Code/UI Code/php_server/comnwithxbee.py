#! /usr/bin/python
'''
/****************************************************/
// Filename: comnwithxbee.py
// Created By: Amit Pathania,Manjunath K, Goutam
// Creation Date: 23-10-2016
// Purpose/Description: For serial communication between bot and Xigbee and sending/recieving data to/from Xigbee. It reads table number from file then checks bot which is fre
//After that initialises free bot and sends start point and destination point to bot over Xigbee.
//After that it saves the bot id,time of deleivery,orderID and table number in a file
//  Theme: Auto Serving bot
// Functions: readtab,writetab,getserialOrNone,printdata,initialise,check,startpt,destn
Global Variables: None
/****************************************************/
'''


from xbee import XBee
import serial
import sys
import time
import fileinput

'''
/*
* Function Name: readtab
* Input: void
* Output:None.Open the file"table_number.txt" and read it's content (table number for order delivery) and store into variable 
* Logic: Open file onatining table number for serving order,read contents line by line and store in empty list "table"
* Example Call: readtab();
*
*/
'''
def readtab():
	fname="table_number.txt"
	
	table=[]
	empty=[]
	
	with open(fname) as f:
		for line in f:
			tab=line.strip()
			table.append(tab)
	f = open(fname, 'w')
	f.writelines(empty)
	f.close()
	return(table)

'''
/*
* Function Name: writetab(var,x)
* Input: integer table number where order served, character variable x containing bot ID which delivered the order
* Output: None.Open the file"table_served.txt" and store table number (table number where order delivered), time of delivery,botno and ID 
* Logic: Open file and store store table number (table number where order delivered), time of delivery,botno and ID 
* Example Call: readtab(10,q);
*
*/
'''
def writetab(var,x):
	fname="table_served.txt"
	localtime = time.asctime( time.localtime(time.time()) )
	if x=='q':
		botno='1'
	if x=='w':
		botno='2'
	if x=='e':
		botno='3'
	with open(fname, 'a') as f:
		stable= str(sys.argv[1])+", Table " + var + " , " + localtime +", bot ID " + x+" bot no"+ botno +"\n"
		f.writelines(stable)
	
	return()

'''
/*
* Function Name: getSerialOrNone(port)
* Input: integer COM port number where Xigbee is connected
* Output: True or false
* Logic: Try to conenct to given COM port for serial communication.If connected return True else return None
* Example Call: getSerialOrNone(COM9)
*
*/
'''
def getSerialOrNone(port):
    try:
       return serial.Serial(port)
    except:
       return None

'''
/*
* Function Name: destn(fp)
* Input: Integer variable.Table number
* Output: None. Send Table number for order delivery to bot over Xigbee
* Logic: Convert table number to table grid number and send it to specific bot over Xigbee
* Example Call: destn(10)
*
*/
'''

def destn(fp):
	coord=0
	if(fp==1):
		coord = 20

	elif(fp==2):
		coord= 21
		
	elif(fp==3):
		coord=22

	elif(fp==4):
		coord=23
		
	elif(fp==5):
		coord=30
		
	elif(fp==6):
		coord=31
		
	elif(fp==7):
		coord=32

	elif(fp==8):
		coord=33

	elif(fp==9):
		coord=40

	elif(fp==10):
		coord=41

	elif(fp==11):
		coord=42

	elif(fp==12):
		coord=43

	elif(fp==13):
		coord=50

	elif(fp==14):
		coord=51

	elif(fp==15):
		coord=52

	elif(fp==16):
		coord=53
		
	else:
		
		coord=00
		print("WRONG TABLE NUMBER")
		

	msg=""
	msg="d"+str(coord)
	xbee.tx(dest_addr='\xFF\xFF', data=msg)
	xbee.tx(dest_addr='\xFF\xFF', data='z')
	print("destn :"+ msg)
	
'''
/*
* Function Name: initialsie(id)
* Input: character variable (bot id)
* Output: None. Send 's' with bot id to initialise bot over Xigbee for further communication
* Logic: Send 's' with bot id to initialise bot over Xigbee for further communication
*Example Call: initialise('q')
*/
'''
def initialise(id):
	print("Initialising bot  " + id)
	xbee.tx(dest_addr='\xFF\xFF', data='s')#initialise q
	xbee.tx(dest_addr='\xFF\xFF', data=id)
	time.sleep(1)
	reply = ser.read(64)
	print(reply)
	
'''
/*
* Function Name: check(id)
* Input: character variable (bot id)
* Output: 1 or 0. 
* Logic: Send 'F' with bot id to check whether bot is free or not
*Example Call: check('q')
*/
'''
def check(id):
	
	xbee.tx(dest_addr='\xFF\xFF', data='F')
	xbee.tx(dest_addr='\xFF\xFF', data=id)
	
	print("Checking bot"+ id+" is free? ")
	time.sleep(1)
	reply = ser.read(24)
	print(reply)

	if(len(reply)==0):
		print("BOT "+ id +" NOT FREE")
		return(1)
	else:
		return(0)

'''
/*
* Function Name: startpt(sp)
* Input: Integer variable.Start point
* Output: None. Sends start point for order delivery to bot over Xigbee
* Logic: Convert start point to arena grid number and send it to specific bot over Xigbee
* Example Call: startpt(01)
*
*/
'''
def startpt(sp):
	msg="i"+str(sp)
	xbee.tx(dest_addr='\xFF\xFF', data=msg)
	print("startpt:  "+msg)
	#xbee.tx(dest_addr='\xFF\xFF', data=sp)


#store table number in list tables
tables=readtab()
#calculate total number of orders
total_orders=len(tables)

# Open serial port.Check whether its free or not
while(1):
	if(getSerialOrNone('COM9') != None):
		break
	else:
		print("trying to connect")
		time.sleep(5)
	
#flags to indicate whether bot is free or not
flag1=0
flag2=0
flag3=0

try:
	#connect to COM port 9
	ser = serial.Serial(port='COM9', baudrate=9600,timeout=0)
	#indicate serial device is Xigbee
	xbee = XBee(ser)
	#print total orders and COM port number
	print("TOTAL ORDERS:" , len(tables))
	print("connected to: " + ser.portstr)
		
	index=0
	#loop while all orders are delivered
	while (total_orders>0):
		print("------------CHECKING AGAIN-------")
		print("ORDERS LEFT:",total_orders)
			
		#check whether bot is free or not
		flag1=check('q')
		time.sleep(2)
		#print(order)
		if(flag1==0 ):
			print("BOT1 READY")
			print("DESTN TABLE:"+ tables[index])
			#initialise bot1 if free
			initialise('q')
			#send start point to bot1 if free
			startpt('01')
			#send destination address to bot1 if free
			destn(int(tables[index]))
			time.sleep(1)
			#read serial for bot reply
			reply=ser.read(25)
			print(reply)
			count=0
			#check for no reply
			while(len(reply)==0):
				initialise('q')
				startpt('01')
				destn(int(tables[index]))
				time.sleep(1)
				reply=ser.read(25)
				print(reply)
				count=count+1
				if(len(reply) >0 or count>5):
					break
			#write entrythat order delivered by bot if reply recieved	
			writetab(tables[index],'q')
			flag1=1
			#reduce one order from total orders
			total_orders=total_orders-1;
			index=index+1;
			
			#check if bot 2 is free
		if (flag1==1 and total_orders>0):	         
			flag2=check('w')
			time.sleep(2)
					
		if (flag1==1 and flag2==0 and total_orders>0):
			print("BOT1 BUSY.... BOT2 READY")
			print("DESTN TABLE  :"+tables[index])
			#initialise bot2 if free
			initialise('w')
			#send start pt to bot2 if free
			startpt('01')
			#send destn point to bot2 if free
			destn(int(tables[index]))
			time.sleep(1)
			#chekc for reply from bot2
			reply=ser.read(25)
			print(reply)
			count=0
			while(len(reply)==0):
				initialise('w')
				startpt('01')
				destn(int(tables[index]))
				time.sleep(1)
				reply=ser.read(25)
				print(reply)
				count=count+1
				if(len(reply) >0 or count>5):
					break
			writetab(tables[index],'w')
			flag2=1
			total_orders=total_orders-1;
			index=index+1;
	
		#chekc if bot3 is free
		if (flag1==1 and flag2==1 and total_orders>0):
			flag3=check('e')
			time.sleep(2)
						


		if (flag1==1 and flag2==1 and flag3==0 and total_orders>0):
			print("BOT1 and 2 BUSY.... BOT3 READY")
			print("DESTN TABLE  :"+tables[index])
			#initialise bot3 if free
			initialise('e')
			#send start pt to bot3 if free
			startpt('01')
			#send destn to bot3 if free
			destn(int(tables[index]))
			time.sleep(1)
			reply=ser.read(25)
			print(reply)
			count=0
			while(len(reply)==0):
				initialise('e')
				startpt('01')
				destn(int(tables[index]))
				time.sleep(1)
				reply=ser.read(25)
				print(reply)
				count=count+1
				if(len(reply) >0 or count>5):
					break
				

			writetab(tables[index],'e')
			flag3=1
			total_orders=total_orders-1;
			index=index+1;
			
				
			#when no bot is free
		if (flag1==1 and flag2==1 and flag3==1 and total_orders>0):
			time.sleep(2)
			print("NO BOT FREE")

	#when all orders are delivered
	if(total_orders == 0):
		print("Good Bye,Saiyonaara")
	
	#clsoe the port					
	ser.close()
#exception/error handler
except Exception as e: print(e)


