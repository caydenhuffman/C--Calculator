#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Token {
    string kind;
    double value;
    string name; //Only used for variables: v
    Token(string s) : kind(s), value(0) {}
    Token(string s, double d) : kind(s), value(d) {}
    Token(string s, string n) : kind(s), value(0), name(n) {
        // cout << "Check To See If We already got this one!\n";
        // //Check to see if this variable already exists, and if it does, get it value
        // //Or else add it to the list of variables. 
    }
};

//Overloads the printing of Token
ostream& operator<<(ostream& os, const Token& tok) {
    if (tok.kind == "d")
        return os << tok.kind << "[" << tok.value << "]";
    else if (tok.kind == "v")
        return os << tok.kind << "[" << tok.name << "][" << tok.value << "]"; 
    return os << tok.kind;
}

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
vector<Token> parse(string& line) {
    vector<Token> tokens;
    int i = 0;

    while (i < line.size()) {

        //Looking for INT_CONST
        if (isdigit(line.at(i)) || line.at(i) == '.') {
            string tempDig = "";     // initializes the digit 
            while (isdigit(line.at(i)) || line.at(i) == '.') {
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
            tokens.push_back(Token("d", stod(tempDig)));
        }

        //Looking for variable names.
        if (isalpha(line.at(i))) {
            string tempName = ""; //initializes the variable name. 
            while (isalnum(line.at(i))) {
                tempName += string(1, line.at(i));
                if (line.size() - 1 <= i) {
                    break;
                }
                else {
                    i++;
                }
            }
            tokens.push_back(Token("v", tempName));
        }

        switch (line.at(i)) {
        case '(':
            tokens.push_back(Token("LP"));
            break;
        case ')':
            tokens.push_back(Token("RP"));
            break;
        case '+':
        case '-':
        case '*':
        case '/':
        case '=':
            tokens.push_back(Token(string(1, line.at(i))));
        }

        //almost need something to itterate 
        i++;
    }

    cout << endl;
    return tokens;
}

int main() {
    cout << "Calculator App!\n";
    string str;
    while (getline(cin, str)) {
        vector<Token> vec = parse(str);
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

