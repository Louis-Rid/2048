#include <ncurses.h>

#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define SIZE 4
typedef array<array<int, SIZE>, SIZE> tBoard;

void displayBoard(const array<array<int, SIZE>, SIZE>& board) {
  clear();
  printw("\n _______________\n");
  for (int i = 0; i < SIZE; i++) {
    printw("| ");
    for (int j = 0; j < SIZE; j++) {
      string val = board[i][j] == 0 ? " " : to_string(board[i][j]);
      printw("%s | ", val.c_str());
    }
    printw("\n|_______________|\n");
  }
}

string takeInput(tBoard board) {
  string input;
  int ch;
  do {
    printw("\nPress Arrow Key: ");
    ch = getch();

    input = (ch == KEY_UP)      ? "u"
            : (ch == KEY_DOWN)  ? "d"
            : (ch == KEY_LEFT)  ? "l"
            : (ch == KEY_RIGHT) ? "r"
                                : "e";
    printw("%s\n", input.c_str());
    refresh();

  } while (input == "e");

  return input;
}

tBoard moveBoard(tBoard board, const string& direction) {
  tBoard originalBoard = board;
  if (direction == "u") {
    for (int col = 0; col < SIZE; col++) {
      bool merged[SIZE] = {false};
      for (int row = 1; row < SIZE; row++) {
        if (board[row][col] != 0) {
          int curPos = row;
          while (curPos > 0 && board[curPos - 1][col] == 0) {
            board[curPos - 1][col] = board[curPos][col];
            board[curPos][col] = 0;
            curPos--;
          }
          if (curPos > 0 && board[curPos - 1][col] == board[curPos][col] &&
              !merged[curPos - 1]) {
            board[curPos - 1][col] *= 2;
            board[curPos][col] = 0;
            merged[curPos - 1] = true;
          }
        }
      }
    }
  } else if (direction == "l") {
    for (int row = 0; row < SIZE; row++) {
      bool merged[SIZE] = {false};
      for (int col = 1; col < SIZE; col++) {
        if (board[row][col] != 0) {
          int curPos = col;
          while (curPos > 0 && board[row][curPos - 1] == 0) {
            board[row][curPos - 1] = board[row][curPos];
            board[row][curPos] = 0;
            curPos--;
          }
          if (curPos > 0 && board[row][curPos - 1] == board[row][curPos] &&
              !merged[curPos - 1]) {
            board[row][curPos - 1] *= 2;
            board[row][curPos] = 0;
            merged[curPos - 1] = true;
          }
        }
      }
    }
  } else if (direction == "d") {
    for (int col = 0; col < SIZE; col++) {
      bool merged[SIZE] = {false};
      for (int row = SIZE - 2; row >= 0; row--) {
        if (board[row][col] != 0) {
          int curPos = row;
          while (curPos < SIZE - 1 && board[curPos + 1][col] == 0) {
            board[curPos + 1][col] = board[curPos][col];
            board[curPos][col] = 0;
            curPos++;
          }
          if (curPos < SIZE - 1 &&
              board[curPos + 1][col] == board[curPos][col] &&
              !merged[curPos + 1]) {
            board[curPos + 1][col] *= 2;
            board[curPos][col] = 0;
            merged[curPos + 1] = true;
          }
        }
      }
    }
  } else if (direction == "r") {
    for (int row = 0; row < SIZE; row++) {
      bool merged[SIZE] = {false};
      for (int col = SIZE - 2; col >= 0; col--) {
        if (board[row][col] != 0) {
          int curPos = col;
          while (curPos < SIZE - 1 && board[row][curPos + 1] == 0) {
            board[row][curPos + 1] = board[row][curPos];
            board[row][curPos] = 0;
            curPos++;
          }
          if (curPos < SIZE - 1 &&
              board[row][curPos + 1] == board[row][curPos] &&
              !merged[curPos + 1]) {
            board[row][curPos + 1] *= 2;
            board[row][curPos] = 0;
            merged[curPos + 1] = true;
          }
        }
      }
    }
  }

  return board;
}

tBoard addRandomNumbers(tBoard board) {
  vector<pair<int, int>> emptyCells;

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (board[i][j] == 0) {
        emptyCells.push_back({i, j});
      }
    }
  }

  if (!emptyCells.empty()) {
    auto [row, col] = emptyCells[rand() % emptyCells.size()];
    board[row][col] = (rand() % 2 + 1) * 2;
  }

  return board;
}

bool hasValidMoves(tBoard board) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (board[i][j] == 0) {
        return true;
      }
    }
  }

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE - 1; j++) {
      if (board[i][j] == board[i][j + 1]) {
        return true;
      }
      if (board[j][i] == board[j + 1][i]) {
        return true;
      }
    }
  }

  return false;
}

bool checkIfGameEnds(tBoard board) {
  bool won = false;
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (board[i][j] == 2048) {
        won = true;
        break;
      }
    }
    if (won) break;
  }

  if (won) {
    printw("\nCongratulations! You've reached 2048!\n");
    refresh();
    exit(0);
    return false;
  }

  if (!hasValidMoves(board)) {
    printw("\nGame Over! No valid moves left.\n");
    refresh();
    exit(0);
  }
}

int main() {
  initscr();

  keypad(stdscr, true);

  tBoard board = {{{0, 0, 0, 0}, {2, 0, 0, 0}, {0, 0, 0, 0}, {2, 0, 0, 0}}};

  bool runGame = false;

  while (true) {
    displayBoard(board);
    string direction = takeInput(board);
    board = moveBoard(board, direction);
    board = addRandomNumbers(board);
    runGame = checkIfGameEnds(board);
  }

  endwin();
  return 0;
}
