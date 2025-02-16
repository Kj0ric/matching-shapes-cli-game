//
// Created by Harun Yılmaz on 16.02.2025.
//

#include "main.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

void getFileName(const int first, string& filename) {
	// Ask for the first time
	if (first) {
		cout << "Please enter the file name: " << endl;
	}
	// Ask again
	else {
		cout << "The file couldn't be opened.\n Please enter a valid file name: " << endl;
	}
	cin >> filename;
}

int main() {

    #ifndef NDEBUG
		cout << "Program start..." << endl;
    #endif

	// Initialize variables
	string					f_name;
	vector<vector<char>>	playground;

	// Get input file name
	#ifndef NDEBUG
		cout << "Getting input file name from cli..." << endl;
	#endif
	getFileName(1, f_name);
	// Try to open the file until success
	ifstream	input(f_name);
	while ( input.fail() ) {
		getFileName(0, f_name);
	}

    // Read from the file
        // Check validity of matrix. If not valid, error message and terminate
	#ifndef NDEBUG
		cout << f_name << " is open. Ready to read from the file..." << endl;
	#endif




}