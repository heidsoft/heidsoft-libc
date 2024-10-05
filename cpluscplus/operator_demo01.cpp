#include <sstream>
#include <iostream>
using namespace std;

std::string operator+(std::string const &a, int b) {
  std::ostringstream oss;
  oss << a << b;
  return oss.str();
}

int main() {
  int i = 4;
  string text = "Player: ";
  cout << (text + i) << endl;
}
