#include <vector>
using namespace std;

class Sudoku
{
    public:
        Sudoku();
        Sudoku(const char initMap[][9]);
        void setMap(int i, int j, const char n);
        char getMap(int i, int j)const;
        void setNote(int i, int j, int n, const char status);
        void setNote(int i, int j, const char n[9]);
        char getNote(int i, int j, int n)const;
        void setDif(const char i);
        char getDif()const;
        void setNumberCount(const int i);
        int getNumberCount()const;
        void printMap()const;
        int getFirstZeroIndex()const;
        void takeNote(int a, int b);
        void clearNote(int a, int b);
        bool checkDuplicate(int a, int b, int n);
        char checkSpecial(int a, int b);
        char check(int a, int b);
        void solve();
        void bruteSolve(Sudoku quest, vector<Sudoku>& ans);
        vector<Sudoku> multiSolve();
        bool isCorrect();
        void create();
        void subCreate();
        void clearData();
        void setDifficulty();
        bool mapIsEmpty();
        bool mapIsFinished();
        bool hasDuplicate();
    private:
        char map[9][9] = {{0}};
        char note[9][9][9] = {{{0}}};
        bool initial = true;
        char dif = 0; // dif = [0,1,2,3,4,5]
        int numberCount = 25;
};

