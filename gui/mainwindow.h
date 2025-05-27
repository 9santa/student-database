#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class StudentGroup {
public:
    StudentGroup(const QString &name, int year, QChar degree,
                 const QString &faculty, const QString &captain, int studentCount)
        : m_name(name), m_year(year), m_degree(degree),
        m_faculty(faculty), m_captain(captain), m_studentCount(studentCount) {}

    QString name() const { return m_name; }
    int year() const { return m_year; }
    QChar degree() const { return m_degree; }
    QString faculty() const { return m_faculty; }
    QString captain() const { return m_captain; }
    int studentCount() const { return m_studentCount; }

    void setName(const QString &name) { m_name = name; }
    void setYear(int year) { m_year = year; }
    void setDegree(QChar degree) { m_degree = degree; }
    void setFaculty(const QString &faculty) { m_faculty = faculty; }
    void setCaptain(const QString &captain) { m_captain = captain; }
    void setStudentCount(int count) { m_studentCount = count; }

    QString toString() const {
        return QString("%1;%2;%3;%4;%5;%6")
        .arg(m_name).arg(m_year).arg(m_degree)
            .arg(m_faculty).arg(m_captain).arg(m_studentCount);
    }

private:
    QString m_name;
    int m_year;
    QChar m_degree;
    QString m_faculty;
    QString m_captain;
    int m_studentCount;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadButtonClicked();
    void saveButtonClicked();
    void addButtonClicked();
    void deleteButtonClicked();
    void searchButtonClicked();
    void sortButtonClicked();
    void filterButtonClicked();
    void facultyButtonClicked();
    void splitButtonClicked();
    void showSeperatedGroups(const QVector<StudentGroup> &groups, const QString &title);

    void updateTable();

private:
    Ui::MainWindow *ui;
    QVector<StudentGroup> m_groups;
    QVector<StudentGroup> m_bachelorGroups;
    QVector<StudentGroup> m_specialistGroups;
    QVector<StudentGroup> m_masterGroups;
};
#endif // MAINWINDOW_H
