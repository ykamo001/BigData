#include <vector>
#include <iostream>
#include <iomanip>

#include "search.h"
#include "util.h"
#include "k_fold.h"

using namespace std;

void forward_search(vector<vector<double> > &matrix, vector<int> &best_features, double &best_acc, char opt)
{	//the forward search
	vector<int> index_features;
	vector<vector<double> > current_list;
	vector<vector<double> > deter_list;
	vector<vector<double> > max_list;
	vector<int> local_features;
	vector<double> temp;
	vector<double> pusher;
	double curr_acc = 0.0;
	double local_acc = 0.0;
	for(int i = 0; i < matrix.size(); ++i)	{	//sets up initialization
		temp = matrix.at(i);
		pusher.push_back(temp.at(0));
		current_list.push_back(pusher);
		pusher.clear();
	}
	int stop = matrix.at(0).size();
	bool used = false;
	for(int i = 1; i < stop; ++i) {		//we iterate as many times as how many features we have
		cout << "Iteration: " << i << ". Searching over features: ";
		for(int z = 0; z < index_features.size(); z++) {
			cout << index_features.at(z) << " ";
		}
		cout << endl;
		for(int j = 1; j < stop; ++j) {
			deter_list = current_list;
			used = checker(index_features, j);
			if(!used) {		//proceed by adding one feature at a time that we havent used yet in each level
				cout << "adding feature: " << j;
				adder(deter_list, matrix, j);
				curr_acc = cross_validation(deter_list, opt);
				cout << " has accuracy of: " << setprecision(10) << curr_acc << endl;
				if(curr_acc > local_acc){
					if(i > index_features.size()) {
						index_features.push_back(j);
					}
					else {
						index_features.at(i-1) = j;
					}

					if(curr_acc > best_acc) {		//keep track of what is globally best as well as locally
						best_acc = curr_acc;
						best_features = index_features;
					}
					max_list = deter_list;
					local_acc = curr_acc;
					local_features = index_features;
				}
			}
		}
		current_list = max_list;
		cout << "Local max is: " << setprecision(10) << local_acc << endl;
		cout << "Keeping feature: " << local_features.at(local_features.size()-1) << endl << endl;
		local_acc = 0.0;
		index_features = local_features;
	}
}

void backward_search(vector<vector<double> > &matrix, vector<int> &best_features, double &best_acc)
{		//backward search, same as forward, just in reverse
	vector<int> index_features;
	vector<vector<double> > current_list;
	vector<vector<double> > deter_list;
	vector<vector<double> > max_list;
	vector<int> local_features;
	double curr_acc = cross_validation(matrix, 1);
	double local_acc = 0.0;
	best_acc = curr_acc;
	current_list = matrix; //initialization
	int stop = matrix.at(0).size();
	for(int i = 1; i < stop; ++i)	{ //sets up initialization
		index_features.push_back(i);
	}
	bool used = false;
	vector<int> last = index_features;
	local_features = index_features;
	best_features = index_features;
	for(int i = 1; i < stop; ++i) {	//iterate as many times as we have features
		cout << "Iteration: " << i << ". Searching over features: ";
		for(int z = 0; z < index_features.size(); ++z) {
			cout << index_features.at(z) << " ";
		}
		cout << endl;
		for(int j = 1; j < stop; ++j) {
			deter_list = current_list;
			used = checker(index_features, j);
			if(used) {			//proceed by removing one feature at a time and checking accruacy
				cout << "removing feature: " << j;
				remover(deter_list, matrix, j, index_features);
				curr_acc = cross_validation(deter_list, 1);
				cout << " has accuracy of: " << setprecision(10) << curr_acc << endl;
				if(curr_acc > local_acc) {
					feat_remove(local_features, last, j); 
					if(curr_acc > best_acc)	{	//keep track of global and local best
						best_acc = curr_acc;
						best_features = local_features;
					}
					max_list = deter_list;
					local_acc = curr_acc;
				}
			}
		}
		current_list = max_list;
		cout << "Local max is: " << setprecision(10) << local_acc << endl;
		cout << "Keeping features: ";
		for(int p = 0; p < local_features.size(); ++p) {
			cout << local_features.at(p) << " ";
		}
		cout << endl << endl;
		local_acc = 0.0;
		index_features = local_features;
		last = index_features;
	}
}