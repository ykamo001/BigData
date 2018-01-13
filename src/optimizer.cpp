#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <iomanip>
#include <math.h>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>

using namespace std;

int k_fold_size(int matrixSize)		//this function will determine the size of K for K fold
{
	int k = 10;			//starts off at 10, and goes higher if it does not divide perfectly
	if(matrixSize % k != 0){
		while((matrixSize % k != 0) && (k > 0)){
			k--;
		}		//stop when it finally divides perfectly
	}
	return k;
}

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

void k_fold(vector<vector<double> > &hold, vector<vector<double> > &test, double &correct, double &tested)
{	//used to conduct k-fold and nearest neighbor algorithm
	vector<double> currentTest, currentHold;
	double dist = 0.0, temp = 0.0, minDist = 100.0, minQual = 100.0;
	for(int i = 0; i < test.size(); ++i) { //this is where we will conduct the nearest neighbor algorithm 
		currentTest = test.at(i);
		for(int j = 0; j < hold.size(); ++j) {
			currentHold = hold.at(j);
			for(int k = 1; k < currentTest.size(); ++k)	{ //use distance formula, eucledian like square root of (x1-x2)^2 + (y1-y2)^2 + ....
				temp = currentTest.at(k) - currentHold.at(k);
				temp *= temp;
				dist += temp;
			}
			dist = sqrt(dist);
			if(dist < minDist)	{	//keep track of what classification the nearest neighbor is
				minDist = dist;
				minQual = currentHold.at(0);
			}
			dist = 0.0;
		}
		if(minQual == currentTest.at(0)) {	//if the nearest neighbor thinks the point is what our data says, then it is correct
			correct++;
		}
		tested++;
		minDist = 100.0;
		minQual = 100.0;
		dist = 0.0;
		temp = 0.0;
	}
}

void k_fold_neigh(vector<vector<double> > &hold, vector<vector<double> > &test, double &correct, double &tested)
{		//this is the custom algorithm, asks a majority of what the neighbors think
	vector<double> currentTest, currentHold;
	double dist = 0.0, temp = 0.0;
	vector<double> distTemp;
	map<double, double> neighbors;

	for(int i = 0; i < test.size(); ++i) {	//this is where we will conduct the nearest neighbor algorithm 
		currentTest = test.at(i);
		for(int j = 0; j < hold.size(); ++j) {
			currentHold = hold.at(j);
			for(int k = 1; k < currentTest.size(); ++k) { //use distance algorithm, just like forward search
				temp = currentTest.at(k) - currentHold.at(k);
				temp = temp*temp;
				dist += temp;
			}
			dist = sqrt(dist);
			fix(neighbors, dist, currentHold.at(0));	//hold all the nearest neighbors
			dist = 0.0;
			temp = 0.0;
		}
		dist = 0.0;
		temp = 0.0;
		validate(currentTest.at(0), neighbors, correct, tested);	//check for what neighbors think
		neighbors.clear();
	}
}

double cross_validation(vector<vector<double> > matrix, char opt)		//sets up the k-fold process by dividing the data into parts
{
	int k = k_fold_size(matrix.size());
	int iter = matrix.size() / k;
	int start = 0, stop = iter - 1;
	double correct = 0.0, tested = 0.0;
	vector<vector<double> > hold, test;
	for(int i = 0; i < k; ++i) {			//must do k-fold k times
		for(int j = 0; j < matrix.size(); ++j)	{	//get the correct section to test
			if((j >= start) && (j <= stop)) {
				test.push_back(matrix.at(j));
			}
			else {
				hold.push_back(matrix.at(j));
			}
		}
		if(opt == '1')	{	//for forward search and backward search
			k_fold(hold, test, correct, tested);
		}
		else {	//only for custom search
			k_fold_neigh(hold, test, correct, tested);
		}
		hold.clear();
		test.clear();
		start = stop + 1;
		stop += iter;
	}
	return (correct/tested)*(100.0);	//return accuracy
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
		for(int z = 0; z > index_features.size(); z++) {
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

void feat_remove(vector<int> &to_remove, vector<int> last, int j)
{		//simply used to remove a feature in our list of features
	to_remove.clear();
	for(int i = 0; i < last.size(); ++i) {
		if(last.at(i) != j) {
			to_remove.push_back(last.at(i));
		}
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

int main(int argc, char **argv)
{
	if(argc == 3) {	//only run if the enter the arguments of file name and algorithm option
		vector<double> columns; //will hold all the columns/features
		vector<vector<double> > rows;	//will hold all the rows/instances
		ifstream file;
		file.open(argv[1]);
		string hold = "", subs = "";	//used for holding the lines fo the file and for parsing strings
		double num = 0.0;
		while(getline(file, hold)) {	//pull all the data from the file
			istringstream iss(hold);
			while(iss){	//parse the string by space
				iss >> subs;
				num = atof(subs.c_str());	//convert the string into a double
				columns.push_back(num);	//save each feature 
			}
			rows.push_back(columns);	//go onto the next line and save the line as an instance of a row
			columns.clear();
		}
		file.close();	//close file since we no longer need it

		cout << "Normalizing data, please wait.." << endl << endl;
		normalize(rows);	//normalize the data

		vector<int> rfeat;
		double racc = 0.0;
		if(*argv[2] == '1') {
			forward_search(rows, rfeat, racc, *argv[2]);
			cout << "Best features are: ";
			for(int i = 0; i < rfeat.size(); ++i) {
				cout << rfeat.at(i) << " ";
			}
			cout << endl;
			cout << "With accuracy of: " << setprecision(10) << racc << endl;
		}
		else if(*argv[2] == '2') {	
			backward_search(rows, rfeat, racc);
			cout << "Best features are: ";
			for(int i = 0; i < rfeat.size(); ++i) {
				cout << rfeat.at(i) << " ";
			}
			cout << endl;
			cout << "With accuracy of: " << setprecision(10) << racc << endl;
		}
		else if(*argv[2] == '3') {
			forward_search(rows, rfeat, racc, *argv[2]);
			cout << "Best features are: ";
			for(int i = 0; i < rfeat.size(); ++i) {
				cout << rfeat.at(i) << " ";
			}
			cout << endl;
			cout << "With accuracy of: " << setprecision(10) << racc << endl;
		}
		else {
			cout << "That is an invalid option, goodbye" << endl;
		}
	}
	else if(argc < 3) {
		cout << "Not enough arguments. ";
		cout << "Please also enter the file from which to extract data, and which algorithm to use." << endl;
	}
	return 0;
}
