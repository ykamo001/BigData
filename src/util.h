#ifndef UTIL_H
#define UTIL_H
#include <map>
#include <vector>

using namespace std;

void fix(map<double, double> &neighbors, double dist, double qual);
void validate(double qual, map<double, double> &neighbors, double &correct, double &tested);
void normalize(vector<vector<double> > &matrix);
bool checker(vector<int> index_features, int index);
void adder(vector<vector<double> > &deter_list, const vector<vector<double> > &matrix, int j);
void remover(vector<vector<double> > &deter_list,  vector<vector<double> > matrix, int j, vector<int> features);
void feat_remove(vector<int> &to_remove, vector<int> last, int j);

#endif