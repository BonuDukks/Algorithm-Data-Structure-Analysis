#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

// Converts the char to an int that represents its cost.
int charToCost(char c) {
  if ('A' <= c && c <= 'Z') {
    return c - 'A';  // 0 to 25
  } else {
    return c - 'a' + 26;  // 26 to 51
  }
}

// Finds all cities in a given connected component.
void dfs(int city, vector<vector<int>>& country, vector<bool>& visited,
         vector<int>& component) {
  visited[city] = true;
  component.push_back(city);  // Adds the city to the component
  for (int i = 0; i < country.size(); i++) {
    if (country[city][i] == 1 &&
        !visited[i]) {  // If there's a road connecting 2 cities, and city isn't
                        // visited, call dfs on the city and mark it as visited.
      dfs(i, country, visited, component);
    }
  }
}

int main() {
  string countryInput, buildInput, destroyInput;
  cin >> countryInput >> buildInput >> destroyInput;

  // Iterate over Input, if ',' read, new row.
  int n = 0;
  for (char c : countryInput) {
    if (c == ',') {
      n++;
    }
  }
  // Count the last row
  n++;

  // Intialise country, build and destory matrix
  vector<vector<int>> country(n, vector<int>(n, 0));
  vector<vector<int>> build(n, vector<int>(n, 0));
  vector<vector<int>> destroy(n, vector<int>(n, 0));

  // Fill the country, build and destory matrix
  int row = 0, col = 0;
  for (int i = 0; i < countryInput.length(); i++) {
    if (countryInput[i] == ',') {  // if comma is reached, start new row.
      col = 0;
      row++;
    } else {
      country[row][col] = countryInput[i] - '0';
      build[row][col] = charToCost(buildInput[i]);
      destroy[row][col] = charToCost(destroyInput[i]);
      col++;
    }
  }

  // Finding connected components
  vector<bool> visited(n, false);
  vector<vector<int>> components;

  for (int i = 0; i < n; i++) {
    if (!visited[i]) {
      vector<int> component;
      dfs(i, country, visited, component);
      components.push_back(component);  // Add each connected component
    }
  }

  int totalCost = 0;

  // For each component, calculate the number of edges and cost of destroying
  // excess edges
  for (int i = 0; i < components.size(); i++) {
    const auto& component = components[i];
    int edges = 0;
    vector<int> edgeCosts;

    for (int j = 0; j < component.size(); j++) {
      int u = component[j];
      for (int k = j + 1; k < component.size();
           k++) {  // Start from j + 1 to avoid duplicates
        int v = component[k];
        if (country[u][v] == 1) {
          edges++;
          edgeCosts.push_back(
              destroy[u][v]);  // Keep track of potential road deletions
        }
      }
    }

    // Calculate excess roads (edges - neededEdges), and destroy extras.
    int componentSize = component.size();
    int neededEdges = componentSize - 1;

    if (edges > neededEdges) {
      sort(edgeCosts.begin(), edgeCosts.end());  // Sort destruction costs
      for (int i = 0; i < edges - neededEdges; i++) {
        totalCost = totalCost + edgeCosts[i];  // Add cost of destroying roads
      }
    }
  }

  // Connect components to get all of them in a single connected compoenet.
  int numComponents = components.size();
  vector<int> minEdgeCost(numComponents, numeric_limits<int>::max());

  for (int i = 0; i < numComponents; i++) {
    for (int j = i + 1; j < numComponents; j++) {
      // Get min cost to connect component i and j
      for (int u : components[i]) {
        for (int v : components[j]) {
          minEdgeCost[j] = min(minEdgeCost[j], build[u][v]);
        }
      }
    }
  }

  // Add cost of building roads to total cost
  for (int i = 1; i < numComponents; i++) {
    totalCost = totalCost + minEdgeCost[i];
  }

  cout << totalCost << endl;

  return 0;
}
