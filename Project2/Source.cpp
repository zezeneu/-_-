#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <clocale>
#include <windows.h>
#include <regex>

using namespace std;

class Lesson {
public:
    string date;
    string time;
    string teacher;

    void Print() const {
        cout << setw(15) << left << date
            << setw(10) << left << time
            << setw(30) << left << teacher << endl;
    }
};

void PrintHeader() {
    cout << "\n==============================================================" << endl;
    cout << setw(15) << left << "Дата"
        << setw(10) << left << "Время"
        << setw(30) << left << "Преподаватель" << endl;
    cout << "==============================================================" << endl;
}

bool IsValidDate(const string& date) {
    regex datePattern(R"(\d{4}\.\d{2}\.\d{2})");
    return regex_match(date, datePattern);
}

bool IsValidTime(const string& time) {
    regex timePattern(R"(\d{2}:\d{2})");
    return regex_match(time, timePattern);
}

Lesson ParseLesson(const string& line) {
    Lesson lesson;
    stringstream ss(line);

    ss >> lesson.date >> lesson.time;

    string teacherName;
    getline(ss, teacherName);

    size_t start = teacherName.find_first_not_of(" \t");
    if (start != string::npos) {
        teacherName = teacherName.substr(start);
    }
    size_t end = teacherName.find_last_not_of(" \t");
    if (end != string::npos) {
        teacherName = teacherName.substr(0, end + 1);
    }

    lesson.teacher = teacherName;
    return lesson;
}

vector<Lesson> ReadLessonsFromFile(const string& filename) {
    vector<Lesson> lessons;
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл " << filename << endl;
        return lessons;
    }

    string line;
    int lineNumber = 0;

    while (getline(inputFile, line)) {
        lineNumber++;
        if (line.empty()) continue;

        Lesson l = ParseLesson(line);

        if (IsValidDate(l.date) && IsValidTime(l.time)) {
            lessons.push_back(l);
        }
        else {
            cerr << "Предупреждение: Неверный формат в строке " << lineNumber
                << ": " << line << endl;
        }
    }

    inputFile.close();
    return lessons;
}

void PrintLessons(const vector<Lesson>& lessons) {
    if (lessons.empty()) {
        cout << "Нет данных для отображения." << endl;
        return;
    }

    PrintHeader();
    for (const auto& lesson : lessons) {
        lesson.Print();
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    cout << "Чтение данных из файла text.txt..." << endl;

    vector<Lesson> lessons = ReadLessonsFromFile("text.txt");

    cout << "\nСписок учебных занятий:" << endl;
    PrintLessons(lessons);

    return 0;
}