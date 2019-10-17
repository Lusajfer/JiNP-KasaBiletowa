#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <regex>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <unordered_map>
#include <climits>
#include <unordered_set>

using ULL = unsigned long long;
const ULL INF = 1e18 + 10;

using BusRoute = std::pair<std::string, std::vector<std::pair<int, std::string>>>;
using Ticket = std::tuple<std::string, ULL, int>;
using TicketRequest = std::pair<std::vector<std::string>, std::vector<std::string>>;

const std::regex reg1("\\d+( (5:5[5-9]|([6-9]|1[0-9]|20):[0-5]\\d|21:1[0-9]|21:2[0-1]) ([a-zA-Z]|_|\\^)+)+");
const std::regex reg2("([a-zA-Z]| )+ ([1-9]\\d*|0)\\.\\d\\d [1-9][0-9]*");
const std::regex reg3("\\?( ([a-zA-Z]|_|\\^)+ \\d+)+ ([a-zA-Z]|\\^|_)+");

std::unordered_map<std::string, std::unordered_set<std::string>> busLines;

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

    std::string lineNum = "";
    int x = 0;
    std::string zero = "0";

    while(std::isdigit(command[x])) {
        if(command[x] != zero[0]) {
            lineNum += command[x];
        }
        x++;
    }

    if(lineNum == "") {
        lineNum = "0";
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
            if(lastMins >= mins) {
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

    for(const auto& p : busRoute.second.second) {
        busLines[busRoute.second.first].insert(p.second);
    }

    busRoute.first = true;
    return busRoute;
}

std::pair<bool, Ticket> parseNewTicketCommand(const std::string& command,
                                              std::unordered_set<std::string>& ticketNameSet) {

    std::pair<bool, Ticket> ticket;

    std::string ticketName = "";

    int x = 0;

    while(!std::isdigit(command[x+1])) {
        ticketName += command[x];
        x += 1;
    }

    if(ticketNameSet.find(ticketName) != ticketNameSet.end()) {
        return {false, {}};
    }

    std::regex reg("([1-9]\\d*|0)\\.\\d\\d|[1-9][0-9]*");
    auto itBegin = std::sregex_iterator(command.begin(), command.end(), reg);
    auto itEnd = std::sregex_iterator();
    std::regex priceReg("([1-9]\\d*|0)\\.\\d\\d");
    std::regex durReg("[1-9][0-9]*");
    ULL ticketPrice;
    int ticketDur;

    for (std::sregex_iterator i = itBegin; i != itEnd; ++i) {
        std::smatch match = *i;
        std::string matchStr = match.str();
        if(std::regex_match(matchStr, priceReg)) {
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
                ticketDur = std::stoi(matchStr);
            }
        }
    }

    ticketNameSet.insert(ticketName);
    return {true, {ticketName, ticketPrice, ticketDur}};
}

std::pair<bool, TicketRequest> parseTicketRequestCommand(const std::string& command,
                                                         const std::unordered_set<std::string>& lineNumSet) {

    std::regex reg("\\d+|([a-zA-Z]|_|\\^)+");
    auto itBegin = std::sregex_iterator(command.begin(), command.end(), reg);
    auto itEnd = std::sregex_iterator();
    std::regex lineNumReg("\\d+");
    std::regex stopNameReg("([a-zA-Z]|_|\\^)+");
    std::vector<std::string> stops;
    std::vector<std::string> lineNumbers;
    std::string zero = "0";

    for (std::sregex_iterator i = itBegin; i != itEnd; ++i) {
        std::smatch match = *i;
        std::string matchStr = match.str();

        if(std::regex_match(matchStr, lineNumReg)) {
            std::string lineNum = "";
            for(size_t i = 0; i < matchStr.length(); i++) {
                if(matchStr[i] != zero[0]) {
                    lineNum += matchStr[i];
                }
            }

            if(lineNum == "") {
                lineNum = "0";
            }

            if(lineNumSet.find(lineNum) == lineNumSet.end()) {
                return {false, {}};
            }
            else {
                lineNumbers.push_back(lineNum);
            }
        }
        else if(std::regex_match(matchStr, stopNameReg)) {
            stops.push_back(matchStr);
        }
    }

    for(size_t i = 0; i < lineNumbers.size(); i++) {
        if(busLines[lineNumbers[i]].find(stops[i]) == busLines[lineNumbers[i]].end()) {
            return {false, {}};
        }
    }

    if(busLines[lineNumbers[lineNumbers.size()-1]].find(stops[stops.size()-1]) == busLines[lineNumbers[lineNumbers.size()-1]].end()) {
        return {false, {}};
    }

    for(size_t i = 1; i < stops.size(); i++) {
        if(stops[i] == stops[i - 1]) {
            return {false, {}};
        }
    }


    return {true, {stops, lineNumbers}};
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
                                 std::vector<Ticket>& tickets,
                                 int& numberOfTickets) {

    auto stops = ticketRequest.first;
    auto routes = ticketRequest.second;

    for(size_t i = 1; i < routes.size(); i++) {
        auto currStop = stops[i];
        auto prevRoute = routes[i - 1];
        auto nextRoute = routes[i];
        
        int prevTime = busStops[currStop][prevRoute];
        int nextTime = busStops[currStop][nextRoute];
        
        if(prevTime < nextTime) {
            return ":-( " + currStop;
        }
        else if(prevTime > nextTime) {
            return "";
        }
    }

    if(routes.size() == 1) {
        if(busStops[stops.back()][routes.back()] < busStops[stops[0]][routes[0]]) {
            return "";
        }
    }


    int time = busStops[stops.back()][routes.back()] - busStops[stops[0]][routes[0]] + 1;

    std::vector<Ticket> bestTickets;
    ULL lowestPrice = INF;

    for(size_t i = 0; i < tickets.size(); i++) {
        for(size_t j = i; j <= tickets.size(); j++) {
            for(size_t k = j; k <= tickets.size(); k++) {

                std::vector<Ticket> curr;

                curr.push_back(tickets[i]);
                if(j != tickets.size())
                    curr.push_back(tickets[j]);
                if(k != tickets.size())
                    curr.push_back(tickets[k]);

                int currTime = 0;
                for(const auto& t : curr)
                    currTime += std::get<2>(t);

                if(currTime >= time) {
                    ULL currPrice = 0;
                    for(const auto& t : curr)
                        currPrice += std::get<1>(t);

                    if(currPrice < lowestPrice) {
                        lowestPrice = currPrice;
                        bestTickets = curr;
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
        ret.append(" " + std::get<0>(t) + ";");
    }
    ret.pop_back();

    numberOfTickets += (int)bestTickets.size();

    return ret;
}

void printBusRoute(BusRoute busRoute) {
    std::cout << "BusRoute command: ";
    std::cout << busRoute.first << " ";
    for(auto p : busRoute.second) {
        std::cout << p.first << " " << p.second << " ";
    }
    std::cout << "\n";
}

void printTicket(Ticket ticket) {
    std::cout << "Ticket: " << std::get<0>(ticket) << " " << std::get<1>(ticket) << " " << std::get<2>(ticket) << "\n";
}

void printTicketRequest(TicketRequest ticketRequest) {
    std::cout << ticketRequest.first.size() << " " << ticketRequest.second.size() << "\n";
    std::cout << "TicketRequest: ";
    for(int i = 0; i < (int)ticketRequest.second.size(); i++) {
        std::cout << ticketRequest.first[i] << " " << ticketRequest.second[i] << " ";
    }
    std::cout << ticketRequest.first.back();
    std::cout << "\n";
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
    int numberOfTickets = 0;
     
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
                if (p.first) {
                    executeBusRoute(p.second, busRoutes, busStops);
                    //printBusRoute(p.second);
                }
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
                if(p.first) {
                    executeNewTicket(p.second, tickets);
                    //printTicket(p.second);
                }
                else
                    badLine = true;
            }
        }
        else if(line[0] == '?') {
            if(!checkTicketRequestCommand(line)) {
                badLine = true;
            }
            else {
                auto p = parseTicketRequestCommand(line, lineNumSet);
                if(p.first) {
                    std::string ticketRequestOutput = executeTicketRequest(p.second, busStops, tickets, numberOfTickets);
                    if(ticketRequestOutput == "")
                        badLine = true;
                    else
                        std::cout << ticketRequestOutput << "\n";
                    //printTicketRequest(p.second);
                }
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

    std::cout << numberOfTickets << "\n";
}

int main() {
    
    getInput();    
    return 0;
}
