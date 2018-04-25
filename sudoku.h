#include <vector>

class Sudoku
{
	public:
		Sudoku();
		Sudoku(const char initMap[][9]);
		void setQuiz(int i, int j, char n);
		char getQuiz(int i, int j);
		void printQuiz();
		void printAns();
		void takeNote(int a, int b);
		void clearNote(int a, int b);
		bool checkDuplicate(int a, int b, int n);
		int checkSpecial(int a, int b);
		int check(int a, int b);
		void solve();
		bool isCorrect();
		void create(int numberCount = 25);
		void subCreate(int numberCount);
		void clearData();
		void setDif(int dif);
	private:
		char map[9][9];
		char note[9][9][9];
		bool initial;
};

