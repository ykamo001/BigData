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
#include "k_fold.h"
#include "util.h"
#include "search.h"

using namespace std;

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
