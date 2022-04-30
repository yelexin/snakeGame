#include <termios.h>
#include <unistd.h>

#include "Board.cpp"
#include "Snake.cpp"

using namespace std;

struct termios t;

void input_enter_off() {
  tcgetattr(STDIN_FILENO, &t);
  t.c_lflag &= ~ICANON;
  tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

int main() {
  input_enter_off();
  srand(time(NULL));
  int w = 80, h = 25;
  int level = 5;
  cout << "Please choose level: 1 - 10：";
  cin >> level;
  Snake s({h / 2, w / 2}, 10);
  Board board(&s, w, h);
  board.sleepTime = -0.44 * 1e5 * level + 5.44 * 1e5;
  board.init();
  while (1) {
    board.updateMap();
    board.print();
    usleep(board.sleepTime);
  }
  return 0;
}