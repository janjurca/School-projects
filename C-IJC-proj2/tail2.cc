/**
 * @file tail2.cc
 * @author Jan Jurča xjurca08
 * @date 15.4.2017
 * FIT VUTBR
 * gcc version 6.3.1
 */
#include <iostream>
#include <queue>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
using namespace std;

int main(int argc, char const *argv[]) {
  std::ios::sync_with_stdio(false);
  bool std_in = false;
  size_t n = 10;
  ifstream stream;
  switch (argc) {
    case 1:
      std_in = true;
      break;
    case 2:
      stream.open(argv[1]);
      break;
      case 3:
        if (strcmp(argv[1],"-n") == 0) {
          if (isdigit(argv[2][0])) {
            n = atoi(argv[2]);
          } else {
            std::cerr << "Neplatny argument:" << " " << argv[2] << '\n';
            return 0;
          }
        } else {
          std::cerr << "Neplatny argument:" << " " << argv[1] << '\n';
          return 0;
        }
        std_in = true;
        break;
      case 4:
        if (strcmp(argv[1],"-n") == 0) {
          if (isdigit(argv[2][0])) {
            n = atoi(argv[2]);
          } else {
            std::cerr << "Neplatny argument:" << " " << argv[2] << '\n';
            return 0;
          }
        } else {
          std::cerr << "Neplatny argument:" << " " << argv[1] << '\n';
          return 0;
        }
        stream.open(argv[3]);
        break;
  }

  if (!std_in) { // test if input stream file is opened
    if (!stream.is_open()) {
      cerr << "Nepodařilo se otevřít soubor: " << argv[argc - 1] << endl;
      return 0;
    }
  }

  queue<string> container;
  string line;
  while (1) { // read from stream all lines and save them to container
    if (std_in) {
      getline(cin,line);
      if (cin.eof()) {
        if (!line.empty()) {
          container.push(line);
        }
        break;
      }
    } else {
      getline(stream,line);
      if (stream.eof()) {
        if (!line.empty()) {
          container.push(line);
        }
        break;
      }

    }
    container.push(line);
    if (container.size() > n) {
      container.pop();
    }
  } // end of reading lines
  size_t line_count = container.size(); // write the rest of lines
  for (size_t i = 0; i < line_count; i++) {
    cout << container.front() << endl;
    container.pop();
  }
  return 0;
}
