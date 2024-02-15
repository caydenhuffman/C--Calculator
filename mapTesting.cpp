#include <iostream>
#include <string>
#include <map>
using namespace std;



template <class InputIterator>
void printContainer(InputIterator start, InputIterator end) {
    int count = 0;
    for (auto itr = start; itr != end; ++itr) {
        // cout << count++ << ") " << *itr << "\n";
        cout << itr->first << "(" << itr->second << ")\n";
    }
    cout << endl;
}


int main() {
    // cout << "Map Testing!" << endl;
    // map<string, int> mp;
    // mp["x"] = 3;
    // mp["ANS"] = 0; 
    // mp["u"] = 9; 
    // mp["t"] = 4; 

    // cout <<"t: " << mp["s"] << endl; 
    // printContainer(mp.begin(), mp.end());

    cout << "HEY DOES THIS WORK?\n";
    double x0 = 5.1;
    int x = 5;
    int y = 2;
    int ANS = int(x0) % y;
    cout << ANS << "\n";
    return 0;
}