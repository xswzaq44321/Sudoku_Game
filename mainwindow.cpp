#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_note->setCheckable(true);

    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            button[i][j] = new QPushButton(this);
            button[i][j]->setGeometry(50 + j * 45 + (j / 3) * 3, 50 + i * 45 + (i / 3) * 3, 50, 50);
            button[i][j]->setFont(QFont("Andy", 22, QFont::Bold));
            connect(button[i][j], &QPushButton::clicked, [this, i, j](){
                button_pressed(i, j);
            });
        }
    }
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
    quiz.printMap();
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(quiz.getMap(i, j) != 0){
                button[i][j]->setText(QString::number(quiz.getMap(i, j)));
                button[i][j]->setStyleSheet(blueText);
            }
        }
    }
}

void MainWindow::on_comboBox_difficulty_currentIndexChanged(int index)
{
    quiz.setDif((char)index);
}

void MainWindow::on_pushButton_solve_clicked()
{
    qDebug() << "solve clicked";
    if(player.mapIsEmpty()){
        ui->player_status->setText("Map is empty!");
        return;
    }
    if(quiz.mapIsEmpty()){
        quiz = player;
    }
    if(quiz.hasDuplicate()){ // if quiz have duplicate, it's bound to be unsolvable!
        ui->player_status->setText("Unsolvable!");
        return;
    }
    ans.clear();
    quiz.printMap();
    ans = quiz.multiSolve();

    qDebug() << "ans.size() = " << ans.size();
    ui->comboBox_ans->addItem("Common Sol");
    ans[0].printMap();
    ans[1].printMap();
    for(int i = 1; static_cast<uint>(i) < ans.size(); ++i){ //set combox texts
        char temp[100];
        sprintf(temp, "Sol %d", i); // Sol 0 = Common Sol
        QString qtemp(temp);
        ui->comboBox_ans->addItem(qtemp);
    }

    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(player.getMap(i, j) != ans[0].getMap(i, j)){
                button[i][j]->setText(QString::number(ans[0].getMap(i, j)));
                if(player.getMap(i, j) != 0){
                    button[i][j]->setStyleSheet(redText);
                }else{
                    button[i][j]->setStyleSheet(greenText);
                }
            }
        }
    }
}

void MainWindow::button_pressed(int i, int j){
    if(nowI != -1 && nowJ != -1){
        button[nowI][nowJ]->setDown(false);
        qDebug() << "release" << nowI << "," << nowJ;
    }
    if(i == nowI && j == nowJ){ // de-focus any button on the board
        nowI = -1;
        nowJ = -1;
        return;
    }
    button[i][j]->setDown(true);
    nowI = i;
    nowJ = j;
}

void MainWindow::keyPressEvent(QKeyEvent *e){
    if(nowI == -1 || nowJ == -1){ // if no button was focused
        return;
    }
    char number = player.getMap(nowI, nowJ);

    switch(e->key()){
    case Qt::Key_W:
        if(nowI == 0) return;
        button_pressed((nowI - 1), nowJ);
        return;
        break;
    case Qt::Key_S:
        if(nowI == 8) return;
        button_pressed((nowI + 1), nowJ);
        return;
        break;
    case Qt::Key_A:
        if(nowJ == 0) return;
        button_pressed(nowI, (nowJ - 1));
        return;
        break;
    case Qt::Key_D:
        if(nowJ == 8) return;
        button_pressed(nowI, (nowJ + 1));
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
    if(nowI == -1 || nowJ == -1){ // if no button was focused
        return;
    }
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
    if(index < 0 || index > static_cast<int>(ans.size())){
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
                    button[i][j]->setStyleSheet(redText);
                }else{
                    button[i][j]->setStyleSheet(blueText);
                }
            }
        }
    }
}
