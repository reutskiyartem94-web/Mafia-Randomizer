#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <vector>
#include <string>
#include <random>
#include <algorithm>

#include <QMessageBox>
#include <QTextCharFormat>
#include <QTextCursor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->playerSpin->setFocus();

    connect(ui->generateButton, &QPushButton::clicked,
            this, &MainWindow::generateRoles);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::formatText(const QColor color, const QFont::Weight style, QString text) {
    // 1. Get the cursor and move it to the end
    QTextCursor cursor = ui->resultText->textCursor();
    cursor.movePosition(QTextCursor::End);

    // 2. Define your color format
    QTextCharFormat lineFormat;
    lineFormat.setForeground(color);
    lineFormat.setFontWeight(style);

    // 3. Apply the format before inserting text
    cursor.setCharFormat(lineFormat);

    // Formating text
    cursor.insertText(text);

    // 5. Reset the formatting
    cursor.setCharFormat(QTextCharFormat());
    ui->resultText->setTextCursor(cursor);
}

void MainWindow::generateRoles()
{

    int playerCount = ui->playerSpin->value();
    int mafiaCount  = ui->mafiaSpin->value();

    if (playerCount < 6) {
        QMessageBox::warning(this, "Error",
                             "Minimum number of players - 6");
        return;
    }

    std::vector<int> count = {};

    for (int i = 1; i <= playerCount; i++) {
        count.push_back(i);
    }

    if ((playerCount < 7 && mafiaCount > 1) ||
        (playerCount < 9 && mafiaCount > 2) ||
        (playerCount < 14 && mafiaCount > 3) || 
        (mafiaCount > 4)) {
        QMessageBox::warning(this, "Error",
                             "Too many Mafia for this number of players");
        return;
    }

    std::vector<std::string> roles;

    // Don
    if (ui->donCheck->isChecked()) {
        roles.push_back("Mafia Don");
        mafiaCount--;
    }

    if (mafiaCount < 1) {
        QMessageBox::warning(this, "Error",
                             "Should be at least 1 Mafia");
        return;
    }

    // Mafia
    for (int i = 0; i < mafiaCount; i++)
        roles.push_back("Mafia");

    if (ui->doctorCheck->isChecked())
        roles.push_back("Doctor");

    if (ui->sheriffCheck->isChecked())
        roles.push_back("Sheriff");

    while (roles.size() < static_cast<size_t>(playerCount))
        roles.push_back("Citizen");

    // Shuffle
    std::mt19937 gen(std::random_device{}());
    std::shuffle(roles.begin(), roles.end(), gen);

    // Output
    ui->resultText->clear();

    // Assign and show roles
    for (int i = 0; i < count.size(); i++) {
        std::uniform_int_distribution<size_t> dist(0, roles.size() - 1);
        size_t r = dist(gen);
        if (roles[r] == "Mafia Don" || roles[r] == "Mafia") {
            formatText (Qt::red, QFont::Normal,
                       QString("\nPlayer #%1: %2")
                           .arg(count[i])
                           .arg(QString::fromStdString(roles[r])));
        }
        else if (roles[r] == "Doctor" || roles [r] == "Sheriff") {
            formatText (Qt::green, QFont::Normal,
                       QString("\nPlayer #%1: %2")
                           .arg(count[i])
                           .arg(QString::fromStdString(roles[r])));
        }
        else {
        ui->resultText->append(
            QString("Player #%1: %2")
                .arg(count[i])
                .arg(QString::fromStdString(roles[r]))
            );
        }
        roles.erase(roles.begin() + r);
    }
}
