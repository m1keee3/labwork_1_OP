#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// Функция преобразующая входную строку в вектор деля строку по пробелам

vector<string> Split(string input) {
    vector<string> splitedWords;
    string presentWord;
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == ' ') {
            splitedWords.push_back(presentWord);
            presentWord = "";
        }
        else {
            presentWord += input[i];
        }
    }
    splitedWords.push_back(presentWord);
    return splitedWords;
}

//Проверяем наличие символа по которому определяется конец строки

char CheckDelimiter(vector<string> commands) {
    for (int i = 1; i < commands.size() - 1; i++) {
        if (commands[i] == "-d") {
            return commands[i + 1][0];
        }
        if (commands[i].substr(0,11) == "--delimiter") {
            return commands[i][12];
        }
    }
    return '\n';
}

//Проверяем какое количество первых строк нужно вывести

int GetLinesNumber(vector<string> commands) {
    for (int i = 0; i < commands.size() - 1; i++) {
        if (commands[i] == "-l") {
            if (commands[i + 1] == "-t") {
                return stoi(commands[i + 2]);
            }
            else {
                return stoi(commands[i + 1]);
            }
        }
        if (commands[i].substr(0, 7) == "--lines") {
            return stoi(commands[i].substr(8));
        }
    }
    return 0;
}

//Проверяем какое количество последних строк нужно вывести

int GetTailNumber(vector<string> commands) {
    for (int i = 0; i < commands.size() - 1; i++) {
        if (commands[i] == "-t") {
            if (commands[i + 1] == "-l") {
                return stoi(commands[i + 2]);
            }
            else {
                return stoi(commands[i + 1]);
            }
        }
        if (commands[i].substr(0, 6) == "--tail") {
            if (commands[i + 1].substr(0, 7) == "--lines") {
                return stoi(commands[i + 1].substr(8));
            }
            if (commands[i - 1].substr(0, 7) == "--lines") {
                return stoi(commands[i - 1].substr(8));
            }
            else {
                return stoi(commands[i].substr(7));
            }
        }
    }
    return 0;
}

int main() {
    string input;
    getline(cin,input);
    vector<string> commands = Split(input); // Делим входную строку на команды

    string fileName = commands[commands.size() - 1]; //Достаем имя файла

    // Открываем файл
    ifstream file;
    file.open(fileName);
    if (!file.is_open()) {
        cout << "This file was not found";
        return 0;
    }

    char delimiter = CheckDelimiter(commands);
    int linesNumber = GetLinesNumber(commands);
    int tailNumber = GetTailNumber(commands);
    if (tailNumber == 0 && linesNumber == 0) {
        linesNumber = 1000000000;
    }

    file.seekg(0, std::ifstream::end);
    long long fileLength = file.tellg();
    file.seekg(0, std::ifstream::beg);

    //Выводим первые строки
    char output;
    int linesCounter;
    for (long long i = 0; i < fileLength; i++) {
        file.get(output);
        if (linesCounter < linesNumber) {
            if (output == delimiter) {
                linesCounter++;
                i++;
                cout << '\n';
            }
            else {
                cout << output;
            }
        }
        else {
            if (output == delimiter) {
                linesCounter++;
                i++;
            }
        }
    }
    linesCounter++;

    //Выводим последние строки
    file.seekg(0, std::ifstream::beg);
    int ctr = 0;
    for (int i = 0; i < fileLength; i++) {
        file.get(output);

        if (ctr == (linesCounter - tailNumber)) {
            if (output == delimiter) {
                cout << '\n';
                i++;
            }
            else {
                cout << output;
            }
        }
        else if (output == delimiter) {
            ctr++;
            i++;
        }
    }
    file.close();
}
