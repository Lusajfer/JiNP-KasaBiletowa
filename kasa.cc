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

inline void printErr(int lineId, const std::string& line) {
    std::cerr << "Error in line " << lineId << ": " << line;
}

bool checkBusRouteCommand(const std::string& command) {
    return regex_match(command, reg1);
}

bool checkNewTicketCommand(const std::string& command) {
    return regex_match(command, reg2);
}

bool checkTicketRequestCommand(const std::string& command) {
    return regex_match(command, reg3);
}



std::pair<std::string, std::vector<std::pair<int, std::string>>> parseBusRouteCommand(const std::string& command) {

}

std::tuple<std::string, ULL, int> parseNewTicketCommand(const std::string& command) {

}

std::pair<std::vector<std::string>, std::vector<std::string>> parseTicketRequestCommand(const std::string& command) {

}


/// executing parsed and checked commands
std::map<std::string, std::map<int, std::string>> busRoutes;
std::map<std::string, std::map<std::string, int>> busStops;
std::vector<std::tuple<std::string, ULL, int>> tickets;

void executeBusRoute(const std::pair<std::string, std::vector<std::pair<int, std::string>>>& cmd) {
    auto name = cmd.first;
    auto timetable = cmd.second;

    for(const auto& busStop : timetable) {
        busRoutes[name][busStop.first] = busStop.second;
        busStops[busStop.second][name] = busStop.first;
    }
}

void executeNewTicket(const std::tuple<std::string, ULL, int>& cmd) {
    tickets.push_back(cmd);
}

bool executeTicketRequest(const std::pair<std::vector<std::string>, std::vector<std::string>>& cmd) {
    int time = 0;
    auto stops = cmd.first;
    auto routes = cmd.second;
    int prevTime = busStops[stops[0]][routes[0]];

    for(int i = 1; i < routes.size(); i++) {
        auto prevRoute = routes[i];
        auto nextRoute = routes[i + 1];
        auto currStop = stops[i];
        // finished
    }

    return true;
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

            executeBusRoute(parseBusRouteCommand(line));
        } 
        else if(isalpha(line[0]) || isspace(line[0])) {
            if(!checkNewTicketCommand(line)) {
                printErr(lineId, line);
                continue;
            }

            executeNewTicket(parseNewTicketCommand(line));
        }
        else if(line[0] == '?') {
            if(!checkTicketRequestCommand(line)) {
                printErr(lineId, line);
                continue;
            }

            executeTicketRequest(parseTicketRequestCommand(line));
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
