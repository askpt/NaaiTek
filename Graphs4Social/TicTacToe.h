#ifndef TIC_TAC_TOE_
#define TIC_TAC_TOE_

#include <string>

void initGameData();
void IAConnection(char);
void writeTextOnBox(int, int, const char*);
void drawTextBox();
void ticTacToeInit();
void drawTable();
void drawX(int, int);
void drawO(int, int);
void displayTicTacToe(void);
void mouseTicTacToe(int, int, int, int);
void keyboardTicTacToe(int, int, int);

void checkMachineWin();

void checkUserWin();

void updateBorderGame(std::string);
bool checkWinnerToMainGame();

void IAConnection(char);

int startTicTacToe();


#endif