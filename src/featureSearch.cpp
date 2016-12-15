#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <iomanip>
#include <math.h>

using namespace std;

int k_fold_size(int matrixSize)		//this function will determine the size of K for K fold
{
	int k = 10;			//starts off at 10, and goes higher if it does not divide perfectly
	if(matrixSize % k != 0)
	{
		while((matrixSize % k != 0) && (k > 0))
		{
			k--;
		}		//stop when it finally divides perfectly
	}
	return k;
}

void fix(vector<vector<double> > &neighbors, double dist, double qual)		//will be used to sort the top nearest neighbors
{
	bool done = false;
	int i = 0;
	vector<double> temp;
	vector<double> temp2;
	double nDist = dist;
	double nQual = qual;
	while(!done)		//keep doing so until we manually tell it to stop
	{
		temp = neighbors.at(i);
		if(dist < temp.at(1))		//if we find that the new distance we found is smaller than the ones we already have
		{
			for(int j = i; j < neighbors.size(); ++j)		//then we simply insert it into the proper location
			{
				temp = neighbors.at(j);
				temp2 = temp;
				temp.at(0) = nQual;
				temp.at(1) = nDist;
				neighbors.at(j) = temp;
				nDist = temp2.at(1);
				nQual = temp2.at(0);
			}
			done = true;
		}
		else			//if we do not find any, stop looking
		{
			if(i+1 < neighbors.size())
			{
				++i;
			}
			else
			{
				done = true;
			}
		}
	}
}

void validate(double qual, vector<vector<double> > neighbors, double &correct, double &tested)	//will be used to check
{								//what the nearest neighbors think the point is
	vector<double> temp;			//and if it is correct, we note that
	int pred = 0;
	for(int i = 0; i < neighbors.size(); ++i)	//check how many neighbors think the point is what the actual point is
	{
		temp = neighbors.at(i);
		if(qual == temp.at(0))
		{
			pred++;
		}
	}
	if(pred > 1)		//if the majority of the neighbors think he is his actual qualifier, mark this as correct guess
	{
		correct++;
	}
	tested++;
}

void k_fold(vector<vector<double> > &hold, vector<vector<double> > &test, double &correct, double &tested)
{		//used to conduct k-fold and nearest neighbor algorithm
	vector<double> currentTest;
	vector<double> currentHold;
	double dist = 0.0;
	double temp = 0.0;
	vector<double> distTemp;
	double minDist = 100.0;
	double minQual = 100.0;
	for(int i = 0; i < test.size(); ++i)	//this is where we will conduct the nearest neighbor algorithm 
	{
		currentTest = test.at(i);
		for(int j = 0; j < hold.size(); ++j)
		{
			currentHold = hold.at(j);
			for(int k = 1; k < currentTest.size(); ++k)		//use distance formula, eucledian like
			{	//square root of (x1-x2)^2 + (y1-y2)^2 + ....
				temp = currentTest.at(k) - currentHold.at(k);
				temp = temp*temp;
				distTemp.push_back(temp);
			}
			for(int l = 0; l < distTemp.size(); ++l)
			{
				dist += distTemp.at(l);
			}
			distTemp.clear();
			dist = sqrt(dist);
			if(dist < minDist)			//keep track of what the nearest neighbor is
			{
				minDist = dist;
				minQual = currentHold.at(0);
			}
			dist = 0.0;
		}
		if(minQual == currentTest.at(0))	//if the nearest neighbor thinks the point is actually what the point is, then correct
		{
			correct++;
		}
		tested++;
		minDist = 100.0;
		minQual = 100.0;
		dist = 0.0;
		temp = 0.0;
		distTemp.clear();
	}
}

void k_fold_neigh(vector<vector<double> > &hold, vector<vector<double> > &test, double &correct, double &tested)
{		//this is the custom algorithm, asks a majority of what the neighbors think
	vector<double> currentTest;
	vector<double> currentHold;
	double dist = 0.0;
	double temp = 0.0;
	vector<double> distTemp;
	vector<vector<double> > neighbors;
	distTemp.push_back(10.0);		//initialize the neighbors to junk value
	distTemp.push_back(100.0);
	for(int q = 0; q < 3; ++q)		//we will ask 3 neighbors
	{
		neighbors.push_back(distTemp);
	}
	distTemp.clear();
	for(int i = 0; i < test.size(); ++i)	//this is where we will conduct the nearest neighbor algorithm 
	{
		currentTest = test.at(i);
		for(int j = 0; j < hold.size(); ++j)
		{
			currentHold = hold.at(j);
			for(int k = 1; k < currentTest.size(); ++k)	//use distance algorithm, just like forward search
			{
				temp = currentTest.at(k) - currentHold.at(k);
				temp = temp*temp;
				distTemp.push_back(temp);
			}
			for(int l = 0; l < distTemp.size(); ++l)
			{
				dist += distTemp.at(l);
			}
			distTemp.clear();
			dist = sqrt(dist);
			fix(neighbors, dist, currentHold.at(0));	//hold all the nearest neighbors
			dist = 0.0;
		}
		dist = 0.0;
		temp = 0.0;
		distTemp.clear();
		validate(currentTest.at(0), neighbors, correct, tested);	//check for what neighbors think
		neighbors.clear();
		distTemp.push_back(10.0);		
		distTemp.push_back(100.0);
		for(int q = 0; q < 3; ++q)		
		{
			neighbors.push_back(distTemp);
		}
		distTemp.clear();
	}
}

double cross_validation(vector<vector<double> > matrix, int opt)		//sets up the k-fold process by dividing the data into parts
{
	int k = k_fold_size(matrix.size());
	int iter = matrix.size() / k;
	int start = 0;
	int stop = iter - 1;
	double correct = 0.0;
	double tested = 0.0;
	vector<vector<double> > hold;
	vector<vector<double> > test;
	vector<double> temp;
	for(int i = 0; i < k; ++i)			//must do k-fold k times
	{
		for(int j = 0; j < matrix.size(); ++j)		//get the correct section to test
		{
			temp = matrix.at(j);
			if((j >= start) && (j <= stop))
			{
				test.push_back(temp);
			}
			else
			{
				hold.push_back(temp);
			}
		}
		if(opt == 1)		//for forward search and backward search
		{
			k_fold(hold, test, correct, tested);
		}
		else	//only for custom search
		{
			k_fold_neigh(hold, test, correct, tested);
		}
		double perc = correct/tested;
		hold.clear();
		test.clear();
		start = stop + 1;
		stop += iter;
	}
	return (correct/tested)*(100.0);	//return accuracy
}

void normalize(vector<vector<double> > &matrix)		//function will normalize data between 0 and 1
{			//simply (xi - mean(x)) / std(x)
	int column = 1;
	vector<double> temp = matrix.at(0);
	int stop = temp.size();
	temp.clear();
	double count = 0.0;
	double average = 0.0;
	vector<double> variance;
	double devi = 0.0;
	for(int i = 1; i < stop; ++i)
	{
		for(int j = 0; j < matrix.size(); ++j)
		{
			temp = matrix.at(j);
			count = count + temp.at(column);
		}
		average = count/(static_cast<double>(matrix.size()));
		count = 0.0;
		for(int j = 0; j < matrix.size(); ++j)
		{
			temp = matrix.at(j);
			count = temp.at(column) - average;
			count = count*count;
			variance.push_back(count);
		}
		count = 0.0;
		for(int j = 0; j < variance.size(); ++j)
		{
			count = count + variance.at(j);
		}
		devi = count/(static_cast<double>(variance.size()));
		devi = sqrt(devi);
		count = 0.0;
		for(int j = 0; j < matrix.size(); ++j)
		{
			temp = matrix.at(j);
			count = temp.at(column);
			temp.at(column) = ((count - average)/devi);
			matrix.at(j) = temp;
		}
		variance.clear();
		count = 0.0;
		devi = 0.0;
		average = 0.0;
		column++;
	}
}

bool checker(vector<int> index_features, int index)		//checks for if the current feature is in the vector
{
	for(int i = 0; i < index_features.size(); ++i)
	{
		if(index_features.at(i) == index)
		{
			return true;
		}
	}
	return false;
}

void adder(vector<vector<double> > &deter_list, const vector<vector<double> > &matrix, int j)
{			//adds the proper features to our searching matrix
	vector<double> temp;
	vector<double> temp2;
	for(int i = 0; i < matrix.size(); ++i)
	{
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
	for(int i = 0; i < matrix.size(); ++i)
	{
		temp = matrix.at(i);
		for(int k = 0; k < temp.size(); ++k)
		{
			if(k == 0)
			{
				temp2.push_back(temp.at(k));
			}
			else
			{
				if(k != j)
				{
					used = checker(features, k);
					if(used)
					{
						temp2.push_back(temp.at(k));
					}
				}
			}
		}
		deter_list.at(i) = temp2;
		temp2.clear();
	}
}

void forward_search(vector<vector<double> > &matrix, vector<int> &best_features, double &best_acc, int opt)
{			//the forward search
	vector<int> index_features;
	vector<vector<double> > current_list;
	vector<vector<double> > deter_list;
	vector<vector<double> > max_list;
	vector<int> local_features;
	vector<double> temp;
	vector<double> pusher;
	double curr_acc = 0.0;
	double local_acc = 0.0;
	for(int i = 0; i < matrix.size(); ++i)		//sets up initialization
	{
		temp = matrix.at(i);
		pusher.push_back(temp.at(0));
		current_list.push_back(pusher);
		pusher.clear();
	}
	int stop = matrix.at(0).size();
	bool used = false;
	for(int i = 1; i < stop; ++i)		//we iterate as many times as how many features we have
	{
		cout << "Iteration: " << i << ". Searching over features: ";
		for(int z = 0; z < index_features.size(); ++z)
		{
			cout << index_features.at(z) << " ";
		}
		cout << endl;
		for(int j = 1; j < stop; ++j)
		{
			deter_list = current_list;
			used = checker(index_features, j);
			if(!used)			//proceed by adding one feature at a time that we havent used yet in each level
			{
				cout << "adding feature: " << j;
				adder(deter_list, matrix, j);
				curr_acc = cross_validation(deter_list, opt);
				cout << " has accuracy of: " << setprecision(10) << curr_acc << endl;
				if(curr_acc > local_acc)
				{
					if(i > index_features.size())
					{
						index_features.push_back(j);
					}
					else
					{
						index_features.at(i-1) = j;
					}
					if(curr_acc > best_acc)		//keep track of what is globally best as well as locally
					{
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
	for(int i = 0; i < last.size(); ++i)
	{
		if(last.at(i) != j)
		{
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
	for(int i = 1; i < stop; ++i)		//sets up initialization
	{
		index_features.push_back(i);
	}
	bool used = false;
	vector<int> last = index_features;
	local_features = index_features;
	best_features = index_features;
	for(int i = 1; i < stop; ++i)		//iterate as many times as we have features
	{
		cout << "Iteration: " << i << ". Searching over features: ";
		for(int z = 0; z < index_features.size(); ++z)
		{
			cout << index_features.at(z) << " ";
		}
		cout << endl;
		for(int j = 1; j < stop; ++j)
		{
			deter_list = current_list;
			used = checker(index_features, j);
			if(used)			//proceed by removing one feature at a time and checking accruacy
			{
				cout << "removing feature: " << j;
				remover(deter_list, matrix, j, index_features);
				curr_acc = cross_validation(deter_list, 1);
				cout << " has accuracy of: " << setprecision(10) << curr_acc << endl;
				if(curr_acc > local_acc)
				{
					feat_remove(local_features, last, j); 
					if(curr_acc > best_acc)		//keep track of global and local best
					{
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
		for(int p = 0; p < local_features.size(); ++p)
		{
			cout << local_features.at(p) << " ";
		}
		cout << endl << endl;
		local_acc = 0.0;
		index_features = local_features;
		last = index_features;
	}
}

int main()
{
	vector<double> columns; //will hold all the columns/features
	vector<vector<double> > rows;	//will hold all the rows/instances
	string input;			//get the input file name that has all the data
	cout << "Please enter the file you would like to extrapolate information from: ";
	cin >> input;

	int feat = 0;	//we will open the file and determine how many features it has by counting e's
	ifstream file;
	file.open(input.c_str());
	string hold;
	getline(file, hold);
	for(int i = 0; i < hold.size(); ++i)
	{
		if(hold.at(i) == 'e')
		{
			feat++;
		}
	}
	file.close();	//close the file because we want to start back at the first line of the file

	file.open(input.c_str());
	int counter = feat;		//counter will help us create the matrix, which determines the columns
	while(file >> hold)	//while there are still numbers left in the file, keep taking them
	{
		hold = hold.substr(0,9);
		//double temp = stod(hold);		//converts the string into a double, which is the type we need to store the data
		double temp = atof(hold.c_str());
		columns.push_back(temp);
		counter--;
		if(counter == 0)
		{
			rows.push_back(columns);
			columns.clear();
			counter = feat;
		}
	}
	file.close();
	cout << "Normalizing data, please wait.." << endl;
	normalize(rows);
	vector<int> rfeat;
	double racc = 0.0;
	cout << endl;
	int option = 0;			//GUI to pick algorithm of choice
	cout << "Please select the search routine you would like to implement: " << endl;
	cout << "1. Forward Search" << endl;
	cout << "2. Backward Search" << endl;
	cout << "3. Custom Search" << endl;
	cout << "option: ";
	cin >> option;
	cout << endl;
	if(option == 1)
	{
		forward_search(rows, rfeat, racc, option);
		cout << "Best features are: ";
		for(int i = 0; i < rfeat.size(); ++i)
		{
			cout << rfeat.at(i) << " ";
		}
		cout << endl;
		cout << "With accuracy of: " << setprecision(10) << racc << endl;
	}
	else if(option == 2)
	{	
		backward_search(rows, rfeat, racc);
		cout << "Best features are: ";
		for(int i = 0; i < rfeat.size(); ++i)
		{
			cout << rfeat.at(i) << " ";
		}
		cout << endl;
		cout << "With accuracy of: " << setprecision(10) << racc << endl;
	}
	else if(option == 3)
	{
		forward_search(rows, rfeat, racc, option);
		cout << "Best features are: ";
		for(int i = 0; i < rfeat.size(); ++i)
		{
			cout << rfeat.at(i) << " ";
		}
		cout << endl;
		cout << "With accuracy of: " << setprecision(10) << racc << endl;
	}
	else
	{
		cout << "That is an invalid option, goodbye" << endl;
	}
	return 0;
}
