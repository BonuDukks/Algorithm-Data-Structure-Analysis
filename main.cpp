#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

int charToCost(char c) {
  if ('A' <= c && c <= 'Z') return c - 'A';  // 0 to 25
  return c - 'a' + 26;                       // 26 to 51
}

void dfs(int city, vector<vector<int>>& country, vector<bool>& visited,
         vector<int>& component) {
  visited[city] = true;
  component.push_back(city);
  for (int i = 0; i < country.size(); ++i) {
    if (country[city][i] == 1 && !visited[i]) {
      dfs(i, country, visited, component);
    }
  }
}

int main() {
  string input;
  getline(cin, input);

  // Split input
  size_t first_space = input.find(' ');
  size_t second_space = input.find(' ', first_space + 1);

  string country_input = input.substr(0, first_space);
  string build_input =
      input.substr(first_space + 1, second_space - first_space - 1);
  string destroy_input = input.substr(second_space + 1);

  // Create 2D arrays
  vector<vector<int>> country;
  vector<vector<int>> build;
  vector<vector<int>> destroy;

  // Parse the country input
  size_t n = 0;
  for (char c : country_input) {
    if (c == ',') n++;
  }
  n++;  // Count the last row

  country.resize(n, vector<int>(n, 0));
  build.resize(n, vector<int>(n, 0));
  destroy.resize(n, vector<int>(n, 0));

  // Fill the country adjacency matrix
  size_t row = 0, col = 0;
  for (char c : country_input) {
    if (c == ',') {
      col = 0;
      row++;
    } else {
      country[row][col++] = c - '0';
    }
  }

  // Fill the build cost matrix
  row = 0;
  col = 0;
  for (char c : build_input) {
    if (c == ',') {
      col = 0;
      row++;
    } else {
      build[row][col++] = charToCost(c);
    }
  }

  // Fill the destroy cost matrix
  row = 0;
  col = 0;
  for (char c : destroy_input) {
    if (c == ',') {
      col = 0;
      row++;
    } else {
      destroy[row][col++] = charToCost(c);
    }
  }

  // Finding connected components
  vector<bool> visited(n, false);
  vector<vector<int>> components;

  for (int i = 0; i < n; ++i) {
    if (!visited[i]) {
      vector<int> component;
      dfs(i, country, visited, component);
      components.push_back(component);
    }
  }

  // Calculate costs
  int total_cost = 0;

  // For each component, calculate the number of edges and cost of destroying
  // excess edges
  for (const auto& component : components) {
    int edges = 0;
    vector<int> edge_costs;

    for (int u : component) {
      for (int v : component) {
        if (u < v && country[u][v] == 1) {
          edges++;
          edge_costs.push_back(destroy[u][v]);
        }
      }
    }

    // If the number of edges is more than (size - 1), we need to destroy roads
    int component_size = component.size();
    int needed_edges = component_size - 1;

    if (edges > needed_edges) {
      sort(edge_costs.begin(), edge_costs.end());
      for (int i = 0; i < edges - needed_edges; ++i) {
        total_cost += edge_costs[i];
      }
    }
  }

  // Connect components
  int num_components = components.size();
  vector<int> min_edge_cost(num_components, numeric_limits<int>::max());

  for (int i = 0; i < num_components; ++i) {
    for (int j = i + 1; j < num_components; ++j) {
      // Try to connect component i and j
      for (int u : components[i]) {
        for (int v : components[j]) {
          min_edge_cost[j] = min(min_edge_cost[j], build[u][v]);
        }
      }
    }
  }

  for (int i = 1; i < num_components; ++i) {
    total_cost += min_edge_cost[i];
  }

  cout << total_cost << endl;

  return 0;
}
