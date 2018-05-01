#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSignalMapper *signalMapper = new QSignalMapper(this);

    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            button[i][j] = new QPushButton(this);
            button[i][j]->setGeometry(50 + j * 45 + (j / 3) * 3, 50 + i * 45 + (i / 3) * 3, 50, 50);
            button[i][j]->setFont(QFont("Andy", 22, QFont::Bold));
            QObject::connect(button[i][j], SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(button[i][j], i * 9 + j);
        }
    }
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(button_pressed(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_new_clicked()
{
    qDebug() << "New Puzzle!" << endl;
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            button[i][j]->setText("");
            button[i][j]->setStyleSheet("");
        }
    }
    quiz.create();
    quiz.printQuiz();
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(quiz.getQuiz(i, j) != 0){
                button[i][j]->setText(QString::number(quiz.getQuiz(i, j)));
                button[i][j]->setStyleSheet("color: rgb(47, 56, 176);");
            }
        }
    }
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if(index == 0){
        quiz.setNumberCount(25);
    }else{
        quiz.setNumberCount(30);
    }
    quiz.setDif((char)index);
}

void MainWindow::on_pushButton_solve_clicked()
{
    if(quiz.mapIsEmpty()){
        return;
    }
    Sudoku temp(quiz);
    temp.solve();
    ans.push_back(temp);
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(quiz.getQuiz(i, j) != ans[0].getQuiz(i, j)){
                button[i][j]->setText(QString::number(ans[0].getQuiz(i ,j)));
            }
        }
    }
    ans.clear();
}

void MainWindow::button_pressed(int id){
    static QPushButton *previousInFocusChain()
    int a = id / 9, b = id % 9;
}
