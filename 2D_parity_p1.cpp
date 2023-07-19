/*
	Input : A bit string 
	Output : The modified message with 2D parity. 
*/

#include <bits/stdc++.h>
using namespace std;

int const WIDTH = 5;
string const START_FLAG = "11111";
string const END_FLAG = "00000";
int const MSG_LEN = 5;

string encode(string msg){

	string rowParity, colParity;
	int w = WIDTH;
	int nbits = msg.size();
	int nrows = ceil((double)nbits/w);

	int last_row_len = nbits % w;

	// FINDING PARITY
	for(int i=0; i<nrows; i++){
		int rowsum = 0;
		for(int j=0; j<w && i*w+j < nbits; j++){
			rowsum = (rowsum + msg[i*w+j]-'0')%2;
		}
		rowParity += to_string(rowsum);
	}

	// PADDING rowParity
	for(int i=0; i<w-nrows; i++){
		rowParity += '0';
	}

	for(int j=0; j<w; j++){
		int colsum = 0;
		int lim = (j < last_row_len) ? nrows : nrows-1;
		for(int i=0; i<lim; i++){
			colsum = (colsum + msg[i*w+j]-'0')%2;
		}
		colParity += to_string(colsum);
	}

	// PRINTING
	cout << "\n\n";
	cout << "** ENCODING **\n";
	for(int i=0; i<nrows; i++){
		for(int j=0; j<w && i*w+j<nbits; j++){
			cout << msg[i*w+j] <<" ";
		} cout <<" * " << rowParity[i] << endl;
	}
	cout << "* * * * *" <<endl;
	for(int j=0; j<min(w,nbits); j++){
		cout << colParity[j] << " ";
	} cout << endl;

	bitset<MSG_LEN> t(nbits);
	string nbinary = t.to_string();

	cout << "S-FLAG     MSG-LEN     BODY     R-PARITY     C-PARITY     E-FLAG\n";
	cout << START_FLAG << "\t" << nbinary << "\t" << msg << "\t" << rowParity << "\t" << colParity << "\t" << END_FLAG << endl; 

	string fullmsg = START_FLAG + nbinary + msg + rowParity + colParity + END_FLAG;

	cout << "\n\n";

	return fullmsg;

}

string decode(string enmsg){

	int n = enmsg.size() - 25;
	cout << "\n\n";
	cout << "** DECODING **\n";
	// n is the length of the pure msg

	

	int w = WIDTH;
	int nrows = ceil((double)n/w);
	int nbits = n;
	string rowParity, colParity;

	int last_row_len = nbits % w;

	// enmsg has start_flag, msg_len and end_flag which must be removed.
	enmsg = enmsg.substr(START_FLAG.size() + MSG_LEN, nbits + 2*w);

	// FINDING PARITY
	for(int i=0; i<nrows; i++){
		int rowsum = 0;
		for(int j=0; j<w && i*w+j < nbits; j++){
			rowsum = (rowsum + enmsg[i*w+j]-'0')%2;
		}
		rowParity += to_string(rowsum);
	}

	for(int j=0; j<w; j++){
		int colsum = 0;
		int lim = (j < last_row_len) ? nrows : nrows-1;
		for(int i=0; i<lim; i++){
			colsum = (colsum + enmsg[i*w+j]-'0')%2;
		}
		colParity += to_string(colsum);
	}

	// ERROR DETECTION (upto 3 bits) **************************
	vector<int> rowError, colError;
	for(int i=0; i<nrows; i++){
		if(rowParity[i] != enmsg[n+i]){
			rowError.push_back(i);
		}
	}
	for(int j=0; j<w; j++){
		if(colParity[j] != enmsg[n+w+j]){
			colError.push_back(j);
		}
	}

	cout << "Erroneous Rows: ";
	for(int i=0; i<rowError.size(); i++){
		cout << rowError[i] << ", ";
	} cout << endl;

	cout << "Erroneous Columns: ";
	for(int j=0; j<colError.size(); j++){
		cout << colError[j] << ", ";
	} cout << endl;

	string msg = enmsg.substr(0,n);
	string correctmsg = msg;
	// ERROR CORRECTION (upto 1 bit) **************************
	if(colError.size() == 1 && rowError.size() == 1){
		int t = rowError[0]*w + colError[0];
		correctmsg[t] = (correctmsg[t]=='0')? '1' : '0';
		cout << "Received msg: " << msg << endl;
		cout << "Correct  msg: " << correctmsg << endl;
	}

	if(colError.size() == 0 && rowError.size() == 0){
		cout << "^^ NO ERRORS ^^\n";
	}

	cout << "\n\n";
	return correctmsg;

}

int main(){
	string msg = "";
	while(msg.size() > 20 || msg.size() < 1){
		cin >> msg;
	}
	

	string encodedMsg = encode(msg);
	
	cout << encodedMsg << endl;
	string recMsg = encodedMsg;
	recMsg[13] = '1';
	// recMsg[8] = '0';

	// cout << recMsg << endl;

	cout << decode(recMsg) << endl;

}
