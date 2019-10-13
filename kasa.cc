#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <regex>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <map>

using ULL = unsigned long long;

const std::regex reg1("\\d+(\\s(5:5[5-9]|([6-9]|1[0-9]|20):\\d\\d|21:1[0-9]|21:2[0-1])\\s([a-zA-Z]|\\^)([a-zA-Z]|\\s|\\^)*)+");
const std::regex reg2("[a-zA-Z]([a-zA-Z]|\\s)*\\s([1-9]\\d*|0)\\.\\d\\d\\s[1-9][0-9]*");
const std::regex reg3("\\?((\\s[a-zA-Z]|\\^)([a-zA-Z]|\\s|\\^)*\\s\\d+)+");

bool checkBusRouteCommand(std::string command) {
    return regex_match(command, reg1);
}

bool checkNewTicketCommand(std::string command) {
    return regex_match(command, reg2);
}

bool checkTicketRequestCommand(std::string command) {
    return regex_match(command, reg3);
}


inline void printErr(int lineId, std::string line) {
    std::cerr << "Error in line " << lineId << ": " << line;
}


std::pair<std::string, std::vector<std::pair<int, std::string>>> parseBusRouteCommand(std::string command) {

}

std::tuple<std::string, ULL, int> parseNewTicketCommand(std::string command) {

}

std::pair<std::vector<std::string>, std::vector<std::string>> parseTicketRequestCommand(std::string command) {

}

void getInput() {

    int lineId = 0;
    std::string line;
     
    while(getline(std::cin, line)) {
    
        lineId++;

        if(line.empty()) {
            continue;
        }

        if(isdigit(line[0]) || line[0] == '_' || line[0] == '^') {
            if(!checkBusRouteCommand(line)) {
                printErr(lineId, line);
                continue;
            }


        } 
        else if(isalpha(line[0]) || isspace(line[0])) {
            if(!checkNewTicketCommand(line)) {
                printErr(lineId, line);
                continue;
            }
            
        }
        else if(line[0] == '?') {
            if(!checkTicketRequestCommand(line)) {
                printErr(lineId, line);
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
