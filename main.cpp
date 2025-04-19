#include <chrono>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

void black(int cell) { std::cout << "\033[48;5;0m" << cell << "\033[0m"; }
void white(int cell) { std::cout << "\033[48;5;15m" << cell << "\033[0m"; }

const int WIDTH = 50;
const int HEIGHT = 25;

int map[HEIGHT][WIDTH];

void sleep(double seconds) {
  int milliseconds = static_cast<int>(seconds * 1000);
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void clear() { system("clear"); }

void readMap() {
  std::ifstream file("map.txt");

  if (!file.is_open())
    return;

  std::string line;
  int num = 0;
  while (std::getline(file, line)) {
    for (int i = 0; i < WIDTH; i++) {
      map[num][i] = line[i] - '0';
    }
    num++;
  }
  file.close();
}

void print() {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      int cell = map[i][j];

      if (cell == 0)
        black(cell);
      else
        white(cell);
    }
    std::cout << std::endl;
  }
}

void checkNeighbour(int &alive, int &dead, int i, int j) {
  int cell = map[i][j];

  if (cell == 0)
    dead++;
  else if (cell == 1)
    alive++;
}

void checkNeighbors(int &alive, int &dead, int i, int j) {
  // Top neighbors
  if (i != 0)
    checkNeighbour(alive, dead, i - 1, j);
  if (i != 0 && j != 0)
    checkNeighbour(alive, dead, i - 1, j - 1);
  if (i != 0 && j != WIDTH - 1)
    checkNeighbour(alive, dead, i - 1, j + 1);

  // Neighbors on the sides
  if (j != 0)
    checkNeighbour(alive, dead, i, j - 1);
  if (j != WIDTH - 1)
    checkNeighbour(alive, dead, i, j + 1);

  // Bottom neighbors
  if (i != HEIGHT - 1)
    checkNeighbour(alive, dead, i + 1, j);
  if (i != HEIGHT - 1 && j != 0)
    checkNeighbour(alive, dead, i + 1, j - 1);
  if (i != HEIGHT - 1 && j != WIDTH - 1)
    checkNeighbour(alive, dead, i + 1, j + 1);
}

void check() {
  int newMap[HEIGHT][WIDTH]{};

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      int alive = 0;
      int dead = 0;

      checkNeighbors(alive, dead, i, j);

      if (map[i][j] == 0 && alive == 3)
        newMap[i][j] = 1;

      else if (map[i][j] == 1 && (alive < 2 || alive > 3))
        newMap[i][j] = 0;

      else
        newMap[i][j] = map[i][j];
    }
  }

  std::memcpy(map, newMap, sizeof(map));
}

int main() {
  readMap();

  while (1) {
    clear();
    print();
    check();
    sleep(0.5);
  }

  return 0;
}
