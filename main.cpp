//
// Created by Harun Yilmaz on 16.02.2025.
//

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
					 << "Exiting the game. Bye bye.";
				exit(EXIT_FAILURE);
			}
		}

		#ifndef NDEBUG
				cout << "[DEBUG]The matrix rows have equal length..." << endl;
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
				cout << "[DEBUG]A valid line is added to the matrix..." << endl;
		#endif
	}
}

void PrintMatrix(const vector<vector<char>>& matrix) {
	// Row
	for (int i = 0; i < matrix.size(); i++) {
		// Column
		for (int j = 0; j < matrix[0].size(); j++) {
			cout << matrix[i][j];
		}
		cout << endl;
	}
	cout << endl;
}

bool CheckForMatch(const vector<vector<char>>& matrix) {
	const int row_size = matrix.size();
	const int col_size = matrix[0].size();
	bool match_found = false;

	// Check horizontal matches
	for (int i = 0; i < row_size; i++) {
		for (int j = 0; j < col_size - 2; j++) {
			// Only check non-empty cells
			if (matrix[i][j] != '-') {
				// Check if we have at least 3 matching elements horizontally
				if ( matrix[i][j] == matrix[i][j+1] && matrix[i][j] == matrix[i][j+2] ) {
					match_found = true;
				}
			}
		}
	}

	// Check for vertical matches
	for (int j = 0; j < col_size; j++) {
		for (int i = 0; i < row_size - 2; i++) {
			// Only check non-empty cells
			if (matrix[i][j] != '-') {
				if ( matrix[i][j] == matrix[i + 1][j] && matrix[i][j] == matrix[i + 2][j] ) {
					match_found = true;
				}
			}
		}
	}

	return match_found;
}

bool SimulateMoveAndCheck(const int row, const int col, const char dir, vector<vector<char>>& matrix) {
	int new_row = row, new_col = col;

	// Determine target cell based on direction
	if		(dir == 'r') new_col++;
	else if (dir == 'l') new_col--;
	else if (dir == 'u') new_row--;
	else if (dir == 'd') new_row++;
	else {
		#ifndef DEBUG
			cout << "[DEBUG]Invalid direction while determining target cell based on direction..." << endl;
		#endif
	}

	// Temporarily swap two cells
	const char original = matrix[row][col];
	const char neighbor = matrix[new_row][new_col];
	matrix[row][col] = neighbor;
	matrix[new_row][new_col] = original;

	#ifndef NDEBUG
		cout << "[DEBUG]Matrix after swap..." << endl;
		PrintMatrix(matrix);
	#endif

	// Check for a match
	const bool match_found = CheckForMatch(matrix);

	// If no match was found, revert the swap
	if (!match_found) {
		matrix[row][col] = original;
		matrix[new_row][new_col] = neighbor;
	}

	return match_found;
}

bool GetValidMove(int row, int col, char dir, vector<vector<char>>& matrix, bool& should_exit) {
	const int matrix_size = matrix.size();
	const int row_size = matrix[0].size();

	cout << "Move:" << endl;
	cin >> row >> col >> dir;

	// Check for exit condition
	if (row == 0 && col == 0 && dir == 'q') {
		should_exit = true;
		return false;
	}

	// Check if inputs are valid
	if (row < 0 || col < 0 || (dir != 'r' && dir != 'l' && dir != 'u' && dir != 'd')) {
		cout << "Invalid input. Try again." << endl;
		return false;
	}

	// Check if coordinates are within bounds
	if ( !(row < matrix_size && col < row_size) ) {
		cout << "Invalid coordinates!" << endl;
		return false;
	}

	// Determine the neighbor's coordinates based on direction
	int neighborRow = row, neighborCol = col;
	if (dir == 'r') {
		neighborCol++;
	} else if (dir == 'l') {
		neighborCol--;
	} else if (dir == 'u') {
		neighborRow--;
	} else if (dir == 'd') {
		neighborRow++;
	}

	// Check if the neighbour is within bounds
	if (neighborRow < 0 || neighborRow >= matrix_size ||
		neighborCol < 0 || neighborCol >= row_size) {
		cout << "Move out of bounds!" << endl;
		return false;
		}

	// Check for empty cells
	if (matrix[row][col] == '-' || matrix[neighborRow][neighborCol] == '-') {
		cout << "Cannot swap with an empty cell!" << endl;
		return false;
	}

	// Check if move creates a match
	if ( !SimulateMoveAndCheck(row, col, dir, matrix) ) {
		cout << "Invalid move: No match found!" << endl;
		return false;
	}

	return true;
}

void MarkHorizontalMatches(const vector<vector<char>>& matrix, vector<vector<bool>>& marks) {
	const int row_size = matrix.size();
	const int col_size = matrix[0].size();

	for (int i = 0; i < row_size; i++) {
		int j = 0;

		while (j < col_size) {
			if (matrix[i][j] == '-') {
				j++;	// Skip empty ('-') cells
			} else {
				const char shape = matrix[i][j];
				const int start_idx = j;

				// Go until the end of the consecutive sequence
				while (j < col_size && matrix[i][j] == shape) {
					j++;
				}
				const int length = j - start_idx;
				if (length >= 3) {
					// Mark all positions in the sequence
					for (int k = start_idx; k < j; k++) {
						marks[i][k] = true;
					}
				}
			}
		}
	}
}

void MarkVerticalMatches(const vector<vector<char>>& matrix, vector<vector<bool>>& marks) {
	const int row_size = matrix.size();
	const int col_size = matrix[0].size();

	for (int j = 0; j < col_size; ++j) {
		int i = 0;
		while (i < row_size) {
			if (matrix[i][j] == '-') {
				i++; // Skip empty ('-') cells
			} else {
				const char shape = matrix[i][j];
				const int start_idx = i;

				// Go until the end of the consecutive sequence
				while (i < row_size && matrix[i][j] == shape) {
					i++;
				}
				const int length = i - start_idx;
				if (length >= 3) {
					// Mark all positions in the sequence
					for (int k = start_idx; k < i; ++k) {
						marks[k][j] = true;
					}
				}
			}
		}
	}
}

void ReplaceMarkedCells(vector<vector<char>>& matrix, vector<vector<bool>>& marks, bool& changed) {
	const int row_size = matrix.size();
	const int col_size = matrix[0].size();

	// Replace marked cells with '-'
	for (int i = 0; i < row_size; i++) {
		for (int j = 0; j < col_size; j++) {
			if (marks[i][j]) {
				matrix[i][j] = '-';
				changed = true;
			}
		}
	}
}

void ClearMatches(vector<vector<char>>& matrix) {
	bool changed;
	int row_size = 0;
	int col_size = 0;

	do {
		changed = false;	// Reset flag for each iteration
		row_size = matrix.size();
		col_size = matrix[0].size();

		// A vector to keep track of matched indices
		vector<vector<bool>> marks(row_size, vector<bool>(col_size, false));

		// Mark horizontal matches
		MarkHorizontalMatches(matrix, marks);

		// Replace marked cells with '-'
		ReplaceMarkedCells(matrix, marks, changed);

		// Reset marks for vertical check
		marks = vector<vector<bool>>(row_size, vector<bool>(col_size, false));

		// Mark vertical matches
		MarkVerticalMatches(matrix, marks);

		// Replace marked cells with '-'
		ReplaceMarkedCells(matrix, marks, changed);

	} while (changed);		// Repeat until no changes
}

void ApplyGravity(vector<vector<char>>& matrix) {
	const int rows = matrix.size();
	const int cols = matrix[0].size();

	// Process each column independently
	for (int col = 0; col < cols; col++) {
		int writePos = rows - 1;  // Start from bottom

		// Move from bottom to top, moving non-empty cells down
		for (int readPos = rows - 1; readPos >= 0; readPos--) {
			if (matrix[readPos][col] != '-') {
				if (writePos != readPos) {
					matrix[writePos][col] = matrix[readPos][col];
					matrix[readPos][col] = '-';
				}
				writePos--;
			}
		}

		// Fill remaining top positions with empty cells
		while (writePos >= 0) {
			matrix[writePos][col] = '-';
			writePos--;
		}
	}
}

void Gameplay(int row, int col, char dir, vector<vector<char>>& matrix) {
	cout << "Enter row, col, and direction (r/l/u/d). Type '0 0 q' to exit." << endl;
	bool valid_move_found = false;
	bool should_exit = false;

	while (true) {
		valid_move_found = GetValidMove(row, col, dir, matrix, should_exit);

		// Exit gameplay
		if (should_exit) return;

		if (valid_move_found) {
			// Handle valid move
			cout << "After swap:" << endl;
			PrintMatrix(matrix);
			cout << "Move successful. Clearing matches..." << endl;

			while (CheckForMatch(matrix)) {
				// Clear matches
				ClearMatches(matrix);
				cout << "After clearing matches:" << endl;
				PrintMatrix(matrix);

				// Apply gravity
				ApplyGravity(matrix);
				cout << "After applying gravity:" << endl;
				PrintMatrix(matrix);
			}
			#ifndef NDEBUG
				cout << "[DEBUG]Valid move found. Ready to handle valid move..." << endl;
			#endif
		}

		// If the move is invalid, keep asking for a move
		// If the move is valid, after processing the move keep asking for a move
	}
}

int main() {
    #ifndef NDEBUG
		cout << "[DEBUG]Program start..." << endl;
    #endif

	// Initialize variables
	string					f_name;
	vector<vector<char>>	playground;
	ifstream				input;
	int						move_row;
	int						move_column;
	char					move_direction;

	// Get input file name
	#ifndef NDEBUG
		cout << "[DEBUG]Getting input file name from cli..." << endl;
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
		cout << "[DEBUG]" << f_name << " is open. Ready to read from the file..." << endl;
	#endif

	ReadMatrix(input, playground);

	#ifndef NDEBUG
		assert( !playground.empty() );		// The matrix should not be empty
		cout << "[DEBUG]The file is read to the matrix. Ready to print the matrix..." << endl;
	#endif

	cout << "The content of the matrix is:" << endl;
	PrintMatrix(playground);

	#ifndef NDEBUG
		cout << "[DEBUG]Gameplay begins..." << endl;
	#endif

	// At each round, get keyboard input from the user
	Gameplay(move_row, move_column, move_direction, playground);

	#ifndef NDEBUG
		cout << "[DEBUG]Gameplay function exited..." << endl;
	#endif

	cout << "Exiting the game. Bye bye." << endl;

	return 0;
}