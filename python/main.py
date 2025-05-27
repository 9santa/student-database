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
    def print():
        #displayFaculty = faculty[:39]
        #if len(faculty) > 25: displayFaculty += "..."
        print(f"| {name:<14} | {year:<10} | {faculty:<40} | {captain:<50} | {studentCount:<8} |")

    def saveToFile(self, output_file):
        output_file.write(
            f"{self.name},{self.year},{self.degree},{self.faculty},{self.captain},{self.student_count}\n")


    # Статический метод для парсинга и загрузки БД из файла
    @staticmethod
    def loadFromFile(input_file):
        line = input_file.readline().strip()
        if not line: raise EOFError("Файл закончен или строка пуста")

        cells = line.split(',')
        if len(cells) !+ 6:
            raise ValueError("Некорректный формат строки в файле!")

        return StudentGroup(
            cells[0],           # name
            int(cells[1]),      # year
            cells[2][0],        # degree (first character)
            cells[3],           # faculty
            cells[4],           # captain
            int(cells[5]))      # studentCount
        )


class Database:
    def __init__(self):
        self.groups = []

    def addGroup():
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




