#include <iostream>
#include <vector>

int main() {
  std::vector<int> myVector; 

  myVector.push_back(10);
  myVector.push_back(20);
  myVector.push_back(30);

  for (int i = 0; i < myVector.size(); ++i) {
    std::cout << myVector[i] << " ";
  }
  std::cout << std::endl;

  myVector.push_back(40);

  std::cout << "Size of the vector: " << myVector.size() << std::endl;

  return 0;
}