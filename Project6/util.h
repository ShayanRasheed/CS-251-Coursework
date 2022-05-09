// Shayan Rasheed
// CS 251 - Project 6
// File Compression App
// This file contains functions used to encode and decode files using the Huffman Algorithm

#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <queue>          // std::priority_queue
#include <vector>         // std::vector
#include <functional>     // std::greater
#include <string>
#include "bitstream.h"
#include "util.h"
#include "hashmap.h"
#include "mymap.h"

struct HuffmanNode {
    int character;
    int count;
    HuffmanNode* zero;
    HuffmanNode* one;
};

// Prioritize class used for the priority queue
class prioritize {
    public:
      bool operator()(const HuffmanNode* h1, const HuffmanNode* h2) const {
        return h1->count > h2->count;
      }
};

//
// *This method frees the memory allocated for the Huffman tree.
//
void freeTree(HuffmanNode* node) {
    // Base Case: null node
    if (node == nullptr) {
      return;
    }

    // Recursive calls to both children nodes
    freeTree(node->zero);
    freeTree(node->one);

    // delete current node
    delete node;
}

//
// *This function build the frequency map.  If isFile is true, then it reads
// from filename.  If isFile is false, then it reads from a string filename.
//
void buildFrequencyMap(string filename, bool isFile, hashmap &map) {
    if (isFile) {
      ifstream inFS(filename);
      char c;
      // Read each character from the stream
      while (inFS.get(c)) {
        // If character is already in map, increment its count
        if (map.containsKey(c)) {
          map.put(c, map.get(c)+1);
        } else {
          // Otherwise, add the character to the map
          map.put(c, 1);
        }
      }
    } else {
      // If input is a string, loop through each character and do the same as before
      for (char& c : filename) {
        if (map.containsKey(c)) {
          map.put(c, map.get(c)+1);
        } else {
          map.put(c, 1);
        }
      }
    }
    // Add PSEUDO_EOF to the map
    map.put(PSEUDO_EOF, 1);
}

//
// *This function builds an encoding tree from the frequency map.
//
HuffmanNode* buildEncodingTree(hashmap &map) {
    // Initialize priority_queue of HuffmanNodes
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, prioritize> pq;
    vector<int> keys = map.keys();

    // For every character in the map, create a new node and push it into the queue
    for (size_t i = 0; i < keys.size(); i++) {
      HuffmanNode* newNode = new HuffmanNode;
      newNode->character = keys[i];
      newNode->count = map.get(keys[i]);
      newNode->zero = nullptr;
      newNode->one = nullptr;
      pq.push(newNode);
    }

    while (pq.size() > 1) {
      // Pop the first two nodes off the queue
      HuffmanNode* first = pq.top();
      pq.pop();
      HuffmanNode* second = pq.top();
      pq.pop();
      // Create a new node with the two nodes as its children
      HuffmanNode* newNode = new HuffmanNode;
      newNode->character = NOT_A_CHAR;
      newNode->count = first->count + second->count;
      newNode->zero = first;
      newNode->one = second;
      // Push the new node back onto the queue
      pq.push(newNode);
    }

    // When size of queue = 1, that node is the root of the entire tree
    HuffmanNode* root = pq.top();
    return root;
}

void buildMap(HuffmanNode* node, mymap<int, string>& m, string s) {
    // Base Case: return if node is null
    if (node == nullptr) {
      return;
    }
    // If the node contains a character, add it to the map
    if (node->character != NOT_A_CHAR) {
      m.put(node->character, s);
    }
    // Recursive calls to both children nodes
    buildMap(node->zero, m, s + "0");
    buildMap(node->one, m, s + "1");
}

//
// *This function builds the encoding map from an encoding tree.
//
mymap <int, string> buildEncodingMap(HuffmanNode* tree) {
    mymap <int, string> encodingMap;

    buildMap(tree, encodingMap, "");

    return encodingMap;
}

//
// *This function encodes the data in the input stream into the output stream
// using the encodingMap.  This function calculates the number of bits
// written to the output stream and sets result to the size parameter, which is
// passed by reference.  This function also returns a string representation of
// the output file, which is particularly useful for testing.
//
string encode(ifstream& input, mymap <int, string> &encodingMap,
              ofbitstream& output, int &size, bool makeFile) {
    char c;
    string str = "";

    string segment;
    // Get each character from the input
    while (input.get(c)) {
      // Get the corresponding string from the map and add it to str
      segment = encodingMap.get(c);
      str += segment;
      if (makeFile) {
        // When writing to an output, loop through segment and add each bit to the output
        for (char& c : segment) {
          if (c == '1') {
            output.writeBit(1);
            size++;
          } else {
            output.writeBit(0);
            size++;
          }
        }
      }
    }
    // Add PSEUDO_EOF to the end of the string and output
    segment = encodingMap.get(PSEUDO_EOF);
    str += segment;
    if (makeFile) {
      for (char& c : segment) {
        if (c == '1') {
          output.writeBit(1);
          size++;
        } else {
          output.writeBit(0);
          size++;
        }
      }
    }

    return str;
}


//
// *This function decodes the input stream and writes the result to the output
// stream using the encodingTree.  This function also returns a string
// representation of the output file, which is particularly useful for testing.
//
string decode(ifbitstream &input, HuffmanNode* encodingTree, ofstream &output) {
    string result = "";
    HuffmanNode* cur = encodingTree;
    // Read each bit from the input
    while (!input.eof()) {
      int bit = input.readBit();
      // If the bit is 0, set cur to the zero child. Otherwise, set it to the one child
      if (bit == 0) {
        cur = cur->zero;
      } else {
        cur = cur->one;
      }
      // If cur contains a character, write that character to the output
      if (cur->character != NOT_A_CHAR) {
        // If cur contains PSEUDO_EOF, the loop ends
        if (cur->character == PSEUDO_EOF) {
          break;
        }
        result += cur->character;
        output.put(cur->character);
        cur = encodingTree;
      }
    }

    return result;
}

//
// *This function completes the entire compression process.  Given a file,
// filename, this function (1) builds a frequency map; (2) builds an encoding
// tree; (3) builds an encoding map; (4) encodes the file (don't forget to
// include the frequency map in the header of the output file).  This function
// should create a compressed file named (filename + ".huf") and should also
// return a string version of the bit pattern.
//
string compress(string filename) {
    ofbitstream output(filename + ".huf");
    hashmap frequencyMap;
    // build the frequency Map
    buildFrequencyMap(filename, true, frequencyMap);
    // build the encoding tree
    HuffmanNode* root = buildEncodingTree(frequencyMap);
    // build the encoding map
    mymap<int, string> encodingMap = buildEncodingMap(root);
    // Use the frequencyMap to add the header of the output file
    output << frequencyMap;
    ifstream input(filename);

    int size = 0;
    // encode the file
    string result = encode(input, encodingMap, output, size, true);
    // Free the allocated memory
    freeTree(root);

    return result;
}

//
// *This function completes the entire decompression process.  Given the file,
// filename (which should end with ".huf"), (1) extract the header and build
// the frequency map; (2) build an encoding tree from the frequency map; (3)
// using the encoding tree to decode the file.  This function should create a
// compressed file using the following convention.
// If filename = "example.txt.huf", then the uncompressed file should be named
// "example_unc.txt".  The function should return a string version of the
// uncompressed file.  Note: this function should reverse what the compress
// function did.
//
string decompress(string filename) {
    ifbitstream input(filename);
    hashmap frequencyMap;
    // Extract the header to build frequencyMap
    input >> frequencyMap;
    // build the encoding tree using the map
    HuffmanNode* root = buildEncodingTree(frequencyMap);
    // Create output file
    int delimiterPos = filename.find('.');
    string newFilename = filename.substr(0, delimiterPos);
    newFilename += "_unc.txt";
    ofstream output(newFilename);
    // Decode the input
    string result = decode(input, root, output);
    // Free the allocated memory
    freeTree(root);
    return result;
}
