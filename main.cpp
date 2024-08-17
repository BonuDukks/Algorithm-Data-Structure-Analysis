#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

using namespace std;

// Function to make the length of the numbers the same
void makeLengthSame(string &num1, string &num2) {
  int max_len = max(num1.size(), num2.size());
  while (num1.size() < max_len) {
    num1 = "0" + num1;
  }
  while (num2.size() < max_len) {
    num2 = "0" + num2;
  }
}

// Function to add two numbers
string add(string num1, string num2, int base) {
  makeLengthSame(num1, num2);
  string result = "";
  int carry = 0;
  for (int i = num1.size() - 1; i >= 0; --i) {
    int sum = (num1[i] - '0') + (num2[i] - '0') + carry;
    carry = sum / base;
    result = char((sum % base) + '0') + result;
  }
  if (carry == 1) {
    result = char(carry + '0') + result;
  }
  return result;
}

// Function to subtract two numbers
string subtract(string num1, string num2, int base) {
  makeLengthSame(num1, num2);
  string result = "";
  int borrow = 0;
  for (int i = num1.size() - 1; i >= 0; --i) {
    int sub = (num1[i] - '0') - (num2[i] - '0') - borrow;
    if (sub < 0) {
      sub += base;
      borrow = 1;
    } else {
      borrow = 0;
    }
    result = char(sub + '0') + result;
  }
  // Remove leading zeros
  result.erase(0, result.find_first_not_of('0'));
  if (result.empty()) {
    result = "0";
  }
  return result;
}

// Karatsuba multiplication function
string karatsuba(string num1, string num2, int base) {
  // Base case
  if (num1.size() == 1 && num2.size() == 1) {
    int input1 = stoi(num1);
    int input2 = stoi(num2);
    int product = input1 * input2;

    string result = "";
    if (product == 0) {
      return "0";
    }

    while (product > 0) {
      result = char((product % base) + '0') + result;
      product = product / base;
    }

    return result;
  }

  // Make lengths of num1 and num2 equal
  makeLengthSame(num1, num2);
  int n = num1.size();

  // Split the strings
  int half = n / 2;
  string num1Left = num1.substr(0, half);
  string num1Right = num1.substr(half);
  string num2Left = num2.substr(0, half);
  string num2Right = num2.substr(half);

  // Recursively calculate products
  string p1 = karatsuba(num1Left, num2Left, base);
  string p2 = karatsuba(num1Right, num2Right, base);
  string p3 = karatsuba(add(num1Left, num1Right, base),
                        add(num2Left, num2Right, base), base);

  p3 = subtract(p3, add(p1, p2, base), base);

  // Add the zeroes
  for (int i = 0; i < 2 * (n - half); i++) {
    p1 += "0";
  }
  for (int i = 0; i < n - half; ++i) {
    p3 += "0";
  }

  // Add all parts
  string result = add(add(p1, p3, base), p2, base);
  return result;
}

int main() {
  string num1, num2;
  int base;

  cin >> num1 >> num2 >> base;

  string sum = add(num1, num2, base);
  string product = karatsuba(num1, num2, base);

  product.erase(0, product.find_first_not_of('0'));
  if (product.empty()) {
    product = "0";
  }
  cout << sum << " " << product << " 0" << endl;

  return 0;
}
