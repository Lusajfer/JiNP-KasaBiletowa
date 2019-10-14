#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <regex>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <climits>

using ULL = unsigned long long;
using INF = 1e18 + 10;

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



// nie mozna zmiennych globalnych
// using zamiast pisania poteznych typow

std::map<std::string, std::map<int, std::string>> busRoutes;
std::map<std::string, std::map<std::string, int>> busStops;
std::vector<std::tuple<std::string, ULL, int>> tickets;

void executeBusRoute(std::pair<std::string, std::vector<std::pair<int, std::string>>> busRoute) {
    auto name = busRoute.first;
    auto timetable = busRoute.second;
    
    for(auto p : timetable) {
        auto time = p.first;
        auto busStop = p.second;
        
        busRoutes[name][time] = busStop;
        busStops[busStop][name] = time;
    }
}

void executeNewTicket(std::tuple<std::string, ULL, int> ticket) {
    tickets.push_back(cmd);
}

bool executeTicketRequest(std::pair<std::vector<std::string>, std::vector<std::string>> busRide) {
    int time = 0;
    auto stops = busRide.first;
    auto routes = busRide.second;
    int prevTimestamp = busStops[stops[0]][routes[0]];
    
    for(int i = 0; i < stops.size(); i++) {
        auto currStop = stops[i];
        auto prevRoute = routes[i];
        auto nextRoute = routes[i + 1];
        
        int prevTime = busStops[currStop][prevRoute];
        int nextTime = busStops[currStop][nextRoute];
        
        if(prevTime != nextTime) {
            // wypisac nazwe przystanku
            return false;
        }
        
        time += prevTime - prevTimestamp + 1;
        prevTimestamp = prevTime;
    }
    
    
    std::vector<std::string>> bestTickets;
    ULL lowestPrice = INF;
    
    if(tickets.size() >= 1) {    
        for(int i = 0; i < tickets.size(); i++) {
        
            if(std::get<2>(tickets[i]) >= time) {
                ULL currPrice = std::get<1>(tickets[i]);
                
                if(currPrice < lowestPrice) {
                    lowestPrice = currPrice;
                    bestTickets = {std::get<0>(tickets[i])};
                }
            }
        }
    } 
    else if(tickets.size() >= 2) {
        for(int i = 0; i < tickets.size(); i++) {
            for(int j = i; j < tickets.size(); j++) {
            
                if(std::get<2>(tickets[i]) + std::get<2>(tickets[j]) >= time) {
                    ULL currPrice = std::get<1>(tickets[i]) + std::get<1>(tickets[j]);
                    
                    if(currPrice < lowestPrice) {
                        lowestPrice = currPrice;
                        bestTickets = {std::get<0>(tickets[i]), std::get<0>(tickets[j])};
                    }
                }
            }
        }
    } 
    else if(tickets.size() >= 3) {
        for(int i = 0; i < tickets.size(); i++) {
            for(int j = i; j < tickets.size(); j++) {
                for(int k = j; k < tickets.size(); k++) {
                    
                    if(std::get<2>(tickets[i]) + std::get<2>(tickets[j]) + std::get<2>(tickets[k]) >= time) {
                        ULL currPrice = std::get<1>(tickets[i]) + std::get<1>(tickets[j]) + std::get<1>(tickets[k]);
                        
                        if(currPrice < lowestPrice) {
                            lowestPrice = currPrice;
                            bestTickets = {std::get<0>(tickets[i]), std::get<0>(tickets[j]), std::get<0>(tickets[k])};
                        }
                    }        
                }
            }
        }
    } 
    
    if(bestTickets.size() == 0) {
        // nie znaleziono odpowiedniego biletu lub nie ma biletow
        return false;
    }
    
    // wypisac bilety z bestTickets;
    
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
