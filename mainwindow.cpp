#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_note->setCheckable(true);

    for(int i = 0; i < 9; ++i){ // set up buttons
        for(int j = 0; j < 9; ++j){
            button[i][j] = new QPushButton(this);
            button[i][j]->setGeometry(50 + j * 45 + (j / 3) * 3, 50 + i * 45 + (i / 3) * 3, 50, 50);
            button[i][j]->setFont(QFont("Andy", QFont::Bold));
            button[i][j]->setStyleSheet(normalNumber);
            button[i][j]->setText("");
            connect(button[i][j], &QPushButton::clicked, [this, i, j](){
                button_pressed(i, j);
            });
        }
    }
    for(int i = 0; i < 9; ++i){ // set up number buttons
        numberButton[i] = new QPushButton(this);
        numberButton[i]->setGeometry(90 + i * 50, 490, 41, 41);
        numberButton[i]->setFont(QFont("Andy", QFont::Bold));
        numberButton[i]->setStyleSheet(normalNumber);
        numberButton[i]->setText(QString::number(1 + i));
        connect(numberButton[i], &QPushButton::clicked, [this, i](void){
            enterHandel(i + 1);
        });
    }
    moveSet.insert(Qt::Key_W);
    moveSet.insert(Qt::Key_A);
    moveSet.insert(Qt::Key_S);
    moveSet.insert(Qt::Key_D);
    numberSet.insert(Qt::Key_1);
    numberSet.insert(Qt::Key_2);
    numberSet.insert(Qt::Key_3);
    numberSet.insert(Qt::Key_4);
    numberSet.insert(Qt::Key_5);
    numberSet.insert(Qt::Key_6);
    numberSet.insert(Qt::Key_7);
    numberSet.insert(Qt::Key_8);
    numberSet.insert(Qt::Key_9);
    myTimer = new QTimer(this);
    myTimer->start(100);
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
            button[i][j]->setStyleSheet(normalNumber);
        }
    }
    quiz.create();
    player = quiz;
    quiz.printMap();
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(quiz.getMap(i, j) != 0){
                button[i][j]->setText(QString::number(quiz.getMap(i, j)));
                button[i][j]->setStyleSheet(normalNumber + blueText);
            }
        }
    }
    timeStart();
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
    ui->comboBox_ans->clear();
    quiz.printMap();
    ans = quiz.multiSolve();

    qDebug() << "ans.size() = " << ans.size();
    ui->comboBox_ans->addItem("Common Sol");
    ans[0].printMap();
    if(ans.size() > 1){
        ans[1].printMap();
        ui->player_status->setText(QString::number(ans.size() - 1) + "\n solves.");
    }else{
        ui->player_status->setText("No solve!");
    }
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
                    button[i][j]->setStyleSheet(normalNumber + redText);
                }else{
                    button[i][j]->setStyleSheet(normalNumber + greenText);
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

    if(moveSet.count(e->key())){
        switch(e->key()){
        case Qt::Key_W:
            if(nowI == 0) return;
            button_pressed((nowI - 1), nowJ);
            break;
        case Qt::Key_S:
            if(nowI == 8) return;
            button_pressed((nowI + 1), nowJ);
            break;
        case Qt::Key_A:
            if(nowJ == 0) return;
            button_pressed(nowI, (nowJ - 1));
            break;
        case Qt::Key_D:
            if(nowJ == 8) return;
            button_pressed(nowI, (nowJ + 1));
            break;
        }
        return;
    }else if(numberSet.count(e->key())){
        char number = player.getMap(nowI, nowJ);
        number = e->key() - Qt::Key_0;
        qDebug() << "noteMode = " << noteMode;
        enterHandel(number);
    }else if(e->key() == Qt::Key_N){
        ui->pushButton_note->setChecked(!(noteMode));
    }
}

void MainWindow::enterNumber(int number){
    if(quiz.getMap(nowI, nowJ) == 0 && clickAble){ // you can't fill a number into quiz
        if(player.getMap(nowI, nowJ) == number){
            player.setMap(nowI, nowJ, 0);
            button[nowI][nowJ]->setText("");
        }else{
            button[nowI][nowJ]->setStyleSheet(normalNumber);
            player.setMap(nowI, nowJ, number);
            button[nowI][nowJ]->setText(QString::number(number));
        }
        if(player.mapIsFinished()){
            if(player.isCorrect()){ // player win
                ui->player_status->setText("You Win!");
                clickAble = false;
                timeStop();
            }else{
                ui->player_status->setText("something \nwrong...");
            }
        }
    }
}

void MainWindow::enterNote(int number){
    if(quiz.getMap(nowI, nowJ) == 0 && clickAble && player.getMap(nowI, nowJ) == 0){
        button[nowI][nowJ]->setStyleSheet(noteNumber);
        if(player.getNote(nowI, nowJ, number - 1)){
            player.setNote(nowI, nowJ, number - 1, 0);
            QString temp = button[nowI][nowJ]->text();
            temp.replace(number - 1 + (number - 1) / 3, 1, " ");
            button[nowI][nowJ]->setText(temp);
        }else{
            player.setNote(nowI, nowJ, number - 1, 1);
            char temp[12] = "   \n   \n   ";
            for(int i = 0; i < 9; ++i){
                if(player.getNote(nowI, nowJ, i) == 1){
                    temp[i + i / 3] = '0' + i + 1;
                }
            }
            qDebug() << "temp = " << temp;
            button[nowI][nowJ]->setText(QString::fromLocal8Bit(temp));
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
    timeStop();
    ui->gameTimeLabel->clear();
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
    if(quiz.getMap(nowI, nowJ) == 0 && player.getMap(nowI, nowJ) == 0){
        Sudoku refAns(quiz);
        refAns.solve();
        if(refAns.getMap(nowI, nowJ) != 0){
            enterNumber(refAns.getMap(nowI, nowJ));
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
                    button[i][j]->setStyleSheet(normalNumber + redText);
                }else{
                    button[i][j]->setStyleSheet(normalNumber + greenText);
                }
            }
        }
    }
}

void MainWindow::displayTime(){
    int time = ceil(gameTime.elapsed() / 1000);
    char temp[100];
    sprintf(temp, "%02d : %02d", time / 60, time % 60);
    ui->gameTimeLabel->setText(QString::fromLocal8Bit(temp));
}

void MainWindow::timeStart(){
    connect(myTimer, &QTimer::timeout, this, &MainWindow::displayTime);
    gameTime.start();
}

void MainWindow::timeStop(){
    disconnect(myTimer, &QTimer::timeout, this, &MainWindow::displayTime);
}

void MainWindow::on_pushButton_note_toggled(bool checked)
{
    noteMode = checked;
}

void MainWindow::enterHandel(int number){
    if(noteMode){
        enterNote(number);
    }else{
        enterNumber(number);
    }
}
