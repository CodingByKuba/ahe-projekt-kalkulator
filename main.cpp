#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include <conio.h>

using namespace std;

struct GridChars {
    char topLeft;
    char topMid;
    char topRight;
    char midLeft;
    char midMid;
    char midRight;
    char bottomLeft;
    char bottomMid;
    char bottomRight;
    char horizontal;
    char vertical;
};

struct CalculatorState {
    string expression;
    string statusMessage;
    int selectedOptionIndex;
    string expressionResult;
};

const GridChars DOUBLED = {
    '\xC9', '\xCB', '\xBB', //top
    '\xCC', '\xCE', '\xB9', //mid
    '\xC8', '\xCA', '\xBC', //bottom
    '\xCD', '\xBA' //horizontal/vertical
};

const GridChars THIN = {
    '\xDA', '\xC2', '\xBF', //top
    '\xC3', '\xC5', '\xB4', //mid
    '\xC0', '\xC1', '\xD9', //bottom
    '\xC4', '\xB3' //horizontal/vertical
};

CalculatorState CurrentState = {
    "", //expression
    "", //status
    0, //index selected
    "" //result
};

const string LOGO_TEXT[] = {
"               KALKULATOR               ",
"              PROJEKT NR 1              ",
"                                        ",
"           SEMESTR 1 GRUPA D1           ",
"        1. TOMASZ ZALISZ (lider)        ",
"        2. MAREK ZALISZ                 ",
"        3. KAMIL WROBEL                 ",
"        4. JAKUB MARCZAK                "
};
const int LOGO_SIZE = sizeof(LOGO_TEXT) / sizeof(LOGO_TEXT[0]);
const int LOGO_LENGTH = 40;

const string CONTROLS_TEXT[] = {
" STEROWANIE:                                                    ",
" 0-9                - dodawanie cyfry do wyrazenia              ",
" .                  - dodawanie separatora dziesietnego         ",
" e p !              - dodawanie liczby e/PI/silnia do wyrazenia ",
" + - * / ^ #        - dodawanie operatora do wyrazenia          ",
" =                  - obliczenie wyrazenia                      ",
" W  S  A  D         - poruszanie wskaznikiem przyciskow         ",
" Enter              - uzycie wskazanej opcji                    ",
" Backspace  B       - usuwanie ostatniego znaku wyrazenia       ",
" C                  - usuwanie calego wyrazenia                 ",
" M                  - dodawanie wyniku do wyrazenia lub pamieci ",
" Q                  - wyjscie z programu                        ",
};
const int CONTROLS_SIZE = sizeof(CONTROLS_TEXT) / sizeof(CONTROLS_TEXT[0]);
const int CONTROLS_LENGTH = 64;

const string BUTTONS[] = {
    "p", "B", "C", "Q",
    "e", "!", "^", "#",
    "7", "8", "9", "/",
    "4", "5", "6", "*",
    "1", "2", "3", "+",
    "0", ".", "=", "-",
};
const int BUTTONS_SIZE = sizeof BUTTONS / sizeof BUTTONS[0];
const int BUTTONS_IN_ROW = 4;
const int BUTTON_LENGTH = 7;

const int EXPRESSION_MAX_LENGTH = 50;

char option;

void printMessageWindow(const string* MESSAGE_ARRAY, const int ARRAY_SIZE, const int TABLE_LENGTH);
void printMessageBox(const string MESSAGE);
void printLogo();
void printState();
void printButtonsTable();
void printCalculator();
void printFinalMessage();

void executeOption();
void selectOptionByArrows(int direction);

bool checkLastDoubleNumber();
bool checkNumberBeforeFactorial();
bool checkDivisionByZero(string &expression);
bool checkIsOperator(char op);

void addToExpression();
void eraseExpression();
void clearExpression();
void evalExpression();
int precedence(char op);
double applyOp(double a, double b, char op);

long long calculateFactorial(int number);
long double calculateEuler();
long double calculatePi();

int main() {
    printLogo();
    printCalculator();
    do {
        option = getch();
        if(option == '\x0D') option = BUTTONS[CurrentState.selectedOptionIndex][0];
        executeOption();

    } while(option != 'q' && option != 'Q');
    printFinalMessage();
    return 0;
}

void printMessageWindow(const string* MESSAGE_ARRAY, const int ARRAY_SIZE, const int TABLE_LENGTH) {
    cout << endl << "\t" << THIN.topLeft << string(TABLE_LENGTH, THIN.horizontal) << THIN.topRight << endl;
    for(int line = 0; line < ARRAY_SIZE; line++) {
        cout << "\t" << THIN.vertical << left << setw(TABLE_LENGTH)
        << MESSAGE_ARRAY[line] << THIN.vertical << endl;
    }
    cout << "\t" << THIN.bottomLeft << string(TABLE_LENGTH, THIN.horizontal) << THIN.bottomRight << endl;
}

void printMessageBox(const string MESSAGE) {
    cout << "\t" << DOUBLED.topLeft << string(MESSAGE.size(), DOUBLED.horizontal) << DOUBLED.topRight << endl
    << "\t" << DOUBLED.vertical << left << setw(MESSAGE.size()) << MESSAGE << DOUBLED.vertical << endl
    << "\t" << DOUBLED.bottomLeft << string(MESSAGE.size(), DOUBLED.horizontal) << DOUBLED.bottomRight << endl;
}

void printLogo() {
    cout << endl;
    printMessageWindow(LOGO_TEXT, LOGO_SIZE, LOGO_LENGTH);
    printMessageWindow(CONTROLS_TEXT, CONTROLS_SIZE, CONTROLS_LENGTH);
    getch();
}

void printState() {
    printMessageBox(" " + (CurrentState.statusMessage != "" ? CurrentState.statusMessage : "Wpisz wyrazenie") + " ");
    cout << "\tWyrazenie :: " << (CurrentState.expression != "" ? CurrentState.expression : "-") << endl;
    cout << "\t    Wynik :: " << CurrentState.expressionResult << endl << endl;
}

void printButtonsTable() {
    if(BUTTONS_SIZE % BUTTONS_IN_ROW != 0) {
        printMessageBox("Blad w trakcie wczytywania przycisków kalkulatora");
        return;
    }

    char cornerSymbol = THIN.topLeft;
    char rightSymbol = THIN.topRight;
    char bottomCornerSymbol = THIN.bottomMid;

    for(int row = 0; row < BUTTONS_SIZE / BUTTONS_IN_ROW; row++) {

        // Górna ramka
        cout << "\t";
        for(int col = 0; col < BUTTONS_IN_ROW; col++) {

            if(row == 0 && col == 0) cornerSymbol = THIN.topLeft;
            if(row == 0 && col > 0) cornerSymbol = THIN.topMid;
            if(row > 0 && col == 0) cornerSymbol = THIN.midLeft;
            if(row > 0 && col > 0) cornerSymbol = THIN.midMid;

            cout << cornerSymbol << string(BUTTON_LENGTH, THIN.horizontal);
        }

        if(row == 0) rightSymbol = THIN.topRight;
        if(row > 0) rightSymbol = THIN.midRight;
        cout << rightSymbol << endl;

        //Wiersze danych - przyciski
        cout << "\t";
        for(int col = 0; col < BUTTONS_IN_ROW; col++) {
            string currentOption = "";
            const int OFFSET_LEFT = floor(BUTTON_LENGTH / 2);
            const int OPTION_INDEX = row * BUTTONS_IN_ROW + col;
            for(int i = 0; i < OFFSET_LEFT; i++) currentOption += CurrentState.selectedOptionIndex == OPTION_INDEX ? ">" : " ";
            currentOption += BUTTONS[OPTION_INDEX];

            cout << THIN.vertical << left << setw(BUTTON_LENGTH)  << currentOption;
        }
        cout << THIN.vertical << endl;
    }

    cout << "\t" << THIN.bottomLeft;

    for(int col = 0; col < BUTTONS_IN_ROW; col++) {
        if(col < BUTTONS_IN_ROW - 1) bottomCornerSymbol = THIN.bottomMid;
        if(col == BUTTONS_IN_ROW - 1) bottomCornerSymbol = THIN.bottomRight;
        cout << string(BUTTON_LENGTH, THIN.horizontal) << bottomCornerSymbol;
    }
}

void printCalculator() {
    system("cls");
    cout << endl << endl;
    printState();
    printButtonsTable();
}

void printFinalMessage() {
    system("cls");
    cout << endl << endl;
    printMessageBox("Dziekujemy za skorzystanie z programu...");
    cout << endl;
}

void executeOption() {
    switch(option) {
        case 'w':
        case 'W':
            selectOptionByArrows(1);
            break;
        case 'd':
        case 'D':
            selectOptionByArrows(2);
            break;
        case 's':
        case 'S':
            selectOptionByArrows(3);
            break;
        case 'a':
        case 'A':
            selectOptionByArrows(4);
            break;
        case 'b':
        case 'B':
        case '\x08':
            eraseExpression();
            break;
        case 'c':
        case 'C':
            clearExpression();
            break;
        case '=':
            evalExpression();
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '.':
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
        case '#':
        case '!':
        case 'e':
        case 'p':
            addToExpression();
            break;
    }
}

void selectOptionByArrows(int direction) {
    int newIndex = 0;
    int currentIndex = CurrentState.selectedOptionIndex;

    if(direction == 1) {
        if(currentIndex < BUTTONS_IN_ROW) return;
        newIndex = currentIndex - BUTTONS_IN_ROW;
    }

    if(direction == 2) {
        if((currentIndex + 1) % BUTTONS_IN_ROW == 0 && currentIndex > 0) return;
        newIndex = currentIndex + 1;
    }

    if(direction == 3) {
        if(currentIndex + BUTTONS_IN_ROW >= BUTTONS_SIZE) return;
        newIndex = currentIndex + BUTTONS_IN_ROW;
    }

    if(direction == 4) {
        if(currentIndex % BUTTONS_IN_ROW == 0 || currentIndex == 0) return;
        newIndex = currentIndex - 1;
    }

    CurrentState.selectedOptionIndex = newIndex;
    printCalculator();
}

bool checkLastDoubleNumber() {
    string reversed = "";
    for(int i = CurrentState.expression.size() - 1; i >= 0; i--) {
        reversed += CurrentState.expression[i];
    }
    int dotCount = 0;
    for(char c : reversed) {
        if(c == '.') dotCount++;
        if(checkIsOperator(c)) break;
    }
    if(dotCount > 0) return true;
    return false;
}

bool checkNumberBeforeFactorial() {
    string reversed = "";
    for(int i = CurrentState.expression.size() - 1; i >= 0; i--) {
        char c = CurrentState.expression[i];
        if(!isdigit(c)) break;
        reversed += c;
    }
    string turnedBack = "";
    for(char c : reversed) turnedBack = c + turnedBack;
    if(stoi(turnedBack) < 0 || stoi(turnedBack) > 20) return true;
    return false;
}

bool checkDivisionByZero(string &expression) {
    for(int i = 0; i < (int)expression.size(); i++) {
        if(expression[i] == '/') {
            int start = i + 1;
            int ending = start;

            while(ending < (int)expression.size() && !checkIsOperator(expression[ending]))
                ending++;

            string fragment = expression.substr(start, ending - start);
            if(stod(fragment) == 0) return true;
        }
    }
    return false;
}

bool checkIsOperator(char op) {
    if(!op) return false;
    if(op == '+' || op == '-' || op == '*' || op == '/' || op == '^' || op == '#') return true;
    return false;
}

void addToExpression() {
    const char LAST_CHAR = CurrentState.expression[CurrentState.expression.size() - 1];
    const string PREFIX = "Dodano [";
    const string SUFFIX = "]";
    string newCharacter{option};
    string errorMessage = "";

    if(CurrentState.expression.size() >= EXPRESSION_MAX_LENGTH)
        errorMessage = "Maksymalna dlugosc wyrazenia [" + to_string(EXPRESSION_MAX_LENGTH) + " znakow]";
    if(option == '.') {
        if(!isdigit(LAST_CHAR)) errorMessage = "Nie mozna wprowadzic separatora dziesietnego przed liczba";
        else if(checkLastDoubleNumber()) errorMessage = "Liczba zawiera juz separator dziesietny";
    }
    if(checkIsOperator(option)
       && (!isdigit(LAST_CHAR) && LAST_CHAR != 'e' && LAST_CHAR != 'p' && LAST_CHAR != '!'))
       errorMessage = "Nie dodano liczby po operatorze";
    if(isdigit(option) && (LAST_CHAR == 'e' || LAST_CHAR == 'p'))
        errorMessage = "Nie dodano operatora po liczbie";
    if((option == 'e' || option == 'p') && !checkIsOperator(LAST_CHAR) && CurrentState.expression.size() > 0)
        errorMessage = "Nie dodano operatora przed liczba";
    if(option == '!') {
        if(!isdigit(LAST_CHAR)) errorMessage = "Brak liczby calkowitej przed silnia";
        else if(checkLastDoubleNumber()) errorMessage = "Silnia moze byc uzyta tylko przy liczbie calkowitej";
        else if(checkNumberBeforeFactorial()) errorMessage = "Wartosc silnii musi miec wartosc miedzy 1-20";
    }

    if(errorMessage.size() > 0) {
        CurrentState.statusMessage = errorMessage;
        printCalculator();
        return;
    }

    CurrentState.statusMessage = PREFIX + newCharacter + SUFFIX;
    CurrentState.expression += newCharacter;
    printCalculator();
}

void eraseExpression() {
    if(CurrentState.expression.size() == 0) return;
    CurrentState.statusMessage = "Usunieto ostatni znak wyrazenia";
    CurrentState.expression.pop_back();
    printCalculator();
}

void clearExpression() {
    CurrentState.statusMessage = "Wyczyszczono wyrazenie";
    CurrentState.expression = "";
    CurrentState.expressionResult = "";
    printCalculator();
}

void evalExpression() {
    string expr = CurrentState.expression;
    string errorMessage = "";

    if(!isdigit(expr[expr.size() - 1]) && expr[expr.size() - 1] != 'p' && expr[expr.size() - 1] != 'e' && expr[expr.size() - 1] != '!')
        errorMessage = "Ostatni znak wyrazenia nie jest liczba";
    if(expr == "") errorMessage = "Nie wprowadzono wyrazenia";

    int factorialMemoPosition = 0;

    for(int ch = 0; ch < (int)expr.size(); ch++) {
        if(expr[ch] == 'p') expr.replace(ch, 1, to_string(calculatePi()));
        if(expr[ch] == 'e') expr.replace(ch, 1, to_string(calculateEuler()));

        if(ch > 0 && checkIsOperator(expr[ch - 1])) factorialMemoPosition = ch;
        if(expr[ch] == '!') {
            int charsToSelect = ch - factorialMemoPosition + 1;
            string factorialToCount = expr.substr(factorialMemoPosition, charsToSelect);
            string convertedFactorialResult = to_string(calculateFactorial(stoi(factorialToCount)));
            expr.replace(factorialMemoPosition, charsToSelect, convertedFactorialResult);
        }
    }

    if(checkDivisionByZero(expr)) errorMessage = "Wykryto dzielenie przez zero";

    if(errorMessage.size() > 0) {
        CurrentState.statusMessage = errorMessage;
        printCalculator();
        return;
    }

    double valuesArray[100];
    char opsArray[100];
    int valTop = -1, opTop = -1;

    for (int i = 0; i < (int)expr.size(); i++) {
        if (isdigit(expr[i]) || expr[i] == '.') {
            double value = 0, fraction = 0.1;
            bool isFloat = false;

            while (i < (int)expr.size() && (isdigit(expr[i]) || expr[i] == '.')) {
                if (expr[i] == '.') isFloat = true;
                else if (!isFloat) value = value * 10 + (expr[i] - '0');
                else {
                    value += (expr[i] - '0') * fraction;
                    fraction *= 0.1;
                }
                i++;
            }
            valuesArray[++valTop] = value;
            i--;
        } else {
            while (opTop >= 0 && precedence(opsArray[opTop]) >= precedence(expr[i])) {
                double b = valuesArray[valTop--], a = valuesArray[valTop--];
                valuesArray[++valTop] = applyOp(a, b, opsArray[opTop--]);
            }
            opsArray[++opTop] = expr[i];
        }
    }

    while (opTop >= 0) {
        double b = valuesArray[valTop--], a = valuesArray[valTop--];
        valuesArray[++valTop] = applyOp(a, b, opsArray[opTop--]);
    }

    CurrentState.statusMessage = "Obliczono wartosc wyrazenia [" + CurrentState.expression + "]";
    CurrentState.expressionResult = to_string(valuesArray[valTop]);
    CurrentState.expression = "";
    printCalculator();
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^' || op == '#') return 3;
    return 0;
}

double applyOp(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return pow(a, b);
        case '#': return pow(b, 1 / a);
    }
    return 0;
}

long long calculateFactorial(int number) {
    if(number == 0) return 1;
    long long result = 1;
    for(int i = 1; i < number + 1; i++) {
        result *= i;
    }
    return result;
}

long double calculateEuler() {
    double result = 0.0;
    for(int i = 0; i < 21; i++) result += 1.0 / calculateFactorial(i);
    return result;
}

long double calculatePi() {
    double base = 1.0;
    // Szereg Madhavy-Leibniza
    for(int i = 1; i < 1000000; i++) {
        double calculation = 1.0 / (i * 2.0 + 1.0);
        if(i % 2 == 0) base += calculation;
        else base -= calculation;
    }
    double result = base * 4;
    return result;
}
