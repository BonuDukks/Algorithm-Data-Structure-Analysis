#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

// Example implementations for schoolAddition and schoolSubtraction
std::vector<int>* schoolAddition(std::vector<int> num1, std::vector<int> num2,
                                 int base) {
  int maxSize = std::max(num1.size(), num2.size());
  num1.insert(num1.begin(), maxSize - num1.size(), 0);
  num2.insert(num2.begin(), maxSize - num2.size(), 0);

  std::vector<int>* sum = new std::vector<int>(maxSize + 1, 0);
  int carry = 0;
  for (int i = maxSize - 1; i >= 0; --i) {
    int total = num1[i] + num2[i] + carry;
    (*sum)[i + 1] = total % base;
    carry = total / base;
  }
  (*sum)[0] = carry;

  // Remove leading zeros
  while (sum->size() > 1 && sum->front() == 0) {
    sum->erase(sum->begin());
  }
  return sum;
}

std::vector<int>* schoolSubtraction(std::vector<int> num1,
                                    std::vector<int> num2, int base) {
  int maxSize = std::max(num1.size(), num2.size());
  num1.insert(num1.begin(), maxSize - num1.size(), 0);
  num2.insert(num2.begin(), maxSize - num2.size(), 0);

  std::vector<int>* diff = new std::vector<int>(maxSize, 0);
  int borrow = 0;
  for (int i = maxSize - 1; i >= 0; --i) {
    int total = num1[i] - num2[i] - borrow;
    if (total < 0) {
      total += base;
      borrow = 1;
    } else {
      borrow = 0;
    }
    (*diff)[i] = total;
  }

  // Remove leading zeros
  while (diff->size() > 1 && diff->front() == 0) {
    diff->erase(diff->begin());
  }
  return diff;
}

std::vector<int>* karatsuba(std::vector<int>* num1, std::vector<int>* num2,
                            int base) {
  // Base case for recursion
  if ((num1->size() <= 4) || (num2->size() <= 4)) {
    // Direct multiplication when numbers are small enough
    int n = num1->size(), m = num2->size();
    std::vector<int>* result = new std::vector<int>(n + m, 0);

    for (int i = n - 1; i >= 0; --i) {
      int carry = 0;
      for (int j = m - 1; j >= 0; --j) {
        int product = (*num1)[i] * (*num2)[j] + (*result)[i + j + 1] + carry;
        (*result)[i + j + 1] = product % base;
        carry = product / base;
      }
      (*result)[i] += carry;
    }

    // Removing leading zeros
    while (result->size() > 1 && result->front() == 0) {
      result->erase(result->begin());
    }
    return result;
  }

  // Pad numbers with leading zeros if necessary
  int size = std::max(num1->size(), num2->size());
  if (num1->size() < size) num1->insert(num1->begin(), size - num1->size(), 0);
  if (num2->size() < size) num2->insert(num2->begin(), size - num2->size(), 0);

  int half = size / 2;

  // Split the numbers
  std::vector<int> low1(num1->begin(), num1->begin() + half);
  std::vector<int> high1(num1->begin() + half, num1->end());
  std::vector<int> low2(num2->begin(), num2->begin() + half);
  std::vector<int> high2(num2->begin() + half, num2->end());

  // Recursive multiplications
  std::vector<int>* z0 = karatsuba(&low1, &low2, base);
  std::vector<int>* z1 = karatsuba(&high1, &high2, base);

  // Sum the splits
  std::vector<int>* sum1 = schoolAddition(low1, high1, base);
  std::vector<int>* sum2 = schoolAddition(low2, high2, base);

  // Intermediate product
  std::vector<int>* z2 = karatsuba(sum1, sum2, base);

  // Calculating z2 - z1 - z0
  std::vector<int>* sub1 = schoolSubtraction(*z2, *z1, base);
  std::vector<int>* z = schoolSubtraction(*sub1, *z0, base);

  // Shift z1 by 2*half positions and z by half positions
  for (int i = 0; i < 2 * half; ++i) {
    z1->push_back(0);
  }
  for (int i = 0; i < half; ++i) {
    z->push_back(0);
  }

  // Final result = z1 + z + z0
  std::vector<int>* result = schoolAddition(*z1, *z, base);
  std::vector<int>* finalResult = schoolAddition(*result, *z0, base);

  // Cleanup
  delete z0;
  delete z1;
  delete z2;
  delete sub1;
  delete sum1;
  delete sum2;
  delete result;

  // Removing leading zeros
  while (finalResult->size() > 1 && finalResult->front() == 0) {
    finalResult->erase(finalResult->begin());
  }

  return finalResult;
}

int main() {
  string input1;
  string input2;
  int base;
  int carry = 0;

  cin >> input1 >> input2 >> base;

  vector<int> num1;
  vector<int> num2;
  vector<int> sum;
  vector<int>* longerInt;
  vector<int>* shorterInt;

  int lengthDifference = input1.length() - input2.length();
  lengthDifference = abs(lengthDifference);

  for (int i = 0; i < input1.length(); i++) {
    num1.push_back(input1[i] - '0');
  }

  for (int i = 0; i < input2.length(); i++) {
    num2.push_back(input2[i] - '0');
  }

  vector<int>* product = karatsuba(&num1, &num2, base);

  if (input1.length() > input2.length()) {
    longerInt = &num1;
    shorterInt = &num2;

  } else if (input1.length() < input2.length()) {
    longerInt = &num2;
    shorterInt = &num1;
  } else {
    longerInt = &num1;
  }

  if (lengthDifference != 0) {
    cout << "Yo" << endl;
    for (int i = 0; i < lengthDifference; i++) {
      shorterInt->insert(shorterInt->begin(), 0);
    }
  }

  for (int i = longerInt->size() - 1; i >= 0; i--) {
    if (i == 0) {
      if (num1[i] + num2[i] + carry > base - 1) {
        cout << "Front: (num1 + num2 + carry) - base: "
             << (num1[i] + num2[i] + carry) - base << endl;
        sum.insert(sum.begin(), (num1[i] + num2[i] + carry) - base);
        carry = 1;
        sum.insert(sum.begin(), carry);
        break;
      } else {
        cout << "Front: num1 + num2 + carry: " << num1[i] + num2[i] + carry
             << endl;
        sum.insert(sum.begin(), num1[i] + num2[i] + carry);
        carry = 0;
        break;
      }
    }

    if (num1[i] + num2[i] + carry > base - 1) {
      cout << "(num1 + num2 + carry) - base: "
           << (num1[i] + num2[i] + carry) - base << endl;
      sum.insert(sum.begin(), (num1[i] + num2[i] + carry) - base);
      carry = 1;
    } else {
      cout << "num1 + num2 + carry: " << num1[i] + num2[i] + carry << endl;
      sum.insert(sum.begin(), num1[i] + num2[i] + carry);
      carry = 0;
    }
  }

  for (int i = 0; i < sum.size(); i++) {
    cout << sum[i];
  }

  cout << " ";

  cout << "Product Size: " << product->size() << endl;

  for (int i = 0; i < product->size(); i++) {
    cout << (*product)[i] << " ";
  }

  cout << endl;

  return 0;
}