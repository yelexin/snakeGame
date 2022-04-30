#ifndef SNAKE_H
#define SNAKE_H
#include <ctime>
#include <utility>
#include <vector>
using namespace std;

typedef pair<int, int> PII;

class SnakeNode {
 public:
  PII pos;
  char dir;
  SnakeNode(PII pos, char dir) : pos(pos), dir(dir){};
};

class Snake {
 private:
  PII head;
  PII tail;
  char dir = 'r';
  char tdir = 'r';
  vector<SnakeNode*> body;
  clock_t lastChangeDir = clock();

 public:
 /**
  * @brief Construct a new Snake
  * 
  * @param head the coordinate of the head
  * @param len the length of the snake
  */
  Snake(PII head, int len) {
    this->head = head;
    for (int i = head.second; i >= head.second - len + 1; i--) {
      cout << head.first << ',' << i << endl;
      body.push_back(new SnakeNode(make_pair(head.first, i), 'r'));
    }
  }
  /**
   * @brief function used by another thread to change the direction of the snake
   * 
   * @param dir can be u, d, l, r
   * @param sleepTime prevent from changing the direction too frequently
   */
  void changeDir(char dir, int sleepTime) {
    auto t = clock();
    if (t - lastChangeDir < sleepTime / 1000) {
      return;
    }
    lastChangeDir = clock();
    if (dir == this->dir) {
      return;
    }
    if (this->dir + dir == 'u' + 'd') {
      return;
    }
    if (this->dir + dir == 'l' + 'r') {
      return;
    }
    this->dir = dir;
  }
  /**
   * @brief move the snake according to the current status
   * 
   */
  void move() {
    tdir = body.back()->dir;
    tail = body.back()->pos;
    // each body node should follow the one ahead, and the head should follow the instruction given by the player
    for (int i = body.size() - 1; i >= 1; i--) {
      body[i]->dir = body[i - 1]->dir;
    }
    body.front()->dir = this->dir;
    for (int i = 0; i < body.size(); i++) {
      switch (body[i]->dir) {
        case 'u':
          body[i]->pos.first--;
          break;
        case 'r':
          body[i]->pos.second++;
          break;
        case 'd':
          body[i]->pos.first++;
          break;
        case 'l':
          body[i]->pos.second--;
          break;
      }
    }
  }
  /**
   * @brief when the snake eats food, it should grow longer
   * 
   */
  void grow() {
    auto node = new SnakeNode(tail, body.back()->dir);
    body.push_back(node);
  }
  /**
   * @brief Get every coordinate of the body of the snake
   * 
   * @return vector<PII> 
   */
  vector<PII> getLoc() {
    vector<PII> loc;
    for (int i = 0; i < body.size(); i++) {
      loc.push_back(body[i]->pos);
    }
    return loc;
  }
  PII getHeadLoc() { return body.front()->pos; }
};
#endif
