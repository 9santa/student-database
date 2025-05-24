#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <format>
#include <limits>


using namespace std;

using ll = long long;

#define F first
#define S second
#define pb push_back
#define all(x) x.begin(), x.end()
#define debug(x) cout << __FUNCTION__ << ":" << __LINE__ << " " << #x << " = " << x << endl

const int MOD = 998244353;
const double PI = 3.141592653589793;
bool flag = 0;

class StudentGroup {
    private:
        string name;        // Наименование *
        int year;           // Год набора **
        char degree;        // B (Bаклавриат) / С (Специалитет) / М (Магистратура)
        string faculty;     // Факультет
        string captain;     // Староста
        int studentCount;   // Кол-во человек

    public:
        // CONSTRUCTOR
        StudentGroup(const string &name, const int &year, const char &degree, const string &faculty,
            const string &captain, const int &studentCount) : name(name), year(year), degree(degree),
            faculty(faculty), captain(captain), studentCount(studentCount) {}

        // GETTERS
        string getName() const { return name; }
        int getYear() const { return year; }
        char getDegree() const { return degree; }
        string getFaculty() const { return faculty; }
        string getCaptain() const { return captain; }
        int getStudentCount() const { return studentCount; }

        // GETTERS
        void setName(string n) { name = n; }
        void setYear(int y) { year = y; }
        void setDegree(char deg) { degree = deg; }
        void setFaculty(string fac) { faculty = fac; }
        void setCaptain(string cap) { captain = cap; }
        void setStudentCount(int count) { studentCount = count; }

        // вывод информации о группе (записи)
        void print() const {
            cout << setw(13) << left << name
                 << setw(7) << left << year
                 << setw(23) << left << faculty
                 << setw(30) << left << captain
                 << setw(13) << right << studentCount << '\n';
        }

        void saveToFile(ofstream &output) const {
            output << name << ';' << year << ';' << degree << ';' << faculty << ';'
                << captain << ';' << studentCount << '\n';
        }

        static StudentGroup loadFromFile(ifstream &input){
            string line;
            getline(input, line);

            stringstream ss(line);
            vector<string> cells;
            string cell;

            while(getline(ss, cell, ';')){
                cells.push_back(cell);
            }

            if(cells.size() != 6){
                throw runtime_error("Некорректный формат строки в файле!\n");
            }

            return StudentGroup(
                    cells[0],           // name
                    stoi(cells[1]),     // year
                    cells[2][0],        // degree (first character)
                    cells[3],           // faculty
                    cells[4],           // captain
                    stoi(cells[5]));    // studentCount

        }
};

class Database {
    private:
        vector<StudentGroup> groups;

    public:
        // добавление группы
        void addGroup() {
            string name, faculty, captain;
            int year, count;
            char degree;

            cout << "Введите данные группы для добавления:" << endl;

            cout << "Название группы: ";
            cin.ignore();
            getline(cin, name);

            cout << "Год набора: "; cin >> year;

            cout << "Степень обучения (char = B/C/M): "; cin >> degree;

            cout << "Факультет: ";
            cin.ignore(); // пропускаем символ новой строки в буфере
            getline(cin, faculty);
            
            cout << "Староста: "; getline(cin, captain);
            cout << "Количество студентов: "; cin >> count;

            groups.emplace_back(name, year, degree, faculty, captain, count);
            cout << "\nГруппа успешно добавлена!\n";
            
        }
        
        // удаление группы (по номеру)
        void deleteGroup(int index) {
            // проверка правильности индекса
            if(index < 0 || index >= groups.size()) {
                cerr << "Введен неверный индекс!\n";
                return;
            }
            
            groups.erase(groups.begin() + index);
            cout << "Группа успешно удалена.\n";
        }

        void view() const {
            if(groups.empty()){
                cout << "База данных абсолютно пуста.\n";
                return;
            }

            cout << setw(20) << left << "\nНазвание"
                 << setw(10) << left << "Год"
                 << setw(23) << left << "Факультет"
                 << setw(30) << left << "Староста"
                 << setw(10) << left << "Кол-во\n";
            cout << string(76, '-') << '\n'; // разделитель

            for(const auto &group : groups){
                group.print();
            }
        }



        void searchByName(const string &name){
            bool found = false;
            for(const auto &group : groups){
                if(group.getName() == name){
                    found = true;
                    cout << "Группа найдена!\n";
                    group.print();
                }
            }
            if(!found) {
                cout << "Группы с таким названием не существует в базе.\n";
            }
        }

        // Загрузка из файла
        void loadFromFile(const string &filename){
            ifstream File(filename);
            if(!File){
                cerr << "Ошибка открытия файла для чтения!" << endl;
                return;
            }

            // Очищаем все предыдущие записи
            groups.clear();

            while(File.peek() != EOF) {
                groups.push_back(StudentGroup::loadFromFile(File));
            }
            File.close();
            cout << "База данных успешно загружена. Записей: " << groups.size() << endl;
        }

        void saveToFile(const string &File) const{
            ofstream File(File);
            if(!File){
                cerr << "Ошибка открытия файла для записи!" << endl;
                return;
            }

            for(const auto &group : groups){
                group.saveToFile(File);
            }

            File.close();
            cout << "База данных сохранена. Записей: " << groups.size() << endl;
        }
};

// print menu to the console
void printMenu() {
    cout << "1. Загрузить БД из файла\n";
    cout << "2. Просмотр БД\n";
    cout << "3. Добавить группу\n";
    cout << "4. Удалить группу\n";
    cout << "5. Сохранить БД в файл\n";
    cout << "6. Сортировать БД по названию\n";
    cout << "7. Поиск группы по названию\n";
    cout << "8. Вывести группы факультета по численности\n";
    cout << "9. Создать отдельные БД по типам обучения\n";
    cout << "0. Выход\n";
}


void cohle(ifstream &input, ofstream &output)
{

}

int main()
{
    /*
    ifstream input("input.txt");
    ofstream output("output.txt");
    if(!input.is_open() || !output.is_open()){
        cerr << "Ошибка открытия файла\n";
        return 1;
    }
    */
    
    //ios::sync_with_stdio(0);
    //cin.tie(0);
    //cout.tie(0);

    clock_t z = clock();

    Database db;

    db.addGroup();
    db.view();

    
    //printMenu();
    //cohle(input, output);

    //input.close();
    //output.close();

    cerr << "\nRun Time : " << ((double)(clock() - z) / CLOCKS_PER_SEC) << '\n';
    

    return 0;
}

