#include "red_black_tree.hpp" // Include your Red-Black Tree implementation
#include "color.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
class MapContainer {
private:
    RedBlackTree mapTree;// Use your Red-Black Tree class with int key and string value

public:
    void processOperations(const string &operation) {
       stringstream ss(operation);
        string command;
        ss >> command;

        if (command == "I") { // Insert
            int key;
            string value;
            ss >> key >> value;
            mapTree.insert(key, value);
        } else if (command == "E") { // Erase
            int key;
            ss >> key;
            mapTree.erase(key);
        } else if (command == "Clr") { // Clear
            mapTree.clear();
        } else if (command == "F") { // Find
            int key;
            ss >> key;
            mapTree.find(key);
        } else if (command == "Em") { // Empty
            mapTree.empty();
        } else if (command == "S") { // Size
           mapTree.Size();
        } else if (command == "Itr") { // Iteration
            mapTree.iteration();
        } else
        {
            std::cerr << "Invalid command: " << command << "\n";
        }
    }

};

int main() {
    MapContainer mapContainer;
    ifstream inputFile("input.txt");
    ofstream outputFile("output.txt");

    if (inputFile.is_open()) {
        string operation;
        while (getline(inputFile, operation)) {
            mapContainer.processOperations(operation);
        }
        inputFile.close();
    }

    outputFile.close();
    return 0;
}

