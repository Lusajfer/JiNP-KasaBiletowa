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
const ULL INF = 1e18 + 10;

using BusRoute = std::pair<std::string, std::vector<std::pair<int, std::string>>>;
using Ticket = std::tuple<std::string, ULL, int>;
using TicketRequest = std::pair<std::vector<std::string>, std::vector<std::string>>;

const std::regex reg1("\\d+(\\s(5:5[5-9]|([6-9]|1[0-9]|20):\\d\\d|21:1[0-9]|21:2[0-1])\\s([a-zA-Z]|\\^)([a-zA-Z]|\\s|\\^)*)+");
const std::regex reg2("[a-zA-Z]([a-zA-Z]|\\s)*\\s([1-9]\\d*|0)\\.\\d\\d\\s[1-9][0-9]*");
const std::regex reg3("\\?((\\s[a-zA-Z]|\\^)([a-zA-Z]|\\s|\\^)*\\s\\d+)+");

bool checkBusRouteCommand(const std::string& command) {
    return regex_match(command, reg1);
}

bool checkNewTicketCommand(const std::string& command) {
    return regex_match(command, reg2);
}

bool checkTicketRequestCommand(const std::string& command) {
    return regex_match(command, reg3);
}


std::pair<bool, Ticket> parseNewTicketCommand(const std::string& command, std::unordered_set<std::string> ticketNameSet, int lineId) {
    std::pair<bool, std::tuple<std::string, ULL, int>> ticket;

    std::regex reg("[a-zA-Z]([a-zA-Z]|\\s)*|([1-9]\\d*|0)\\.\\d\\d|[1-9][0-9]*");
    auto itBegin = std::sregex_iterator(command.begin(), command.end(), reg);
    auto itEnd = std::sregex_iterator();
    std::regex nameReg("[a-zA-Z]([a-zA-Z]|\\s)*");
    std::regex priceReg("([1-9]\\d*|0)\\.\\d\\d");
    std::regex durReg("[1-9][0-9]*");
    std::string ticketName;
    ULL ticketPrice;
    int ticketDur;

    for (std::sregex_iterator i = itBegin; i != itEnd; ++i) {
        std::smatch match = *i;                                                 
        std::string matchStr = match.str();
        if(std::regex_match(matchStr, nameReg)) {
            if(ticketNameSet.find(matchStr) != ticketNameSet.end()) {
                printErr(lineId, command);
                ticket.first = false;
                return ticket;
            }
            else {
                ticketNameSet.insert(matchStr);
                ticketName = matchStr;
            }
        }
        else if(std::regex_match(matchStr, priceReg)) {
            matchStr.erase(matchStr.length()-3, 1);
            if(18 < matchStr.length()) {
                printErr(lineId, command);
                ticket.first = false;
                return ticket;
            }
            else {
                ticketPrice = std::stoull(matchStr);
            }
        }
        else if(std::regex_match(matchStr, durReg)) {
            if(std::stoi(matchStr) > 60*24) {
                printErr(lineId, command);
                ticket.first = false;
                return ticket;
            }
            else {
                ticketPrice = std::stoi(matchStr);
            }
        }
    }
    
    std::tuple<std::string, ULL, int> t(ticketName, ticketPrice, ticketDur);
    ticket.first = true;
    ticket.second = t;
    return ticket;
}

std::pair<bool, BusRoute> parseBusRouteCommand(const std::string& command, std::unordered_set<std::string> lineNumSet, int lineId) {
    std::pair<bool, std::pair<std::string, std::vector<std::pair<int, std::string> > > > busRoute;

    std::string lineNum;
    auto a = command[0];
    int x = 0;

    while(std::isdigit(a)) {
        a = command[x];
        lineNum += a;
        x += 1;
    }

    if(lineNumSet.find(lineNum) != lineNumSet.end()) {
        busRoute.first = false;
        printErr(lineId, command);
        //return busRoute;
    }
    else
    {
        lineNumSet.insert(lineNum);
        busRoute.second.first = lineNum;
    }

    std::regex reg("(5:5[5-9]|([6-9]|1[0-9]|20):\\d\\d|21:1[0-9]|21:2[0-1])|([a-zA-Z]|_|\\^)+");
    auto itBegin = std::sregex_iterator(command.begin(), command.end(), reg);
    auto itEnd = std::sregex_iterator();
    std::regex stopsReg("([a-zA-Z]|_|\\^)+");
    std::regex timeReg("\\d+:\\d\\d");
    std::unordered_set<std::string> stopsSet;
    std::vector<std::string> stops;
    std::vector<int> stopTimes;
    int mins;
    int mxMins = -1;
 
    for (std::sregex_iterator i = itBegin; i != itEnd; ++i) {
        std::smatch match = *i;                                                 
        std::string matchStr = match.str();
        if(std::regex_match(matchStr, stopsReg)) {
            if(stopsSet.find(matchStr) != stopsSet.end()) {
                printErr(lineId, command);
                busRoute.first = false;
                return busRoute;
            }
            else {
                stopsSet.insert(matchStr);
                stops.push_back(matchStr);
            }
        }
        else if(std::regex_match(matchStr, timeReg)) {
            mins = timeToMin(matchStr);
            if(mxMins == -1) {
                mxMins = mins;
            }
            else if(mxMins <= mins) {
                printErr(lineId, command);
                busRoute.first = false;
                return busRoute;
            }

            stopTimes.push_back(mins);
        }
        std::cout << matchStr << '\n';
    }

   std::pair<int, std::string> p;
    for(int i = 0; i < stopTimes.size(); i++) {
        p.first = stopTimes[i]; 
        p.second = stops[i];
        busRoute.second.second.push_back(p);
    }

    busRoute.first = true;
    return busRoute;
}

std::pair<bool, TicketRequest> parseTicketRequestCommand(const std::string& command) {
    return {true, {}};
}



// nie mozna zmiennych globalnych
// using zamiast pisania poteznych typow

std::map<std::string, std::map<int, std::string>> busRoutes;
std::map<std::string, std::map<std::string, int>> busStops;
std::vector<Ticket> tickets;

void executeBusRoute(const BusRoute& busRoute) {
    auto name = busRoute.first;
    auto timetable = busRoute.second;
    
    for(auto p : timetable) {
        auto time = p.first;
        auto busStop = p.second;
        
        busRoutes[name][time] = busStop;
        busStops[busStop][name] = time;
    }
}

void executeNewTicket(const Ticket& ticket) {
    tickets.push_back(ticket);
}

std::string executeTicketRequest(const TicketRequest& ticketRequest) {
    int time = 0;
    auto stops = ticketRequest.first;
    auto routes = ticketRequest.second;
    int prevTimestamp = busStops[stops[0]][routes[0]];
    
    for(size_t i = 0; i < stops.size(); i++) {
        auto currStop = stops[i];
        auto prevRoute = routes[i];
        auto nextRoute = routes[i + 1];
        
        int prevTime = busStops[currStop][prevRoute];
        int nextTime = busStops[currStop][nextRoute];
        
        if(prevTime != nextTime) {
            return ":-( " + currStop;
        }
        
        time += prevTime - prevTimestamp + 1;
        prevTimestamp = prevTime;
    }
    
    
    std::vector<std::string> bestTickets;
    ULL lowestPrice = INF;
    
    if(tickets.size() >= 1) {    
        for(size_t i = 0; i < tickets.size(); i++) {
        
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
        for(size_t i = 0; i < tickets.size(); i++) {
            for(size_t j = i; j < tickets.size(); j++) {
            
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
        for(size_t i = 0; i < tickets.size(); i++) {
            for(size_t j = i; j < tickets.size(); j++) {
                for(size_t k = j; k < tickets.size(); k++) {
                    
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

    if(bestTickets.empty()) {
        return ":-|";
    }

    std::string ret = "!";
    for(auto t : bestTickets) {
        ret.append(" " + t);
    }

    return ret;
}


void getInput() {

    int lineId = 0;
    std::string line;
     
    while(getline(std::cin, line)) {
    
        lineId++;

        if(line.empty()) {
            continue;
        }

        bool badLine = false;

        if(isdigit(line[0]) || line[0] == '_' || line[0] == '^') {
            if(!checkBusRouteCommand(line)) {
                badLine = true;
            }
            else {
                auto p = parseBusRouteCommand(line);
                if (p.first)
                    executeBusRoute(p.second);
                else
                    badLine = true;
            }
        } 
        else if(isalpha(line[0]) || isspace(line[0])) {
            if(!checkNewTicketCommand(line)) {
                badLine = true;
            }
            else {
                auto p = parseNewTicketCommand(line);
                if(p.first)
                    executeNewTicket(p.second);
                else
                    badLine = true;
            }
        }
        else if(line[0] == '?') {
            if(!checkTicketRequestCommand(line)) {
                badLine = true;
            }
            else {
                auto p = parseTicketRequestCommand(line);
                if(p.first)
                    std::cout << executeTicketRequest(p.second) << "\n";
                else
                    badLine = true;
            }
        }
        else {
            badLine = true;
        }

        if(badLine) {
            std::cerr << "Error in line " << lineId << ": " << line << "\n";
        }
    }
}

int main() {
    
    getInput();    
    return 0;
}
