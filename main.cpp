#include <iostream>
#include <iomanip>
#include <limits>
#include <conio.h>
#include <cmath>

using namespace std;

/***** DEKLARACJE ZMIENNYCH, STALYCH, FUNKCJI I PROCEDUR *****/
double x, y;
char theme = 'D';
char menuOption;

const char SYMBOL_TL = '\xC9'; // ┌
const char SYMBOL_TC = '\xCB'; // ┬
const char SYMBOL_TR = '\xBB'; // ┐
const char SYMBOL_H  = '\xCD'; // ─
const char SYMBOL_V  = '\xBA'; // │
const char SYMBOL_CL = '\xCC'; // ├
const char SYMBOL_CC = '\xCE'; // ┼
const char SYMBOL_CR = '\xB9'; // ┤
const char SYMBOL_BL = '\xC8'; // └
const char SYMBOL_BC = '\xCA'; // ┴
const char SYMBOL_BR = '\xBC'; // ┘

const string LOGO_TEXT[] = {
"  *            KALKULATOR            *  ",
" *            PROJEKT NR 1            * ",
"*                                      *",
"           SEMESTR 1 GRUPA D1           ",
"        1. TOMASZ ZALISZ (lider)        ",
"*       2. MAREK ZALISZ                *",
" *      3. KAMIL WROBEL               * ",
"  *     4. JAKUB MARCZAK             *  "
};
const int LOGO_LENGTH = sizeof(LOGO_TEXT) / sizeof(LOGO_TEXT[0]);
const int LOGO_WIDTH = 40;

void printLogo();
void printMainMenu();
void printErrorBox(const string ERROR_MESSAGE);

void switchTheme();

long double addNumbers(double x, double y);
long double subtractNumbers(double x, double y);
long double multiplyNumbers(double x, double y);
long double divideNumbers(double x, double y);
long double exponentiateNumbers(double x, int y);
long double squareNumbers(double x);
long long calculateFactorial(int x);

double calculateBmi(double weight, double height);

struct ResultStructure {
    string operation;
    string result;
};

const int OPERATIONS_WIDTH = 20;
const int RESULTS_WIDTH = 40;
void printResultTable(const ResultStructure VALUES[], const int ARRAY_SIZE);

void classicCalculator();
void financialCalculator();
void bmiCalculator();
string defineBmi(const double BMI);

/***** GLOWNA FUNKCJA PROGRAMU *****/
int main() {
    printLogo();

    do {
        system("cls");
        printMainMenu();

        menuOption = getch();

        if((menuOption == 't') | (menuOption == 'T')) {
            switchTheme();
        }
        else if((menuOption == 'c') | (menuOption == 'C')) {
            classicCalculator();
        }
        else if((menuOption == 'f') | (menuOption == 'F')) {
            financialCalculator();
        }
        else if((menuOption == 'b') | (menuOption == 'B')) {
            bmiCalculator();
        }
        else if((menuOption == 'q') | (menuOption == 'Q')) {
            cout << "Zakonczenie programu...";
        }
        else {
            printErrorBox("Nieprawidlowa opcja menu...");
            system("pause");
        }

    } while((menuOption != 'q') & (menuOption != 'Q'));

    system("cls");
    printErrorBox("Dziekujemy za skorzystanie z programu...");

    return 0;
}

/***** FUNKCJE I PROCEDURY *****/
void printLogo() {
    cout << endl << "\t" << SYMBOL_TL << string(LOGO_WIDTH, SYMBOL_H) << SYMBOL_TR << endl;
    for(int line = 0; line < LOGO_LENGTH; line++) {
        cout << "\t" << SYMBOL_V << left << setw(LOGO_WIDTH)
        << LOGO_TEXT[line] << SYMBOL_V << endl;
    }
    cout << "\t" << SYMBOL_BL << string(LOGO_WIDTH, SYMBOL_H) << SYMBOL_BR << endl << endl;
    system("pause");
}

void printMainMenu() {
    cout << endl << " Menu: " << endl
    << " - [T] Zmiana motywu" << endl
    << " - [C] Kalkulator klasyczny" << endl
    << " - [F] Kalkulator finansowy" << endl
    << " - [B] Kalkulator BMI" << endl
    << " - [Q] Wyjscie" << endl;
}

void printErrorBox(const string ERROR_MESSAGE) {
    cout << endl << "\a\t" << SYMBOL_TL << string(ERROR_MESSAGE.length(), SYMBOL_H) << SYMBOL_TR << endl
    << "\t" << SYMBOL_V << left << setw(ERROR_MESSAGE.length()) << ERROR_MESSAGE << SYMBOL_V << endl
    << "\t" << SYMBOL_BL << string(ERROR_MESSAGE.length(), SYMBOL_H) << SYMBOL_BR << endl << endl;
}

void switchTheme() {
    if(theme == 'D') {
        system("COLOR 70");
        theme = 'L';
    }
    else {
        system("COLOR 07");
        theme = 'D';
    }
}

long double addNumbers(double x, double y) {
    return x + y;
}

long double subtractNumbers(double x, double y) {
    return x - y;
}

long double multiplyNumbers(double x, double y) {
    return x * y;
}

long double divideNumbers(double x, double y) {
    return x / y;
}

long double exponentiateNumbers(double x, int y) {
    return pow(x, y);
}

long double squareNumbers(double x) {
    return sqrt(x);
}

long long calculateFactorial(int x) {
    if(x == 0) return 1;
    long long result = 1;
    for(int i = 1; i < x + 1; i++) {
        result *= i;
    }
    return result;
}

double calculateBmi(double weight, double height) {
    return weight / ((height / 100) * (height / 100));
}

void printResultTable(const ResultStructure VALUES[], const int ARRAY_SIZE) {
    // Górna ramka
    cout << "\t" << SYMBOL_TL << string(OPERATIONS_WIDTH, SYMBOL_H)
    << SYMBOL_TC << string(RESULTS_WIDTH, SYMBOL_H) << SYMBOL_TR << endl;

    // Nagłówki
    cout << "\t" << SYMBOL_V << left << setw(OPERATIONS_WIDTH) << "Operacje:"
    << SYMBOL_V << left << setw(RESULTS_WIDTH) << "Wyniki:" << SYMBOL_V << endl;

    // Środkowa ramka
    cout << "\t" << SYMBOL_CL << string(OPERATIONS_WIDTH, SYMBOL_H)
    << SYMBOL_CC << string(RESULTS_WIDTH, SYMBOL_H) << SYMBOL_CR << endl;

    //Wiersze danych
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        cout << "\t" << SYMBOL_V << left << setw(OPERATIONS_WIDTH) << VALUES[i].operation
        << SYMBOL_V << left << setw(RESULTS_WIDTH) << setprecision(5) << VALUES[i].result
        << SYMBOL_V << endl;

        if(i < ARRAY_SIZE - 1) {
            cout << "\t" << SYMBOL_CL << string(OPERATIONS_WIDTH, SYMBOL_H)
            << SYMBOL_CC << string(RESULTS_WIDTH, SYMBOL_H) << SYMBOL_CR << endl;
        }
    }

    // Dolna ramka
    cout << "\t" << SYMBOL_BL << string(OPERATIONS_WIDTH, SYMBOL_H)
    << SYMBOL_BC << string(RESULTS_WIDTH, SYMBOL_H) << SYMBOL_BR << endl;
}

void classicCalculator() {
    char key;

    do {
        system("cls");

        cout << "\t* KALKULATOR KLASYCZNY *" << endl
        << " Oblicza wartosci dodawania, odejmowania i wiele innych..."
        << endl << endl;

        cin.clear();
        cout << " Podaj wartosc X: ";
        cin >> x;
        cout << " Podaj wartosc Y: ";
        cin >> y;

        if(cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            printErrorBox("Wprowadzono nieprawidlowe dane...");
        }
        else {
            const ResultStructure VALUES[] = {
                {"X + Y", to_string(addNumbers(x, y))},
                {"X - Y", to_string(subtractNumbers(x, y))},
                {"X * Y", to_string(multiplyNumbers(x, y))},
                {"X / Y", to_string(divideNumbers(x, y))},
                {"X ^ Y", to_string(exponentiateNumbers(x, y))},
                {"/X", to_string(squareNumbers(x))},
                {"X!", to_string(calculateFactorial(x))}
            };

            const int ARRAY_SIZE = sizeof(VALUES) / sizeof(VALUES[0]);
            printResultTable(VALUES, ARRAY_SIZE);
        }

        cout << "\n Czy chcesz ponownie skorzystac z kalkulatora? [T/N] ";
        key = getch();
    } while((key != 'n') & (key != 'N'));
}

void financialCalculator() {
    char key;

    do {
        system("cls");

        cout << "\t* KALKULATOR FINANSOWY *" << endl
        << " Przelicza podana ilosc PLN na przyblizona wartosc w innych walutach..."
        << endl << endl;

        cin.clear();
        cout << " Podaj ilosc PLN: ";
        cin >> x;

        if(cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            printErrorBox("Wprowadzono nieprawidlowe dane...");
        }
        else {
             const ResultStructure VALUES[] = {
                {"EUR", to_string(x / 4.23)},
                {"USD", to_string(x / 3.66)},
                {"GBP", to_string(x / 4.82)},
                {"JPY", to_string(x / 0.0237)}
            };

            const int ARRAY_SIZE = sizeof(VALUES) / sizeof(VALUES[0]);
            printResultTable(VALUES, ARRAY_SIZE);
        }

        cout << "\n Czy chcesz ponownie skorzystac z kalkulatora? [T/N] ";
        key = getch();
    } while((key != 'n') & (key != 'N'));
}

void bmiCalculator() {
    char key;

    do {
        system("cls");

        cout << "\t* KALKULATOR BMI *" << endl
        << " Oblicza wskaznik BMI na podstawie masy ciala i wzrostu..."
        << endl << endl;

        cin.clear();
        cout << " Podaj swoja wage w KG: ";
        cin >> x;
        cout << " Podaj swoj wzrost w CM: ";
        cin >> y;

        if(cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            printErrorBox("Wprowadzono nieprawidlowe dane...");
        }
        else if(x < 30 || x > 500) {
            printErrorBox("Podana waga jest nieprawidlowa...");
        }
        else if(y < 50 || y > 250) {
            printErrorBox("Podany wzrost jest nieprawidlowy...");
        }
        else {
            const double BMI = calculateBmi(x, y);

            const ResultStructure VALUES[] = {
                {"Waga [KG]", to_string(x)},
                {"Wzrost [M]", to_string(y / 100)},
                {"BMI", to_string(BMI)}
            };

            const int ARRAY_SIZE = sizeof(VALUES) / sizeof(VALUES[0]);
            printResultTable(VALUES, ARRAY_SIZE);
            cout << defineBmi(BMI);
        }

        cout << "\n Czy chcesz ponownie skorzystac z kalkulatora? [T/N] ";
        key = getch();
    } while((key != 'n') & (key != 'N'));
}

string defineBmi(const double BMI) {
    string result = "\n\n Twoje BMI wskazuje na:\n";

    if(BMI < 16) result += " * WYGLODZENIE";
    else if(BMI >= 16 && BMI < 17) result += " * WYCHUDZENIE";
    else if(BMI >= 17 && BMI < 18.5) result += " * NIEDOWAGE";
    else if(BMI >= 18.5 && BMI < 25) result += " * WAGE PRAWIDLOWA";
    else if(BMI >= 25 && BMI < 30) result += " * NADWAGE";
    else if(BMI >= 30 && BMI < 35) result += " * OTYLOSC I STOPNIA";
    else if(BMI >= 35 && BMI < 40) result += " * OTYLOSC II STOPNIA";
    else result += " * OTYLOSC III STOPNIA";

    result += "\n";
    return result;
}
