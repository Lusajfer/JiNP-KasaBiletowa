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
#include <unordered_set>

using ULL = unsigned long long;
const ULL INF = 1e18 + 10;

using BusRoute = std::pair<std::string, std::vector<std::pair<int, std::string>>>;
using Ticket = std::tuple<std::string, ULL, int>;
using TicketRequest = std::pair<std::vector<std::string>, std::vector<std::string>>;

const std::regex reg1("\\d+(\\s(5:5[5-9]|([6-9]|1[0-9]|20):\\d\\d|21:1[0-9]|21:2[0-1])\\s([a-zA-Z]|\\^)([a-zA-Z]|\\s|\\^)*)+");
const std::regex reg2("[a-zA-Z]([a-zA-Z]|\\s)*\\s([1-9]\\d*|0)\\.\\d\\d\\s[1-9][0-9]*");
const std::regex reg3("\\?(\\s([a-zA-Z]|_|\\^)+\\s\\d+)+\\s([a-zA-Z]|\\^|_)+");

bool checkBusRouteCommand(const std::string& command) {
    return regex_match(command, reg1);
}

bool checkNewTicketCommand(const std::string& command) {
    return regex_match(command, reg2);
}

bool checkTicketRequestCommand(const std::string& command) {
    return regex_match(command, reg3);
}

int timeToMin(std::string time) {
    std::string hours;
    std::string mins;
    hours += time[0];
    std::string semi = ":";
    std::string zero = "0";
    if(time[2] == semi[0]) {
        hours += time[1];
        mins += time[3];
        mins += time[4];
    }
    else {
        mins += time[2];
        mins += time[3];
    }

    return std::stoi(hours) * 60 + std::stoi(mins);
}

std::pair<bool, BusRoute> parseBusRouteCommand(const std::string& command,
                                               std::unordered_set<std::string>& lineNumSet) {

    std::pair<bool, BusRoute> busRoute;

    std::string lineNum;
    auto a = command[0];
    int x = 0;

    while(std::isdigit(a)) {
        a = command[x];
        lineNum += a;
        x += 1;
    }

    if(lineNumSet.find(lineNum) != lineNumSet.end()) {
        return {false, {}};
    }
    else {
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
    int lastMins = -1;

    for (std::sregex_iterator i = itBegin; i != itEnd; ++i) {
        std::smatch match = *i;
        std::string matchStr = match.str();
        if(std::regex_match(matchStr, stopsReg)) {
            if(stopsSet.find(matchStr) != stopsSet.end()) {
                return {false, {}};
            }
            else {
                stopsSet.insert(matchStr);
                stops.push_back(matchStr);
            }
        }
        else if(std::regex_match(matchStr, timeReg)) {
            mins = timeToMin(matchStr);
            if(lastMins == -1) {
                lastMins = mins;
            }
            else if(lastMins >= mins) {
                return {false, {}};
            }
            stopTimes.push_back(mins);
            lastMins = mins;
        }
    }

    lineNumSet.insert(lineNum);

    std::pair<int, std::string> p;
    for(size_t i = 0; i < stopTimes.size(); i++) {
        p.first = stopTimes[i];
        p.second = stops[i];
        busRoute.second.second.push_back(p);
    }

    busRoute.first = true;
    return busRoute;
}

std::pair<bool, Ticket> parseNewTicketCommand(const std::string& command,
                                              std::unordered_set<std::string>& ticketNameSet) {

    std::pair<bool, Ticket> ticket;

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
                return {false, {}};
            }
            else {
                ticketName = matchStr;
            }
        }
        else if(std::regex_match(matchStr, priceReg)) {
            matchStr.erase(matchStr.length()-3, 1);
            if(18 < matchStr.length()) {
                return {false, {}};
            }
            else {
                ticketPrice = std::stoull(matchStr);
            }
        }
        else if(std::regex_match(matchStr, durReg)) {
            if(std::stoi(matchStr) > 60*24) {
                return {false, {}};
            }
            else {
                ticketPrice = std::stoi(matchStr);
            }
        }
    }

    ticketNameSet.insert(ticketName);
    return {true, {ticketName, ticketPrice, ticketDur}};
}

std::pair<bool, TicketRequest> parseTicketRequestCommand(const std::string& command) {
    return {true, {}};
}


void executeBusRoute(const BusRoute& busRoute,
                     std::map<std::string, std::map<int, std::string>>& busRoutes,
                     std::map<std::string, std::map<std::string, int>>& busStops) {

    auto name = busRoute.first;
    auto timetable = busRoute.second;
    
    for(const auto& p : timetable) {
        auto time = p.first;
        auto busStop = p.second;
        
        busRoutes[name][time] = busStop;
        busStops[busStop][name] = time;
    }
}

void executeNewTicket(const Ticket& ticket, std::vector<Ticket>& tickets) {
    tickets.push_back(ticket);
}

std::string executeTicketRequest(const TicketRequest& ticketRequest,
                                 std::map<std::string, std::map<std::string, int>>& busStops,
                                 std::vector<Ticket>& tickets) {

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
    // tutaj wrzucam "globalne" strukturki (potem mozna je wrzucic do maina)
    std::unordered_set<std::string> ticketNameSet;
    std::unordered_set<std::string> lineNumSet;

    std::map<std::string, std::map<int, std::string>> busRoutes;
    std::map<std::string, std::map<std::string, int>> busStops;
    std::vector<Ticket> tickets;


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
                auto p = parseBusRouteCommand(line, lineNumSet);
                if (p.first)
                    executeBusRoute(p.second, busRoutes, busStops);
                else
                    badLine = true;
            }
        } 
        else if(isalpha(line[0]) || isspace(line[0])) {
            if(!checkNewTicketCommand(line)) {
                badLine = true;
            }
            else {
                auto p = parseNewTicketCommand(line, ticketNameSet);
                if(p.first)
                    executeNewTicket(p.second, tickets);
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
                    std::cout << executeTicketRequest(p.second, busStops, tickets) << "\n";
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