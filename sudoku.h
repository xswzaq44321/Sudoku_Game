#ifndef SUDOKU_H
#define SUDOKU_H


class Sudoku
{
    public:
        Sudoku();
        Sudoku(const char initMap[][9]);
        void setQuiz(int i, int j, char n);
        char getQuiz(int i, int j);
        void printQuiz();
        void takeNote(int a, int b);
        void clearNote(int a, int b);
        bool checkDuplicate(int a, int b, int n);
        int checkSpecial(int a, int b);
        int check(int a, int b);
        bool isCorrect();
        void solve();
    private:
        char quiz[9][9] = {0};
        char note[9][9][9] = {0};
        bool initial = true;
};

#endif // SUDOKU_H
