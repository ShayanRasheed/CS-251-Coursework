// Shayan Rasheed - driver.h
// CS 251, Project 2
// File containing functions that manipulate maps and sets
// To be used in personality quiz program

#include <math.h>
#include <limits>
#include <string>
#include <map>
#include <set>
#include "myrandom.h"

using namespace std;

constexpr double lowest_double = std::numeric_limits<double>::lowest();

/* Type: Question
 *
 * Type representing a personality quiz question.
 */
struct Question {
    string questionText;  // Text of the question
    map<char, int> factors;   // Map from factors to +1 or -1
    friend bool operator< (const Question& lhs, const Question& rhs) {
        return lhs.questionText < rhs.questionText;
    }
    friend bool operator== (const Question& lhs, const Question& rhs) {
        return lhs.questionText == rhs.questionText;
    }
    friend bool operator!= (const Question& lhs, const Question& rhs) {
        return lhs.questionText != rhs.questionText;
    }
};

/* Type: Person
 *
 * Type representing a person, used to represent people when determining
 * who's the closest match to the user.
 */
struct Person {
    string name;      // Name of the person
    map<char, int> scores;  // Map from factors to +1 or -1
    friend bool operator< (const Person& lhs,   const Person& rhs) {
        return lhs.name < rhs.name;
    }
    friend bool operator== (const Person& lhs, const Person& rhs) {
        return lhs.name == rhs.name;
    }
    friend bool operator!= (const Person& lhs, const Person& rhs) {
        return lhs.name != rhs.name;
    }
};

/* randomElement
 *
 * This function selects, at random, a Question from the inputted questions set
 * and returns the question.  Note, this function does not remove the randomly
 * selected question from the set.
*/
Question randomElement(set<Question>& questions) {
    int ind = randomInteger(0, (int)questions.size()-1);
    int i = 0;
    for (auto e : questions) {
        if (i == ind) {
            return e;
        }
        i++;
    }
    return {};
}

// randomQuestionFrom
// returns a randomly selected element from a set of questions
// and removes the element from the set
Question randomQuestionFrom(set<Question>& questions) {
    // throw error if set is empty
    if (questions.empty()) {
        throw out_of_range("Empty set");
    } else {
        // Call randomElement to get question
        Question q = randomElement(questions);
        // Remove question from set
        questions.erase(q);
        return q;
    }
}

// scoresFrom takes in a map of questions asked and
// the user's responses and calculates the score for each
// character (OCEAN Score)
// returns the score as a map
map<char, int> scoresFrom(map<Question, int>& answers) {
    map<char, int> scores;
    // Loop through the answers map to access
    // question values and user (1-5) responses
    for (auto const& q : answers) {
      // Loop through the questions factor map to get its OCEAN factors
      for (auto const& x : q.first.factors) {
        // Multiply the question value (1 or -1) and the user's response - 3
        int temp = x.second * (q.second-3);
        // If the question's character already exists in the map,
        // then add this questions value to the existing value
        // Otherwise, create a new element in the set
        if (scores.count(x.first) > 0) {
          scores[x.first] += temp;
        } else {
          scores.emplace(x.first, temp);
        }
      }
    }
    return scores;
}

// normalize
// Takes in the map of scores and divides each element by the
// magnitude of all elements in the set
// The function returns these values in a map of doubles
map<char, double> normalize(map<char, int>& scores) {
    map<char, double> normScores;
    double sumPowers = 0;
    bool allZero = true;
    // first, square each element in the set and add them together
    // Then take the square root of the sum
    for (auto const& x : scores) {
      sumPowers += pow(x.second, 2);
      if (x.second != 0) {
        allZero = false;
      }
    }
    // If all elements in the set are zero, throw and error
    if (allZero) {
      throw runtime_error("All zero input");
    }
    sumPowers = sqrt(sumPowers);
    // Now divide each element from scores
    // by the calculated magnitude and add them to the new set
    for (auto const& x : scores) {
      normScores.emplace(x.first, (x.second / sumPowers));
    }
    return normScores;
}

// cosineSimilarityOf
// calculates how close the values in two maps are
// by determining their cosine similarity
// returns this value as a double
double cosineSimilarityOf(const map<char, double>& lhs,
                          const map<char, double>& rhs) {
    double cosSimilarity = 0;
    // Check if the key is present in both maps
    // If it is, multiply the double values from each map
    // and add them to the cosSimilarity
    for (auto const& x : lhs) {
      if (rhs.count(x.first) > 0) {
          cosSimilarity += x.second * rhs.at(x.first);
        }
    }
    return cosSimilarity;
}

// mostSimilarTo
// Takes in the map of user's OCEAN scores and a set of people
// and determines with person in the set is the most similar to the user
Person mostSimilarTo(map<char, int>& scores, set<Person>& people) {
    // If the set of people is empty, throw an error
    if (people.empty()) {
      throw runtime_error("Empty Set");
    }
    Person mostSimilar;
    double highestSimilarity = lowest_double;
    // Normalize the user's ocean scores
    map<char, double> userScores = normalize(scores);
    for (auto p : people) {
      // For each person in the set, normalize their scores
      // and calculate their cosine Similarity with the user's scores
      map<char, double> pScores = normalize(p.scores);
      double pSim = cosineSimilarityOf(userScores, pScores);
      // If their cosine Similarity is the highest,
      // update the mostSimilar variable
      if (pSim > highestSimilarity) {
        highestSimilarity = pSim;
        mostSimilar = p;
      }
    }
    return mostSimilar;
}
