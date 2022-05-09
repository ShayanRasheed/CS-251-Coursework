// application.cpp
// Shayan Rasheed
//
// University of Illinois at Chicago
// CS 251: Fall 2021
// Project #7 - Openstreet Maps
//
// References:
// TinyXML: https://github.com/leethomason/tinyxml2
// OpenStreetMap: https://www.openstreetmap.org
// OpenStreetMap docs:
//   https://wiki.openstreetmap.org/wiki/Main_Page
//   https://wiki.openstreetmap.org/wiki/Map_Features
//   https://wiki.openstreetmap.org/wiki/Node
//   https://wiki.openstreetmap.org/wiki/Way
//   https://wiki.openstreetmap.org/wiki/Relation
//

// CREATIVE COMPONENT:
// Takes in a list of locations and checks
// to see if a destination is reachable
// You will first be prompted to enter a destination building
// Then you will be prompted to enter locations of different people
// Until # is entered
// The console will return the distances and paths for each person
// If invalid input or unreachable destination occurs,
// error message will be displayed
// The application allows for up to four people's locations

#include <iostream>
#include <iomanip>  /*setprecision*/
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <queue>
#include <stack>

#include "tinyxml2.h"
#include "dist.h"
#include "osm.h"
#include "graph.h"

using namespace std;
using namespace tinyxml2;

const double INF = numeric_limits<double>::max();
const long long MAX = numeric_limits<long long>::max();

class prioritize {
      public:
      bool operator()(const pair<long long, double>& p1,
                      const pair<long long, double>& p2) const {
        return p1.second > p2.second;
      }
};

// Function searchBuilding:
// Returns the first building in the vector that contains the search query
bool searchBuilding(string query, BuildingInfo& building,
           vector<BuildingInfo>& Buildings) {
  for (auto b : Buildings) {
    if (b.Fullname.find(query) != string::npos) {
      building = b;
      return true;
    }
    if (b.Abbrev.find(query) != string::npos) {
      building = b;
      return true;
    }
  }
  return false;
}

// Function nearestBuilding:
// Returns the info of the building that is closest to the given coordinate
BuildingInfo nearestBuilding(Coordinates midpoint,
        vector<BuildingInfo>& Buildings, set<string>& unreachableBuildings) {
  double min = INF;
  BuildingInfo nearest;
  for (auto b : Buildings) {
    double dist = distBetween2Points(midpoint.Lat, midpoint.Lon, b.Coords.Lat, b.Coords.Lon);
    // Check that building is reachable
    if (dist < min && unreachableBuildings.count(b.Fullname) == 0) {
      min = dist;
      nearest = b;
    }
  }
  return nearest;
}

// Function: nearestNode
// returns the value of the node that
// is closest to the cooridnates of the given building
long long nearestNode(BuildingInfo b, map<long long,
         Coordinates>& Nodes, vector<FootwayInfo>& Footways) {
  double min = INF;
  long long nearest;
  for (auto f : Footways) {
    for (unsigned int i = 0; i < f.Nodes.size(); i++) {
      Coordinates c = Nodes[f.Nodes[i]];
      double dist = distBetween2Points(b.Coords.Lat, b.Coords.Lon, c.Lat, c.Lon);
      if (dist < min) {
        min = dist;
        nearest = c.ID;
      }
    }
  }
  return nearest;
}

// Function dijkstra:
// Performs the dijkstra algorithm to return a vector of visited nodes
// as well as a map of distances from a starting vertex
vector<long long> dijkstra(long long startV, graph<long long, double>& G,
      map<long long, double>& distances, map<long long,
      long long>& predecessors) {
  priority_queue<
    pair<long long, double>,
    vector<pair<long long, double>>,
    prioritize> unvisitedQueue;
  // Initialize a vector and set of visited vertices
  vector<long long> visited;
  set<long long> visitedSet;

  // Get a vector of all vertices in the graph
  vector<long long> graphVerts = G.getVertices();
  for (auto v : graphVerts) {
    // Set the distances to all vertices to 0
    distances[v] = INF;
    // Set the predecessors of all vertices to null
    predecessors[v] = 0;
    // push the vertex into the queue with a value of INF
    unvisitedQueue.push(make_pair(v, INF));
  }

  // Set the distance to the starting vertex to 0
  distances[startV] = 0;
  unvisitedQueue.push(make_pair(startV, 0));

  while (!unvisitedQueue.empty()) {
    // get the top of the queue
    long long currV = unvisitedQueue.top().first;

    // if the value of currV is INF, break out of the loop
    if (unvisitedQueue.top().second == INF) {
      break;
      // If the vertex has already been visited, continue the loop
    } else if (visitedSet.count(currV) != 0) {
      unvisitedQueue.pop();
      continue;
    }

    unvisitedQueue.pop();

    // add curV to the visited list
    visited.push_back(currV);
    visitedSet.insert(currV);

    // Loop through the neighbors of currV
    set<long long> neighbors = G.neighbors(currV);
    for (auto n : neighbors) {
      double edgeWeight;
      G.getWeight(currV, n, edgeWeight);
      // Update the distance in the map for currV
      double altDistance = distances[currV] + edgeWeight;

      if (altDistance < distances[n]) {
        distances[n] = altDistance;
        // Update predecessors for the neighbor
        predecessors[n] = currV;
        // push the neighbor into the queue with its new distance
        unvisitedQueue.push(make_pair(n, altDistance));
      }
    }
  }
  return visited;
}

// Function getPath:
// returns a vector that represents a path
// to an end vertex based on a predecessors map
vector<long long> getPath(map<long long, long long>& predecessors,
       long long endV) {
  vector<long long> result;
  // The ending vertex will be pushed onto the stack first
  long long currV = endV;
  stack<long long> vStack;
  while (currV != 0) {
    vStack.push(currV);
    // set currV to the corresponding value in the predecessors map
    currV = predecessors[currV];
  }
  // Now push each item of the stack into the vector
  while (!vStack.empty()) {
    currV = vStack.top();
    vStack.pop();
    result.push_back(currV);
  }
  return result;
}

// Function printPath:
// Prints out the contents of two path vectors to the console
void printPath(vector<long long>& paths1, vector<long long>& paths2,
     map<long long, double>& distances1, map<long long, double>& distances2,
     long long& centerNode) {
  cout << "Person 1's distance to dest: ";
  cout << distances1[centerNode] << " miles" << endl;
  cout << "Path: ";
  for (unsigned int i = 0; i < paths1.size(); i++) {
    if (i == paths1.size()-1) {
      cout << paths1[i] << endl;
    } else {
      cout << paths1[i] << "->";
    }
  }

  cout << endl;

  cout << "Person 2's distance to dest: ";
  cout << distances2[centerNode] << " miles" << endl;
  cout << "Path: ";
  for (unsigned int i = 0; i < paths2.size(); i++) {
    if (i == paths2.size()-1) {
      cout << paths2[i] << endl;
    } else {
      cout << paths2[i] << "->";
    }
  }
}

//
// Implement your creative component application here
//
void creative(graph<long long, double>& G,
    map<long long, Coordinates>& Nodes, vector<FootwayInfo>& Footways,
    vector<BuildingInfo>& Buildings) {
  string name;
  vector<string> names;
  cout << endl;

  cout << "Enter destination for all people: ";
  string destination;
  getline(cin, destination);

  cout << "Enter 1st person's location (partial name or abbreviation), or #> ";
  getline(cin, name);
  int i = 0;

  // Continue to get names and store into vector
  // until # is entered
  while (name != "#" && i < 3) {
    cout << "Enter next person's location (partial name or abbreviation) or # to stop>";
    names.push_back(name);
    getline(cin, name);
    i++;
  }

  vector<BuildingInfo> allInfo;
  vector<long long> nearestNodes;

  // Get building info and nodes for all inputs
  for(unsigned int i = 0; i < names.size(); i++) {
    BuildingInfo info;
    if (searchBuilding(names[i], info, Buildings) == false) {
      cout << "At least one building was not found" << endl;
      return;
    }
    allInfo.push_back(info);
 
    long long nearest = nearestNode(allInfo[i], Nodes, Footways);
    nearestNodes.push_back(nearest);
  }

  // BuildingInfo and node for the destination
  BuildingInfo destInfo;
  if (searchBuilding(destination, destInfo, Buildings) == false) {
    cout << "Destination building not found" << endl;
    return;
  }
  long long destNode = nearestNode(destInfo, Nodes, Footways);

  cout << "Destination:" << endl;
  cout << " " << destInfo.Fullname << endl;
  cout << " (" << destInfo.Coords.Lat << ", ";
  cout << destInfo.Coords.Lon << ")" << endl;

  cout << "Nearest destination node:" << endl;
  cout << " " << destNode << endl;
  cout << " (" << Nodes[destNode].Lat << ", ";
  cout << Nodes[destNode].Lon << ")" << endl;

  cout << endl;

  // For each element in the vector
  // Perform dijkstra's algorithm
  // and print path to dest
  for(unsigned int i = 0; i < names.size(); i++) {
    map<long long, double> distances;
    map<long long, long long> predecessors;
    vector<long long> search = dijkstra(nearestNodes[i], G, distances, predecessors);

    cout << "Starting point:" << endl;
    cout << " " << allInfo[i].Fullname << endl;
    cout << " (" << allInfo[i].Coords.Lat << ", ";
    cout << allInfo[i].Coords.Lon << ")" << endl;

    cout << "Nearest node:" << endl;
    cout << " " << nearestNodes[i] << endl;
    cout << " (" << Nodes[nearestNodes[i]].Lat << ", ";
    cout << Nodes[nearestNodes[i]].Lon << ")" << endl;

    cout << endl;

    // Check if dest is reachable
    if(distances[destNode] >= INF) {
      cout << "Destination is unreachable for this person." << endl;
    } else {
      vector<long long> paths = getPath(predecessors, destNode);

      cout << "This person's distance to dest: ";
      cout << distances[destNode] << " miles" << endl;
      cout << "Path: ";
      for (unsigned int j = 0; j < paths.size(); j++) {
        if (j == paths.size()-1) {
          cout << paths[j] << endl;
        } else {
          cout << paths[j] << "->";
        }
      }
    }

    cout << endl;
  }
}

//
// Implement your standard application here
//
void application(graph<long long, double>& G,
    map<long long, Coordinates>& Nodes, vector<FootwayInfo>& Footways,
    vector<BuildingInfo>& Buildings) {
  string person1Building, person2Building;

  cout << endl;
  cout << "Enter person 1's building (partial name or abbreviation), or #> ";
  getline(cin, person1Building);

  while (person1Building != "#") {
    cout << "Enter person 2's building (partial name or abbreviation)> ";
    getline(cin, person2Building);


    // Search for buildings 1 and 2 based on user input
    BuildingInfo building1;
    BuildingInfo building2;
    bool validInput = true;

    // if either building does not exist,
    // print error message and prompt user again
    if (searchBuilding(person1Building, building1, Buildings) == false) {
      cout << "Person 1's building not found" << endl;
      validInput = false;
    } else if (searchBuilding(person2Building, building2, Buildings) == false) {
      cout << "Person 2's building not found" << endl;
      validInput = false;
    }

    if (validInput) {
      // Calculate the midpoint between the two buildings
      Coordinates midpoint;
      midpoint = centerBetween2Points(building1.Coords.Lat, building1.Coords.Lon, building2.Coords.Lat, building2.Coords.Lon);
      set<string> unreachableBuildings;
      // Find the closest building based on the midpoint
      BuildingInfo center = nearestBuilding(midpoint, Buildings, unreachableBuildings);

      // Find the nearest node to all three buildings
      long long centerNode = nearestNode(center, Nodes, Footways);
      long long nearestNode1 = nearestNode(building1, Nodes, Footways);
      long long nearestNode2 = nearestNode(building2, Nodes, Footways);

      // Perform the dijkstra algorithm for each of the two nodes
      map<long long, double> distances1;
      map<long long, long long> predecessors1;
      vector<long long> search1 = dijkstra(nearestNode1, G, distances1, predecessors1);

      map<long long, double> distances2;
      map<long long, long long> predecessors2;
      vector<long long> search2 = dijkstra(nearestNode2, G, distances2, predecessors2);

      // Print out all of the information found at this point
      cout << "Person 1's point:" << endl;
      cout << " " << building1.Fullname << endl;
      cout << " (" << building1.Coords.Lat << ", ";
      cout << building1.Coords.Lon << ")" << endl;

      cout << "Person 2's point:" << endl;
      cout << " " << building2.Fullname << endl;
      cout << " (" << building2.Coords.Lat << ", ";
      cout << building2.Coords.Lon << ")" << endl;

      cout << "Destination Building:" << endl;
      cout << " " << center.Fullname << endl;
      cout << " (" << center.Coords.Lat << ", ";
      cout << center.Coords.Lon << ")" << endl;

      cout << endl;

      cout << "Nearest P1 node:" << endl;
      cout << " " << nearestNode1 << endl;
      cout << " (" << Nodes[nearestNode1].Lat << ", ";
      cout << Nodes[nearestNode1].Lon << ")" << endl;

      cout << "Nearest P2 node:" << endl;
      cout << " " <<  nearestNode2 << endl;
      cout << " (" << Nodes[nearestNode2].Lat << ", ";
      cout << Nodes[nearestNode2].Lon << ")" << endl;

      cout << "Nearest destination node:" << endl;
      cout << " " <<  centerNode << endl;
      cout << " (" << Nodes[centerNode].Lat << ", ";
      cout << Nodes[centerNode].Lon << ")" << endl;

      cout << endl;

      // Check if there is a path to node2 from node1 and vice versa
      if (distances1[nearestNode2] >= INF || distances2[nearestNode1] >= INF) {
        cout << "Sorry, destination unreachable." << endl;
        cout << endl;
        // Check if a path to the center node exists from both starting nodes
      } else if (distances1[centerNode] >= INF || distances2[centerNode] >= INF) {
        // If not, repeat the process with the next closest point
        // add the center to the set of unreachable Buildings
        unreachableBuildings.insert(center.Fullname);
        cout << "At least one person was unable to reach the destination building. Finding next closest building..." << endl;
        cout << endl;

        BuildingInfo nextCenter = nearestBuilding(midpoint, Buildings, unreachableBuildings);
        centerNode = nearestNode(nextCenter, Nodes, Footways);

        cout << "New destination building:" << endl;
        cout << " " << nextCenter.Fullname << endl;
        cout << " (" << nextCenter.Coords.Lat << ", ";
        cout << nextCenter.Coords.Lon << ")" << endl;

        cout << "Nearest destination node:" << endl;
        cout << " " <<  centerNode << endl;
        cout << " (" << Nodes[centerNode].Lat << ", ";
        cout << Nodes[centerNode].Lon << ")" << endl;

        cout << endl;

        // Again, check if path to new center exists
        if (distances1[centerNode] >= INF || distances2[centerNode] >= INF) {
          unreachableBuildings.insert(nextCenter.Fullname);
          cout << "At least one person was unable to reach the destination building. Finding next closest building..." << endl;
          cout << endl;

          nextCenter = nearestBuilding(midpoint, Buildings, unreachableBuildings);
          centerNode = nearestNode(nextCenter, Nodes, Footways);

          cout << "New destination building:" << endl;
          cout << " " << nextCenter.Fullname << endl;
          cout << " (" << nextCenter.Coords.Lat << ", ";
          cout << nextCenter.Coords.Lon << ")" << endl;

          cout << "Nearest destination node:" << endl;
          cout << " " <<  centerNode << endl;
          cout << " (" << Nodes[centerNode].Lat << ", ";
          cout << Nodes[centerNode].Lon << ")" << endl;

          cout << endl;

          vector<long long> paths1 = getPath(predecessors1, centerNode);
          vector<long long> paths2 = getPath(predecessors2, centerNode);

          printPath(paths1, paths2, distances1, distances2, centerNode);
        } else {
          vector<long long> paths1 = getPath(predecessors1, centerNode);
          vector<long long> paths2 = getPath(predecessors2, centerNode);

          printPath(paths1, paths2, distances1, distances2, centerNode);
        }

      } else {
        // Find and print the paths to the center node
        vector<long long> paths1 = getPath(predecessors1, centerNode);
        vector<long long> paths2 = getPath(predecessors2, centerNode);

        printPath(paths1, paths2, distances1, distances2, centerNode);
      }
    }


    //
    // another navigation?
    //
    cout << endl;
    cout << "Enter person 1's building (partial name or abbreviation), or #> ";
    getline(cin, person1Building);
  }
}

int main() {
  // maps a Node ID to it's coordinates (lat, lon)
  map<long long, Coordinates>  Nodes;
  // info about each footway, in no particular order
  vector<FootwayInfo>          Footways;
  // info about each building, in no particular order
  vector<BuildingInfo>         Buildings;
  XMLDocument                  xmldoc;

  cout << "** Navigating UIC open street map **" << endl;
  cout << endl;
  cout << std::setprecision(8);

  string def_filename = "map.osm";
  string filename;

  cout << "Enter map filename> ";
  getline(cin, filename);

  if (filename == "") {
    filename = def_filename;
  }

  //
  // Load XML-based map file
  //
  if (!LoadOpenStreetMap(filename, xmldoc)) {
    cout << "**Error: unable to load open street map." << endl;
    cout << endl;
    return 0;
  }

  //
  // Read the nodes, which are the various known positions on the map:
  //
  int nodeCount = ReadMapNodes(xmldoc, Nodes);

  //
  // Read the footways, which are the walking paths:
  //
  int footwayCount = ReadFootways(xmldoc, Footways);

  //
  // Read the university buildings:
  //
  int buildingCount = ReadUniversityBuildings(xmldoc, Nodes, Buildings);

  //
  // Stats
  //
  assert(nodeCount == (int)Nodes.size());
  assert(footwayCount == (int)Footways.size());
  assert(buildingCount == (int)Buildings.size());

  cout << endl;
  cout << "# of nodes: " << Nodes.size() << endl;
  cout << "# of footways: " << Footways.size() << endl;
  cout << "# of buildings: " << Buildings.size() << endl;


  //
  // TO DO: build the graph, output stats:
  //
  graph<long long, double> G;

  for (auto e : Nodes) {
    G.addVertex(e.first);
  }

  for (auto f : Footways) {
    for (unsigned int i = 0; i < f.Nodes.size()-1; i++) {
      Coordinates c1 = Nodes[f.Nodes[i]];
      Coordinates c2 = Nodes[f.Nodes[i+1]];
      double dist = distBetween2Points(c1.Lat, c1.Lon, c2.Lat, c2.Lon);
      G.addEdge(f.Nodes[i], f.Nodes[i+1], dist);
      G.addEdge(f.Nodes[i+1], f.Nodes[i], dist);
    }
  }


  cout << "# of vertices: " << G.NumVertices() << endl;
  cout << "# of edges: " << G.NumEdges() << endl;
  cout << endl;

  //
  // Menu
  //
  string userInput;
  cout << "Enter \"a\" for the standard application or "
        << "\"c\" for the creative component application> ";
  getline(cin, userInput);
  if (userInput == "a") {
    application(G, Nodes, Footways, Buildings);
  } else if (userInput == "c") {
    creative(G, Nodes, Footways, Buildings);
  }
  //
  // done:
  //
  cout << "** Done **" << endl;
  return 0;
}
