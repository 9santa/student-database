# Python Version

class StudentGroup:
    # Constructor
    def __init__(self, name, year, degree, faculty, captain, studentCount):
        self.name = name
        self.year = year
        self.degree = degree
        self.faculty = faculty
        self.captain = captain
        self.studentCount = studentCount

    # Вывод информации о группе
    def print(self):
        #displayFaculty = faculty[:39]
        #if len(faculty) > 25: displayFaculty += "..."
        print(f"| {self.name:<10} | {self.year:<6} | {self.faculty:<30} | {self.captain:<30} | {self.studentCount:<8} |")

    def saveToFile(self, output_file):
        output_file.write(
            f"{self.name},{self.year},{self.degree},{self.faculty},{self.captain},{self.studentCount}\n")


    # Статический метод для парсинга и загрузки БД из файла
    @staticmethod
    def loadFromFile(line):
        if not line: raise EOFError("Файл закончен или строка пуста")

        cells = line.split(',')
        if len(cells) != 6:
            raise ValueError("Некорректный формат строки в файле!")

        return StudentGroup(
            cells[0],           # name
            int(cells[1]),      # year
            cells[2][0],        # degree (first character)
            cells[3],           # faculty
            cells[4],           # captain
            int(cells[5]))      # studentCount
        


class Database:
    def __init__(self):
        self.groups = []

    def addGroup(self):
        print("Введите данные группы для добавления:\n")
        name = input("Название группы: ")
        year = int(input("Год набора: "))
        degree = input("Степень обучения (Eng char = B/C/M): ").upper()[0]
        faculty = input("Факультет: ")
        captain = input("Староста: ")
        count = int(input("Количество студентов: "))

        group = StudentGroup(name, year, degree, faculty, captain, count)
        self.groups.append(group)
        print("\nГруппа успешно добавлена!\n")

    def deleteGroup(self, index):
        if index < 0 or index >= len(self.groups):
            raise ValueError("Введен неверный индекс!\n")
            return

        del self.groups[index]
        print("Группа успешно удалена.\n")

    def view(self):
        if not self.groups:
            print("База данных абсолютно пуста.\n")
            return

        header = "| {:<10} | {:<6} | {:<30} | {:<30} | {:<8} |".format(
        "Название", "Год", "Факультет", "Староста", "Кол-во")

        print("\n" + header)
        print("-" * len(header))

        for group in self.groups:
            group.print()


    def searchByName(self, name):
        found = False
        for group in self.groups:
            if group.name == name:
                found = True
                print("Группа найдена!\n")
                group.print()
                break
        if not found:
            print("Группы с таким названием не существует в базе.\n")


    def sortByName(self):
        self.groups.sort(key=lambda g: g.name)
        print("Сортировка по названию успешна.\n")

    
    def loadFromFile(self, filename):
        try:
            with open(filename, 'r', encoding='utf-8') as File:
                header = File.readline()
                self.groups.clear()

                for line in File:
                    line = line.strip()
                    if not line:
                        continue # Пропуск пустых строк

                    group = StudentGroup.loadFromFile(line)
                    self.groups.append(group)

                print(f"База данных успешно загружена. Записей: {len(self.groups)}")
        except IOError:
            print("Ошибка открытия файла для чтения!\n")

    def saveToFile(self, filename):
        try:
            with open(filename, 'w') as File:
                for group in self.groups:
                    group.saveToFile(File)

                print(f"База данных успешно сохранена. Записей: {len(self.groups)}")
        except IOError:
            print("Ошибка при открытия файла для записи!\n")


    def filterYear(self, start, end):
        filtered = [group for group in self.groups
                    if start <= group.year <= end]
        if not filtered:
            print(f"Группы в диапазоне {start}-{end} не найдены.\n")
            return
        
        print(f"Группы в диапазоне {start}-{end}:\n")
        for group in filtered:
            group.print()


    def printFacultyByCount(self, faculty):
        filtered = [group for group in self.groups
                    if group.faculty == faculty]
        if not filtered:
            print(f"Группы факультета {faculty} не найдены.\n")
            return

        filtered.sort(key=lambda g: g.studentCount)

        print(f"Группы факультета {faculty} упорядоченные по численности\n")
        for group in filtered:
            group.print()


    def createSeperateDBs(self, bachelorDB, specialistDB, masterDB):
        for g in self.groups:
            if g.degree == 'B':
                bachelorDB.groups.append(g)
            elif g.degree == 'C':
                specialistDB.groups.append(g)
            elif g.degree == 'M':
                masterDB.groups.append(g)

            self.groups.clear()
            print("База данных разделена по типам обучения.\n")


def printMenu():
    print("\n1. Загрузить БД из файла")
    print("2. Просмотр БД")
    print("3. Добавить группу")
    print("4. Удалить группу")
    print("5. Сохранит БД в файл")
    print("6. Сортировать БД по названию")
    print("7. Поиск группы по названию")
    print("8. Отфильтровать БД по году (диапазон)")
    print("9. Вывести группы факультета, отсортированные по численности")
    print("10. Разделить БД по типам обучения")
    print("0. Выход\n")


def main():
    db = Database()
    bachelorDB = Database()
    specialistDB = Database()
    masterDB = Database()

    flag = True
    while flag:
        printMenu()
        choice = int(input())

        if choice == 1:
            file = input("Введите имя файла: ")
            db.loadFromFile(file)

        elif choice == 2:
            db.view()

        elif choice == 3:
            db.addGroup()

        elif choice == 4:
            index = int(input("Введите порядковый номер группы для удаления: "))
            db.deleteGroup(index)

        elif choice == 5:
            file = input("Введите имя файла: ")
            db.saveToFile(file)

        elif choice == 6:
            db.sortByName()

        elif choice == 7:
            name = input("Введите название группы для поиска: ")
            db.searchByName(name)

        elif choice == 8:
            start = int(input("Введите начальный год: "))
            end = int(input("Введите конечный год: "))
            db.filterYear(start, end)

        elif choice == 9:
            faculty = input("Введите названиe факультета: ")
            db.printFacultyByCount(faculty)

        elif choice == 10:
            db.createSeperateDBs(bachelorDB, specialistDB, masterDB)
            print(f"Бакалавриат ({len(bachelorDB.groups)} групп)\n")
            print(f"Специалитет ({len(specialistDB.groups)} групп)\n")
            print(f"Магистратура ({len(masterDB.groups)} групп)\n")

        elif choice == 0:
            print("Выход из программы.\n")
            flag = False

        else:
            print("Неверный выбор. Попробуйте снова.\n")


if __name__ == "__main__":
    main()













