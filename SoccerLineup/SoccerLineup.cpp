//Defines the entry point for the console application.
#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <sstream>
using namespace std;

/*
This function reads a line of int values into a vector function and returns that
vector.
*/
vector<int> readlineofints(int count) {
    // Input values
    vector<int> linevalues(count);
    for (int j = 0; j < count; j++) {
        int val;
        cin >> val;
        linevalues[j] = val;
    }
    return linevalues; // Return line values as a vector
}

int numbers[11] = { 1,2,3,4,5,6,7,8,9,10,11 }; //Set up jersey number array as global array to avoid perpetually redoing this

/* This function tests to see if the addition of a new jersey number to this position is allowed
*/
bool jersey_allowed(int answer, int new_jersey_number_to_test) {
    if (new_jersey_number_to_test == 1) { return true; } //If the new jersey is 1 it can't sum to 13, so return true
    int opposing_value_index = 13 - new_jersey_number_to_test-1; //Get value of index of problem jersey
        if (((answer >> (11 - opposing_value_index - 1)) & 1) == 1) { //If the opposing jersey number exists in this row
                return false; //Return false if it does        
    }
    return true;
}

void recursive_check(int (&possible_solutions),int column, int (&answer)[4], const int(&orig)[4]) {
    if (column == 11) { //If we've reached the end successfully
        //Note: This chunk of code taken from https://stackoverflow.com/questions/14682641/count-number-of-1s-in-binary-format-of-decimal-number
        //Since your compiler didn't like my _mm_popcnt_u32 use and I can't change compiler flags
        unsigned int v = answer[0]; // count the number of bits set in v
        unsigned int bitsOn; // c accumulates the total bits set in v
        for (bitsOn = 0; v; bitsOn++) {
            v &= v - 1; // clear the least significant bit set
        }
        if (bitsOn != 1) { return; } //If none of them are on return without logging
        possible_solutions++; //Increment possible solutions
        return;
    }

    for (int i = 0; i < 4; i++) { //Iterate through rows
        if ((orig[i] >> (11 - column-1) & 1) == 1) { //If this position for this player is allowed
            if (jersey_allowed(answer[i], column + 1)) { //If adding this jersey number doesn't create a sum of 13 with anyone already in the row
                int hole_punch = 1 << (11 - column-1); //Set hole puncher
                int new_answer[4]; //Create a new answer array
                for (int j = 0; j < 4; j++) { //Set it equal to the current answer array
                    new_answer[j] = answer[j];
                }
                new_answer[i] = new_answer[i] | hole_punch; //Punch hole in it
                recursive_check(possible_solutions, column + 1, new_answer, orig); //Recurse
            }
        }
    }
    return;
}

int main() {
    std::ios_base::sync_with_stdio(false);

    // get test case count
    int t;
    std::cin >> t;

    // loop over all the test cases
    for (int i = 1; i <= t; i++) {
        // Read in params
        int allowed_positions[4] = { 0 };
        string response;
        for (int j = 0; j < 11; j++) {            
            getline(cin, response); //Get input line
            if (response == "") { getline(cin, response); } //If this is the blank preceding line, absorb it
            vector<int> row_to_punch;
            for (char& s : response) { //For each character in the response
                //Set up rows to punch
                if (s == 'G') { row_to_punch.push_back(0); }
                if (s == 'D') { row_to_punch.push_back(1); }
                if (s == 'M') { row_to_punch.push_back(2); }
                if (s == 'S') { row_to_punch.push_back(3); }
            }
            int hole_punch = 1 << (11 - j-1); //Set hole puncher to this player's jersey number
            for (auto row : row_to_punch) {
                allowed_positions[row] = allowed_positions[row] | hole_punch; //Punch hole
            }
        } 

        int answer[4] = { 0 };        
        int possible_solutions = 0;
        recursive_check(possible_solutions, 0, answer, allowed_positions); //Start the recursive check

        //Print result, incrementing to account for indexing offset at input if necessary
        std::cout << "Case #" << i << ": " << possible_solutions << std::endl;
        
    }
    return 0;
}