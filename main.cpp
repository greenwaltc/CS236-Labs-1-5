#include <iostream>
#include <string>
#include "Scanner.h"
#include "Parser.h"
#include "Interpreter.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2){
        cout << "Please provide one file as an argument" << '\n';
        return 0;
    }

    string filename = argv[1];
    Scanner myScan(filename);
    //cout << myScan.printQueries() << '\n';

    //Project 2 stuff
    Parser myParse(myScan.getAllTokens());
    DatalogProgram myProgram = myParse.parse();
    //if ( myParse.success() ) { cout << myProgram.printQueries(); }

    //Project 3
    Interpreter interpret(myProgram);
    cout << interpret.printQueries();
    return 0;
}