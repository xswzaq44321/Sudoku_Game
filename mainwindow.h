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
    void on_comboBox_currentIndexChanged(int index);
    void on_pushButton_solve_clicked();
    void button_pressed(int id);

private:
    Sudoku quiz;
    std::vector<Sudoku> ans;
    QPushButton *button[9][9];

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
