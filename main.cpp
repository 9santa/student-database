#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <format>
#include <limits>

using namespace std;


// Класс записи одной группы (ячейки) в БД
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

        // SETTERS
        void setName(string n) { name = n; }
        void setYear(int y) { year = y; }
        void setDegree(char deg) { degree = deg; }
        void setFaculty(string fac) { faculty = fac; }
        void setCaptain(string cap) { captain = cap; }
        void setStudentCount(int count) { studentCount = count; }

        // вывод информации о группе (записи)
        void print() const {
            string displayFaculty = faculty.substr(0,39);
            if(faculty.length() > 25) displayFaculty += "...";
            cout << left
                 << "| " << setw(14) << name
                 << " | " << setw(10) << year
                 << " | " << setw(40) << faculty
                 << " | " << setw(50) << captain
                 << " | " << setw(8) << studentCount << '\n';
        }

        // Сохранение одной записи в файл
        void saveToFile(ofstream &output) const {
            output << name << ',' << year << ',' << degree << ',' << faculty << ','
                << captain << ',' << studentCount << '\n';
        }

        // Статический метод для парсинга и загрузки строки из файла в БД
        static StudentGroup loadFromFile(ifstream &input){
            string line;
            getline(input, line);

            stringstream ss(line);
            vector<string> cells;
            string cell;

            while(getline(ss, cell, ',')){
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

// Основной класс для управления БД
class Database {
    private:
        vector<StudentGroup> groups;

    public:

        // GETTERS
        int getGroupCount() const { return groups.size(); }

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

            cout << "Степень обучения (eng char = B/C/M): "; cin >> degree;

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

        // Вывод БД в консоль
        void view() const {
            if(groups.empty()){
                cout << "База данных абсолютно пуста.\n";
                return;
            }

            cout << left
                 << setw(14) << "\nНазвание"
                 << " | " << setw(10) << "Год"
                 << " | " << setw(50) << "Факультет"
                 << " | " << setw(40) << "Староста"
                 << " | " << setw(8) << "Кол-во\n";
            cout << string(112, '-') << '\n'; // разделитель

            for(const auto &group : groups){
                group.print();
            }
        }


        // Поиск записи по имени (идентификатору)
        void searchByName(const string &name){
            bool found = false;
            for(const auto &group : groups){
                if(group.getName() == name){
                    found = true;
                    cout << "Группа найдена!\n";
                    group.print();
                    break;
                }
            }
            if(!found) {
                cout << "Группы с таким названием не существует в базе.\n";
            }
        }
        
        // Сортировка по названию
        void sortByName() {
            sort(groups.begin(), groups.end(), [](const StudentGroup &a, const StudentGroup &b){
                    return a.getName() < b.getName();
                    });
            cout << "Сортировка по названию успешна." << endl;
        }

        // Загрузка БД из файла
        void loadFromFile(const string &filename){
            ifstream File(filename);

            if(!File){
                cerr << "Ошибка открытия файла для чтения!" << endl;
                return;
            }

            string header;
            getline(File, header);

            // Очищаем все предыдущие записи
            groups.clear();

            while(File.peek() != EOF) {
                groups.push_back(StudentGroup::loadFromFile(File));
            }
            File.close();
            cout << "База данных успешно загружена. Записей: " << groups.size() << endl;
        }

        // Сохранить БД в файл
        void saveToFile(const string &filename) const{
            ofstream File(filename);
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

        // Фильр по году (диапазон)
        void filterYear(int start, int end){
            auto filtered = count_if(groups.begin(), groups.end(),
                    [start, end](const StudentGroup &g){
                    return g.getYear() >= start && g.getYear() <= end;
                    });

            if(filtered == 0){
                cout << "Группы в диапазоне " << start << "-" << end << " не найдены.\n";
                return;
            }
            
            cout << "Группы в диапазоне " << start << "-" << end << ":\n";
            for_each(groups.begin(), groups.end(),
                    [start, end](const StudentGroup &g){
                    if(g.getYear() >= start && g.getYear() <= end){
                        g.print();
                    }
                });
        }

        // Кол-во групп по факультету
        void printFacultyByCount(const string &faculty){
            vector<StudentGroup> filtered;
            for(const auto &g : groups){
                if(g.getFaculty() == faculty) filtered.push_back(g);
            }

            if(filtered.empty()){
               cout << "Группы факультета " << faculty << " не найдены." << endl;
               return;

            sort(filtered.begin(), filtered.end(), [](const StudentGroup &a, const StudentGroup &b){
                    return a.getStudentCount() < b.getStudentCount();
                    });
            
            }
            cout << "Группы факультета " << faculty << "упорядеченные по численности\n";
            for(const auto &g : filtered){
                g.print();
            }
        }

        // Разделяем на три БД по степени обучения
        void createSeperateDBs(Database &bachelorDB, Database &specialistDB, Database &masterDB){
            for(const auto &g : groups){
                switch(g.getDegree()){
                    case 'B':
                        bachelorDB.groups.push_back(g);
                        break;
                    case 'C':
                        specialistDB.groups.push_back(g);
                        break;
                    case 'M':
                        masterDB.groups.push_back(g);
                        break;
                }
            }
            // Очищаем старую DB
            groups.clear();
            cout << "База данных разделена по типам обучения." << endl;
        }
};

// print menu to the console
void printMenu() {
    cout << "\n1. Загрузить БД из файла\n";
    cout << "2. Просмотр БД\n";
    cout << "3. Добавить группу\n";
    cout << "4. Удалить группу\n";
    cout << "5. Сохранить БД в файл\n";
    cout << "6. Сортировать БД по названию\n";
    cout << "7. Поиск группы по названию\n";
    cout << "8. Отфильтровать БД по году (диапазон)\n";
    cout << "9. Вывести группы факультета, отсортированные по численности\n";
    cout << "10. Создать отдельные БД по типам обучения\n";
    cout << "0. Выход\n";
}

int main()
{
    
    clock_t z = clock();

    // Создание основной БД (db)
    Database db;
    // Создание БД по степени обучения
    Database bachelorDB, specialistDB, masterDB;

    string filename;
    int choice;

    // Главный цикл программы, отвечающий за меню и выбор пользователя
    do{
        printMenu();
        cin >> choice;
        cin.ignore();

        string searchName, faculty;

        switch(choice){
            case 1:
                cout << "Введите имя файла: ";
                getline(cin, filename);
                db.loadFromFile(filename);
                break;

            case 2:
                db.view();
                break;

            case 3:
                db.addGroup();
                break;

            case 4:
                int index;
                cout << "Введите порядковый номер группы для удаления: ";
                cin >> index;
                db.deleteGroup(index);
                break;

            case 5:
                cout << "Введите имя файла: ";
                getline(cin, filename);
                db.saveToFile(filename);
                break;

            case 6:
                db.sortByName();
                break;

            case 7:
                cout << "Введите название группы для поиска: ";
                getline(cin, searchName);
                db.searchByName(searchName);
                break;

            case 8:
                int start, end;
                cout << "Введите начальный год: ";
                cin >> start;
                cout << "Введите конечный год: ";
                cin >> end;
                db.filterYear(start, end);
                break;

            case 9:
                cout << "Введите название факультета: ";
                getline(cin, faculty);
                db.printFacultyByCount(faculty);
                break;

            case 10:
                db.createSeperateDBs(bachelorDB, specialistDB, masterDB);
                cout << "Бакалавриват (" << bachelorDB.getGroupCount() << " групп)\n";
                cout << "Специалитет (" << specialistDB.getGroupCount() << " групп)\n";
                cout << "Магистратура (" << masterDB.getGroupCount() << " групп)\n";
                break;

            case 0:
                cout << "Выход из программы.\n";
                break;

            default:
                cout << "Неверный выбор. Попробуйте снова.\n";
        }
    } while(choice != 0);

    
    cerr << "\nRun Time : " << ((double)(clock() - z) / CLOCKS_PER_SEC) << '\n';
    

    return 0;
}

