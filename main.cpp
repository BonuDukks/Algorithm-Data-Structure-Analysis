#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

vector<int>* schoolSubtraction(vector<int> num1, vector<int> num2, int base) {
  vector<int>* sum = new vector<int>;
  vector<int>* longerInt;
  vector<int>* shorterInt;
  vector<int>* biggerInt;
  vector<int>* smallerInt;
  vector<int> biggerIntCopy;

  int lengthDifference = num1.size() - num2.size();
  lengthDifference = abs(lengthDifference);

  if (num1.size() > num2.size()) {
    longerInt = &num1;
    shorterInt = &num2;

  } else if (num1.size() < num2.size()) {
    longerInt = &num2;
    shorterInt = &num1;
  } else {
    longerInt = &num1;
  }

  if (lengthDifference != 0) {
    for (int i = 0; i < lengthDifference; i++) {
      shorterInt->insert(shorterInt->begin(), 0);
    }
  }

  for (int i = 0; i < longerInt->size(); i++) {
    if (num1[i] > num2[i]) {
      biggerInt = &num1;
      smallerInt = &num2;
      break;
    } else if (num1[i] < num2[i]) {
      biggerInt = &num2;
      smallerInt = &num1;
      break;
    }
  }

  for (int i = 0; i < longerInt->size(); i++) {
    biggerIntCopy[i] = (*biggerInt)[i];
  }

  for (int i = longerInt->size() - 1; i >= 0; i--) {
    if (biggerIntCopy[i] - (*smallerInt)[i] < 0) {
      biggerIntCopy[i + 1] = biggerIntCopy[i + 1] - 1;
      biggerIntCopy[i] = biggerIntCopy[i] + base;
      sum->insert(sum->begin(), biggerIntCopy[i] - (*smallerInt)[i]);
    } else {
      sum->insert(sum->begin(), biggerIntCopy[i] - (*smallerInt)[i]);
    }
  }

  return sum;
}

vector<int>* schoolAddition(vector<int> num1, vector<int> num2, int base) {
  vector<int>* sum = new vector<int>;
  vector<int>* longerInt;
  vector<int>* shorterInt;

  int lengthDifference = num1.size() - num2.size();
  lengthDifference = abs(lengthDifference);

  if (num1.size() > num2.size()) {
    longerInt = &num1;
    shorterInt = &num2;

  } else if (num1.size() < num2.size()) {
    longerInt = &num2;
    shorterInt = &num1;
  } else {
    longerInt = &num1;
  }

  if (lengthDifference != 0) {
    for (int i = 0; i < lengthDifference; i++) {
      shorterInt->insert(shorterInt->begin(), 0);
    }
  }

  int carry = 0;

  for (int i = longerInt->size() - 1; i >= 0; i--) {
    if (i == 0) {
      if (num1[i] + num2[i] + carry > base - 1) {
        // cout << "Front: (num1 + num2 + carry) - base: " << ((*num1)[i] +
        // (*num2)[i] + carry) - base << endl;
        sum->insert(sum->begin(), (num1[i] + num2[i] + carry) - base);
        carry = 1;
        sum->insert(sum->begin(), carry);
        break;
      } else {
        // cout << "Front: num1 + num2 + carry: " << num1[i] + num2[i] + carry
        // << endl;
        sum->insert(sum->begin(), num1[i] + num2[i] + carry);
        carry = 0;
        break;
      }
    }

    if (num1[i] + num2[i] + carry > base - 1) {
      // cout << "(num1 + num2 + carry) - base: " << (num1[i] + num2[i] + carry)
      // - base << endl;
      sum->insert(sum->begin(), (num1[i] + num2[i] + carry) - base);
      carry = 1;
    } else {
      // cout << "num1 + num2 + carry: " << num1[i] + num2[i] + carry << endl;
      sum->insert(sum->begin(), num1[i] + num2[i] + carry);
      carry = 0;
    }
  }
  return sum;
}

vector<int>* karatsuba(vector<int>* num1, vector<int>* num2, int base) {
  if ((num1->size() <= 4) || (num2->size() <= 4)) {
    // Multiply the two numbers.

    vector<int>* longerNumber;
    vector<int>* shorterNumber;

    if ((num1->size() > num2->size()) || (num1->size() == num2->size())) {
      longerNumber = num1;
      shorterNumber = num2;
    } else {
      longerNumber = num2;
      shorterNumber = num1;
    }

    int numOfPp = shorterNumber->size();
    vector<int>* partial[numOfPp];

    for (int i = shorterNumber->size() - 1; i >= 0; i--) {
      partial[i] = new vector<int>;

      for (int k = 0; k < (shorterNumber->size() - 1 - i); k++) {
        partial[i]->insert(partial[i]->begin(), 0);
      }
      int carry = 0;
      for (int j = longerNumber->size() - 1; j >= 0; j--) {
        int product = (*shorterNumber)[i] * (*longerNumber)[j] + carry;

        if (j == 0) {
          if (product > base - 1) {
            carry = floor(product / base);
            int remainder = product % base;
            partial[i]->insert(partial[i]->begin(), remainder);
            partial[i]->insert(partial[i]->begin(), carry);
            break;
            // partialProduct = partialProduct + (remainder * pow(10,
            // longerNumber->size() - 1 - j)+ (shorterNumber->size() - 1 - i));
          } else {
            carry = 0;
            partial[i]->insert(partial[i]->begin(), product);
            break;
            // partialProduct = partialProduct + (product * pow(10,
            // longerNumber->size() - 1 - j) + (shorterNumber->size() - 1 - i));
          }
        }

        if (product > base - 1) {
          carry = floor(product / base);
          int remainder = product % base;
          partial[i]->insert(partial[i]->begin(), remainder);
          // partialProduct = partialProduct + (remainder * pow(10,
          // longerNumber->size() - 1 - j)+ (shorterNumber->size() - 1 - i));
        } else {
          carry = 0;
          partial[i]->insert(partial[i]->begin(), product);
          // partialProduct = partialProduct + (product * pow(10,
          // longerNumber->size() - 1 - j) + (shorterNumber->size() - 1 - i));
        }
      }
    }

    vector<int>* firstSum;
    vector<int>* secondSum;

    switch (numOfPp) {
      case 1:

        return partial[0];
      case 2:

        return schoolAddition(*partial[0], *partial[1], base);
      case 3:

        firstSum = schoolAddition(*partial[2], *partial[1], base);
        return schoolAddition(*partial[0], *firstSum, base);
      case 4:

        firstSum = schoolAddition(*partial[3], *partial[2], base);
        secondSum = schoolAddition(*partial[1], *partial[0], base);
        return schoolAddition(*firstSum, *secondSum, base);
    }
  }
  vector<int> num1Left, num1Right, num2Left, num2Right;
  vector<int>* p1;
  vector<int>* p2;
  vector<int>* p3;

  int size = max(num1->size(), num2->size());

  int indexLeft = floor(size / 2);
  int indexRight = ceil(size / 2);

  for (int i = 0; i < indexLeft; i++) {
    num1Left.push_back((*num1)[i]);
    num2Left.push_back((*num2)[i]);
  }

  for (int i = indexRight; i < num1->size(); i++) {
    num1Right.push_back((*num1)[i]);
    num2Right.push_back((*num2)[i]);
  }

  // P1 = num1Left * num2Left
  p1 = karatsuba(&num1Left, &num2Left, base);

  // P2 = num1Right * num2Right
  p2 = karatsuba(&num1Right, &num2Right, base);
  // P3 = ((num1Left + num1Right) * (num2Left + num2Right)) - P1 - P2

  vector<int>* p3Part1 = schoolAddition(num1Left, num1Right, base);
  vector<int>* p3Part2 = schoolAddition(num2Left, num2Right, base);
  vector<int>* p3Part3 = karatsuba(p3Part1, p3Part2, base);
  vector<int>* p3Part6 = schoolSubtraction(*p3Part3, *p1, base);

  p3 = schoolSubtraction(*p3Part6, *p2, base);

  for (int i = 0; i < indexRight * 2; i++) {
    p1->push_back(0);
  }

  for (int i = 0; i < indexRight; i++) {
    p3->push_back(0);
  }

  // Last call return P1 + P3 + P2;

  vector<int>* sumP1 = schoolAddition(*p1, *p3, base);

  return schoolAddition(*sumP1, *p2, base);
};

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
    for (int i = 0; i < lengthDifference; i++) {
      shorterInt->insert(shorterInt->begin(), 0);
    }
  }

  for (int i = longerInt->size() - 1; i >= 0; i--) {
    if (i == 0) {
      if (num1[i] + num2[i] + carry > base - 1) {
        sum.insert(sum.begin(), (num1[i] + num2[i] + carry) - base);
        carry = 1;
        sum.insert(sum.begin(), carry);
        break;
      } else {
        sum.insert(sum.begin(), num1[i] + num2[i] + carry);
        carry = 0;
        break;
      }
    }

    if (num1[i] + num2[i] + carry > base - 1) {
      sum.insert(sum.begin(), (num1[i] + num2[i] + carry) - base);
      carry = 1;
    } else {
      sum.insert(sum.begin(), num1[i] + num2[i] + carry);
      carry = 0;
    }
  }

  for (int i = 0; i < sum.size(); i++) {
    cout << sum[i];
  }

  cout << " ";

  for (int i = 0; i < product->size(); i++) {
    cout << (*product)[i];
  }

  cout << " 0" << endl;

  return 0;
}