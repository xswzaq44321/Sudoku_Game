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

Sudoku::Sudoku(){
	this->clearData();
}

Sudoku::Sudoku(const char initMap[][9]){
	this->clearData();
	for(int i = 0; i < 9; ++i){
		for(int j = 0; j < 9; ++j){
			map[i][j] = initMap[i][j];
		}
	}
}

void Sudoku::setQuiz(int i, int j, char n){
	map[i][j] = n;
}

char Sudoku::getQuiz(int i, int j){
	return map[i][j];
}

void Sudoku::printQuiz(){
	printf("=====================\n");
	for(int i = 0; i < 9; ++i){
		for(int j = 0; j < 9; ++j){
			printf(map[i][j] ? GRN "%d " RESET : "%d ", map[i][j]);
			printf(j % 3 == 2 ? "|" : "");
		}
		printf(i % 3 == 2 ? "\n---------------------\n" :"\n");
	}
	printf("=====================\n");
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
		char blockI, blockJ;
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
	char blockI, blockJ;
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

int Sudoku::checkSpecial(int a, int b){
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

	return checkSpecial(a, b);
}

void Sudoku::solve(){
	int upDate;
	for(int i = 0; i < 9; ++i){
		for(int j = 0; j < 9; ++j){
			takeNote(i, j);
		}
	}
	do{
		upDate = 0;
		for(int i = 0; i < 9; ++i){
			for(int j = 0; j < 9; ++j){
				if(map[i][j] == 0){ // only execute where are needed
					map[i][j] = check(i, j);
					if(map[i][j] != 0){ // if up date success
						++upDate;
						clearNote(i, j);
					}
				}
			}
		}
		initial = false;
	}while(upDate);
}

bool Sudoku::isCorrect(){
	bool checkResult;
	for(int i = 0; i < 9; ++i){
		for(int j = 0; j < 9; ++j){
			if((map[i][j] == 0) || checkDuplicate(i, j, map[i][j])){
				return false;
			}
		}
	}
	return true;
}

void Sudoku::subCreate(int numberCount){
	char temp, tempI, tempJ, count = 0;
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

void Sudoku::create(int numberCount){
	Sudoku temp;
	srand(time(NULL));
	bool result = false;
	while(!result){
		this->clearData();
		subCreate(numberCount);
		temp = *this;
		temp.solve();
		result = temp.isCorrect();
	}
}

void Sudoku::clearData(){
	for(int i = 0; i < 9; ++i){
		for(int j = 0; j < 9; ++j){
			this->setQuiz(i, j, 0);
			this->clearNote(i, j);
		}
	}
	initial = true;
}

void Sudoku::setDif(int dif){ // dif = [0,1,2,3,4,5]
	char number = 25 - dif * 5;
	if(number + dif > 80){
		return;
	}
	srand(time(NULL));
	char tempI, tempJ;
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
