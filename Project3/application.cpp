// Bar Chart Animation
//
// Shayan Rasheed
// U. of Illinois, Chicago
// CS 251 - Project 3
//
// CREATIVE COMPONENT:
// After running the program, it will prompt you for a file name
// File names will be printed and you will be prompted until a valid name is entered
// Then the program will prompt you to input which frame to start the animation on
// This frame can be any between 0 and the final frame
// If you input -1 for the starting frame, then the entire animation will be displayed
// Otherwise, the program will prompt you for the ending frame
// This value can be any between the starting frame + 1 and the final frame + 1
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "barchartanimate.h"
using namespace std;


int main() {
  cout << "baby-names.txt" << endl;
  cout << "brands.txt" << endl;
  cout << "cities-usa.txt" << endl;
  cout << "cities.txt" << endl;
  cout << "countries.txt" << endl;
  cout << "endgame.txt" << endl;
  cout << "football.txt" << endl;
  cout << "game-of-thrones.txt" << endl;
  cout << "infinity-war.txt" << endl;
  cout << "Enter a file name: " << endl;
	string filename;
  cin >> filename;
	ifstream inFile;
  inFile.open(filename);
  while(!inFile.is_open()) {
    cout << "Could not open file. Please try again." << endl;
    cin >> filename;
    inFile.open(filename);
  }
	string title;
	getline(inFile, title);
	string xlabel;
	getline(inFile, xlabel);
	string source;
	getline(inFile, source);

	BarChartAnimate bca(title, xlabel, source);
	
	while (!inFile.eof()) {
		bca.addFrame(inFile);
	}
  
  int startIter = -999;
  int endIter = -999;
  while(-1 > startIter || startIter >= bca.getSize()) {
    cout << "Enter which frame to start the animation on." << endl;
    cout << "Valid frames: 0 - " << bca.getSize()-1 << endl;
    cout << "Or enter -1 to play entire animation." << endl;
    cin >> startIter;
  }
  if(startIter == -1) {
    bca.animate(cout, 12, -1);
  } else {
    while(startIter >= endIter || endIter > bca.getSize()) {
      cout << "Enter which frame to end the animation on." << endl;
      cout << "Valid frames: " << startIter+1 << " - " << bca.getSize() << endl;
      cin >> endIter;
    }
    bca.animate(cout, 12, startIter, endIter);
  }

    return 0;
}
