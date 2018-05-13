#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include "sudoku.h"
#define BLOCKI(a, i) (i / 3 + static_cast<int>(a / 3) * 3)
#define BLOCKJ(b, i) (i % 3 + static_cast<int>(b / 3) * 3)
#define GRN   "\x1B[32m"
#define CYN   "\x1B[36m"
#define RESET "\x1B[0m"
#define ANS_SIZE 2000

Sudoku::Sudoku(){
    srand(time(NULL));
    this->clearData();
}

Sudoku::Sudoku(const char initMap[][9]){
    srand(time(NULL));
    this->clearData();
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            map[i][j] = initMap[i][j];
        }
    }
}

void Sudoku::setMap(int i, int j, const char n){
    if(i < 0 || i > 8 || j < 0 || j > 8){
        return;
    }
    map[i][j] = n;
}

char Sudoku::getMap(int i, int j)const {
    if(i < 0 || i > 8 || j < 0 || j > 8){
        return -1; // -1 represents fail to get map data
    }
    return map[i][j];
}

void Sudoku::setNote(int i, int j, int n, char status){
    if(i < 0 || i > 8 || j < 0 || j > 8 || n < 0 || n > 8){
        return;
    }
    note[i][j][n] = status;
}

void Sudoku::setNote(int i, int j, const char n[9]){
    if(i < 0 || i > 8 || j < 0 || j > 8){
        return;
    }
    for(int k = 0; k < 9; ++k){
        note[i][j][k] = n[k];
    }
}

char Sudoku::getNote(int i, int j, int n)const {
    return note[i][j][n];
}

void Sudoku::setDif(const char i){
    dif = i;
}

char Sudoku::getDif()const {
    return dif;
}

void Sudoku::setNumberCount(const int i){
    numberCount = i;
}

int Sudoku::getNumberCount()const {
    return numberCount;
}

void Sudoku::printMap()const {
    fprintf(stderr, "=====================\n");
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            fprintf(stderr, map[i][j] ? GRN "%d " RESET : "%d ", map[i][j]);
            fprintf(stderr, j % 3 == 2 ? "|" : "");
        }
        fprintf(stderr, i % 3 == 2 ? "\n---------------------\n" :"\n");
    }
    fprintf(stderr, "=====================\n");
    fflush(stderr);
}

int Sudoku::getFirstZeroIndex()const {
    for(int i = 0; i < 81; ++i){
        if(map[i / 9][i % 9] == 0){
            return i;
        }
    }
    return -1;
}

void Sudoku::takeNote(int a, int b){
    if(initial){
        for(int i = 0; i < 9; ++i){
            note[a][b][i] = 1;
        }
    }
    for(int i = 0; i < 9; ++i){ // check row
        if(map[a][i] != 0){
            note[a][b][ map[a][i] - 1 ] = 0;
        }
    }
    for(int i = 0; i < 9; ++i){ // check column
        if(map[i][b] != 0){
            note[a][b][ map[i][b] - 1 ] = 0;
        }
    }
    for(int i = 0; i < 9; ++i){ // check block
        int blockI, blockJ;
        blockI = BLOCKI(a, i);
        blockJ = BLOCKJ(b, i);
        if(map[blockI][blockJ] != 0){
            note[a][b][ map[blockI][blockJ] - 1 ] = 0;
        }
    }
}

void Sudoku::clearNote(int a, int b){
    for(int i = 0; i < 9; ++i){
        note[a][b][i] = 0;
    }
}

bool Sudoku::checkDuplicate(int a, int b, int n){
    int blockI, blockJ;
    for(int i = 0; i < 9; ++i){ // check row
        if(i != b && map[a][i] == n){
            return true;
        }
    }
    for(int i = 0; i < 9; ++i){ // check column
        if(i != a && map[i][b] == n){
            return true;
        }
    }
    for(int i = 0; i < 9; ++i){ // check block
        blockI = BLOCKI(a, i);
        blockJ = BLOCKJ(b, i);
        if((blockI != a && blockJ != b) && map[blockI][blockJ] == n){
            return true;
        }
    }
    return false;
}

char Sudoku::checkSpecial(int a, int b){
    char filled[9] = {0}; //"filled" is used to record how many time a number appears in the "notes of row, column, block"

    for(int i = 0; i < 9; ++i){ //check row
        for(int j = 0; j < 9; ++j){
            if(note[a][i][j]){
                filled[j]++; //record how many times the number appears
            }
        }
    }
    for(int i = 0; i < 9; ++i){
        //if a number only appears once in the "note of the row, column, block",
        //that means that number is the only one that can fit in the point.
        //since the note of the point will only update when "check()" processed
        //therefore, we need to check what had filled in before
        if(filled[i] == 1 && !checkDuplicate(a, b, i+1)){
            return static_cast<char>(i + 1);
        }
    }

    for(int i = 0; i < 9; ++i){
        filled[i] = 0;
    }
    for(int i = 0; i < 9; ++i){ //check column
        for(int j = 0; j < 9; ++j){
            if(note[i][b][j]){
                filled[j]++; //record how many times the number appears
            }
        }
    }
    for(int i = 0; i < 9; ++i){
        if(filled[i] == 1 && !checkDuplicate(a, b, i+1)){
            return static_cast<char>(i + 1);
        }
    }

    for(int i = 0; i < 9; ++i){
        filled[i] = 0;
    }
    for(int i = 0; i < 9; ++i){ //check block
        for(int j = 0; j < 9; ++j){
            int blockI, blockJ;
            blockI = BLOCKI(a, i);
            blockJ = BLOCKJ(b, i);
            if(note[blockI][blockJ][j]){
                filled[j]++;
            }
        }
    }
    for(int i = 0; i < 9; ++i){
        if(filled[i] == 1 && !checkDuplicate(a, b, i+1)){
            return static_cast<char>(i + 1);
        }
    }
    return 0;
}

char Sudoku::check(int a, int b){
    char howMany = 0, whatIsIt = 0;
    for(int i = 0; i < 9; ++i){
        if(note[a][b][i] == 1){ // check how many element are in the note
            howMany++;
            whatIsIt = i;
        }
    }
    if(howMany == 0){
        fprintf(stderr, "error! Can't fill any number on [%d, %d]\n", a, b);
        return 0;
    }else if(howMany == 1){
        return whatIsIt + 1;
    }

    return checkSpecial(a, b);
}

void Sudoku::solve(){
    int upDate;
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(map[i][j] == 0){
                takeNote(i, j);
            }
        }
    }
    initial = false;
    do{
        upDate = 0;
        for(int i = 0; i < 9; ++i){
            for(int j = 0; j < 9; ++j){
                if(map[i][j] == 0){ // only execute where are needed
                    map[i][j] = check(i, j);
                    if(map[i][j] != 0){ // if up date success
                        ++upDate;
                        clearNote(i, j);
                        for(int k = 0; k < 9; ++k){
                            takeNote(k, j);
                            takeNote(i, k);
                            takeNote(BLOCKI(i, k), BLOCKJ(j, k));
                        }
                    }
                }
            }
        }
    }while(upDate);
}

void Sudoku::bruteSolve(Sudoku quest, vector<Sudoku>& ans){
    int firstZero = quest.getFirstZeroIndex();
    int a = firstZero / 9, b = firstZero % 9;
    if(firstZero == -1){
        if(quest.isCorrect()){
            ans.push_back(quest);
        }
        return;
    }else{
        for(int num = 1; num <=9; ++num){
            if(ans.size() > ANS_SIZE){
                return;
            }
            if(quest.checkDuplicate(a, b, num)){
                continue;
            }
            quest.setMap(a, b, num);
            bruteSolve(quest, ans);
        }
        return;
    }
}

vector<Sudoku> Sudoku::multiSolve(){
    Sudoku temp(*this);
    vector<Sudoku> answer;
    temp.solve();
    answer.push_back(temp);
    if(temp.isCorrect()){
        answer.push_back(temp);
    }else{
        temp.bruteSolve(temp, answer);
    }
    return answer;
}

bool Sudoku::isCorrect(){
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if((map[i][j] == 0) || checkDuplicate(i, j, map[i][j])){
                return false;
            }
        }
    }
    return true;
}

void Sudoku::subCreate(){
    int temp, tempI, tempJ, count = 0;
    while(count < numberCount){
        temp = rand() % 9 + 1;
        tempI = rand() % 9;
        tempJ = rand() % 9;
        if(map[tempI][tempJ] == 0 && !checkDuplicate(tempI, tempJ, temp)){
            map[tempI][tempJ] = temp;
            ++count;
        }
    }
}

void Sudoku::create(){
    Sudoku temp;
    bool result = false;
    while(!result){
        this->clearData();
        subCreate();
        temp = *this;
        temp.solve();
        result = temp.isCorrect();
    }
    setDifficulty();
}

void Sudoku::clearData(){
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            this->setMap(i, j, 0);
            this->clearNote(i, j);
        }
    }
    initial = true;
}

void Sudoku::setDifficulty(){
    if(dif == 0) return;
    int number = dif * 5;
    int tempI, tempJ;
    Sudoku answer;
    answer = *this;
    answer.solve();
    for(int i = 0; i < number; ++i){
        tempI = rand() % 9;
        tempJ = rand() % 9;
        if(map[tempI][tempJ] == answer.map[tempI][tempJ]){
            --i;
            continue;
        }else{
            map[tempI][tempJ] = answer.map[tempI][tempJ];
        }
    }
}

bool Sudoku::mapIsEmpty(){
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(map[i][j]){
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::mapIsFinished(){
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            if(map[i][j] == 0){
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::hasDuplicate(){
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            int num = this->getMap(i, j);
            if(num == 0) continue;
            if(this->checkDuplicate(i, j, num)){
                return true;
            }
        }
    }
    return false;
}
