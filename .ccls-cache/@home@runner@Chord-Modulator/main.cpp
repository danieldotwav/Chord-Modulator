#include <algorithm>
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
  vector<string> tabs = {"| D   | Cadd9 | G   | G   |",
                         "| D   | Cadd9 | G   | G   |",
                         "| D   | Cadd9 | G   | G   |",
                         "| D   | Cadd9 | G   | G   |",
                         "",
                         "| D   | Cadd9 | G   | G   |",
                         "| D   | Cadd9 | G   | G   |",
                         "| F   | C     | D   | Cadd9 | G |",
                         "| D   | Cadd9 | G   | G   |",
                         "| D   | Cadd9 | G   | G   |",
                         "| D   | Cadd9 | G   | G   |",
                         "",
                         "| D   | Cadd9 | G   |"};

  cout << "Initial Chords:\n";
  printTabs(tabs);
  vector<string> m_tabs = modulateTabs(tabs, 7); // Modulate tabs by 7 overtones

  cout << "\nModulated Tabs:\n";
  printTabs(m_tabs);
}

// Function to modulate the given chord to a new key
string modulateChord(string chord, int semitones) {
  static const std::map<std::string, int> noteToInt{
      {"A", 0},  {"A#", 1}, {"Bb", 1}, {"B", 2},   {"C", 3},  {"C#", 4},
      {"Db", 4}, {"D", 5},  {"D#", 6}, {"Eb", 6},  {"E", 7},  {"F", 8},
      {"F#", 9}, {"Gb", 9}, {"G", 10}, {"G#", 11}, {"Ab", 11}};

  // Extract the root note including sharps and flats
  string rootNote;
  if (chord[1] == '#' || chord[1] == 'b') {
    rootNote = chord.substr(0, 2); // Include the accidental
  } else {
    rootNote = chord.substr(0, 1); // Just the note
  }

  // Find the numeric value of the root note
  int root = noteToInt.at(rootNote);

  // Apply the semitone modulation
  root = (root + semitones + 12) % 12;

  // Find the new root note after modulation
  string newRootNote;
  for (const auto &kv : noteToInt) {
    if (kv.second == root) {
      if (kv.first.length() == rootNote.length()) { // Match sharp/flat status
        newRootNote = kv.first;
        break;
      } else if (rootNote.length() == 1 &&
                 kv.first[1] != '#') { // Prefer natural notes
        newRootNote = kv.first;
      }
    }
  }

  // Reconstruct the new chord with the modulated root note
  string newChord = newRootNote;
  // Append the rest of the chord if there is any
  if (rootNote.length() < chord.length()) {
    newChord += chord.substr(rootNote.length());
  }

  return newChord;
}

// Function to modulate the given tabs to a new key
vector<string> modulateTabs(const vector<string> &tabs, int semitones) {
  vector<string> newTabs;
  for (const string &tab : tabs) {
    string newTab;
    size_t i = 0;
    while (i < tab.size()) {
      if (isdigit(tab[i]) || tab[i] == '|') {
        // Directly add numbers and divider characters to the new tab
        newTab += tab[i++];
      } else if (isalpha(tab[i]) || tab[i] == '#' || tab[i] == 'b') {
        // Start of a chord
        size_t start = i;
        while (i < tab.size() && (isalpha(tab[i]) || tab[i] == '#' || tab[i] == 'b')) {
          // Increment i to capture the full root note including accidentals
          i++;
        }
        string rootNote = tab.substr(start, i - start);
        string extension = "";

        // Capture additional chord characters like 'add9', 'm7', 'sus4', etc.
        while (i < tab.size() && tab[i] != '|' && tab[i] != ' ') {
          extension += tab[i++];
        }

        // Modulate the root note and then append the extension
        newTab += modulateChord(rootNote, semitones) + extension;
      } else {
        // Anything else, just add to the new tab
        newTab += tab[i++];
      }
    }
    newTabs.push_back(newTab); // Add the newly created tab to the vector
  }
  return newTabs;
}

void printTabs(const vector<string> &tabs) {
  for (auto &tab : tabs) {
    cout << tab << endl;
  }
}