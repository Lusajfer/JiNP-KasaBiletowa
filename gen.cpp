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

using namespace std;

inline int rr(int a, int b) {
    return rand() % (b - a + 1) + a;
}

string genString(int length) {
    string ret;
    for(int i = 1; i <= length; i++) {
        bool caps = rr(0, 1);
        char c = 'a' + rr(0, 25);
        if(caps) {
            c = 'A' + rr(0, 25);
        }
        ret.push_back(c);
    }
    return ret;
}

string genBusRoute(int length) {
    string ret;

}

string genTicket() {

}

string genTicketRequest() {

}

void genCommand() {
    int type = rr(1, 3);
    if(type == 1) {

    }
    else if(type == 2) {

    }
    else {

    }
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        cout << "OGARNIJ ZIARNO GENERATORA DOWNIE XD\n";
        return 0;
    }
    srand(atoi(argv[1]));

    int commands = rr(4, 12);

    while(commands--) {
        genCommand();
    }

}