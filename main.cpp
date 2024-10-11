#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct HashSlot {
  string key;
  string status;  // Can be "never used", "tombstone", or "occupied"

  HashSlot() {
    key = "";
    status = "never used";
  }
};

class HashTable {
 private:
  vector<HashSlot> table;  // Hash table to hold key,value pairs

 public:
  HashTable() {
    table.resize(26);  // 26 slots, one for each letter a-z
  }

  // Hash function: uses the last character of the word
  int hash(string word) {
    return word[word.length() - 1] - 'a';  // 'a' is 0, 'z' is 25
  }

  // Insert a key into the hash table
  void insert(const string &word) {
    if (search(word) != -1) {
      return;  // If key already exists, do nothing
    }

    // Get starting index from hash function.
    int index = hash(word);
    int originalIndex = index;

    // Linear probing to find an empty or tombstone slot
    while (table[index].status == "occupied") {
      index = (index + 1) % 26;
      if (index == originalIndex) {
        return;  // Returned to starting index, table is full.
      }
    }

    // Place the word in the found unoccupied slot
    table[index].key = word;
    table[index].status = "occupied";
  }

  // Delete a key from the hash table
  void remove(const string &word) {
    int index = search(word);
    if (index == -1) {
      return;  // Key not found, do nothing
    }

    // Mark the slot as a tombstone
    table[index].status = "tombstone";
  }

  // Search for a key in the hash table
  int search(const string &word) {
    int index = hash(word);
    int originalIndex = index;

    // Linear probing to search for the key
    while (table[index].status != "never used") {
      if (table[index].status == "occupied" && table[index].key == word) {
        return index;  // Key found
      }
      index = (index + 1) % 26;
      if (index == originalIndex) {
        break;  // Gone through whole table, key not found
      }
    }

    return -1;  // Key not found
  }

  // Print the contents of the table
  void print() {
    for (int i = 0; i < 26; i++) {
      if (table[i].status == "occupied") {
        cout << table[i].key << " ";
      }
    }
    cout << endl;
  }
};

int main() {
  HashTable hashTable;
  string input;

  // Take the input line
  getline(cin, input);

  // Use a stringstream to break the input into words
  stringstream ss(input);
  string command;

  // Process each command (AWord or DWord)
  while (ss >> command) {
    char operation =
        command[0];  // First character is the operation (add or delete)
    string word =
        command.substr(1);  // Rest of the string is the value being stored

    if (operation == 'A') {
      hashTable.insert(word);
    } else if (operation == 'D') {
      hashTable.remove(word);
    }
  }

  // Print the final state of the hash table
  hashTable.print();

  return 0;
}
