#include <vector>

class Sudoku
{
	public:
		Sudoku();
		Sudoku(const char initMap[][9]);
        void setMap(int i, int j, char n);
        char getMap(int i, int j);
        void setDif(char i);
        char getDif();
        void setNumberCount(int i);
        int getNumberCount();
		void printQuiz();
		void printAns();
		void takeNote(int a, int b);
		void clearNote(int a, int b);
		bool checkDuplicate(int a, int b, int n);
		int checkSpecial(int a, int b);
		int check(int a, int b);
		void solve();
		bool isCorrect();
        void create();
        void subCreate();
		void clearData();
        void setDifficulty();
        bool mapIsEmpty();
        bool mapIsFinished();
	private:
		char map[9][9];
		char note[9][9][9];
		bool initial;
        char dif = 0; // dif = [0,1,2,3,4,5]
        int numberCount = 25;
};

