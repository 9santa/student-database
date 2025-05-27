#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMetaObject>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set up table columns
    ui->tableWidget->setColumnCount(6);
    QStringList headers = {"Name", "Year", "Degree", "Faculty", "Captain", "Students"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Connect buttons
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::loadButtonClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::saveButtonClicked);
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::addButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::deleteButtonClicked);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::searchButtonClicked);
    connect(ui->sortButton, &QPushButton::clicked, this, &MainWindow::sortButtonClicked);
    connect(ui->filterButton, &QPushButton::clicked, this, &MainWindow::filterButtonClicked);
    connect(ui->facultyButton, &QPushButton::clicked, this, &MainWindow::facultyButtonClicked);
    connect(ui->splitButton, &QPushButton::clicked, this, &MainWindow::splitButtonClicked);

    // Set icons to buttons
    ui->loadButton->setIcon(QIcon::fromTheme("document-open"));
    ui->loadButton->setIconSize(QSize(32,32));
    ui->saveButton->setIcon(QIcon::fromTheme("document-save"));
    ui->saveButton->setIconSize(QSize(32,32));
    ui->searchButton->setIcon(style()->standardIcon(QStyle::SP_FileDialogListView));
    ui->searchButton->setIconSize(QSize(16, 16));
    ui->deleteButton->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
    ui->deleteButton->setIconSize(QSize(16, 16));
    ui->sortButton->setIcon((style()->standardIcon(QStyle::SP_ArrowDown)));
    ui->sortButton->setIconSize(QSize(32,32));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTable()
{
    ui->tableWidget->setRowCount(m_groups.size());

    for (int i = 0; i < m_groups.size(); ++i) {
        const StudentGroup &group = m_groups[i];

        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(group.name()));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(group.year())));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(group.degree()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(group.faculty()));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(group.captain()));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(group.studentCount())));
    }

    ui->statusLabel->setText(QString("Total groups: %1").arg(m_groups.size()));
}

void MainWindow::loadButtonClicked()
{
    ui->loadButton->setEnabled(false);

    QString filename = QFileDialog::getOpenFileName(
        this,                   // Parent widget
        tr("Open Database"),    // Dialog title
        QDir::homePath(),       // Starting directory
        tr("CSV Files (*.csv);;All Files (*)") // File filters
        );

    if (filename.isEmpty()){
        ui->loadButton->setEnabled(true);
        return;
    }

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
        ui->loadButton->setEnabled(true);
        return;
    }

    // clear existing database
    m_groups.clear();

    QTextStream in(&file);
    bool firstLine = true;

    while (!in.atEnd()) {
        QString line = in.readLine();

        // Skip empty lines
        if (line.trimmed().isEmpty()) continue;

        // Handle CSV format (either comma or semicolon separated)
        QStringList parts;
        if (line.contains(';')) {
            parts = line.split(';');
        } else {
            parts = line.split(',');
        }

        // Skip header row if it exists
        if (firstLine && (parts.contains("Name") || parts.contains("Year"))) {
            firstLine = false;
            continue;
        }

        if (parts.size() >= 6) {
            try {
                StudentGroup group(
                    parts[0].trimmed(),
                    parts[1].trimmed().toInt(),
                    parts[2].trimmed()[0],
                    parts[3].trimmed(),
                    parts[4].trimmed(),
                    parts[5].trimmed().toInt()
                    );
                m_groups.append(group);
            } catch (...) {
                qDebug() << "Error parsing line:" << line;
            }
        } else {
            qDebug() << "Skipping malformed line:" << line;
        }
        firstLine = false;
    }

    file.close();
    updateTable();

    // Re-enable the button after comletion
    ui->loadButton->setEnabled(true);

    QMessageBox::information(
        this,
        tr("Success"),
        tr("Loaded %1 groups").arg(m_groups.size())
        );
}

void MainWindow::saveButtonClicked()
{
    if (m_groups.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Database is empty");
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, "Save File", "", "CSV Files (*.csv);;All Files (*)");
    if (filename.isEmpty()) return;

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not save file");
        return;
    }

    QTextStream out(&file);
    for (const StudentGroup &group : m_groups) {
        out << group.toString() << "\n";
    }

    file.close();
    QMessageBox::information(this, "Success", "Database saved successfully");
}

void MainWindow::addButtonClicked()
{
    QString name = ui->nameEdit->text();
    int year = ui->yearSpin->value();
    QChar degree = ui->degreeCombo->currentText()[0];
    QString faculty = ui->facultyEdit->text();
    QString captain = ui->captainEdit->text();
    int count = ui->countSpin->value();

    if (name.isEmpty() || faculty.isEmpty() || captain.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please fill all fields");
        return;
    }

    StudentGroup group(name, year, degree, faculty, captain, count);
    m_groups.append(group);

    // Clear input fields
    ui->nameEdit->clear();
    ui->facultyEdit->clear();
    ui->captainEdit->clear();

    updateTable();
    QMessageBox::information(this, "Success", "Group added successfully");
}

void MainWindow::deleteButtonClicked()
{
    int row = ui->tableWidget->currentRow();
    if (row < 0 || row >= m_groups.size()) {
        QMessageBox::warning(this, "Warning", "Please select a group to delete");
        return;
    }

    m_groups.remove(row);
    updateTable();
    QMessageBox::information(this, "Success", "Group deleted successfully");
}

void MainWindow::searchButtonClicked()
{
    QString name = ui->searchEdit->text();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter a name to search");
        return;
    }

    bool found = false;
    for (int i = 0; i < m_groups.size(); ++i) {
        if (m_groups[i].name().contains(name, Qt::CaseInsensitive)) {
            ui->tableWidget->selectRow(i);
            found = true;
            break;
        }
    }

    if (!found) {
        QMessageBox::information(this, "Not Found", "No group with that name was found");
    }
}

void MainWindow::sortButtonClicked()
{
    std::sort(m_groups.begin(), m_groups.end(), [](const StudentGroup &a, const StudentGroup &b) {
        return a.name() < b.name();
    });

    updateTable();
    QMessageBox::information(this, "Success", "Database sorted by name");
}

void MainWindow::filterButtonClicked()
{
    int start = ui->startYearSpin->value();
    int end = ui->endYearSpin->value();

    if (start > end) {
        QMessageBox::warning(this, "Warning", "Start year must be less than or equal to end year");
        return;
    }

    QVector<StudentGroup> filtered;
    for (const StudentGroup &group : m_groups) {
        if (group.year() >= start && group.year() <= end) {
            filtered.append(group);
        }
    }

    if (filtered.isEmpty()) {
        QMessageBox::information(this, "No Results", "No groups found in this year range");
        return;
    }

    // Create a temporary table to display results
    QDialog *dialog = new QDialog(this);
    QTableWidget *table = new QTableWidget(dialog);
    table->setColumnCount(6);
    table->setRowCount(filtered.size());
    QStringList headers = {"Name", "Year", "Degree", "Faculty", "Captain", "Students"};
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int i = 0; i < filtered.size(); ++i) {
        const StudentGroup &group = filtered[i];
        table->setItem(i, 0, new QTableWidgetItem(group.name()));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(group.year())));
        table->setItem(i, 2, new QTableWidgetItem(group.degree()));
        table->setItem(i, 3, new QTableWidgetItem(group.faculty()));
        table->setItem(i, 4, new QTableWidgetItem(group.captain()));
        table->setItem(i, 5, new QTableWidgetItem(QString::number(group.studentCount())));
    }

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(table);
    dialog->setLayout(layout);
    dialog->setWindowTitle(QString("Groups from %1 to %2").arg(start).arg(end));
    dialog->resize(800, 400);
    dialog->exec();
}

void MainWindow::facultyButtonClicked()
{
    QString faculty = ui->facultyFilterEdit->text();
    if (faculty.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter a faculty name");
        return;
    }

    QVector<StudentGroup> filtered;
    for (const StudentGroup &group : m_groups) {
        if (group.faculty().contains(faculty, Qt::CaseInsensitive)) {
            filtered.append(group);
        }
    }

    if (filtered.isEmpty()) {
        QMessageBox::information(this, "No Results", "No groups found for this faculty");
        return;
    }

    // Sort by student count
    std::sort(filtered.begin(), filtered.end(), [](const StudentGroup &a, const StudentGroup &b) {
        return a.studentCount() < b.studentCount();
    });

    // Create a temporary table to display results
    QDialog *dialog = new QDialog(this);
    QTableWidget *table = new QTableWidget(dialog);
    table->setColumnCount(6);
    table->setRowCount(filtered.size());
    QStringList headers = {"Name", "Year", "Degree", "Faculty", "Captain", "Students"};
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int i = 0; i < filtered.size(); ++i) {
        const StudentGroup &group = filtered[i];
        table->setItem(i, 0, new QTableWidgetItem(group.name()));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(group.year())));
        table->setItem(i, 2, new QTableWidgetItem(group.degree()));
        table->setItem(i, 3, new QTableWidgetItem(group.faculty()));
        table->setItem(i, 4, new QTableWidgetItem(group.captain()));
        table->setItem(i, 5, new QTableWidgetItem(QString::number(group.studentCount())));
    }

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(table);
    dialog->setLayout(layout);
    dialog->setWindowTitle(QString("Groups in faculty %1 (sorted by size)").arg(faculty));
    dialog->resize(800, 400);
    dialog->exec();
}

void MainWindow::showSeperatedGroups(const QVector<StudentGroup> &groups, const QString &title){
    QDialog *dialog = new QDialog(this);
    QTableWidget *table = new QTableWidget(dialog);
    table->setColumnCount(6);
    table->setRowCount(groups.size());

    QStringList headers = {"Name", "Year", "Degree", "Faculty", "Captain", "Students"};
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for(int i = 0; i < groups.size(); i++){
        const StudentGroup &group = groups[i];
        table->setItem(i, 0, new QTableWidgetItem(group.name()));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(group.year())));
        table->setItem(i, 2, new QTableWidgetItem(group.degree()));
        table->setItem(i, 3, new QTableWidgetItem(group.faculty()));
        table->setItem(i, 4, new QTableWidgetItem(group.captain()));
        table->setItem(i, 5, new QTableWidgetItem(QString::number(group.studentCount())));
    }

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(table);
    dialog->setLayout(layout);
    dialog->setWindowTitle(title);
    dialog->resize(800, 400);
    dialog->exec();

}

void MainWindow::splitButtonClicked()
{
    m_bachelorGroups.clear();
    m_specialistGroups.clear();
    m_masterGroups.clear();

    for (const StudentGroup &group : m_groups) {
        if (group.degree() == 'B') {
            m_bachelorGroups.append(group);
        } else if (group.degree() == 'C') {
            m_specialistGroups.append(group);
        } else if (group.degree() == 'M') {
            m_masterGroups.append(group);
        }
    }

    m_groups.clear();
    updateTable();

    QMessageBox::information(this, "Success",
                             QString("Database split into:\n"
                                     "Bachelor: %1 groups\n"
                                     "Specialist: %2 groups\n"
                                     "Master: %3 groups")
                                 .arg(m_bachelorGroups.size())
                                 .arg(m_specialistGroups.size())
                                 .arg(m_masterGroups.size()));

    showSeperatedGroups(m_bachelorGroups, "Bachelor Groups");
    showSeperatedGroups(m_specialistGroups, "Specialist Groups");
    showSeperatedGroups(m_masterGroups, "Master Groups");
}
