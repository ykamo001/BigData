#ifndef K_FOLD_H
#define K_FOLD_H
#include<vector>

using namespace std;

int k_fold_size(int matrixSize);
void k_fold(vector<vector<double> > &hold, vector<vector<double> > &test, double &correct, double &tested);
void k_fold_neigh(vector<vector<double> > &hold, vector<vector<double> > &test, double &correct, double &tested);
double cross_validation(vector<vector<double> > matrix, char opt);

#endif