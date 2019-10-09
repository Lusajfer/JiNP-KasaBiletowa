#include <bits/stdc++.h>
using namespace std;

/**
 * tab to 4 spacje xDDD
 */

// zaimplementuj te 3 funkcje xdddddddddd
// zwracaja true jesli linijka poprawna, false w przeciwnym
bool checkBusRouteCommand(string command) {

}

bool checkNewTicketCommand(string command) {

}

bool checkTicketRequestCommand(string command) {

}



inline void printErr(int lineId, string line) {
    cerr << "Error in line " << lineId << ": " << line;
}


void getInput() {

    int lineId = 0;
    string line;
     
    while(getline(cin, line)) {
    
        lineId++;
        
        // pusta linijka
        if(line.empty()) {
            continue;
        }
        
        // determinowanie typu komendy za pomoca pierwszego znaku
        if(isDigit(line[0]) {
            if(!checkBusRouteCommand(line)) {
                printErr(lineId, line);
                continue;
            }        
            
        } 
        else if(isalpha(line[0]) || line[0] == '_' || line[0] == '^') {
            if(!checkNewTicketCommand(line)) {
                printErr(lineId, line);
                continue;
            }
            
        }
        else if(line[0] == '?') {
            if(!checkTicketRequestCommand(line)) {
                printfErr(lineId, line);
                continue;
            }
            
        }
        else {
            printErr(lineId, line);
            continue;           
        }
    }
}

int main() {

    getInput();    
    return 0;
}
