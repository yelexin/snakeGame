#ifndef BOARD_H
#define BOARD_H
#include <pthread.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

#include "Snake.cpp"

using namespace std;
const int N = 100;
void *input_thread_work(void *arg);
class Board {
 private:
  char map[N][N];
  int w, h;
  PII food;
  pthread_t input_thread;

 public:
  Snake *snake;
  int sleepTime = 1000000;
  Board(Snake *snake, int width, int height)
      : snake(snake), w(width), h(height){};
  /**
   * @brief init the board, generate food at a random point, and start a new thread to handle user input
   *
   */
  void init() {
    for (int i = 0; i < h; i++) {
      for (int j = 0; j < w; j++) {
        if (i == 0 || i == h - 1 || j == 0 || j == w - 1) {
          map[i][j] = '#';
        } else {
          map[i][j] = ' ';
        }
      }
    }
    generateFood();
    pthread_create(&input_thread, NULL, input_thread_work, this);
  }
  void setSleepTime(int t) { sleepTime = t; }
  void generateFood() {
    int x;
    int y;
    do {
      x = rand() % h;
      y = rand() % w;
    } while (map[x][y] != ' ');
    food = {x, y};
  }
  void clearMap() {
    for (int i = 1; i < h - 1; i++) {
      for (int j = 1; j < w - 1; j++) {
        map[i][j] = ' ';
      }
    }
  }
  /**
   * @brief clean the board, let the snake move, detect whether it is dead and repaint
   * 
   */
  void updateMap() {
    clearMap();
    snake->move();
    auto loc = snake->getLoc();
    for (auto &v : loc) {
      map[v.first][v.second] = 'O';
    }
    auto headLoc = snake->getHeadLoc();
    map[headLoc.first][headLoc.second] = '@';

    if (isOutOfBound() || isEatingSelf()) {
      cout << "You are dead." << endl;
      exit(0);
    }

    if (headLoc == food) {
      snake->grow();
      generateFood();
    }

    map[food.first][food.second] = '*';
  }
  bool isOutOfBound() {
    auto p = snake->getHeadLoc();
    return p.first <= 0 || p.first >= h - 1 || p.second <= 0 ||
           p.second >= w - 1;
  }
  bool isEatingSelf() {
    auto body = snake->getLoc();
    auto head = snake->getHeadLoc();
    for (int i = 1; i < body.size(); i++) {
      auto &v = body[i];
      if (v.first == head.first && v.second == head.second) {
        return true;
      }
    }
    return false;
  }
  /**
   * @brief print the board
   * 
   */
  void print() {
    for (int i = 0; i < h; i++) {
      for (int j = 0; j < w; j++) {
        cout << map[i][j];
      }
      cout << endl;
    }
  }
};
void *input_thread_work(void *arg) {
  Board *board = (Board *)arg;
  while (true) {
    char dir = cin.get();
    switch (dir) {
      case 'w':
        board->snake->changeDir('u', board->sleepTime);
        break;
      case 'a':
        board->snake->changeDir('l', board->sleepTime);
        break;
      case 's':
        board->snake->changeDir('d', board->sleepTime);
        break;
      case 'd':
        board->snake->changeDir('r', board->sleepTime);
        break;
      default:
        break;
    }
  }
}
#endif
