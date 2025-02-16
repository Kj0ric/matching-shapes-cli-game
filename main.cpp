//
// Created by Harun Yılmaz on 16.02.2025.
//

#include "main.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>	// for file operations
#include <cstdlib>	// for exit()
#include <cassert>	// for assert()
using namespace std;

void GetFileName(const int first, string& filename) {
	// Ask for the first time
	if (first) {
		cout << "Please enter the file name:" << endl;
	}
	// Ask again
	else {
		cout << "The file couldn't be opened." << endl << "Please enter a valid file name:" << endl;
	}
	cin >> filename;
}

void ReadMatrix(ifstream& input, vector<vector<char>>& matrix) {
	string line;
	int expected_length = 0;
	bool is_first_line = true;

	while( getline(input, line) ) {
		// First, check that all lines have equal length
		if (is_first_line) {
			expected_length = line.size();
			is_first_line = false;
		}
		else {
			if (line.size() != expected_length) {
				cout << "The matrix either has invalid dimensions or contains invalid characters." << endl
					 << "Exiting the game. Bye bye." << endl;
				exit(EXIT_FAILURE);
			}
		}

		#ifndef NDEBUG
				cout << "The matrix rows have equal length..." << endl;
		#endif

		// Check each character to be either 'O', 'S', or 'X'. If yes, add it to the vector.
		vector<char> line_vector;	// Create a vector for the current line

		for (const char c : line) {
			if (c != 'O' && c!= 'S' && c != 'X') {
				cout << "The matrix either has invalid dimensions or contains invalid characters." << endl
					 << "Exiting the game. Bye bye." << endl;
				exit(EXIT_FAILURE);
			}
			line_vector.push_back(c);
		}

		// Add the valid line to the matrix
		matrix.push_back(line_vector);

		#ifndef NDEBUG
				cout << "A valid line is added to the matrix..." << endl;
		#endif
	}
}

void PrintMatrix(vector<vector<char>> matrix) {
	// Row
	for (int i = 0; i < matrix.size(); i++) {
		// Column
		for (int j = 0; j < matrix[0].size(); j++) {
			cout << matrix[i][j];
		}
		cout << endl;
	}
}

int main() {
    #ifndef NDEBUG
		cout << "Program start..." << endl;
    #endif

	// Initialize variables
	string					f_name;
	vector<vector<char>>	playground;
	ifstream				input;

	// Get input file name
	#ifndef NDEBUG
		cout << "Getting input file name from cli..." << endl;
	#endif
	GetFileName(1, f_name);
	input.open(f_name);

	// Try to open the file until success
	while ( input.fail() ) {
		GetFileName(0, f_name);
		input.clear();
		input.open(f_name);
	}

    // Read from the file
        // Check validity of matrix. If not valid, error message and terminate
	#ifndef NDEBUG
		cout << f_name << " is open. Ready to read from the file..." << endl;
	#endif

	ReadMatrix(input, playground);

	#ifndef NDEBUG
		assert( !playground.empty() );		// The matrix should not be empty
		cout << "The file is read to the matrix. Ready to print the matrix..." << endl;
	#endif
	cout << "The content of the matrix is:" << endl;
	PrintMatrix(playground);

}