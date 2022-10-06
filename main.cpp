#include <iostream>
#include <fstream>
#include <string>

#include "ArgumentManager.h"
#include "BTree.h"

using namespace std;

int main(int argc, char* argv[]) {
  ArgumentManager am(argc, argv);
  ifstream input(am.get("input"));
  ifstream command(am.get("command"));
  ofstream output(am.get("output"));
  /*
  ifstream input("input44.txt");
  ifstream command("command44.txt");
  ofstream output("output44.txt");*/
  
  int degree;
  command.ignore(7);
  command >> degree;
  
  BTree myTree(degree);

  int numToInsert;
  while (input >> numToInsert) {
    myTree.insert(numToInsert);
    /*
    DEBUG MODE:
    
    cout << "inserting " << numToInsert << endl;
    for (int i = 0; i < myTree.getHeight(); i++) {
      myTree.printLevel(i + 1, output);
    }
    cout << endl;
    */
  }

  output << "Height=" << myTree.getHeight() << endl;
  
  int levelToPrint;
  while(command.ignore(6) && command >> levelToPrint) {
    myTree.printLevel(levelToPrint, output);
  }
}