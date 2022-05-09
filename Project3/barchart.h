// barchart.h
// Shayan Rasheed
// CS 251 - Project 36
// file containing private and public members for the barchart class
// which is used for each individual frame of the animation

#include <iostream>
#include <algorithm>
#include <map>
#include "myrandom.h" // used in autograder, do not remove
#include "bar.h"

using namespace std;

// Constants used for bar chart animation.  You will primarily
// use these in barchartanimate.h, but you might find some useful here.
const string BOX = "\u29C8";
const string CLEARCONSOLE = "\033[2J";

// Color codes for Mimir (light mode)
// const string RED("\033[1;36m");
// const string PURPLE("\033[1;32m");
// const string BLUE("\033[1;33m");
// const string CYAN("\033[1;31m");
// const string GREEN("\033[1;35m");
// const string GOLD("\033[1;34m");
const string BLACK("\033[1;37m");
// const string RESET("\033[0m");
// const vector<string> COLORS = {RED, PURPLE, BLUE, CYAN, GREEN, GOLD, BLACK};

// Color codes for Codio (light/dark mode)
const string CYAN("\033[1;36m");
const string GREEN("\033[1;32m");
const string GOLD("\033[1;33m");
const string RED("\033[1;31m");
const string PURPLE("\033[1;35m");
const string BLUE("\033[1;34m");
const string WHITE("\033[1;37m");
const string RESET("\033[0m");
const vector<string> COLORS = {CYAN, GREEN, GOLD, RED, PURPLE, BLUE, WHITE};

//
// BarChart
//
class BarChart {
 private:
    Bar* bars;  // pointer to a C-style array
    int capacity;
    int size;
    string frame;
    
 public:
    
    // default constructor:
    // Does not allocate memory and sets all private variables to default values
    BarChart() {
        bars = nullptr;
        capacity = 0;
        size = 0;
        frame = "";
    }
    
    // parameterized constructor:
    // Parameter passed in determines memory allocated for bars.
    BarChart(int n) {
        bars = new Bar[n];
        capacity = n;
        size = 0;
        frame = "";
    }

    //
    // copy constructor:
    // Creates a copy of a Barchart object 
    // including all private variables and every element of the bars array
    BarChart(const BarChart& other) {
        this->bars = new Bar[other.capacity];
        this->size = other.size;
        this->capacity = other.capacity;
        this->frame = other.frame;
        for (int i = 0; i < other.size; ++i) {
          this->bars[i] = other.bars[i];
        }
    }
    //
    // copy operator=
    // Overloads the = operator so that Barcharts can be copied via an = statement
    BarChart& operator=(const BarChart& other) {
        // Check if object on both sides of = are the same
        if (this == &other)
          return *this;
        // Start by removing elements in this array so that the other array can be copied
        delete[] bars;
        // Now copy all private variables in other and return
        this->bars = new Bar[other.capacity];
        this->size = other.size;
        this->capacity = other.capacity;
        this->frame = other.frame;
        for (int i = 0; i < other.size; ++i) {
          this->bars[i] = other.bars[i];
        }
        return *this;
    }

    // clear
    // frees memory and resets all private member variables to default values.
    void clear() {
        delete[] bars;
        bars = nullptr;
        capacity = 0;
        size = 0;
        frame = "";
    }
    
    //
    // destructor:
    //
    // Called automatically by C++ to free the memory associated by the
    // BarChart.
    //
    virtual ~BarChart() {
      if(bars != nullptr) {
        delete[] bars;
      }
    }
    
    // setFrame
    void setFrame(string frame) {
        this->frame = frame;
    }
    
    // getFrame()
    // Returns the frame of the BarChart oboject.
    string getFrame() {
        return frame;
    }

    // addBar
    // adds a Bar to the BarChart.
    // returns true if successful
    // returns false if there is not room
    bool addBar(string name, int value, string category) {
        // Check if BarChart is full
        if(size == capacity) {
          return false;
        }
        // Add new Bar to the array
        Bar b(name, value, category);
        bars[size] = b;
        size++;
        return true;
    }
    
    // getSize()
    // Returns the size (number of bars) of the BarChart object.
    int getSize() {
        return size;
    }
    
    // operator[]
    // Returns Bar element in BarChart.
    // This gives public access to Bars stored in the Barchart.
    // throws an error if i is not within the range of the array
    Bar& operator[](int i) {
        if(i >= size) {
          throw out_of_range("Barchart: i out of bounds");
        }
        return bars[i];
    }
    
    // dump
    // Used for printing the BarChart object.
    // Sorts and then outputs all elements in the array
    void dump(ostream &output) {
        sort(bars, bars+size, greater<Bar>());
        output << "frame: " << frame << endl;
        for (int i = 0; i < size; i++) {
          output << bars[i].getName() << " ";
          output << bars[i].getValue() << " ";
          output << bars[i].getCategory();
          output << endl;
        }
    }
    
    // graph
    // Used for printing out the bar.
    // output is any stream (cout, file stream, string stream)
    // colorMap maps category -> color
    // top is number of bars you'd like plotted on each frame (top 10? top 12?)
    // Sorts and then outputs top # of bars to display each frame
    void graph(ostream &output, map<string, string> &colorMap, int top) {
        int lenMax = 60;  // this is number of BOXs that should be printed
                          // for the top bar (max value)
        // Check if bars is empty
        if(size == 0) {
          return;
        }
        // Sort the array with the greatest value bar at the top 
        sort(bars, bars+size, greater<Bar>());
        for (int i = 0; i < top; i++) {
          // If the current bar's category is in the color map
          // Set the color to the one stored in the map
          // Otherwise, set color to black
          string color;
          if (colorMap.count(bars[i].getCategory()) == 0) {
            color = COLORS[6];
          } else {
            color = colorMap[bars[i].getCategory()];
          }
          string barstr = "";
          // Calculate length of each bar
          double temp = double(bars[i].getValue()) / double(bars[0].getValue());
          int barLength = temp * lenMax;
          if(i == 0){
             barLength = lenMax;
          } // Create the bar using boxes
          for(int j = 0; j < barLength; j++) {
              barstr += BOX;
          } // Print out the bar with its name and value
          output << color << barstr << " " << bars[i].getName() << " " << bars[i].getValue() << endl;
        }
    } 
};

