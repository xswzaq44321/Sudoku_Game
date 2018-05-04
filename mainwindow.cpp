#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSignalMapper *signalMapper = new QSignalMapper(this);
    ui->pushButton_note->setCheckable(true);

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
    ans.clear();
    clickAble = true;
    ui->player_status->setText("Ongoing...\n:)");
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            button[i][j]->setText("");
            button[i][j]->setStyleSheet("");
        }
    }
    quiz.create();
    player = quiz;
    quiz.printQuiz();
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(quiz.getMap(i, j) != 0){
                button[i][j]->setText(QString::number(quiz.getMap(i, j)));
                button[i][j]->setStyleSheet("color: rgb(47, 56, 176);");
            }
        }
    }
}

void MainWindow::on_comboBox_difficulty_currentIndexChanged(int index)
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
    qDebug() << "solve clicked";
    if(player.mapIsEmpty()){
        return;
    }
    if(quiz.mapIsEmpty()){
        quiz = player;
    }
    Sudoku comAns(quiz);
    ans.clear();
    quiz.printQuiz();
    comAns.solve();
    ans = quiz.multiSolve();
    ans.insert(ans.begin(), comAns);
    qDebug() << ans.size();
    ui->comboBox_ans->addItem("Common Sol");
    for(int i = 1; i < ans.size(); ++i){ //set combox texts
        char temp[100];
        sprintf(temp, "Sol %d", i); // Sol 0 = Common Sol
        QString qtemp(temp);
        ui->comboBox_ans->addItem(qtemp);
    }
    for(std::vector<Sudoku>::iterator it = ans.begin(); it != ans.end(); ++it){
        it->printQuiz(); // debug print
    }

    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(player.getMap(i, j) != ans[0].getMap(i, j)){
                button[i][j]->setText(QString::number(ans[0].getMap(i, j)));
                if(player.getMap(i, j) != 0){
                    button[i][j]->setStyleSheet("color: rgb(237, 28, 36)");
                }else{
                    button[i][j]->setStyleSheet("color: rgb(0, 181, 46);");
                }
            }
        }
    }
}

void MainWindow::button_pressed(int id){
    int a = id / 9, b = id % 9;
    button[nowI][nowJ]->setDown(false);
    qDebug() << "release" << nowI << "," << nowJ;
    button[a][b]->setDown(true);
    nowI = a;
    nowJ = b;
}

void MainWindow::keyPressEvent(QKeyEvent *e){
    char number = player.getMap(nowI, nowJ);

    switch(e->key()){
    case Qt::Key_W:
        if(nowI == 0) return;
        button_pressed((nowI - 1) * 9 + nowJ);
        return;
        break;
    case Qt::Key_S:
        if(nowI == 8) return;
        button_pressed((nowI + 1) * 9 + nowJ);
        return;
        break;
    case Qt::Key_A:
        if(nowJ == 0) return;
        button_pressed(nowI * 9 + (nowJ - 1));
        return;
        break;
    case Qt::Key_D:
        if(nowJ == 8) return;
        button_pressed(nowI * 9 + (nowJ + 1));
        return;
        break;
    case Qt::Key_1:
        number = 1;
        break;
    case Qt::Key_2:
        number = 2;
        break;
    case Qt::Key_3:
        number = 3;
        break;
    case Qt::Key_4:
        number = 4;
        break;
    case Qt::Key_5:
        number = 5;
        break;
    case Qt::Key_6:
        number = 6;
        break;
    case Qt::Key_7:
        number = 7;
        break;
    case Qt::Key_8:
        number = 8;
        break;
    case Qt::Key_9:
        number = 9;
        break;
    }
    if(quiz.getMap(nowI, nowJ) == 0 && clickAble){ // you can't fill a number into quiz
        if(player.getMap(nowI, nowJ) == number){
            player.setMap(nowI, nowJ, 0);
            button[nowI][nowJ]->setText("");
        }else{
            player.setMap(nowI, nowJ, number);
            button[nowI][nowJ]->setText(QString::number(number));
        }
        if(player.mapIsFinished()){
            if(player.isCorrect()){
                ui->player_status->setText("You Win!");
                clickAble = false;
            }else{
                ui->player_status->setText("something \nwrong...");
            }
        }
    }
}

void MainWindow::on_pushButton_clear_clicked()
{
    quiz.clearData();
    player.clearData();
    ans.clear();
    clickAble = true;
    ui->player_status->setText("Status");
    ui->comboBox_ans->clear();
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            button[i][j]->setText("");
            button[i][j]->setStyleSheet("");
        }
    }
}

void MainWindow::on_pushButton_hint_clicked()
{
    if(quiz.getMap(nowI, nowJ) == 0){
        Sudoku refAns(quiz);
        refAns.solve();
        if(refAns.getMap(nowI, nowJ) != 0){
            player.setMap(nowI, nowJ, refAns.getMap(nowI, nowJ));
            button[nowI][nowJ]->setText(QString::number(player.getMap(nowI, nowJ)));
        }
    }
}

void MainWindow::on_comboBox_ans_currentIndexChanged(int index)
{
    qDebug() << "ans index = " << index;
    if(index < 0 || index > ans.size()){
        return;
    }
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(player.getMap(i, j) == 0){
                button[i][j]->setText("");
                button[i][j]->setStyleSheet("");
            }
        }
    }
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(player.getMap(i, j) != ans.at(index).getMap(i, j)){
                button[i][j]->setText(QString::number(ans.at(index).getMap(i ,j)));
                if(player.getMap(i, j) != 0){
                    button[i][j]->setStyleSheet("color: rgb(237, 28, 36)");
                }else{
                    button[i][j]->setStyleSheet("color: rgb(0, 181, 46);");
                }
            }
        }
    }
}
