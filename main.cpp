#include <iostream>
#include <string>
#include <vector>
using namespace std;

// struct token {

// };
//Will Print All The values within a container. 
template <class InputIterator>
void printContainer(InputIterator start, InputIterator end) {
    int count = 0;
    for (auto itr = start; itr != end; ++itr) {
        // cout << count++ << ") " << *itr << "\n";
        cout << "(" << *itr << ")";
    }
    cout << endl;
}

//This will take the vector of tokens and determine what to do. 



//Will parse all the characters into tokens. 
vector<string> parse(string& line) {
    vector<string> vs;
    int i = 0;

    while (i < line.size()) {

        //Looking for INT_CONST
        if (isdigit(line.at(i))) {
            string tempDig = "n: ";     // initializes the digit 
            while (isdigit(line.at(i))) {
                tempDig += string(1, line.at(i));
                if (line.size() - 1 <= i) {
                    break;
                }

                if (isalpha(line.at(i + 1))) {
                    tempDig = "SYNTAX ERROR: " + tempDig;
                    //Clear the input for this line and return that there was an error. 
                }
                i++;
            }
            vs.push_back(tempDig);
        }

        //Looking for variable names.
        if (isalpha(line.at(i))) {
            string tempName = "w: "; //initializes the variable name. 
            while (isalnum(line.at(i))) {
                tempName += string(1, line.at(i));
                if (line.size() - 1 <= i) {
                    break;
                }
                else {
                    i++;
                }
            }
            vs.push_back(tempName);
        }

        switch (line.at(i)) {
        case '(':
            vs.push_back("LP");
            break;
        case ')':
            vs.push_back("RP");
            break;
        case '+':
        case '-':
        case '*':
        case '/':
            vs.push_back(string(1, line.at(i)));
        }

        //almost need something to itterate 
        i++;
    }

    cout << endl;
    return vs;
}

int main() {
    cout << "Calculator App!\n";
    string str;
    while (getline(cin, str)) {
        vector<string> vec = parse(str);
        printContainer(vec.begin(), vec.end());
        cout << endl;
    }
    return 0;
}

/*
    for (int i = 0; i < str.length(); i++) {
        cout << "(" << i << "): " << str[i] << "\n";
    }
*/

