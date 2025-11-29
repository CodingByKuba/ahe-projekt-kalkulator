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
    long double memory;
    string expression;
    string statusMessage;
    int selectedOptionIndex;
    long double result;
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
    0, //memory
    "", //expression
    "", //status
    0, //index selected
    0, //result
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

const string BUTTONS[] = {
    "B", "C", "M", "Q",
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
bool checkDivisionByZero();

void handleMemory();
void addToExpression();
void eraseExpression();
void clearExpression();
void evalExpression();
int precedence(char op);
double applyOp(double a, double b, char op);

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
    cout << "\t" << THIN.bottomLeft << string(TABLE_LENGTH, THIN.horizontal) << THIN.bottomRight << endl << endl;
}

void printMessageBox(const string MESSAGE) {
    cout << "\t" << DOUBLED.topLeft << string(MESSAGE.size(), DOUBLED.horizontal) << DOUBLED.topRight << endl
    << "\t" << DOUBLED.vertical << left << setw(MESSAGE.size()) << MESSAGE << DOUBLED.vertical << endl
    << "\t" << DOUBLED.bottomLeft << string(MESSAGE.size(), DOUBLED.horizontal) << DOUBLED.bottomRight << endl;
}

void printLogo() {
    cout << endl;
    printMessageWindow(LOGO_TEXT, LOGO_SIZE, LOGO_LENGTH);
    printMessageBox(" STEROWANIE ");
    cout
    << "\t 0-9  +  -  *  /  . - dodawanie znaku do wyrazenia" << endl
    << "\t =                  - obliczenie wyrazenia" << endl
    << "\t W  S  A  D         - poruszanie wskaznikiem przyciskow" << endl
    << "\t Enter              - uzycie wskazanej opcji" << endl
    << "\t Backspace  B       - usuwanie ostatniego znaku wyrazenia" << endl
    << "\t C                  - usuwanie calego wyrazenia" << endl
    << "\t M                  - dodawanie wyniku do wyrazenia lub pamieci" << endl
    << "\t Q                  - wyjscie z programu" << endl << endl
    << "\t Wcisnij dowolny przycisk, by rozpoczac...";
    getch();
}

void printState() {
    printMessageBox(" " + (CurrentState.statusMessage != "" ? CurrentState.statusMessage : "Wpisz wyrazenie") + " ");
    cout << "\tWyrazenie :: " << (CurrentState.expression != "" ? CurrentState.expression : "-") << endl;
    cout << "\t   Pamiec :: " << CurrentState.memory << endl;
    cout << "\t    Wynik :: " << CurrentState.result << endl << endl;
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
        case 'm':
        case 'M':
            handleMemory();
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
    for(int i = CurrentState.expression.size(); i >= 0; i--) {
        reversed += CurrentState.expression[i];
    }
    int dotCount = 0;
    for(char c : reversed) {
        if(c == '.') dotCount++;
        if(c == '+' || c == '-' || c == '*' || c == '/') break;
    }
    if(dotCount > 0) return false;
    return true;
}

bool checkDivisionByZero() {
    for(int i = 0; i < (int)CurrentState.expression.size(); i++) {
        if(CurrentState.expression[i] == '/') {
            int start = i + 1;
            int ending = start;

            while(ending < (int)CurrentState.expression.size()
                  && CurrentState.expression[ending] != '/'
                  && CurrentState.expression[ending] != '*'
                  && CurrentState.expression[ending] != '-'
                  && CurrentState.expression[ending] != '+') ending++;

            string fragment = CurrentState.expression.substr(start, ending - start);
            if(stod(fragment) == 0) return true;
        }
    }
    return false;
}

void handleMemory() {
    if(CurrentState.result != 0) {
        CurrentState.statusMessage = "Zapisano w pamieci dodatni wynik [" + to_string(abs(CurrentState.result)) + "]";
        CurrentState.memory = abs(CurrentState.result);
        CurrentState.result = 0;

    } else {
        char expressionLast = CurrentState.expression[CurrentState.expression.size() - 1];
        if(CurrentState.expression == ""
           || expressionLast == '+'
           || expressionLast == '-'
           || expressionLast == '*'
           || expressionLast == '/') {
            CurrentState.statusMessage = "Dodano do wyrazenia liczbe z pamieci [" + to_string(CurrentState.memory) + "]";
            CurrentState.expression += to_string(CurrentState.memory);
        }
    }
    printCalculator();
}

void addToExpression() {
    const char LAST_CHAR = CurrentState.expression[CurrentState.expression.size() - 1];
    const string PREFIX = "Dodano [";
    const string SUFFIX = "]";
    string newCharacter{option};
    string errorMessage = "";

    if(CurrentState.expression.size() >= EXPRESSION_MAX_LENGTH)
        errorMessage = "Maksymalna dlugosc wyrazenia [" + to_string(EXPRESSION_MAX_LENGTH) + " znakow]";
    if(option == '.' && !isdigit(LAST_CHAR))
        errorMessage = "Nie mozna wprowadzic separatora dziesietnego przed liczba";
    if(option == '.' && !checkLastDoubleNumber())
        errorMessage = "Liczba zawiera juz separator dziesietny";
    if((option == '+' || option == '-' || option == '*' || option == '/') && !isdigit(LAST_CHAR))
       errorMessage = "Nie dodano liczby po operatorze";

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
    CurrentState.result = 0;
    printCalculator();
}

void evalExpression() {
    string expr = CurrentState.expression;
    string errorMessage = "";

    if(!isdigit(expr[expr.size() - 1])) errorMessage = "Ostatni znak wyrazenia nie jest cyfra";
    if(expr == "") errorMessage = "Nie wprowadzono wyrazenia";
    if(checkDivisionByZero()) errorMessage = "Wykryto dzielenie przez zero";

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

    CurrentState.statusMessage = "Obliczono wartosc wyrazenia [" + expr + "]";
    CurrentState.result = valuesArray[valTop];
    CurrentState.expression = "";
    printCalculator();
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

double applyOp(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
    }
    return 0;
}
