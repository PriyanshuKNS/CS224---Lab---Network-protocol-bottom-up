from playsound import playsound
import time

def send_sound(msg):
	for i in range(len(msg)):
		if(msg[i]=='0'):
			playsound("C:\\Users\\lenovo\\Desktop\\252_lab-3\\audio\\jump.mp3")
			time.sleep(1)
		else:
			playsound("C:\\Users\\lenovo\\Desktop\\252_lab-3\\audio\\pistol.mp3")


def encode(msg, error=-1):
	rowP = ""
	colP = ""
	w = 5
	nbits = len(msg)
	
	for i in range(5):
		rowsum = 0
		for j in range(5):
			if(i*w+j<nbits):
				rowsum += int(msg[i*w+j])
		rowsum = rowsum%2
		rowP += str(rowsum)
	
	for j in range(5):
		colsum = 0
		for i in range(5):
			if(i*w+j<nbits):
				colsum += int(msg[i*w+j])
		colsum = colsum%2
		colP += str(colsum)
		
	print("\n")
	print("**Encoding**")
	for i in range(5):
		for j in range(5):
			if(i*w+j<nbits):
				print(msg[i*w+j], end=" ")
			else:
				print("*", end=" ")
		print(f" * {rowP[i]}")
	print("\n* * * * *")
	for i in range(5):
		print(colP[i], end=" ")
	print("\n\n")
	
	l = bin(nbits)[2:]
	while(len(l) < 5):
		l = '0' + l
	
	print("START-FLAG  MSG-LEN  BODY  R-PARITY  C-PARITY")
	print(f"11111   {l}   {msg}   {rowP}   {colP}")
	
	if(error != -1):
		if(msg[error] == '0'):
			msg = msg[0:error] + '1' + msg[error+1:]
		else:
			msg = msg[0:error] + '0' + msg[error+1:]
	
	final_msg = "11111" + l + msg + rowP + colP 
	print(f"\nPacket: {final_msg}")
	return(final_msg)
	

def decode(msg):
	s = len(msg)-20
	s1 = int(msg[5:10],2)
	if(s!=s1):
		return("Error")
	
	error = False
	print("\n")
	print("**DECODING**")
	w = 5
	growP = ""
	gcolP = ""
	
	for i in range(5):
		rowsum = 0
		for j in range(5):
			if(i*w+j<s):
				rowsum += int(msg[10+i*w+j])
		rowsum = rowsum%2
		growP += str(rowsum)
	
	for j in range(5):
		colsum = 0
		for i in range(5):
			if(i*w+j<s):
				colsum += int(msg[10+i*w+j])
		colsum = colsum%2
		gcolP += str(colsum)
	
	row_error = []
	col_error = []
	
	rowP = msg[-10:-5]
	colP = msg[-5:-1]+msg[-1]
	for i in range(5):
		if(rowP[i] != growP[i]):
			row_error.append(i)
		if(colP[i] != gcolP[i]):
			col_error.append(i)
		
	print("Erroneous rows : ", end="")
	print(row_error)
	
	print("Erroneous columns : ", end="")
	print(col_error)
	
	rec_msg = msg[10:10+s]
	if(len(row_error)==1 and len(col_error)==1):
		t = row_error[0]*w + col_error[0]
		if(t>=s):
			error=True
		rec_msg = rec_msg[0:t] + str(int(rec_msg[t])^1) + rec_msg[t+1:]
	
	if(error):
		return("Error")
	print(f"Decoded Message : {rec_msg}")
	
	if(len(col_error) == 1 and len(row_error)==1):
		print("                  ", end= "")
		for i in range(col_error[0] + row_error[0]*5):
			print(" ", end = "")
		print("*")
		
		print(f"error at bit {col_error[0] + row_error[0]*5} with 0-indexing")
	return(rec_msg)
	
print("Enter e for encoding two messages")
print("Enter d for decoding combined message")
print("Enter r for retransmission")
print("Enter ds for decoding single message")
print("Enter es for encoding single message")
print("----------------------------------------------")

choice = input()
if choice == 'e':
	num1 = input("Enter first number: ")
	num2 = input("Enter second number: ")
	error1 = int(input("Enter place where you want error in first message(-1 for no error):"))
	error2 = int(input("Enter place where you want error in second message(-1 for no error):"))
	final_msg = encode(num1,error1) + encode(num2,error2)
	print(f"\nSending final message which is : {final_msg}")
	send_sound(final_msg)
	
	
elif choice == 'd':
	en_msg = input("Enter encoded msg: ")
	l1 = en_msg[5:10]
	l1 = int(l1, 2)
	first_msg = decode(en_msg[0:20+l1])
	assert(en_msg[0:5] == "11111")
	assert(first_msg != "Error")
	second_msg = decode(en_msg[20+l1:])
	assert(en_msg[20+l1: 25+l1] == "11111")
	assert(second_msg != "Error")
	
	print(f"\nFirst Message : {first_msg}")
	print(f"Second Message : {second_msg}")


elif choice == "ds":
	en_msg = input("Enter encoded msg: ")
	assert(en_msg[0:5] == "11111")
	first_msg =decode(en_msg)
	print(f"Message : {first_msg}")


elif choice == "es":
	msg = input("Enter message:")
	error1 = int(input("Enter place where you want error in first message(-1 for no error):"))
	en_msg = encode(msg, error1)
	send_sound(en_msg)
	
	
elif choice == 'r':
	s = input("Press 1 for positive ack and 0 for negative ack: ")
	s = s+s+s
	send_sound(s)
