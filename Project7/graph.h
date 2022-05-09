// graph.h
// Shayan Rasheed
//
// Graph class implemented using adjacency lists
//
// University of Illinois at Chicago
// CS 251: Fall 2021
// Project #7 - Openstreet Maps
//

#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <set>
#include <map>

using namespace std;

template<typename VertexT, typename WeightT>
class graph {
 private:
  struct node {
    VertexT vertex;
    WeightT weight;
 
    bool operator<(const node& other) const {
      return vertex < other.vertex;
    }
  };
  vector<VertexT> Vertices;
  map<VertexT, set<node>> adjList;

 public:
  //
  // constructor:
  //
  // Constructs an empty graph where n is the max # of vertices
  // you expect the graph to contain.
  //
  // NOTE: the graph is implemented using an adjacency matrix.
  // If n exceeds the dimensions of this matrix, an exception
  // will be thrown to let you know that this implementation
  // will not suffice.
  //
  graph() {
  }

  //
  // NumVertices
  //
  // Returns the # of vertices currently in the graph.
  //
  int NumVertices() const {
    return static_cast<int>(this->Vertices.size());
  }

  //
  // NumEdges
  //
  // Returns the # of edges currently in the graph.
  //
  int NumEdges() const {
    int count = 0;

    // Add the size of every set in the adjacency list
    for (auto v : adjList) {
      count += v.second.size();
    }

    return count;
  }

  //
  // addVertex
  //
  // Adds the vertex v to the graph if there's room, and if so
  // returns true.  If the graph is full, or the vertex already
  // exists in the graph, then false is returned.
  //
  bool addVertex(VertexT v) {
    //
    // is the vertex already in the graph?  If so, we do not
    // insert again otherwise Vertices may fill with duplicates:
    //
    if (adjList.count(v) == 1) {
      return false;
    }

    //
    // if we get here, vertex does not exist so insert.
    //
    this->Vertices.push_back(v);
    set<node> newSet;
    adjList.emplace(v, newSet);

    return true;
  }

  //
  // addEdge
  //
  // Adds the edge (from, to, weight) to the graph, and returns
  // true.  If the vertices do not exist or for some reason the
  // graph is full, false is returned.
  //
  // NOTE: if the edge already exists, the existing edge weight
  // is overwritten with the new edge weight.
  //
  bool addEdge(VertexT from, VertexT to, WeightT weight) {
    // Check if from and to are valid vertices
    if (adjList.count(from) == 0 || adjList.count(to) == 0) {
      return false;
    }

    // Initialize new node
    node newNode;
    newNode.vertex = to;
    newNode.weight = weight;

    // Loop through adjacency list for from vertex
    for (auto &n : adjList.at(from)) {
      if (n.vertex == to) {
        // If edge already exists, delete old edge and add the new one
        adjList.at(from).erase(n);
        adjList.at(from).insert(newNode);
        return true;
      }
    }

    // Otherwise, simply insert new edge
    adjList.at(from).insert(newNode);

    return true;
  }

  //
  // getWeight
  //
  // Returns the weight associated with a given edge.  If
  // the edge exists, the weight is returned via the reference
  // parameter and true is returned.  If the edge does not
  // exist, the weight parameter is unchanged and false is
  // returned.
  //
  bool getWeight(VertexT from, VertexT to, WeightT& weight) const {
    // Check if from and to are valid vertices
    if (adjList.count(from) == 0 || adjList.count(to) == 0) {
      return false;
    }

    // Loop through adjacency list for from vertex
    for (auto n : adjList.at(from)) {
      // Return weight of node with to as vertex
      if (n.vertex == to) {
        weight = n.weight;
        return true;
      }
    }

    // If we get here, edge does not exist
    return false;
  }

  //
  // neighbors
  //
  // Returns a set containing the neighbors of v, i.e. all
  // vertices that can be reached from v along one edge.
  // Since a set is returned, the neighbors are returned in
  // sorted order; use foreach to iterate through the set.
  //
  set<VertexT> neighbors(VertexT v) const {
    set<VertexT>  S;
    // Check is v is a valid vertex
    if (adjList.count(v) == 0) {
      return S;
    }

    // Loop through adjList of v and add each vertex to the set
    for (auto n : adjList.at(v)) {
      S.emplace(n.vertex);
    }

    return S;
  }

  //
  // getVertices
  //
  // Returns a vector containing all the vertices currently in
  // the graph.
  //
  vector<VertexT> getVertices() const {
    return this->Vertices;  // returns a copy:
  }

  //
  // dump
  //
  // Dumps the internal state of the graph for debugging purposes.
  //
  // Example:
  //    graph<string,int>  G(26);
  //    ...
  //    G.dump(cout);  // dump to console
  //
  void dump(ostream& output) const {
    output << "***************************************************" << endl;
    output << "********************* GRAPH ***********************" << endl;

    output << "**Num vertices: " << this->NumVertices() << endl;
    output << "**Num edges: " << this->NumEdges() << endl;

    output << endl;
    output << "**Vertices:" << endl;
    for (int i = 0; i < this->NumVertices(); ++i) {
      output << " " << i << ". " << this->Vertices[i] << endl;
    }

    output << endl;
    output << "**Edges:" << endl;
    for (auto v : Vertices) {
      cout << v << ": ";
      for (auto n : adjList.at(v)) {
        cout << "(" << v << ", " << n.vertex << ", " << n.weight << ") ";
      }
      cout << endl;
    }
    output << "**************************************************" << endl;
  }
};
