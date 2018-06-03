#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QPainter>
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
#include <QToolButton>
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
    void loadResource();

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
    void on_toolButton_note_toggled(bool checked);
    void enterHandel(int number);
    void animation(int i, int j);


private:
    Sudoku quiz;
    Sudoku player;
    std::vector<Sudoku> ans;
    QToolButton *button[9][9];
    QToolButton *numberButton[9];
    int nowI = -1, nowJ = -1;
    bool clickAble = true;
    bool noteMode = false;
    QTimer *myTimer;
    QTimer *imageTimer;
    QTime gameTime;
    set<int> moveSet, numberSet, eraseSet;
    QIcon numberIcon[10];
    QIcon numberIconRed[10];
    QIcon numberIconGreen[10];
    QIcon numberIconBlue[10];
    QString blockIconBlue[6];
    QString blockIconWhite[6];
    QPixmap numberPixmap[10];
    QPixmap *collage;
    QPainter *painter;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
