#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void buildWord(
    std::string& currentWord,
    const std::string& input,
    std::string floatingLetters,
    const std::set<std::string>& dict,
    std::set<std::string>& resultSet,
    int currentIndex);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    std::set<std::string> workingWords;
    std::string currentWord = in;
    buildWord(currentWord, in, floating, dict, workingWords, 0);
    return workingWords;

}

// Define any helper functions here
void buildWord(
    std::string& currentWord,
    const std::string& input,
    std::string floatingLetters,
    const std::set<std::string>& dict,
    std::set<std::string>& resultSet,
    int currentIndex)
{
    // Base Case - we've reached the end of the word
    if(currentIndex == input.length()){
        if(dict.find(currentWord) != dict.end() && floatingLetters.empty()){
            resultSet.insert(currentWord);
        }
        return;
    }
    
    // Case where current letter is already fixed
    if(currentWord[currentIndex] != '-'){
        buildWord(currentWord, input, floatingLetters, dict, resultSet, currentIndex + 1);
        return;
    }

    // Pruning
    int dashesLeft = 0;
    for(size_t i = currentIndex; i < input.length(); ++i){
      if(currentWord[i] == '-'){
        dashesLeft ++;
      }
    }

    // Case where current letter is a "-", meaning we have to search for valid letters
    if(currentWord[currentIndex] == '-'){
        
        for(char c = 'a'; c <= 'z'; ++c){
            size_t position = floatingLetters.find(c);

            // If c is in floatingLetters, we'll remove it from floatingLetters because we will have "used it"
            std::string floatingCopy = floatingLetters;
            if(position != std::string::npos){
                floatingCopy.erase(position, 1);
                currentWord[currentIndex] = c;
                buildWord(currentWord, input, floatingCopy, dict, resultSet, currentIndex + 1);
                currentWord[currentIndex] = '-'; // Backtracking
            }
            
            // Place c in the word and recurse, including pruning logic
            else if(floatingLetters.length() < dashesLeft){
              currentWord[currentIndex] = c;
              buildWord(currentWord, input, floatingCopy, dict, resultSet, currentIndex + 1);
              currentWord[currentIndex] = '-';
            }
            
        }
    }

}
