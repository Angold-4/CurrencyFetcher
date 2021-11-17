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

    void SMA() {
	// TODO
	// caculate the SMA -> each day just caculate prev 10 days SMA
	// just move the iterator - 10
	cout << "Caculate SMA..." << endl;

	// caculate first 10 days SMA
	double suma = 0;     // sum  (sum/10 = simple moving average)
	double sma = 0;
	double monthlybias = 0;

	int ct = 0; // valid count
	int index = 0;
	while (ct < 10) {
	    if (days[index] == 0) {
		index++;
	    } else {
		index++;
		ct++;
		suma += days[index];
	    }
	}

	sma = suma/10; // first sma
	double today = 0;

	// then go on (Using iterator)
	// TODO: Maybe use pointer will be faster
	vector<double>::iterator dit = days.begin();
	int offset = 10; // iterator offset 

	for (dit += 10; dit != days.end(); dit++) {
	    if (*dit == 0) {
	//	cout << monthlybias << endl;
		biasmons.push_back(monthlybias);
		monthlybias = 0;
		// next month (change state)
		offset = 11; // add 1
		mons.push_back(0); // divide different months

		continue;
	    } 

	    // caculate it
	    if (!*(dit-offset)) {
		// we get into next month
		offset = 10;
	    }

	    today = *(dit); 
	    monthlybias += (today - sma);
	    // cout << today << " ,  "  << sma << " , " << (today - sma) << monthlybias << endl;

	    suma -= *(dit-offset); // minus the 10th day before it
	    suma += today;
	    sma = suma/10;
	    mons.push_back(sma);
	}


    }

    void print_monbias() {
	int y = ybegin;
	int m = mbegin;
	int d = dbegin;


	for (double bias : biasmons) {
	    m += 1;
	    if (m == 13) {
		y += 1;
		cout << y << ": " << endl;
		m = 1;
	    }
	    cout << m << "'s SMA bias: " << bias << endl;
	}
    }


private:
    string csvdata; // the string stream where fetch the csv data
    vector<double> days;     // Sequencial container recorded all open price

    vector<double> mons;     // Recorded all smas each month
    vector<double> biasmons; // each month only one number for print
    int ybegin;
    int mbegin;
    int dbegin;

    void init_obj(string data) {
	// put csv data into our days 
	string line;
	istringstream iss(data); // istringstream has a pointer which indicate the next line
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
	    if (line.substr(11, 4) == "null") continue;
	    int m = get_month(line);
	    if (m != premonth) {
		// month change 
		days.push_back(0); // divide it
		premonth = m;
	    }

	    open = stof(line.substr(11, 10));
	    // cout << setprecision(100) << open << endl;
	    // https://stackoverflow.com/questions/2550281/floating-point-vs-integer-calculations-on-modern-hardware
	    // since div in long number costes a lot of time (about 3 times slower than double)
	    // we still use double to caculate
	    days.push_back(open);
	}
    }
};

/*
int main() {
    // just for test
    ifstream infile("./Test.txt"); // test file
    stringstream buffer; // read buffer
    buffer << infile.rdbuf();
    // cout << buffer.str() << endl;
    string data = buffer.str();

    HandleObj* TObj = new HandleObj(data);
    TObj->SMA();
    TObj->print_monbias();
}
*/
