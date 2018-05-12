#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QIcon>
#include <QSize>
#include <QDebug>
#include <QPushButton>
#include <QLayout>
#include <vector>
#include <QSignalMapper>
#include <set>
#include <QKeyEvent>
#include <QTimer>
#include <QTime>
#include "sudoku.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_pushButton_new_clicked();
    void on_comboBox_difficulty_currentIndexChanged(int index);
    void on_pushButton_solve_clicked();
    void button_pressed(int i, int j);
    virtual void keyPressEvent(QKeyEvent *e);
    void enterNumber(int number);
    void enterNote(int number);
    void on_pushButton_clear_clicked();
    void on_pushButton_hint_clicked();
    void on_comboBox_ans_currentIndexChanged(int index);
    void displayTime();
    void timeStart();
    void timeStop();
    void on_pushButton_note_toggled(bool checked);
    void enterHandel(int number);

private:
    Sudoku quiz;
    Sudoku player;
    std::vector<Sudoku> ans;
    QPushButton *button[9][9];
    QPushButton *numberButton[9];
    int nowI = -1, nowJ = -1;
    bool clickAble = true;
    bool noteMode = false;
    QString redText = "color: rgb(237, 28, 36);";  // error color
    QString greenText = "color: rgb(0, 181, 46);"; // answer
    QString blueText = "color: rgb(47, 56, 176);"; // quiz
    QString normalNumber = "font:36px;";
    QString noteNumber = "font:13px; color: rgb(100, 100, 100);";
    set<int> moveSet, numberSet;
    QTimer *myTimer;
    QTime gameTime;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
