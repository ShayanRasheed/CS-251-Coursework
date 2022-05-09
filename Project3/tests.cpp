#include <iostream>
#include "barchartanimate.h"
using namespace std;


bool testBarDefaultConstructor() {
	Bar b;
    if (b.getName() != "") {
    	cout << "testBarDefaultConstructor: getName failed" << endl;
    	return false;
    } else if (b.getValue() != 0) {
    	cout << "testBarDefaultConstructor: getValue failed" << endl;
    	return false;
    } else if (b.getCategory() != "") {
    	cout << "testBarDefaultConstructor: getCategory failed" << endl;
    	return false;
    }
    cout << "testBarDefaultConstructor: all passed!" << endl;
    return true;
}

bool testBarParamConstructor() {
	Bar b("Chicago", 9234324, "US");
    if (b.getName() != "Chicago") {
    	cout << "testBarParamConstructor: getName failed" << endl;
    	return false;
    } else if (b.getValue() != 9234324) {
    	cout << "testBarParamConstructor: getValue failed" << endl;
    	return false;
    } else if (b.getCategory() != "US") {
    	cout << "testBarParamConstructor: getCategory failed" << endl;
    	return false;
    }
    cout << "testBarParamConstructor: all passed!" << endl;
    return true;
}

// This function tests the operators <, >, <=, and >= in bar.h
bool testOperators() {
  Bar b1("a", 1, "cat");
  Bar b2("b", 2, "cat");
  if(!(b1 < b2)) {
    cout << "testOperators: < failed" << endl;
    return false;
  } else if(b2 < b1) {
    cout << "testOperators: < failed" << endl;
    return false;
  } else if(b1 > b2) {
    cout << "testOperators: > failed" << endl;
    return false;
  } else if(!(b2 > b1)) {
    cout << "testOperators: > failed" << endl;
    return false;
  } else if(!(b1 <= b1)) {
    cout << "testOperators: <= failed" << endl;
    return false;
  } else if(!(b2 >= b2)) {
    cout << "testOperators: >= failed" << endl;
    return false;
  } else if(b1 >= b2) {
    cout << "testOperators: >= failed" << endl;
    return false;
  } else if(b2 <= b1) {
    cout << "testOperators: <= failed" << endl;
    return false;
  }
  cout << "testOperators: all passed!" << endl;
  return true;
} 

// This function tests the Barchart constructors as well as getFrame and setFrame 
bool testBarchartConstructors() {
  BarChart bc;
  if(!(bc.getFrame() == "")) {
    cout << "BarchartConstructors: getFrame failed" << endl;
    return false;
  } else if(!(bc.getSize() == 0)) {
    cout << "BarchartConstructors: getSize failed" << endl;
    return false;
  }
  BarChart bc2(10);
  if(!(bc2.getFrame() == "")) {
    cout << "BarchartConstructors: getFrame failed" << endl;
    return false;
  } else if(!(bc2.getSize() == 0)) {
    cout << "BarchartConstructors: getSize failed" << endl;
    return false;
  }
  // Test setFrame
  bc.setFrame("1950");
  if(!(bc.getFrame() == "1950")) {
    cout << "BarchartConstructors: setFrame failed" << endl;
    return false;
  }
  cout << "BarchartConstructors: all passed!" << endl;
  return true;
}

// This function tests the copy constructor and operator= for BarChart
bool testCopyConstructor(BarChart bc) {
  if(bc.addBar("testName4", 400, "testCategory4")) {
    cout << "testCopyConstructor: addBar no room test failed" << endl;
    return false;
  } else if(bc[0].getName() != "testName") {
    cout << "testCopyConstructor: addBar getName test 1 failed" << endl;
    return false;
  } else if(bc[1].getName() != "testName2") {
    cout << "testCopyConstructor: addBar getName test 2 failed" << endl;
    return false;
  } else if(bc[2].getName() != "testName3") {
    cout << "testCopyConstructor: addBar getName test 3 failed" << endl;
    return false;
  }
  // Test operator=
  BarChart bcCopy = bc;
  if(bc.addBar("testName4", 400, "testCategory4")) {
    cout << "testOperator=: addBar no room test failed" << endl;
    return false;
  } else if(bc[0].getCategory() != "testCategory") {
    cout << "testOperator=: addBar getCategory test 1 failed" << endl;
    return false;
  } else if(bc[1].getCategory() != "testCategory2") {
    cout << "testOperator=: addBar getCategory test 2 failed" << endl;
    return false;
  } else if(bc[2].getCategory() != "testCategory3") {
    cout << "testOperator=: addBar getCategory test 3 failed" << endl;
    return false;
  }
  cout << "testCopyConstructor: all passed!" << endl;
  return true;
}

// This function tests addBar, operator[], and clear() for Barchart
bool testAddBar() {
  BarChart bc(3);
  bc.setFrame("1950");
  bc.addBar("testName", 100, "testCategory");
  bc.addBar("testName2", 200, "testCategory2");
  bc.addBar("testName3", 300, "testCategory3");
  if(bc.addBar("testName4", 400, "testCategory4")) {
    cout << "AddBar: addBar no room test failed" << endl;
    return false;
  } else if(bc[0].getValue() != 100) {
    cout << "AddBar: addBar getValue test 1 failed" << endl;
    return false;
  } else if(bc[1].getValue() != 200) {
    cout << "AddBar: addBar getValue test 2 failed" << endl;
    return false;
  } else if(bc[2].getValue() != 300) {
    cout << "AddBar: addBar getValue test 3 failed" << endl;
    return false;
  }
  cout << "AddBar: all passed!" << endl;
  testCopyConstructor(bc);
  //test clear();
  bc.clear();
  if(!(bc.getFrame() == "")) {
    cout << "AddBar: Clear test 1 failed" << endl;
    return false;
  } else if(!(bc.getSize() == 0)) {
    cout << "AddBar: Clear test 2 failed" << endl;
    return false;
  }
  cout << "Clear: all passed!" << endl;
  return true;
}

// This function tests animate() for BarChartAnimate
bool testAnimate(BarChartAnimate& bca) {
  stringstream ss;
  bca.animate(ss, 2, -1);
  string test;
  getline(ss, test);
  getline(ss, test);
  if(test != "Sources: SEDAC; United Nations; Demographia") {
    cout << "BCA: Animate test 1 failed" << endl;
    return false;
  }
  string line2 = CYAN;
  string line3 = GOLD;
  for(int i = 0; i < 60; i++) {
    line2 += BOX;
    if(i < 44)
      line3 += BOX;
  }
  line2 += " Beijing 672";
  line3 += " Vijayanagar 500";
  getline(ss, test);
  if(line2 != test) {
    cout << "BCA: Animate test 2 failed" << endl;
    return false;
  }
  getline(ss, test);
  if(line3 != test) {
    cout << "BCA: Animate test 3 failed" << endl;
    return false;
  }
  getline(ss, test);
  if(test != COLORS[6] + "Population (thousands)") {
    cout << "BCA: Animate test 4 failed" << endl;
    return false;
  }
  getline(ss, test);
  if(test != "Frame: 1500") {
    cout << "BCA: Animate test 5 failed" << endl;
    return false;
  }
  cout << "Animate: all passed!" << endl;
  return true;
}

// This function tests the constructor and operator[] for barchartanimate
// and the dump function for barchart
bool testBCA(BarChartAnimate& bca){
  stringstream ss;
  bca[0].dump(ss);
  string test;
  getline(ss, test);
  if(test != "frame: 1500") {
    cout << "BCA: Dump test 1 failed" << endl;
    return false;
  }
  getline(ss, test);
  if(test != "Beijing 672 East Asia") {
    cout << "BCA: Dump test 2 failed" << endl;
    return false;
  }
  getline(ss, test);
  if(test != "Vijayanagar 500 South Asia") {
    cout << "BCA: Dump test 3 failed" << endl;
    return false;
  }
  getline(ss, test);
  if(test != "Cairo 400 Middle East") {
    cout << "BCA: Dump test 4 failed" << endl;
    return false;
  }
  
  testAnimate(bca);
  
  cout << "BCA: all passed!" << endl;
  return true;
}

int main() {
	testBarDefaultConstructor();
	testBarParamConstructor();
  testOperators();
  testBarchartConstructors();
	testAddBar();
  
  // instantiate a barchartanimate object for the next tests
  ifstream inFile;
  inFile.open("cities.txt");
  string title;
	getline(inFile, title);
	string xlabel;
	getline(inFile, xlabel);
	string source;
	getline(inFile, source);
  BarChartAnimate bca(title, xlabel, source);
  
  bca.addFrame(inFile);
  
  testBCA(bca);
  
    return 0;
}