// barchartanimate.h
// Shayan Rasheed
// CS 251 - Project 3
// File to parse and store data from files for the barchart animation
// Contains an array of BarCharts in which each element represents a frame in the animation

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <map> 
#include <vector>
#include <unistd.h>
#include "myrandom.h" // used in autograder, do not remove
#include "barchart.h"
  
using namespace std;

//
// BarChartAnimate
//
class BarChartAnimate {
 private:
    //
    // Private member variables for the abstraction.
    // Contains a dynamic array of BarChart objects
    // Size and Capacity variables keep track of number of array elements
    // colorMap contains a color value for each unique category in the data
    BarChart* barcharts;  // pointer to a C-style array
    int size;
    int capacity;
    map<string, string> colorMap;
    int colorIndex;
    string title;
    string xlabel;
    string source;
  
  // addToArray
  // Takes a barchart object as a parameter
  // and adds it to the barcharts array
  void addToArray(BarChart& bc) {
    // If barchart is full, then double the capacity
      if (size == capacity) {  
        int newCapacity = capacity * 2;
        BarChart* newBarCharts = new BarChart[newCapacity];
        for (int i = 0; i < size; ++i) {
          newBarCharts[i] = barcharts[i];
        }
        delete[] barcharts;
        barcharts = newBarCharts;
        capacity = newCapacity;
      }
      // add the new barchart to the array
      barcharts[size] = bc;
      size++;
  }

 public:

    // a parameterized constructor:
    // Like the ourvector, the barcharts C-array should be constructed here
    // with a capacity of 4.
    BarChartAnimate(string title, string xlabel, string source) {
        barcharts = new BarChart[4];
        size = 0;
        capacity = 4;
        colorIndex = 0;
        this->title = title;
        this->xlabel = xlabel;
        this->source = source;
    }

    //
    // destructor:
    //
    // Called automatically by C++ to free the memory associated
    // by BarChartAnimate.
    //
    virtual ~BarChartAnimate() {
      if(barcharts != nullptr) {
        delete[] barcharts;
      }
    }

    // addFrame:
    // adds a new BarChart to the BarChartAnimate object from the file stream.
    // if the barcharts has run out of space, double the capacity
    void addFrame(ifstream &file) {
      // get the number of bars in the frame
    	int numBars = 0;
      file >> numBars;
      if(numBars == 0) {
        return;
      }
      BarChart bc(numBars);
      string currFrame, currName, currCountry, currCategory, currValue;
      // Use temp to get empty line from file
      string temp;
      getline(file, temp);
      // Get the frame from the file and store it in the new barchart
      getline(file, currFrame, ',');
      bc.setFrame(currFrame);
      // Create loop to get all bars in the frame
      for(int i = 0; i < numBars; i++) {
        // Use getline to get the name, value, and category of the bar
        if(i != 0) {
          getline(file, currFrame, ',');
        }
        getline(file, currName, ',');
        getline(file, currCountry, ',');
        getline(file, currValue, ',');
        int val = stoi(currValue);
        getline(file, currCategory);
        // If the category is not in the color map, then create a new element in the map
        if(colorMap.count(currCategory) == 0) {
          colorMap.emplace(currCategory, COLORS[colorIndex % 7]);
          colorIndex++;
        }
        // add a new bar to the barchart with the data from the file
        bc.addBar(currName, val, currCategory);
      }
      addToArray(bc);
    }

    // animate:
    // this function plays each frame stored in barcharts.
    // The function pauses and clears the console in between each frame to create the animation
	void animate(ostream &output, int top, int endIter) {
        unsigned int microsecond = 50000;
        if (endIter == -1) {
          endIter = size;
        }
        for (int i = 0; i < endIter; i++) {
          // Set delay period and clear console
          usleep(3 * microsecond);
          output << CLEARCONSOLE;
          // Output title and source in black
          output << COLORS[6] << title << endl << source << endl;
          // Graph the current frame
          barcharts[i].graph(output, colorMap, top);
          // Output xlabel and Frame number in black
          output << COLORS[6] << xlabel << endl << "Frame: " << barcharts[i].getFrame() << endl;
        } 
	}
  
  // Overloaded function animate for use in creative function
  // animates chart within a specific boundary set by user
  void animate(ostream &output, int top, int startIter, int endIter) {
        unsigned int microsecond = 50000;
        if (endIter == -1) {
          endIter = size;
        }
        for (int i = startIter; i < endIter; i++) {
          // Set delay period and clear console
          usleep(3 * microsecond);
          output << CLEARCONSOLE;
          // Output title and source in black
          output << COLORS[6] << title << endl << source << endl;
          // Graph the current frame
          barcharts[i].graph(output, colorMap, top);
          // Output xlabel and Frame number in black
          output << COLORS[6] << xlabel << endl << "Frame: " << barcharts[i].getFrame() << endl;
        } 
	}

    //
    // Public member function.
    // Returns the size of the BarChartAnimate object.
    //
    int getSize(){
        return size;
    }

    //
    // Public member function.
    // Returns BarChart element in BarChartAnimate.
    // This gives public access to BarChart stored in the BarChartAnimate.
    // If i is out of bounds, throw an out_of_range error message:
    // "BarChartAnimate: i out of bounds"
    //
    BarChart& operator[](int i){
        if (i >= size) {
          throw out_of_range("BarChartAnimate: i out of bounds");
        }
      return barcharts[i];
    }
};
