// Shayan Rasheed - main.cpp
// CS 251, Project 2
// Peronality quiz that asks user questions
// and matches them to a specific individual or character
// based on their responses

#include <set>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "driver.h"

using namespace std;

// load_set
// takes in a file stream as a parameter and fills
// either a set of questions or a set of People
// with data from the set depending on whether isPeople
// is true or false
void load_set(bool isPeople, ifstream& inFS, set<Question>& questions, set<Person>& people) {
    while (!inFS.eof()) {
      Question q;
      Person p;

      if (isPeople) {
        getline(inFS, p.name, '.');
        if (p.name.empty()) {
          return;
        }
      } else {
        getline(inFS, q.questionText, '.');
        if (q.questionText.empty()) {
          return;
        }
        q.questionText.append(".");
      }

      string temp;
      getline(inFS, temp);
      stringstream ss(temp);
      while (ss.good()) {
        char c;
        int val;
        ss >> c;
        ss.ignore();
        ss >> val;
        if (isPeople) {
          p.scores[c] = val;
        } else {
          q.factors[c] = val;
        }
      }

      if (isPeople) {
        people.insert(p);
      } else {
        questions.insert(q);
      }
    }
}

// askQuestions
// repeatedly asks the user questions that are selected randomly
// The user's responses are stored in a map
// the parameter numQuestions determines the number
// of questions asked
void askQuestions(int numQuestions, set<Question>& questions, map<Question, int>& answers) {
  for (int i = 0; i < numQuestions; i++) {
      cout << "How much do you agree with this statement?" << endl;
      Question q = randomQuestionFrom(questions);
      cout << "\"" << q.questionText << "\"" << endl;
      cout << endl << "1. Strongly disagree" << endl;
      cout << "2. Disagree" << endl;
      cout << "3. Neutral" << endl;
      cout << "4. Agree" << endl;
      cout << "5. Strongly agree" << endl;
      cout << endl << "Enter your answer here (1-5): ";
      int a;
      cin >> a;
      cout << endl;
      answers.emplace(q, a);
    }
}

// findSimilar
// Performs the algorithm to find the most similar person
// in the set to the user's reponses
// filename determines which file will be opened
void findSimilar(string filename, map<Question, int>& answers, set<Question>& questions, set<Person>& people) {
  // if the people set has already been filled, clear it
  if (people.size() > 0) {
    people.clear();
  }
  // open the file and fill the people set with its data
  ifstream inFS;
  inFS.open(filename);
  load_set(true, inFS, questions, people);
  // calculate the user's OCEAN scores
  map<char, int> userScores = scoresFrom(answers);
  // find the most similar person the the user
  Person result = mostSimilarTo(userScores, people);

  cout << "You got " << result.name << "!" << endl << endl;
}

// getTestNum
// Repeatedly asks the user to select a test number until
// they input 0
// The parameters of this function will be sent to
// findSimilar to perform the test(s)
void getTestNum(map<Question, int>& answers, set<Question>& questions, set<Person>& people) {
    int command;
    // Loop to repeatedly administer tests until user inputs 0
    do {
      cout << "1. BabyAnimals" << endl;
      cout << "2. Brooklyn99" << endl;
      cout << "3. Disney" << endl;
      cout << "4. Hogwarts" << endl;
      cout << "5. MyersBriggs" << endl;
      cout << "6. SesameStreet" << endl;
      cout << "7. StarWars" << endl;
      cout << "8. Vegetables" << endl;
      cout << "9. mine" << endl;
      cout << "0. To end program." << endl << endl;
      cout << "Choose test number (1-9, or 0 to end): ";
      cin >> command;
      // Set filename variable to the file of the corresponding
      // test for each number 1-9
      string filename;
      if (command == 1) {
        filename = "BabyAnimals.people";
      } else if (command == 2) {
        filename = "Brooklyn99.people";
      } else if (command == 3) {
        filename = "Disney.people";
      } else if (command == 4) {
        filename = "Hogwarts.people";
      } else if (command == 5) {
        filename = "MyersBriggs.people";
      } else if (command == 6) {
        filename = "SesameStreet.people";
      } else if (command == 7) {
        filename = "StarWars.people";
      } else if (command == 8) {
        filename = "Vegetables.people";
      } else if (command == 9) {
        filename = "mine.people";
      }
      // If command is valid, find the most similar person
      if (command > 0 && command < 10) {
        findSimilar(filename, answers, questions, people);
      }
    } while (command != 0);
}

int main() {
    set<Question> questions;
    map<Question, int> answers;
    set<Person> people;
    cout << "Welcome to the Personality Quiz!" << endl << endl;
    // Prompt user for number of questions
    cout << "Choose number of questions: ";
    int numQuestions;
    cin >> numQuestions;
    cout << endl;
    // Open questions.txt and call function to fill questions set
    ifstream inFS;
    inFS.open("questions.txt");
    load_set(false, inFS, questions, people);
    // Call function to repeatedly ask user questions
    askQuestions(numQuestions, questions, answers);
    // Calculate user's OCEAN scores
    map<char, int> scores = scoresFrom(answers);
    // Call function to ask user which test they would like
    getTestNum(answers, questions, people);
    cout << "Goodbye!" << endl;
    return 0;
}
