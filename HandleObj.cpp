#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>


/*
 * Author: Jiawei Wang (Angold-4)
 * Caculate Data first and store then print them all
 */


using namespace::std; // this HandleObj will only use STL features

class HandleObj {
public:
    HandleObj(string data) {
	csvdata = data;
	init_obj(data);
    }


private:
    string csvdata; // the string stream where fetch the csv data
    vector<double> days; // Sequencial container recorded all
    int ybegin;
    int mbegin;
    int dbegin;

    void init_obj(string data) {
	// put csv data into our days 
	string line;
	istringstream iss(data); // istringstream has a pointer
	getline(iss, line); // do not need first line

	getline(iss, line); // second line is our first data
	// get the ybegin mbegin and dbegin
	ybegin = stoi(line.substr(0, 4));
	mbegin = stoi(line.substr(5, 2));
	dbegin = stoi(line.substr(8, 2));

	handle_data(iss);
    }

    int get_month(string line) {
	return stoi(line.substr(5, 2));
    }

    void handle_data(istringstream& iss) {
	// put data into days sequencial vector
	string line;
	int premonth = mbegin;
	double open;
	while (getline(iss, line)) {
	    // read a line from iss
	    int m = get_month(line);
	    if (m != premonth) {
		// month change 
		days.push_back(0); // divide it
		premonth = m;
	    }

	    open = stof(line.substr(11, 11));
	    // cout << setprecision(100) << open << endl;
	    // https://stackoverflow.com/questions/2550281/floating-point-vs-integer-calculations-on-modern-hardware
	    // since div in long number costes a lot of time (about 3 times slower than double)
	    // we still use doubel to caculate
	    days.push_back(open);

	    for (double day : days) {
		cout << setprecision(100) << day << endl;
	    }
	}

    }



    // TODO
    // caculate the SMA -> each day just caculate prev 10 days SMA
    // just move the iterator - 10



};

int main() {
    // just for test
    ifstream infile("./Test.txt"); // test file
    stringstream buffer; // read buffer
    buffer << infile.rdbuf();
    // cout << buffer.str() << endl;
    string data = buffer.str();

    HandleObj* TObj = new HandleObj(data);
}
