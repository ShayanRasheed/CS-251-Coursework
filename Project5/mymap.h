// mymap.h
// Shayan Rasheed
// CS 251 - Data Structures
// Project 5
// This file contains the private and public members of the mymap class
// which allows users to create a map object that functions using a binary search tree
//

#pragma once

#include <iostream>
#include <sstream>

using namespace std;

template<typename keyType, typename valueType>
class mymap {
 private:
    struct NODE {
        keyType key;  // used to build BST
        valueType value;  // stored data for the map
        NODE* left;  // links to left child
        NODE* right;  // links to right child
        int nL;  // number of nodes in left subtree
        int nR;  // number of nodes in right subtree
        bool isThreaded;
    };
    NODE* root;  // pointer to root node of the BST
    int size;  // # of key/value pairs in the mymap
    
    // ************** PRIVATE MEMBER FUNCTIONS ***************

    // MakeString function
    // Used for toString()
    // Recursively adds to string stream from least to greatest node
    void makeString(NODE* curr, ostream& output) {
      // Base Case: return if node is null
      if(curr == nullptr) {
        return;
      }
      makeString(curr->left, output);
      output << "key: " << curr->key << " value: " << curr->value << endl;
      // Check if right pointer is threaded
      NODE* rightptr = (curr->isThreaded) ? nullptr : curr->right;
      makeString(rightptr, output);
    }

    // MakeBalanceString function
    // Used for checkBalance()
    // Recursively adds to string stream in pre-order
    void makeBalanceString(NODE* curr, ostream& output) {
      // Base Case: return if node is null
      if(curr == nullptr) {
        return;
      }
      output << "key: " << curr->key << ", nL: " << curr->nL << ", nR: " << curr->nR << endl;
      makeBalanceString(curr->left, output);
      // Check if right pointer is threaded
      NODE* rightptr = (curr->isThreaded) ? nullptr : curr->right;
      makeBalanceString(rightptr, output);
    }

    // subtreeValues
    // Recursive helper function for toVector
    // Adds all pair values from a subtree to a vector
    void subtreeValues(NODE* curr, vector<pair<keyType, valueType>>& sol) {
      // Base Case: return if node is null
      if(curr == nullptr) {
        return;
      }
      subtreeValues(curr->left, sol);
      sol.push_back({curr->key, curr->value});
      // Check if right pointer is threaded
      NODE* rightptr = (curr->isThreaded) ? nullptr : curr->right;
      subtreeValues(rightptr, sol);
    }
    
    // getAllNodes and toNodeVector
    // Functions to put all node*s in a subtree into a vector
    void getAllNodes(NODE* curr, vector<NODE*>& sol) {
      // Base Case: return if node is null
      if(curr == nullptr) {
        return;
      }
      getAllNodes(curr->left, sol);
      sol.push_back(curr);
      // Check if right pointer is threaded
      NODE* rightptr = (curr->isThreaded) ? nullptr : curr->right;
      getAllNodes(rightptr, sol);
    }

    vector<NODE*> toNodeVector(NODE* start) {
      vector<NODE*> sol;
      getAllNodes(start, sol);
      return sol;
    }
    
    // buildSubtree
    // Recursive function used to balance a subtree
    // Parameters: a vector of NODE ptrs, left and right ints that denote a segment of the vector
    // and a pointer to thread that is passed to each leaf node
    NODE* buildSubtree(vector<NODE*> v, int left, int right, NODE* thread) {
      // Base Case: there is only one NODE* left in the vector
      // This node should be threaded since it is a leaf node that has no children
      if(right - left == 0) {
        v[left]->left = nullptr;
        v[left]->right = thread;
        v[left]->nL = 0;
        v[left]->nR = 0;
        v[left]->isThreaded = true;
        return v[left];
      }
      // Base Case: There are two NODE*s left in the vector
      // In this case, the left (smaller) node should have the right (greater) node
      // as its right child, the right child should be threaded
      if(right - left == 1) {
         NODE* subtree = v[left];
         subtree->left = nullptr;
         subtree->right = v[right];
         subtree->nL = 0;
         subtree->nR = 1;
         subtree->isThreaded = false;
         v[right]->left = nullptr;
         v[right]->right = thread;
         v[right]->isThreaded = true;
         v[right]->nL = 0;
         v[right]->nR = 0;
         return subtree;
      }
      // Get the NODE* that is in the middle of the vector
      int mid = (left+right) / 2;
      NODE* subtree = v[mid];
      v[mid]->isThreaded = false;
      // Recursively call function for left and right children and update heights
      subtree->left = buildSubtree(v, left, mid-1, subtree);
      subtree->nL = (mid - left);
      subtree->right = buildSubtree(v, mid+1, right, thread);
      subtree->nR = (right - mid);
      return subtree;
    }
    
    // RemoveNodes
    // Used for clear() and destructor
    void removeNodes(NODE* first) {
      // Base Case: return if node is null
      if(first == nullptr) {
        return;
      }
      
      // leaf nodes should be deleted first, then delete nodes going up the tree
      removeNodes(first->left);
      NODE* rightptr = (first->isThreaded) ? nullptr : first->right;
      removeNodes(rightptr);
      
      delete first;
    }
    
    // copyTree
    // Recursive function used for copy constructor and operator=
    // Copies every node in one tree to another
    NODE* copyTree(NODE* thisNode, NODE* otherNode, NODE* thread) {
      // Base Case: return if node is null
      if(otherNode == nullptr) {
        return nullptr;
      }
      // Create a new node object and copy all values from corresponding node in other tree
      thisNode = new NODE();
      thisNode->key = otherNode->key;
      thisNode->value = otherNode->value;
      thisNode->nR = otherNode->nR;
      thisNode->nL = otherNode->nL;
      // Keep track of current node so that leaf nodes can use this reference for their thread
      NODE* noderef = thisNode;
      thisNode->left = copyTree(nullptr, otherNode->left, noderef);
      if(otherNode->isThreaded) {
        thisNode->right = thread;
        thisNode->isThreaded = true;
      } else {
        thisNode->right = copyTree(nullptr, otherNode->right, thread);
        thisNode->isThreaded = false;
      }
      return thisNode;
    }
    
    void handleViolation(NODE* violator, NODE* violatorParent) {
      // Create a vector of NODE*s starting from the violating node
      vector<NODE*> subtreeVector = toNodeVector(violator);
      int right = subtreeVector.size() - 1;
      // build a new, balanced subtree
      NODE* newSubtree = buildSubtree(subtreeVector, 0, right, nullptr);
      // Add the balanced subtree back into the tree
      if(violatorParent != nullptr) {
        if(violatorParent->left == violator) {
          violatorParent->left = newSubtree;
        } else {
          violatorParent->right = newSubtree;
        }
      // if the violator does not have a parent node, then the violator was the root
      } else {
        root = newSubtree;
      }
    }
    //
    // iterator:
    // This iterator is used so that mymap will work with a foreach loop.
    //
    struct iterator {
     private:
        NODE* curr;  // points to current in-order node for begin/end

     public:
        iterator(NODE* node) {
            curr = node;
        }

        keyType operator *() {
            return curr -> key;
        }

        bool operator ==(const iterator& rhs) {
            if (curr == nullptr && rhs.curr == nullptr) {
              return this == &rhs;
            }
            return curr == rhs.curr;
        }

        bool operator !=(const iterator& rhs) {
            if (curr == nullptr && rhs.curr == nullptr) {
              return this == &rhs;
            }
            return curr != rhs.curr;
        }

        bool isDefault() {
            return !curr;
        }

        //
        // operator++:
        //
        // This function should advance curr to the next in-order node.
        // O(logN)
        //
        iterator operator++() {
           // if curr is threaded then go right
           if(curr->isThreaded) {
             curr = curr->right;
           } else {
           // if it isn't then go right and then to the leftmost node
             curr = curr->right;
             NODE* temp = curr;
             NODE* prev = nullptr;

             while(temp != nullptr) {
               prev = temp;
               temp = temp->left;
             }
             curr = prev;
           }
           return (*this);
        }
    };

 public:
    //
    // default constructor:
    //
    // Creates an empty mymap.
    // Time complexity: O(1)
    //
    mymap() {
			root = nullptr;
			size = 0;
    }

    //
    // copy constructor:
    //
    // Constructs a new mymap which is a copy of the "other" mymap.
    // Sets all member variables appropriately.
    // Time complexity: O(n), where n is total number of nodes in threaded,
    // self-balancing BST.
    //
    mymap(const mymap& other) {
       this->size = other.size;
       this->root = nullptr;
       this->root = copyTree(this->root, other.root, nullptr);
    }

    //
    // operator=:
    //
    // Clears "this" mymap and then makes a copy of the "other" mymap.
    // Sets all member variables appropriately.
    // Time complexity: O(n), where n is total number of nodes in threaded,
    // self-balancing BST.
    //
    mymap& operator=(const mymap& other) {
        // Handle case x = x
        if(this == &other) {
          return *this;
        }
        // delete all nodes that are already in current object
        clear();
        this->size = other.size;
        this->root = copyTree(this->root, other.root, nullptr);
        return *this;
    }

    // clear:
    //
    // Frees the memory associated with the mymap; can be used for testing.
    // Time complexity: O(n), where n is total number of nodes in threaded,
    // self-balancing BST.
    //
    void clear() {
       size = 0;
       removeNodes(root);
       root = nullptr;
    }

    //
    // destructor:
    //
    // Frees the memory associated with the mymap.
    // Time complexity: O(n), where n is total number of nodes in threaded,
    // self-balancing BST.
    //
    ~mymap() {
      clear();
    }

    //
    // put:
    //
    // Inserts the key/value into the threaded, self-balancing BST based on
    // the key.
    // Time complexity: O(logn + mlogm), where n is total number of nodes in the
    // threaded, self-balancing BST and m is the number of nodes in the
    // sub-tree that needs to be re-balanced.
    // Space complexity: O(1)
    //
    void put(keyType key, valueType value) {
        // Create new node object and initialize key and value variables
        // all other variables of node get set to default values for now
				NODE* newNode = new NODE();
				newNode->key = key;
				newNode->value = value;
				newNode->left = nullptr;
				newNode->right = nullptr;
        newNode->nL = 0;
        newNode->nR = 0;
        newNode->isThreaded = false;
				
        // If root is null, then make the root point to the new node
				if(root == nullptr) {
					root = newNode;
          size++;
					return;
				}
        
        // Check if the key is already in the tree
        bool alreadyIn = false;
        if(contains(key)) {
          alreadyIn = true;
        }
        // Set up pointers to detect a violation of the seesaw rule
        // as well as the potential violater's parent node
        NODE* violatorParent = nullptr;
        NODE* violator = nullptr;

				NODE* curr = root;
        NODE* prev = nullptr;

				while (curr != nullptr) {
          // if the key is already in the tree, then update its value
          // delete the new node since we no longer need it
          if (key == curr->key) {
            curr->value = value;
            delete newNode;
            return;
          }

          // if key is less than current key, advance to the left node
          if (key < curr->key) {
            prev = curr;
            curr = curr->left;
            // if key is not already in the tree, we can update the current node's number of left nodes
            if(!alreadyIn) {
              prev->nL += 1;
            }
            // Check for a violation of the seesaw rule, update violater ptr if detected
            if(max(prev->nL, prev->nR) > 2 * min(prev->nL, prev->nR) + 1) {
              if(violator == nullptr) {
                 violator = prev;
              }
            }
          }
          // if key is greater than current key, advance to the right node
          else {
            prev = curr;
            curr = (curr->isThreaded) ? nullptr : curr->right;
            // if key is not already in the tree, we can update the current node's number of right nodes
            if(!alreadyIn) {
              prev->nR += 1;
            }
            // Check for a violation of the seesaw rule, update violater ptr if detected
            if(max(prev->nL, prev->nR) > 2 * min(prev->nL, prev->nR) + 1) {
              if(violator == nullptr) {
                 violator = prev;
              }
            }
          }
          // update violatorParent as we traverse, until a violation is detected
          if(violator == nullptr) {
            violatorParent = prev;
          }
        }
        // Now add the new node to the left or the right of prev depending on key's value
        if(key < prev->key) {
          prev->left = newNode;
          // if key is added to the left, its thread is it's parent node (prev)
          newNode->right = prev;
          newNode->isThreaded = true;
        } else {
          // if key is added to the right, it will inherit the thread of its parent node
          newNode->right = prev->right;
          prev->right = newNode;
          prev->isThreaded = false;
          newNode->isThreaded = true;
        }
        // Increment size
        size++;
        
        // Check if a violation occured when adding the new node
        if(violator != nullptr) {
          handleViolation(violator, violatorParent);
        }
    }

    //
    // contains:
    // Returns true if the key is in mymap, return false if not.
    // Time complexity: O(logn), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    bool contains(keyType key) {
      NODE* cur = root;

      while (cur != nullptr)
      {
        if (key == cur->key)  // already in tree
          return true;

        if (key < cur->key)  // search left:
        {
          cur = cur->left;
        }
        else
        {
          // check if current node is threaded
          cur = (cur->isThreaded) ? nullptr : cur->right;
        }
    }
  
    // if get here, not found
    return false;
    }

    //
    // get:
    //
    // Returns the value for the given key; if the key is not found, the
    // default value, valueType(), is returned (but not added to mymap).
    // Time complexity: O(logn), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    valueType get(keyType key) {
      NODE* cur = root;

      while (cur != nullptr)
      {
        if (key == cur->key)  // already in tree
          return cur->value;

        if (key < cur->key)  // search left:
        {
          cur = cur->left;
        }
        else
        {
          // check if current node is threaded
          cur = (cur->isThreaded) ? nullptr : cur->right;
        }
      }
  
    // if get here, not found
      return valueType();
    }

    //
    // operator[]:
    //
    // Returns the value for the given key; if the key is not found,
    // the default value, valueType(), is returned (and the resulting new
    // key/value pair is inserted into the map).
    // Time complexity: O(logn + mlogm), where n is total number of nodes in the
    // threaded, self-balancing BST and m is the number of nodes in the
    // sub-trees that need to be re-balanced.
    // Space complexity: O(1)
    //
    valueType operator[](keyType key) {
      if(!contains(key)) {
        put(key, valueType());
      }
      
      return get(key);
    }

    //
    // Size:
    //
    // Returns the # of key/value pairs in the mymap, 0 if empty.
    // O(1)
    //
    int Size() {
      return size;
    }

    //
    // begin:
    //
    // returns an iterator to the first in order NODE.
    // Time complexity: O(logn), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    iterator begin() {
        // returns an iterator to the leftmost node in the tree
        NODE* curr = root;
        NODE* prev = nullptr;

        while(curr != nullptr) {
          prev = curr;
          curr = curr->left;
        }
        return iterator(prev);
    }

    //
    // end:
    //
    // returns an iterator to the last in order NODE.
    // this function is given to you.
    // 
    // Time Complexity: O(1)
    //
    iterator end() {
        return iterator(nullptr);
    }

    //
    // toString:
    //
    // Returns a string of the entire mymap, in order.
    // Format for 8/80, 15/150, 20/200:
    // "key: 8 value: 80\nkey: 15 value: 150\nkey: 20 value: 200\n
    // Time complexity: O(n), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    string toString() {
       stringstream ss;
       makeString(root, ss);
       return ss.str();
    }

    //
    // toVector:
    //
    // Returns a vector of the entire map, in order.  For 8/80, 15/150, 20/200:
    // {{8, 80}, {15, 150}, {20, 200}}
    // Time complexity: O(n), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    vector<pair<keyType, valueType>> toVector() {
        vector<pair<keyType, valueType>> sol;
        subtreeValues(root, sol);
        return sol;
    }

    //
    // checkBalance:
    //
    // Returns a string of mymap that verifies that the tree is properly
    // balanced.  For example, if keys: 1, 2, 3 are inserted in that order,
    // function should return a string in this format (in pre-order):
    // "key: 2, nL: 1, nR: 1\nkey: 1, nL: 0, nR: 0\nkey: 3, nL: 0, nR: 0\n";
    // Time complexity: O(n), where n is total number of nodes in the
    // threaded, self-balancing BST
    //
    string checkBalance() {
       stringstream ss;
       makeBalanceString(root, ss);
       return ss.str();
    }
};
