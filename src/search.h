#ifndef SEARCH_H
#define SEARCH_H
#include <vector>

using namespace std;

void forward_search(vector<vector<double> > &matrix, vector<int> &best_features, double &best_acc, char opt);
void backward_search(vector<vector<double> > &matrix, vector<int> &best_features, double &best_acc);

#endif