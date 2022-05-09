// bar.h
// Shayan Rasheed
// CS 251 - Project 3
// File containing the private and public members of the bar class
// that is used for each individual bar in the bar chart

#include <iostream>
#include <string>
#include "myrandom.h" // used in autograder, do not remove
using namespace std;

//
// Bar
//
class Bar {
 private:
    // Private member variables for a Bar object
    string name;
    int value;
    string category;

 public:

    // default constructor:
    Bar() {
        name = "";
        value = 0;
        category = "";
    }

    //
    // a second constructor:
    //
    // Parameter passed in constructor Bar object.
    //
    Bar(string name, int value, string category) {
        this->name = name;
        this->value = value;
        this->category = category;
    }

    // destructor:
    virtual ~Bar() {
        
    }

    // getName:
	string getName() {
        return name;
	}

    // getValue:
	int getValue() {
        return value;
	}

    // getCategory:
	string getCategory() {
        return category;
	}

	// operators
	bool operator<(const Bar &other) const {
        return (this->value < other.value);
	}

	bool operator<=(const Bar &other) const {
        return (this->value <= other.value);  
	}

	bool operator>(const Bar &other) const {
        return (this->value > other.value);  
	}

	bool operator>=(const Bar &other) const {
        return (this->value >= other.value);  
	}
};

