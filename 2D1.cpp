/*
	Input : A bit string 
	Output : The modified message with 2D parity. 
*/

#include <bits/stdc++.h>
#include <windows.h>
using namespace std;


int const FREQ = 1000;
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
		
	}

	cout << "Correct  msg: " << correctmsg << endl;

	if(colError.size() == 0 && rowError.size() == 0){
		cout << "^^ NO ERRORS ^^\n";
	}

	
	return correctmsg;

}

void decode2msg(string fullmsg){
	string str = fullmsg.substr(5, 5);

	int t = 1;
	int len1 = 25;
	for(int i=4; i>=0; i--){
		len1 += t*(str[i]-'0');
		t *= 2;
	}
	// cout << len1 << ", " << fullmsg.size()-len1 << endl;
	string msg1, msg2;
	msg1 = fullmsg.substr(0,len1);
	msg2 = fullmsg.substr(len1, fullmsg.size()-len1);

	decode(msg1);
	decode(msg2);
}


void beeep(string str)
{

	for(int i=0; i<str.size(); i++){
		if(str[i] == '0'){
			Beep(FREQ, 300);
			Sleep(100);
			Beep(FREQ,300);

			Sleep(1000);

		} else {
			Beep(FREQ, 1000);
			Sleep(1000);
		}
	}
 
    // Function that beeps a sound of
    // frequency 750 for 0.8 sec
    // Beep(750, 800);
 
    // getch();
}


int main(){
	char choice;
	cin >> choice;
	if(tolower(choice) == 'd'){
		string s;
		cin >> s;
		decode2msg(s);
	}
	else{
		string s1, s2;
		cin >> s1 >> s2;
		srand(0);
		int x = rand()%s1.size();
		srand(1);
		int y = rand()%s2.size();
		string e1 = encode(s1);
		string e2 = encode(s2);
		e1[10+x] = 1^e1[10+x];
		e2[10+y] = 1^e2[10+y];
		string f = e1+e2;
		cout << f << endl;

		char tmp;
		cin >> tmp;
		if(tmp == 'b'){
			beeep(f);
		}
		
	}
}
