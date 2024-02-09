#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

enum nums { S = 1, R = 2, G = 3 };

struct Token {
    string kind;
    double value;
    Token(string s) : kind(s), value(0) {}
    Token(string s, double d) : kind(s), value(d) {}
};

//Overloads the printing of Token
ostream& operator<<(ostream& os, const Token& tok) {
    return os << tok.kind << "(" << tok.value << ")";
}

struct Action {
    int actionType; //Can either be null=0 | S=1 | R=2 | G=3 | accept = 4
    int number; //Holds the value for the row or instruction. 
    Action(int action, int numb) : actionType(action), number(numb) {}
    Action() : actionType(0), number(0) {}

};

ostream& operator<<(ostream& os, const Action& act) {
    if (act.number < 10) {
        if (act.actionType == 0)
            return os << "   ";
        else if (act.actionType == 1)
            return os << " S" << act.number;
        else if (act.actionType == 2)
            return os << " R" << act.number;
        else if (act.actionType == 3)
            return os << " G" << act.number;
        else if (act.actionType == 4)
            return os << "YES";
    }
    else { //Where act.number is between 0 - 9
        if (act.actionType == 0)
            return os << "   ";
        else if (act.actionType == 1)
            return os << "S" << act.number;
        else if (act.actionType == 2)
            return os << "R" << act.number;
        else if (act.actionType == 3)
            return os << "G" << act.number;
        else if (act.actionType == 4)
            return os << "YES";
    }
    return os << "Error!!";

}

template <class InputIterator>
void printContainer(InputIterator start, InputIterator end) {
    int count = 0;
    for (auto itr = start; itr != end; ++itr) {
        // cout << count++ << ") " << *itr << "\n";
        cout << "(" << *itr << ")";
    }
    cout << endl;
}


//Takes the kind of Token and returns which column it translates to. 
int getColumn(string kind) {
    if (kind == "v")
        return 0;
    else if (kind == "=")
        return 1;
    else if (kind == "+")
        return 2;
    else if (kind == "-")
        return 3;
    else if (kind == "*")
        return 4;
    else if (kind == "/")
        return 5;
    else if (kind == "%")
        return 6;
    else if (kind == "(" || kind == "LP")
        return 7;
    else if (kind == ")" || kind == "RP")
        return 8;
    else if (kind == "d")
        return 9;
    else if (kind == "$")
        return 10;
    else if (kind == "A")
        return 11;
    else if (kind == "E")
        return 12;
    else if (kind == "T")
        return 13;
    else if (kind == "P")
        return 14;
    else
        return -1;
}

//Takes a Token and returns which column it translates to. 
int getColumn(Token token) {
    return getColumn(token.kind);
}


int stateCount = 0;
//Prints out the Current State!
void printState(vector<Token>& tokens, vector<int>& rowStack, vector<Token>& tokenStack) {
    cout << "\nState: " << stateCount++ << "\n";
    cout << "\tTokens: ";
    printContainer(tokens.begin(), tokens.end());
    cout << "\tRowStack: ";
    printContainer(rowStack.begin(), rowStack.end());
    cout << "\tTokenStack: ";
    printContainer(tokenStack.begin(), tokenStack.end());
    cout << endl;
}

Action getAction(int row, int col) {
    const Action array[23][15] = {
        /*0*/    {Action(S,2), Action(), Action(), Action(), Action(), Action(), Action(), Action(S, 6), Action(), Action(S, 7), Action(), Action(G, 1), Action(G, 3), Action(G, 4), Action(G, 5) },
        /*1*/    {Action(), Action(),  Action(),  Action(),  Action(), Action(), Action(),  Action(),  Action(),  Action(), Action(4, 0),  Action(), Action(),  Action(),  Action()},
        /*2*/    {Action(R, 11), Action(S, 8),  Action(R, 11),  Action(R, 11),  Action(R, 11), Action(R, 11), Action(R, 11),  Action(R, 11),  Action(R, 11),  Action(R, 11), Action(R, 11),  Action(), Action(),  Action(),  Action()},
        /*3*/    {Action(R, 2), Action(R,2),  Action(S,9),  Action(S,10),  Action(R,2), Action(R,2), Action(R,2),  Action(R,2),  Action(R,2),  Action(R,2), Action(R,2),  Action(), Action(),  Action(),  Action()},
        /*4*/    {Action(R, 3), Action(R, 3),  Action(R,3),  Action(R,3),  Action(S, 11), Action(S,12), Action(S,13),  Action(R,3),  Action(R,3),  Action(R,3), Action(R,3),  Action(), Action(),  Action(),  Action()},
        /*5*/    {Action(R, 6), Action(R, 6),  Action(R, 6),  Action(R, 6),  Action(R, 6), Action(R, 6), Action(R, 6),  Action(R, 6),  Action(R, 6),  Action(R, 6), Action(R, 6),  Action(), Action(),  Action(),  Action()},
        /*6*/    {Action(S, 15), Action(),  Action(),  Action(),  Action(), Action(), Action(),  Action(S,6),  Action(),  Action(S, 7), Action(0, 0),  Action(), Action(G, 14),  Action(G, 4),  Action(G, 5)},
        /*7*/    {Action(R, 12), Action(R, 12),  Action(R, 12),  Action(R, 12),  Action(R, 12), Action(R, 12), Action(R, 12),  Action(R, 12),  Action(R, 12),  Action(R, 12), Action(R, 12),  Action(), Action(),  Action(),  Action()},
        /*8*/    {Action(S, 15), Action(),  Action(),  Action(),  Action(), Action(), Action(),  Action(S,6),  Action(),  Action(S, 7), Action(0, 0),  Action(), Action(G, 16),  Action(G, 4),  Action(G, 5)},
        /*9*/    {Action(S, 15), Action(),  Action(),  Action(),  Action(), Action(), Action(),  Action(S,6),  Action(),  Action(S, 7), Action(0, 0),  Action(), Action(),  Action(G, 17),  Action(G, 5)},
        /*10*/   {Action(S, 15), Action(),  Action(),  Action(),  Action(), Action(), Action(),  Action(S,6),  Action(),  Action(S, 7), Action(0, 0),  Action(), Action(),  Action(G, 18),  Action(G, 5)},
        /*11*/   {Action(S, 15), Action(),  Action(),  Action(),  Action(), Action(), Action(),  Action(S,6),  Action(),  Action(S, 7), Action(0, 0),  Action(), Action(),  Action(),  Action(G, 19)},
        /*12*/   {Action(S, 15), Action(),  Action(),  Action(),  Action(), Action(), Action(),  Action(S,6),  Action(),  Action(S, 7), Action(0, 0),  Action(), Action(),  Action(),  Action(G, 20)},
        /*13*/   {Action(S, 15), Action(),  Action(),  Action(),  Action(), Action(), Action(),  Action(S,6),  Action(),  Action(S, 7), Action(0, 0),  Action(), Action(),  Action(),  Action(G, 21)},
        /*14*/   {Action(), Action(),  Action(S, 9),  Action(S, 10),  Action(), Action(), Action(),  Action(),  Action(S, 22),  Action(), Action(),  Action(), Action(),  Action(),  Action()},
        /*15*/   {Action(R, 11), Action(R, 11),  Action(R, 11),  Action(R, 11),  Action(R, 11), Action(R, 11), Action(R, 11),  Action(R, 11),  Action(R, 11),  Action(R, 11), Action(R, 11),  Action(), Action(),  Action(),  Action()},
        /*16*/   {Action(R, 1), Action(R, 1),  Action(S, 9),  Action(S, 10),  Action(R,1), Action(R, 1), Action(R, 1),  Action(R, 1),  Action(R, 1),  Action(R, 1), Action(R, 1),  Action(), Action(),  Action(),  Action()},
        /*17*/   {Action(R,4), Action(R,4),  Action(R,4),  Action(R,4),  Action(S, 11), Action(S, 12), Action(S,13),  Action(R,4),  Action(R,4),  Action(R,4), Action(R,4),  Action(), Action(),  Action(),  Action()},
        /*18*/   {Action(R,5), Action(R,5),  Action(R,5),  Action(R,5),  Action(S, 11), Action(S, 12), Action(S,13),  Action(R,5),  Action(R,5),  Action(R,5), Action(R,5),  Action(), Action(),  Action(),  Action()},
        /*19*/   {Action(R,7), Action(R,7),  Action(R,7),  Action(R,7),  Action(R,7), Action(R,7), Action(R,7),  Action(R,7),  Action(R,7),  Action(R,7), Action(R,7),  Action(), Action(),  Action(),  Action()},
        /*20*/   {Action(R,8), Action(R,8),  Action(R,8),  Action(R,8),  Action(R,8), Action(R,8), Action(R,8),  Action(R,8),  Action(R,8),  Action(R,8), Action(R,8),  Action(), Action(),  Action(),  Action()},
        /*21*/   {Action(R,9), Action(R,9),  Action(R,9),  Action(R,9),  Action(R,9), Action(R,9), Action(R,9),  Action(R,9),  Action(R,9),  Action(R,9), Action(R,9),  Action(), Action(),  Action(),  Action()},
        /*22*/   {Action(R,10), Action(R,10),  Action(R,10),  Action(R,10),  Action(R,10), Action(R,10), Action(R,10),  Action(R,10),  Action(R,10),  Action(R,10), Action(R,10),  Action(), Action(),  Action(),  Action()},
    };
    /*
    cout << "Actions: \n";
    cout << "[ v ][ = ][ + ][ - ][ * ][ / ][ % ][ ( ][ ) ][ d ][ $ ][ A ][ E ][ T ][ P ]\n";
    for(int i = 0; i < 23; i++){
        printContainer(begin(array[i]), end(array[i]));
    }
    */

    return array[row][col];
}


//Should return a single token at the very end with the answer. 
double compute(vector<Token>& tokens) {
    vector<int> rowStack{ 0 };
    vector<Token> tokenStack;


    printState(tokens, rowStack, tokenStack); //Shows Initial State 

    //Begin Loop
    //Should itterate twice. 
    for (int i = 0; i < 20; i++) {
        cout << "Next Token: " << tokens.back().kind << "\n";
        cout << "Get Next Token Column:  " << getColumn(tokens.back()) << "\n";
        Action nextAction = getAction(rowStack.back(), getColumn(tokens.back()));
        cout << "Action: " << nextAction << '\n';
        if (nextAction.actionType == S) {
            rowStack.push_back(nextAction.number);
            tokenStack.push_back(tokens.back());
            tokens.pop_back();
        }
        else if (nextAction.actionType == R) {
            if (nextAction.number == 1) {
                //Rule 1: A -> v = E
                if (tokenStack.back().kind == "E" && tokenStack.at(tokenStack.size() - 2).kind == "=" && tokenStack.at(tokenStack.size() - 3).kind == "v") {
                    cout << "REMOVING: " << tokenStack.at(tokenStack.size() - 3) << " = " << tokenStack.back() << "\n";
                    Token tempA = Token("A", tokenStack.back().value); //Gets value of E. 
                    cout << "(ACTUALLY IMPLEMENT!!) Sets v = " << tokenStack.back().value;

                    // Actually implement setting the variable to the number!
                    tokenStack.pop_back();
                    tokenStack.pop_back();
                    tokenStack.pop_back();
                    tokenStack.push_back(tempA);
                    rowStack.pop_back(); //Removes the last number from rowStack.
                    rowStack.pop_back();
                    rowStack.pop_back();
                    rowStack.push_back(getAction(rowStack.back(), getColumn("A")).number); //Adds the correct 
                    return tokenStack.back().value;
                }
                else {
                    cout << "MAJOR ERROR: Token Not 'v = E'\n";
                }
            }
            else if (nextAction.number == 2) {
                //Rule 2: A -> E
                if (tokenStack.back().kind == "E") {
                    cout << "REMOVING: " << tokenStack.back() << "\n";
                    cout << "(ACTUALLY IMPLEMENT!!) Sets ANS = " << tokenStack.back().value;
                    Token tempA = Token("A", tokenStack.back().value); //Gets value of E. 
                    tokenStack.pop_back();
                    tokenStack.push_back(tempA);
                    rowStack.pop_back(); //Removes the last number from rowStack.
                    rowStack.push_back(getAction(rowStack.back(), getColumn("A")).number); //Adds the correct 
                    return tokenStack.back().value;
                }
                else {
                    cout << "MAJOR ERROR: Token Not 'E'\n";
                }
            }
            else if (nextAction.number == 3) {
                //Rule 3: E -> T
                if (tokenStack.back().kind == "T") {
                    cout << "REMOVING: " << tokenStack.back() << "\n";
                    Token tempE = Token("E", tokenStack.back().value); //Gets value of v. 
                    tokenStack.pop_back();
                    tokenStack.push_back(tempE);
                    rowStack.pop_back(); //Removes the last number from rowStack.
                    rowStack.push_back(getAction(rowStack.back(), getColumn("E")).number); //Adds the correct 
                }
                else {
                    cout << "MAJOR ERROR: Token Not 'v'\n";
                }
            }
            else if (nextAction.number == 5) {
                //Rule 5: E -> E - T
                if (tokenStack.back().kind == "T" && tokenStack.at(tokenStack.size() - 2).kind == "-" && tokenStack.at(tokenStack.size() - 3).kind == "E") {
                    cout << "REMOVING: " << tokenStack.at(tokenStack.size() - 3) << " - " << tokenStack.back() << "\n";
                    Token tempT = Token("E", (tokenStack.at(tokenStack.size() - 3).value - tokenStack.back().value)); //Gets value of v. 
                    tokenStack.pop_back();
                    tokenStack.pop_back();
                    tokenStack.pop_back();
                    tokenStack.push_back(tempT);
                    rowStack.pop_back(); //Removes the last number from rowStack.
                    rowStack.pop_back();
                    rowStack.pop_back();
                    rowStack.push_back(getAction(rowStack.back(), getColumn("E")).number); //Adds the correct 
                }
                else {
                    cout << "MAJOR ERROR: Token Not 'v'\n";
                }
            }
            else if (nextAction.number == 4) {
                //Rule 4: E -> E + T
                if (tokenStack.back().kind == "T" && tokenStack.at(tokenStack.size() - 2).kind == "+" && tokenStack.at(tokenStack.size() - 3).kind == "E") {
                    cout << "REMOVING: " << tokenStack.at(tokenStack.size() - 3) << " + " << tokenStack.back() << "\n";
                    Token tempT = Token("E", (tokenStack.at(tokenStack.size() - 3).value + tokenStack.back().value));
                    tokenStack.pop_back();
                    tokenStack.pop_back();
                    tokenStack.pop_back();
                    tokenStack.push_back(tempT);
                    rowStack.pop_back(); //Removes the last number from rowStack.
                    rowStack.pop_back();
                    rowStack.pop_back();
                    rowStack.push_back(getAction(rowStack.back(), getColumn("E")).number); //Adds the correct 
                }
                else {
                    cout << "MAJOR ERROR: Token Not 'E + T'\n";
                }
            }
            else if (nextAction.number == 6) {
                //Rule 6: T -> P
                if (tokenStack.back().kind == "P") {
                    cout << "REMOVING: " << tokenStack.back() << "\n";
                    Token tempT = Token("T", tokenStack.back().value); //Gets value of v. 
                    tokenStack.pop_back();
                    tokenStack.push_back(tempT);
                    rowStack.pop_back(); //Removes the last number from rowStack.
                    rowStack.push_back(getAction(rowStack.back(), getColumn("T")).number); //Adds the correct 
                }
                else {
                    cout << "MAJOR ERROR: Token Not 'v'\n";
                }
            }
            else if (nextAction.number == 7) {
                //Rule 7: T -> T *  P
                if (tokenStack.back().kind == "P" && tokenStack.at(tokenStack.size() - 2).kind == "*" && tokenStack.at(tokenStack.size() - 3).kind == "T") {
                    cout << "REMOVING: " << tokenStack.at(tokenStack.size() - 3) << " * " << tokenStack.back() << "\n";
                    Token tempT = Token("T", (tokenStack.at(tokenStack.size() - 3).value * tokenStack.back().value)); //Gets value of v. 
                    tokenStack.pop_back();
                    tokenStack.pop_back();
                    tokenStack.pop_back();
                    tokenStack.push_back(tempT);
                    rowStack.pop_back(); //Removes the last number from rowStack.
                    rowStack.pop_back();
                    rowStack.pop_back();
                    rowStack.push_back(getAction(rowStack.back(), getColumn("T")).number); //Adds the correct 
                }
                else {
                    cout << "MAJOR ERROR: Token Not 'T * P'\n";
                }
            }
            else if (nextAction.number == 8) {
                //Rule 8: T -> T /  P
                if (tokenStack.back().kind == "P" && tokenStack.at(tokenStack.size() - 2).kind == "/" && tokenStack.at(tokenStack.size() - 3).kind == "T") {
                    cout << "REMOVING: " << tokenStack.at(tokenStack.size() - 3) << " / " << tokenStack.back() << "\n";
                    Token tempT = Token("T", (tokenStack.at(tokenStack.size() - 3).value / tokenStack.back().value)); //Gets value of v. 
                    tokenStack.pop_back();
                    tokenStack.pop_back();
                    tokenStack.pop_back();
                    tokenStack.push_back(tempT);
                    rowStack.pop_back(); //Removes the last number from rowStack.
                    rowStack.pop_back();
                    rowStack.pop_back();
                    rowStack.push_back(getAction(rowStack.back(), getColumn("T")).number); //Adds the correct 
                }
                else {
                    cout << "MAJOR ERROR: Token Not 'T / P'\n";
                }
            }
            else if (nextAction.number == 9) {
                //Rule 9: T -> T %  P
                if (tokenStack.back().kind == "P" && tokenStack.at(tokenStack.size() - 2).kind == "%" && tokenStack.at(tokenStack.size() - 3).kind == "T") {
                    cout << "REMOVING: " << tokenStack.at(tokenStack.size() - 3) << " % " << tokenStack.back() << "\n";
                    Token tempT = Token("T", (int(tokenStack.at(tokenStack.size() - 3).value) % int(tokenStack.back().value))); //Gets value of v. 
                    tokenStack.pop_back();
                    tokenStack.pop_back();
                    tokenStack.pop_back();
                    tokenStack.push_back(tempT);
                    rowStack.pop_back(); //Removes the last number from rowStack.
                    rowStack.pop_back();
                    rowStack.pop_back();
                    rowStack.push_back(getAction(rowStack.back(), getColumn("T")).number); //Adds the correct 
                }
                else {
                    cout << "MAJOR ERROR: Token Not 'T % P'\n";
                }
            }
            else if (nextAction.number == 10) {
                //Rule 10: P -> ( E )
                if (tokenStack.back().kind == "RP" && tokenStack.at(tokenStack.size() - 2).kind == "E" && tokenStack.at(tokenStack.size() - 3).kind == "LP") {
                    cout << "REMOVING: (" << tokenStack.at(tokenStack.size() - 2)  << ")\n";
                    Token tempP = Token("P",tokenStack.at(tokenStack.size() - 2).value); //Gets value of v. 
                    tokenStack.pop_back();
                    tokenStack.pop_back();
                    tokenStack.pop_back();
                    tokenStack.push_back(tempP);
                    rowStack.pop_back(); //Removes the last number from rowStack.
                    rowStack.pop_back();
                    rowStack.pop_back();
                    rowStack.push_back(getAction(rowStack.back(), getColumn("P")).number); //Adds the correct number
                }
                else {
                    cout << "MAJOR ERROR: Token Not '( E )'\n";
                }
            }

            else if (nextAction.number == 11) {
                //Rule 11: P -> v
                if (tokenStack.back().kind == "v") {
                    cout << "REMOVING: " << tokenStack.back() << "\n";
                    Token tempP = Token("P", tokenStack.back().value); //Gets value of v. 
                    tokenStack.pop_back();
                    tokenStack.push_back(tempP);
                    rowStack.pop_back(); //Removes the last number from rowStack.
                    rowStack.push_back(getAction(rowStack.back(), getColumn("P")).number); //Adds the correct number
                }
                else {
                    cout << "MAJOR ERROR: Token Not 'v'\n";
                }
            }
            else if (nextAction.number == 12) {
                //Rule 12: P -> d
                if (tokenStack.back().kind == "d") {
                    cout << "REMOVING: " << tokenStack.back() << "\n";
                    Token tempP = Token("P", tokenStack.back().value); //Gets value of v. 
                    tokenStack.pop_back();
                    tokenStack.push_back(tempP);
                    rowStack.pop_back(); //Removes the last number from rowStack.
                    rowStack.push_back(getAction(rowStack.back(), getColumn("P")).number); //Adds the correct number
                }
                else {
                    cout << "MAJOR ERROR: Token Not 'v'\n";
                }
            }
        }

        printState(tokens, rowStack, tokenStack); //Shows second state
    }

    return -1;
}










int main() {
    cout << "Hello Second!\n";
    vector<Token> tokens;

    // 3 + 4 * 2 = 11
    // (3 + 4)*2 = 14
    //okay lets reverse this lol.
    // tokens.push_back(Token("v"));
    // tokens.push_back(Token("="));
    tokens.push_back(Token("LP"));
    tokens.push_back(Token("d", 3));
    tokens.push_back(Token("+"));
    tokens.push_back(Token("d", 4));
    tokens.push_back(Token("RP"));
    tokens.push_back(Token("*"));
    tokens.push_back(Token("d", 2));
    tokens.push_back(Token("$"));

    reverse(tokens.begin(), tokens.end()); //We're reversing the order of tokens, because we want it to function as a queue.



    cout << "\n\nFinal Answer: " << compute(tokens);


    cout << endl;


}



