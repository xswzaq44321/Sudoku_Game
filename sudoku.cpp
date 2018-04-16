#include "sudoku.h"
#define BLOCKI(a, i) (i / 3 + static_cast<int>(a / 3) * 3)
#define BLOCKJ(b, i) (i % 3 + static_cast<int>(b / 3) * 3)

Sudoku::Sudoku(){
}

Sudoku::Sudoku(const char initMap[][9]){
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            quiz[i][j] = initMap[i][j];
        }
    }
}

void Sudoku::takeNote(int a, int b){
    if(initial){
        for(int i = 0; i < 9; ++i){
            note[a][b][i] = 1;
        }
    }
    for(int i = 0; i < 9; ++i){ // check row
        if(quiz[a][i] != 0){
            note[a][b][ quiz[a][i] - 1 ] = 0;
        }
    }
    for(int i = 0; i < 9; ++i){ // check column
        if(quiz[i][b] != 0){
            note[a][b][ quiz[i][b] - 1 ] = 0;
        }
    }
    for(int i = 0; i < 9; ++i){ // check block
        char blockI, blockJ;
        blockI = BLOCKI(a, i);
        blockj = BLOCKJ(b, i);
        if(quiz[blockI][blockJ] != 0){
            note[a][b][ quiz[blockI][blockJ] - 1 ] = 0;
        }
    }
}

void Sudoku::chearNote(int a, int b){
    for(int i = 0; i < 9; ++i){
        note[a][b][i] = 0;
    }
}

bool Sudoku::checkDuplicate(int a, int b, int n){
    char blockI, blockJ;
    for(int i = 0; i < 9; ++i){ // check row
        if(quiz[a][i] == n){
            return true;
        }
    }
    for(int i = 0; i < 9; ++i){ // check column
        if(quiz[i][b] == n){
            return true;
        }
    }
    for(int i = 0; i < 9; ++i){ // check block
        blockI = BLOCKI(a, i);
        blockJ = BLOCKJ(b, i);
        if(quiz[blockI][blockJ] == n){
            return true;
        }
    }
    return false;
}

int Sudoku::checkSpecial(int a, int b){
    char filled[9] = {0}; //"filled" is used to record how many time a number appears in the notes
    for(int i = 0; i < 9; ++i){ //check row
        for(int j = 0; j < 9; ++j){
            if(note[a][i][j]){
                filled[j]++; //record how many times the number appears
            }
        }
    }

    //if a number only appears once in the note,
    //that means that number is the only one that can fit in the point.

    for(int i = 0; i < 9; ++i){
        //since the note of a point will only update when processed by "check()"
        //therefore, we need to check what had filled in before
        if(filled[i] == 1 && !checkDuplicate(a, b, i+1)){
            return i + 1;
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
            return i + 1;
        }
    }

    for(int i = 0; i < 9; ++i){
        filled[i] = 0;
    }
    for(int i = 0; i < 9; ++i){ //check block
        for(int j = 0; j < 9; ++j){
            char blockI, blockJ;
            blockI = BLOCKI(a, i);
            blockJ = BLOCKJ(b, i);
            if(note[blockI][blockJ][j]){
                filled[j]++;
            }
        }
    }
    for(int i = 0; i < 9; ++i){
        if(filled[i] == 1 && !checkDuplicate(a, b, i+1)){
            return i + 1;
        }
    }
    return 0;
}

int Sudoku::check(int a, int b){
    int howMany = 0, whatIsIt = 0;
    for(int i = 0; i < 9; ++i){
        takeNote(a, b);
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

    return initial ? 0 : checkSpecial(a, b);
}

bool Sudoku::checkUnity(int arr[]){
    int arr_unity[9];

    for(int i = 0; i < 9; ++i){
        arr_unity[i] = 0;
    }
    for(int i = 0; i < 9; ++i){
        ++arr_unity[arr[i] - 1];
    }
    for(int i = 0; i < 9; ++i){
        if(arr_unity[i] != 1){
            return false;
        }
    }
    return true;
}

bool Sudoku::isCorrect(){
    bool check_result;
    int check_arr[9];
    int location;
    for(int i = 0; i < 81; i += 9){
        for(int j = 0; j < 9; ++j){
            check_arr[j] = quiz[i + j];
        }
        check_result = checkUnity(check_arr);
        if(check_result == false){
            return false;
        }
    }
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            check_arr[j] = quiz[i + 9 * j];
        }
        check_result = checkUnity(check_arr);
        if(check_result == false){
            return false;
        }
    }
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            location = 27 * (i / 3) + 3 * (i % 3) + 9 * (j / 3) + (j % 3);
            check_arr[j] = quiz[location];
        }
        check_result = checkUnity(check_arr);
        if(check_result == false){
            return false;
        }
    }
    return true;
}

void Sudoku::solve(){
    do{
        upDate = 0;
        for(int i = 0; i < 9; ++i){
            for(int j = 0; j < 9; ++j){
                if(quiz[i][j] == 0){ // only execute where are needed
                    quiz[i][j] = check(i, j);
                    if(quiz[i][j] != 0){ // if up date success
                        ++upDate;
                        clearNote(i, j);
                    }
                }
            }
        }
        initial = 0;
    }while(upDate);
}
