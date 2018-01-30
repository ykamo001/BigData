#include <iterator>
#include <map>
#include <algorithm>
#include <math.h>
#include "k_fold.h"
#include "util.h"

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