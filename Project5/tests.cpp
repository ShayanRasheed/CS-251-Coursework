#include <gtest/gtest.h>
#include "mymap.h"

TEST(mymap, balancedOne) {
    mymap<int, int> m;
    
    // size should initially be 0
    EXPECT_EQ(0, m.Size());
    
    // Testing size, contains, and get when new keys are added using put
    m.put(5, 2);
    
    EXPECT_EQ(1, m.Size());
    EXPECT_EQ(true, m.contains(5));
    EXPECT_EQ(2, m.get(5));
    
    m.put(3, 7);
  
    EXPECT_EQ(2, m.Size());
    EXPECT_EQ(true, m.contains(3));
    EXPECT_EQ(7, m.get(3));
  
    m.put(8, 1);
    
    EXPECT_EQ(3, m.Size());
    EXPECT_EQ(true, m.contains(8));
    EXPECT_EQ(1, m.get(8));
    
    // Testing toString
    string sol = "key: 3 value: 7\nkey: 5 value: 2\nkey: 8 value: 1\n";
    EXPECT_EQ(sol, m.toString());
    
    // Testing [] operator
    EXPECT_EQ(m[8], 1);
    // If key does not exist, Get should return default value without adding new key to map
    EXPECT_EQ(m.get(4), 0);
    EXPECT_EQ(false, m.contains(4));
    // [] operator should add key to map if it DNE
    EXPECT_EQ(m[4], 0);
    EXPECT_EQ(true, m.contains(4));
    
    m.put(2, 10);
    EXPECT_EQ(5, m.Size());
    EXPECT_EQ(true, m.contains(2));
    EXPECT_EQ(10, m.get(2));
              
    sol = "key: 2 value: 10\nkey: 3 value: 7\nkey: 4 value: 0\nkey: 5 value: 2\nkey: 8 value: 1\n";
    EXPECT_EQ(sol, m.toString());
    
    // Test modifying an existing key
    m.put(8, 3);
    
    // Size should not increase
    EXPECT_EQ(5, m.Size());
    EXPECT_EQ(true, m.contains(8));
    EXPECT_EQ(3, m.get(8));
  
    sol = "key: 2 value: 10\nkey: 3 value: 7\nkey: 4 value: 0\nkey: 5 value: 2\nkey: 8 value: 3\n";
    EXPECT_EQ(sol, m.toString());
}

TEST(mymap, balancedTwo) {
    // Same tests as one but with <int, string> pairs
    mymap<int, string> m;
  
    EXPECT_EQ(0, m.Size());
  
    m.put(5, "test");
  
    EXPECT_EQ(1, m.Size());
    EXPECT_EQ(true, m.contains(5));
    EXPECT_EQ("test", m.get(5));
    
    m.put(3, "lorem");
  
    EXPECT_EQ(2, m.Size());
    EXPECT_EQ(true, m.contains(3));
    EXPECT_EQ("lorem", m.get(3));
  
    m.put(8, "ipsum");
    
    EXPECT_EQ(3, m.Size());
    EXPECT_EQ(true, m.contains(8));
    EXPECT_EQ("ipsum", m.get(8));
  
    string sol = "key: 3 value: lorem\nkey: 5 value: test\nkey: 8 value: ipsum\n";
    EXPECT_EQ(sol, m.toString());
  
    EXPECT_EQ(m[8], "ipsum");
    EXPECT_EQ(m.get(4), "");
    EXPECT_EQ(false, m.contains(4));
    EXPECT_EQ(m[4], "");
    EXPECT_EQ(true, m.contains(4));
    
    m.put(2, "dolor");
    EXPECT_EQ(5, m.Size());
    EXPECT_EQ(true, m.contains(2));
    EXPECT_EQ("dolor", m.get(2));
              
    sol = "key: 2 value: dolor\nkey: 3 value: lorem\nkey: 4 value: \nkey: 5 value: test\nkey: 8 value: ipsum\n";
    EXPECT_EQ(sol, m.toString());
  
    m.put(8, "sit");
    
    EXPECT_EQ(5, m.Size());
    EXPECT_EQ(true, m.contains(8));
    EXPECT_EQ("sit", m.get(8));
  
    sol = "key: 2 value: dolor\nkey: 3 value: lorem\nkey: 4 value: \nkey: 5 value: test\nkey: 8 value: sit\n";
    EXPECT_EQ(sol, m.toString());
}

TEST(mymap, balancedThree) {
  // larger map using chars and doubles
  mymap<char, double> m;
  
  m.put('h', 1.0);
  m.put('d', 2.0);
  m.put('l', 3.5);
  m.put('b', 5.0);
  m.put('f', 6.5);
  m.put('j', 7.0);
  m.put('n', 7.5);
  m.put('a', 8.0);
  m.put('c', 8.5);
  m.put('e', 9.0);
  m.put('g', 10.0);
  m.put('i', 10.5);
  m.put('k', 11.0);
  m.put('m', 12.5);
  m.put('o', 15.0);
  
  // Tests for [] operator
  EXPECT_EQ(m['h'], 1.0);
  EXPECT_EQ(m['d'], 2.0);
  EXPECT_EQ(m['l'], 3.5);
  EXPECT_EQ(m['b'], 5.0);
  EXPECT_EQ(m['f'], 6.5);
  EXPECT_EQ(m['j'], 7.0);
  EXPECT_EQ(m['n'], 7.5);
  EXPECT_EQ(m['a'], 8.0);
  EXPECT_EQ(m['c'], 8.5);
  EXPECT_EQ(m['e'], 9.0);
  EXPECT_EQ(m['g'], 10.0);
  EXPECT_EQ(m['i'], 10.5);
  EXPECT_EQ(m['k'], 11.0);
  EXPECT_EQ(m['m'], 12.5);
  EXPECT_EQ(m['o'], 15.0);
  EXPECT_EQ(15, m.Size());
  EXPECT_EQ(m['p'], 0.0);
  EXPECT_EQ(16, m.Size());
}

TEST(mymap, iterator) {
  mymap<int, int> map;

  int arr[] = {5, 3, 7, 2, 4, 6, 8};

  int order[] = {2, 3, 4, 5, 6, 7, 8};

  for (int i = 0; i < 7; i++) {
     map.put(arr[i], arr[i]);
  }

  int i = 0;

  for (auto key : map) {
    EXPECT_EQ(order[i++], key);
  }
  
  // Test if iterator works after operator=
  mymap<int, int> otherMap = map;
  i = 0;
  
  for (auto key : otherMap) {
    EXPECT_EQ(order[i++], key);
  }
}

TEST(mymap, copyConstructor) {
  mymap<int, int> m1;
  
  m1.put(3, 2);
  m1.put(2, 1);
  m1.put(4, 3);
  
  mymap<int, int> m2(m1);
  
  EXPECT_EQ(m1.Size(), m2.Size());
  EXPECT_EQ(m2.Size(), 3);
  EXPECT_EQ(m1[3], m2[3]);
  EXPECT_EQ(m1[2], m2[2]);
  EXPECT_EQ(m1[4], m2[4]);
}

TEST(mymap, copyConstructorBig) {
  mymap<char, double> m;
  
  m.put('h', 1.0);
  m.put('d', 2.0);
  m.put('l', 3.5);
  m.put('b', 5.0);
  m.put('f', 6.5);
  m.put('j', 7.0);
  m.put('n', 7.5);
  m.put('a', 8.0);
  m.put('c', 8.5);
  m.put('e', 9.0);
  m.put('g', 10.0);
  m.put('i', 10.5);
  m.put('k', 11.0);
  m.put('m', 12.5);
  m.put('o', 15.0);
  
  mymap<char, double> m2(m);
  
  EXPECT_EQ(m2['h'], 1.0);
  EXPECT_EQ(m2['d'], 2.0);
  EXPECT_EQ(m2['l'], 3.5);
  EXPECT_EQ(m2['b'], 5.0);
  EXPECT_EQ(m2['f'], 6.5);
  EXPECT_EQ(m2['j'], 7.0);
  EXPECT_EQ(m2['n'], 7.5);
  EXPECT_EQ(m2['a'], 8.0);
  EXPECT_EQ(m2['c'], 8.5);
  EXPECT_EQ(m2['e'], 9.0);
  EXPECT_EQ(m2['g'], 10.0);
  EXPECT_EQ(m2['i'], 10.5);
  EXPECT_EQ(m2['k'], 11.0);
  EXPECT_EQ(m2['m'], 12.5);
  EXPECT_EQ(m2['o'], 15.0);
  EXPECT_EQ(15, m2.Size());
  // Modify new object and make sure that old object is the same
  EXPECT_EQ(m2['p'], 0.0);
  EXPECT_EQ(16, m2.Size());
  EXPECT_EQ(15, m.Size());
}

TEST(mymap, operatorEqual) {
  mymap<int, int> m1;
  
  m1.put(3, 2);
  m1.put(2, 1);
  m1.put(4, 3);
  
  mymap<int, int> m2 = m1;
  
  EXPECT_EQ(m1.Size(), m2.Size());
  EXPECT_EQ(m2.Size(), 3);
  EXPECT_EQ(m1[3], m2[3]);
  EXPECT_EQ(m1[2], m2[2]);
  EXPECT_EQ(m1[4], m2[4]);
  
  m1 = m1;
  EXPECT_EQ(m1.Size(), 3);
  EXPECT_EQ(m1[3], 2);
  EXPECT_EQ(m1[2], 1);
  EXPECT_EQ(m1[4], 3);
}

TEST(mymap, toVector) {
  mymap<int, int> m1;
  
  m1.put(3, 2);
  m1.put(2, 1);
  m1.put(4, 3);
  
  vector<pair<int, int>> v = m1.toVector();
  
  EXPECT_EQ(v[0].first, 2);
  EXPECT_EQ(v[0].second, 1);
  EXPECT_EQ(v[1].first, 3);
  EXPECT_EQ(v[1].second, 2);
  EXPECT_EQ(v[2].first, 4);
  EXPECT_EQ(v[2].second, 3);
  
  mymap<double, string> m2;
  
  m2.put(10.5, "lorem");
  m2.put(5.5, "ipsum");
  m2.put(15.5, "dolor");
  m2.put(3.2, "sit");
  m2.put(6.7, "amet");
  m2.put(12.3, "consetetur");
  m2.put(18.9, "sadipscing");
  
  vector<pair<double, string>> v1 = m2.toVector();
  
  EXPECT_EQ(v1[0].first, 3.2);
  EXPECT_EQ(v1[0].second, "sit");
  EXPECT_EQ(v1[1].first, 5.5);
  EXPECT_EQ(v1[1].second, "ipsum");
  EXPECT_EQ(v1[2].first, 6.7);
  EXPECT_EQ(v1[2].second, "amet");
  EXPECT_EQ(v1[3].first, 10.5);
  EXPECT_EQ(v1[3].second, "lorem");
  EXPECT_EQ(v1[4].first, 12.3);
  EXPECT_EQ(v1[4].second, "consetetur");
  EXPECT_EQ(v1[5].first, 15.5);
  EXPECT_EQ(v1[5].second, "dolor");
  EXPECT_EQ(v1[6].first, 18.9);
  EXPECT_EQ(v1[6].second, "sadipscing");
}

TEST(mymap, checkBalance) {
  mymap<int, int> map;

  int arr[] = {2, 1, 3};

  for (int i = 0; i < 3; i++) {
     map.put(arr[i], arr[i]);
  }

  string sol = "key: 2, nL: 1, nR: 1\nkey: 1, nL: 0, nR: 0\nkey: 3, nL: 0, nR: 0\n";

  EXPECT_EQ(map.checkBalance(), sol);
  
  mymap<int, int> m2 = map;
  
  EXPECT_EQ(m2.checkBalance(), map.checkBalance());
}

TEST(mymap, AscendingAndDescendingTree) {
  mymap<int, int> m;
  
  m.put(1, 10);
  m.put(2, 11);
  m.put(3, 12);
  m.put(4, 13);
  m.put(5, 14);
  m.put(6, 15);
  m.put(7, 12);
  m.put(8, 13);
  m.put(9, 14);
  m.put(10, 15);
  
  string sol = "key: 5, nL: 4, nR: 5\nkey: 2, nL: 1, nR: 2\nkey: 1, nL: 0, nR: 0\nkey: 3, nL: 0, nR: 1\nkey: 4, nL: 0, nR: 0\nkey: 7, nL: 1, nR: 3\nkey: 6, nL: 0, nR: 0\nkey: 9, nL: 1, nR: 1\nkey: 8, nL: 0, nR: 0\nkey: 10, nL: 0, nR: 0\n";
  EXPECT_EQ(m.checkBalance(), sol);
  
  m.clear();
  
  m.put(10, 15);
  m.put(9, 14);
  m.put(8, 13);
  m.put(7, 12);
  m.put(6, 15);
  m.put(5, 14);
  m.put(4, 13);
  m.put(3, 12);
  m.put(2, 11);
  m.put(1, 10);
  
  sol = "key: 7, nL: 6, nR: 3\nkey: 3, nL: 2, nR: 3\nkey: 1, nL: 0, nR: 1\nkey: 2, nL: 0, nR: 0\nkey: 5, nL: 1, nR: 1\nkey: 4, nL: 0, nR: 0\nkey: 6, nL: 0, nR: 0\nkey: 9, nL: 1, nR: 1\nkey: 8, nL: 0, nR: 0\nkey: 10, nL: 0, nR: 0\n";
  EXPECT_EQ(m.checkBalance(), sol);
}

TEST(mymap, bigTest) {
  mymap<int, int> mapMine;
  map<int, int> mapSol;
  
  int n = 1000;
  
  for(int i = 1; i <= n; i++) {
    int key = rand();
    int value = rand();
    mapMine.put(key, value);
    mapSol[key] = value;
  }
  
  EXPECT_EQ(mapMine.Size(), mapSol.size());
  
  stringstream solution("");
  for(auto e : mapSol) {
    solution << "key: " << e.first << " value: " << e.second << endl;
    EXPECT_TRUE(mapMine.contains(e.first));
    EXPECT_EQ(mapMine.get(e.first), e.second);
  }
  
  EXPECT_EQ(mapMine.toString(), solution.str());
}