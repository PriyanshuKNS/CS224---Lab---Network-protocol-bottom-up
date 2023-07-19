#include <stdio.h>
#include <windows.h>
#include <bits/stdc++.h>
#include <conio.h>
 
using namespace std;

int const FREQ = 1000;

// Driver Code
int main()
{

	string str;
	cin >> str;

	for(int i=0; i<str.size(); i++){
		if(str[i] == '0'){
			Beep(FREQ, 350);
			Sleep(200);
			Beep(FREQ,350);

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
    return 0;
}