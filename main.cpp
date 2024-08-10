#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  string input1;
  string input2;
  int base;
  int carry = 0;

  cin >> input1 >> input2 >> base;

  vector<int> num1;
  vector<int> num2;
  vector<int> sum;
  vector<int>::iterator sumIt;
  vector<int>* longerInt;
  vector<int>* shorterInt;
  bool isFront = false;

  int lengthDifference = input1.length() - input2.length();
  lengthDifference = abs(lengthDifference);

  for (int i = 0; i < input1.length(); i++) {
    num1.push_back(input1[i] - '0');
  }

  for (int i = 0; i < input2.length(); i++) {
    num2.push_back(input2[i] - '0');
  }

  if (input1.length() > input2.length()) {
    sumIt = num1.end();
    longerInt = &num1;
    shorterInt = &num2;

  } else if (input1.length() < input2.length()) {
    sumIt = num2.end();
    longerInt = &num2;
    shorterInt = &num1;
  } else {
    sumIt = num1.end();
    longerInt = &num1;
  }

  if (lengthDifference != 0) {
    cout << "Yo" << endl;
    for (int i = 0; i < lengthDifference; i++) {
      shorterInt->insert(shorterInt->begin(), 0);
    }
  }

  vector<int>::iterator it1 = num1.end() - 1;
  vector<int>::iterator it2 = num2.end() - 1;

  while (!isFront) {
    if (sumIt == longerInt->begin()) {
      if (*it1 + *it2 + carry > base - 1) {
        sum.push_back((*it1 + *it2 + carry) - base);
        carry = 1;
        sum.push_back(carry);
        break;
      } else {
        sum.push_back((*it1 + *it2 + carry));
        break;
      }
    }

    if (*it1 + *it2 + carry > base - 1) {
      cout << "(*it1 + *it2 + carry) - base: " << (*it1 + *it2 + carry) - base
           << endl;
      sum.push_back((*it1 + *it2 + carry) - base);
      carry = 1;
    } else {
      cout << "*it1 + *it2 + carry: " << *it1 + *it2 + carry << endl;
      sum.push_back(*it1 + *it2 + carry);
      carry = 0;
    }

    --it1;
    --it2;
    --sumIt;
  }

  cout << sum.size() << endl;

  for (sumIt = sum.end() - 2; sumIt >= sum.begin(); --sumIt) {
    cout << *sumIt;
  }
  cout << endl;

  return 0;
}