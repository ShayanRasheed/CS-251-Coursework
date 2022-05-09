//
//  Project 1 - DNA Profiling
//  Shayan Rasheed
//  An app that stores and analyzes DNA sequences
//  and matches them to specific individuals
//
// Creative Component: After loading a database file and a DNA file,
// type in the command "process_all"
// This will print out the number of ALL instances of the STRs
//

#include <string>
#include <fstream>
#include <sstream>
#include "ourvector.h"

using namespace std;

typedef struct Profile {
	string name;
	ourvector<int> numSequences;
}profile;

// Function load_db: Fills an ourvector with structs
// containing the names and STR counts of each individual listed in the file
// Parameters: stream of the input file,
// reference to an ourvector of profile structs
void load_db(ifstream& inFS, ourvector<profile>& database) {
	string temp;
	getline(inFS, temp);
	// Loop until the end of the file, each file line has its own profile struct
	while(!inFS.eof()) {
		stringstream stream(temp);
		profile p;
		// Get the name for the profile from the stream
		getline(stream, p.name, ',');
		// Repeatedly get the STR counts until the end of the line
		while(stream.good()) {
			int getNum;
			stream >> getNum;
			p.numSequences.push_back(getNum);
			if(stream.peek() == ',') {
				stream.ignore();
			}
		}
		// Add the new profile to the ourvector
		database.push_back(p);
		getline(inFS, temp);
	}
	inFS.close();
}

// Function load_STR: Fills an ourvector with
// all of the STRs listed in the first
// line of the database file (AGAT, TATC, etc)
// Parameters: stream of input file, ourvector for the STRs,
// ourvector for the profiles
void load_STR(ifstream& inFS, ourvector<ourvector<string>>& STR, ourvector<profile>& database) {
	// If either of the ourvectors are already filled, clear them
	if(STR.size() > 0) {
		STR.clear();
	}
	if(database.size() > 0) {
		for(int i = 0; i < database.size(); i++) {
			database[i].numSequences.clear();
		}
		database.clear();
	}
	inFS.ignore(5);
	string temp;
	getline(inFS, temp);
	stringstream ss(temp);
	// Loop until all of the STRs have been pushed into the ourvector
	while (ss.good()) {
		ourvector<string> newVector;
    	string str;
    	getline(ss, str, ',');
    	for(unsigned int i = 0; i < str.length(); i++) {
       		newVector.push_back(str.substr(i, 1));
    	}
    	STR.push_back(newVector);
    }
    // Call to load_db to finish reading from the file
    load_db(inFS, database);
}

// Function displayCounts: Prints all of the counts from the process() function
// Also prints the counts from the creative component (findAllSTR())
// Parameters: boolean to distinguish between
// normal function and creative function
// Ourvectors for the STRs and STR counts to print them
void displayCounts(bool allSTR, ourvector<ourvector<string>>& STR, ourvector<int> STRcounts) {
	// Check if DNA has been processed. If not, return.
	if(STRcounts.size() == 0) {
		cout << endl << "No DNA has been processed." << endl;
	} else {
		cout << endl;
		if(allSTR) {
			cout << "DNA processed, counts of all STR instances:" << endl;
		} else {
			cout << "DNA processed, STR counts: " << endl;
		}
		for(int i = 0; i < STR.size(); i++) {
			for(int j = 0; j < STR[i].size(); j++) {
				cout << STR[i][j];
			}
			cout << ": " << STRcounts[i] << endl;
		}
		cout << endl;
	}
}

// Function display: Prints out the database and DNA sequence
// Parameters: ourvectors for database, dna, STR,
// and STRcounts to print all of their contents
void display(ourvector<ourvector<string>>& STR, ourvector<profile>& database, ourvector<string>& dna, ourvector<int> STRcounts) {
	// Check if database has been loaded. If it has, print names and STR counts
	if(database.size() == 0) {
		cout << "No database loaded." << endl;
	} else {
		cout << "Database loaded: " << endl;
		for(int i = 0; i < database.size(); i++) {
			cout << database[i].name;
			for(int j = 0; j < database[i].numSequences.size(); j++) {
				cout << " " << database[i].numSequences[j];
			}
			cout << endl;
		}
		cout << endl;
	}
	// Check if dna has been loaded. If it has, print the sequence
	if(dna.size() == 0) {
		cout << "No DNA loaded." << endl;
	} else {
		cout << "DNA loaded: " << endl;
		for(int i = 0; i < dna.size(); i++) {
			cout << dna[i];
		}
		cout << endl;
	}
	// Call function to print results of the process algorithm
	displayCounts(false, STR, STRcounts);
}

// Function load_dna: Takes a file stream containing a DNA sequence
// and inserts it into an ourvector base by base
void load_dna(ifstream& inFS, ourvector<string>& dna) {
	if(dna.size() > 0) {
		dna.clear();
	}
    string s;
    getline(inFS, s);
    for (unsigned int i = 0; i < s.length(); i++) {
    	dna.push_back(s.substr(i, 1));
    }
}

// CREATIVE COMPONENT - Function findAllSTR:
// Counts every instance of a specific STR and returns the count
// Parameters: An STR and the DNA sequence from the input
int findAllSTR(ourvector<string>& STR, ourvector<string>& dna) {
	int count = 0;
	for(int i = 0; i < dna.size(); i++) {
		// Check to see if character matches the first character of the sequence
		if(dna[i] == STR[0]) {
			// Make sure that search does not go out of bounds
			if(i + STR.size() > dna.size())
				return count;
			// Check if subsequent characters match the STR
			for(int j = 1; j < STR.size(); j++) {
				if(i+j >= dna.size()) {
					return count;
				}
				// If they don't match, break out of the loop
				if(dna[i+j] != STR[j]) {
					break;
				} else if (j == STR.size()-1) {
					count++;
				}
			}
		}
	}
	return count;
}

// Function findSTR: Algorithm to find the longest, consecutive STR sequence
// Returns the number of times this sequence repeats
// Parameters: An STR and the DNA sequence from the input
int findSTR(ourvector<string>& STR, ourvector<string>& dna) {
	int count = 0;
	int j = 0;
	for(int i = 0; i < dna.size(); i++) {
		// Check to see if character matches the first character of the sequence
		if(dna[i] == STR[0]) {
			while(1) {
				j++;
				// Make sure that search does not go out of bounds
				if(i+j >= dna.size()) {
					if((j / STR.size()) > count) {
						count = j / STR.size();
					}
					return count;
				}
				// If any subsequent characters do not match the STR, break out of the loop
				if(dna[i+j] != STR[j % (STR.size())])
					break;
			}
			// Determine the number of times, STR has repeated
			// If the number is greater than the current count, update the count variable
			if((j / STR.size()) > count) {
				count = j / STR.size();
			}
		}
		// Reset iterator variable
		j = 0;
	}
	return count;
}

// Function process: Checks if database and DNA have been loaded
// Then, calls a function to fill an ourvector of STR counts
// Also calls a function if creative component command is input
// Parameters: bool to distinguish between normal function and creative function
// ourvectors for STR and DNA to search through them
// and ourvector for STRcounts to fill with results of search
void process(bool allSTR, ourvector<ourvector<string>>& STR, ourvector<string>& dna, ourvector<int>& STRcounts) {
    if (STR.size() == 0) {
    	cout << "No database loaded." << endl;
    	return;
    } else if (dna.size() == 0) {
    	cout << "No DNA loaded." << endl;
    	return;
    }
    cout << "Processing DNA..." << endl;
    // Check if creative component was input
    if (allSTR) {
    	// Create new ourvector, then call function for each STR
    	ourvector<int> allSTRcount;
    	for (int i = 0; i < STR.size(); i++) {
    		int countSTR = findAllSTR(STR[i], dna);
    		allSTRcount.push_back(countSTR);
    	}
    	// Print the results of the search
    	displayCounts(true, STR, allSTRcount);
    }
    for (int i = 0; i < STR.size(); i++) {
    	// Call function for each individual STR
    	int count = findSTR(STR[i], dna);
    	STRcounts.push_back(count);
    }
}

// Function search: Checks to see if input DNA sequence
// matches an individual in the database
// Parameters: ourvectors for database and STRcounts to compare their data
// and ourvectors for STR and dna to ensure that files were input
void search(ourvector<ourvector<string>>& STR, ourvector<string>& dna, ourvector<int>& STRcounts, ourvector<profile>& database) {
	// Check if database and DNA were loaded, and DNA was processed
    if (STR.size() == 0) {
    	cout << "No database loaded." << endl;
    	return;
    } else if (dna.size() == 0) {
    	cout << "No DNA loaded." << endl;
    	return;
    } else if (STRcounts.size() == 0) {
    	cout << "No DNA processed." << endl;
    	return;
    }
    cout << "Searching database..." << endl;
    // Compare each value from the database vector and 
    // STRcounts vector to try and find a match
    for (int i = 0; i < database.size(); i++) {
    	for (int j = 0; j < database[i].numSequences.size(); j++) {
    		if (database[i].numSequences[j] != STRcounts[j]) {
    			break;
    		} else if (j == database[i].numSequences.size()-1) {
    			cout << "Found in database!  DNA matches: " << database[i].name << endl;
    			return;
    		}
    	}
    }
    cout << "Not found in database." << endl;
}

// Function getFile: Used to open and validate files
// The files will be sent to either load_db or load_dna
// depending on which was input by the user
// Parameters: bool to distinguish between load_db and load_dna
// Vectors for STR, database, and dna so they can be filled with data from files
void getFile(bool isDB, ourvector<ourvector<string>>& STR, ourvector<profile>& database, ourvector<string>& dna) {
	if(isDB) {
		cout << "Loading database..." << endl;
	} else {
		cout << "Loading DNA..." << endl;
	}
    string file_name;
    cin >> file_name;
    ifstream inFS;
    inFS.open(file_name);
    // Check if the input file is able to be opened,
    // otherwise print error message and return
    if (!inFS.is_open()) {
    	cout << "Error: unable to open '" << file_name << "'" << endl;
    	return;
    } else if (isDB) {
    	load_STR(inFS, STR, database);
    } else {
    	load_dna(inFS, dna);
    }
}

int main() {
    cout << "Welcome to the DNA Profiling Application." << endl;
    string command;
    // Create ourvectors to store data from files
    ourvector<ourvector<string>> STR;
    ourvector<profile> database;
    ourvector<string> dna;
    ourvector<int> STRcounts;
    // Create a loop to repeatedly get user input until '#' is typed
    while (1) {
        cout << "Enter command or # to exit: ";
        cin >> command;
        if (command == "#") {
            break;
        } else if (command == "load_db") {
            getFile(true, STR, database, dna);
        } else if (command == "display") {
            display(STR, database, dna, STRcounts);
        } else if (command == "load_dna") {
            getFile(false, STR, database, dna);
        } else if (command == "process") {
            process(false, STR, dna, STRcounts);
        } else if (command == "search") {
            search(STR, dna, STRcounts, database);
        } else if (command == "process_all") {
        	process(true, STR, dna, STRcounts);
        } else {
            cout << endl << "Invalid input. Please try again." << endl;
        }
    }
    return 0;
}
