#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include "util.h"

using namespace std;

void fix(map<double, double> &neighbors, double dist, double qual)	//will be used to sort the top nearest neighbors
{
	if(neighbors.size() > 2){
		map<double, double>::reverse_iterator rit = neighbors.rbegin();
		if(dist < rit->first){
			while(neighbors.insert(pair<double, double>(dist, qual)).second == false){
				dist += 0.00001;
			}
			neighbors.erase(neighbors.rbegin()->first);
		}
	}
	else{
		neighbors.insert(pair<double, double>(dist, qual));
	}
}

void validate(double qual, map<double, double> &neighbors, double &correct, double &tested)
{ //will be used to check what the nearest neighbors think the point is
	map<double, double>::iterator itr;
	int pred = 0;
	for(itr = neighbors.begin(); itr != neighbors.end(); itr++) {
		if(qual == itr->second) {
			pred++;
		}
	}
	if(pred > 1) {	//if the majority of the neighbors think he is his actual qualifier, mark this as correct guess
		correct++;
	}
	tested++;
}

void normalize(vector<vector<double> > &matrix)		//function will normalize data between 0 and 1
{	//simply (xi - mean(x)) / std(x)
	int stop = matrix.at(0).size();
	double count = 0.0;	//used for arithmetic and holding values
	double average = 0.0;	//used for mean(x)
	double devi = 0.0;	//used to hold the deviation
	double variance = 0.0;
	for(int i = 1; i < stop; ++i) {
		for(int j = 0; j < matrix.size(); ++j) {
			count += matrix.at(j).at(i);	//find total of each feature/column
		}
		average = count/(static_cast<double>(matrix.size()));	//get the average
		count = 0.0;
		for(int j = 0; j < matrix.size(); ++j) {	//calculate the varianvce and hold the values
			count = matrix.at(j).at(i) - average;
			count *= count;
			variance += count;
			//variance.push_back(count);
		}
		count = 0.0;
		devi = variance/(static_cast<double>(matrix.size()));
		devi = sqrt(devi);
		for(int j = 0; j < matrix.size(); ++j) {
			count = matrix.at(j).at(i);
			matrix.at(j).at(i) = ((count - average)/devi);
		}
		variance = 0.0;
		count = 0.0;
		devi = 0.0;
		average = 0.0;
	}
}

bool checker(vector<int> index_features, int index)		//checks for if the current feature is in the vector
{
	for(int i = 0; i < index_features.size(); ++i) {
		if(index_features.at(i) == index) {
			return true;
		}
	}
	return false;
}

void adder(vector<vector<double> > &deter_list, const vector<vector<double> > &matrix, int j)
{	//adds the proper features to our searching matrix
	vector<double> temp;
	vector<double> temp2;
	for(int i = 0; i < matrix.size(); ++i) {
		temp = matrix.at(i);
		temp2 = deter_list.at(i);
		temp2.push_back(temp.at(j));
		deter_list.at(i) = temp2;
	}
}

void remover(vector<vector<double> > &deter_list,  vector<vector<double> > matrix, int j, vector<int> features)
{		//removes the proper features from our searching matrix
	vector<double> temp;
	vector<double> temp2;
	bool used = false;
	for(int i = 0; i < matrix.size(); ++i) {
		temp = matrix.at(i);
		for(int k = 0; k < temp.size(); ++k) {
			if(k == 0) {
				temp2.push_back(temp.at(k));
			}
			else{
				if(k != j) {
					used = checker(features, k);
					if(used) {
						temp2.push_back(temp.at(k));
					}
				}
			}
		}
		deter_list.at(i) = temp2;
		temp2.clear();
	}
}

void feat_remove(vector<int> &to_remove, vector<int> last, int j)
{		//simply used to remove a feature in our list of features
	to_remove.clear();
	for(int i = 0; i < last.size(); ++i) {
		if(last.at(i) != j) {
			to_remove.push_back(last.at(i));
		}
	}
}