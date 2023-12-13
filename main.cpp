#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

// Function to modulate the given chord to a new key
string modulateChord(string chord, int semitones);

// Function to modulate the given tabs to a new key
vector<string> modulateTabs(const vector<string> &tabs, int semitones);

// Function to print tabs
void printTabs(const vector<string> &tabs);

int main() {
  vector<string> tabs = {"| B   | E7    | B   | E7    |",
                         "| A7  | D#7   | G#7 | C#7   |",
                         "| F#7 | B     | F#7 | B     |",
                         "",
                         "| E7  | %     | E7  | %     |",
                         "| B   | %     | B   | %     |",
                         "| G#7 | C#7   | F#7 | B     |",
                         "| F7  | Bb7   | Eb7 | Ab7   |",
                         "| D7  | G7    | C7  | F7    |",
                         "| B7  | E7    | A7  | D#7   |",
                         "",
                         "| G7  | C7    | F7 "};
  printTabs(tabs);
  modulateTabs(tabs, 7); // Modulate tabs by 7 overtones
  printTabs(tabs);
  
}

// Function to modulate the given chord to a new key
string modulateChord(string chord, int semitones) {
  static const map<string, int> noteToInt{
      {"A", 0},  {"A#", 1}, {"Bb", 1}, {"B", 2},   {"C", 3},  {"C#", 4},
      {"Db", 4}, {"D", 5},  {"D#", 6}, {"Eb", 6},  {"E", 7},  {"F", 8},
      {"F#", 9}, {"Gb", 9}, {"G", 10}, {"G#", 11}, {"Ab", 11}};

  // Extract the root note without modifiers
  string rootNote = chord.substr(0, 1);

  int root = noteToInt.at(rootNote);

  // Apply any '#' or 'b' modifiers to the root note
  for (int i = 1; i < chord.size(); ++i) {
    if (chord[i] == '#') {
      root++;
    } else if (chord[i] == 'b') {
      root--;
    }
  }

  // Normalize the root value to [0, 11]
  root = (root + semitones + 12) % 12;

  // Find the new root note
  string newRootNote;
  for (const auto &kv : noteToInt) {
    if (kv.second == root) {
      newRootNote = kv.first;
      break;
    }
  }

  // Reconstruct the new chord with the new root note
  string newChord = newRootNote;

  if (chord.size() > 1) {
    newChord += chord.substr(1);
  }

  return newChord;
}

// Function to modulate the given tabs to a new key
vector<string> modulateTabs(const vector<string> &tabs, int semitones) {
  vector<string> newTabs;
  for (const string &tab : tabs) {
    string newTab;
    for (size_t i = 0; i < tab.size(); i++) {
      if (isdigit(tab[i])) {
        newTab += tab[i];
      } else if (isalpha(tab[i])) {
        string chord;
        while (i < tab.size() && isalpha(tab[i])) {
          chord += tab[i++];
        }
        i--;
        newTab += modulateChord(chord, semitones);
      } else {
        newTab += tab[i];
      }
    }
    newTabs.push_back(newTab);
  }
  return newTabs;
}

void printTabs(const vector<string> &tabs) {
  for (auto& tab : tabs) {
    cout << tab << endl;
  }
}