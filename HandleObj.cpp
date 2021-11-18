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

    void SMA_bias() {
	// TODO
	// caculate the SMA -> each day just caculate prev 10 days SMA
	// just move the iterator - 10
	cout << "Caculate Monthly SMA_bias..." << endl;

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
	    // Simulation Start
	    // Buy and Sel strategy (Simple)
	    if (*dit == 0) {
		biasmons.push_back(monthlybias);
		monthlybias = 0;
		// next month (change state)
		offset = 11; // add 1
		mons.push_back(0); // divide different months

		continue;
	    } 

	    // caculate it
	    if (*(dit-offset) == 0) {
		// we get into next month
		offset = 10;
	    }

	    today = *(dit); 
	    monthlybias += (today - sma);

	    suma -= *(dit-offset); // minus the 10th day before it
	    suma += today;
	    sma = suma/10;
	    mons.push_back(sma);
	}
    }

    // Test Buy stock Logic
    // TODO For each day, if the signal is Buy


    void print_monbias() {
	int y = ybegin;
	int m = mbegin;
	int d = dbegin;


	for (double bias : biasmons) {
	    m += 1;
	    if (m == 13) {
		y += 1;
		// cout << y << ": " << endl;
		m = 1;
	    }
	    // cout << m << "'s SMA bias: " << bias << endl;
	}
    }


    void SMA() {
	// little project -> see if I can make money
	// improving algorithm
	
	double suma = 0; // sum of prev 10 days
	double sma = 0;  // prev 10 days sma (suma / 10)
	double Property = 100000; // 100k US Dollars initial
	bool buyed = false;
	
	int index = 0;
	int offset = 10; // prev 10 days
	while (index < 10) {
	    suma += cdays[index];
	    index++;
	}

	sma = suma / offset;
	int cshare = 0; // how many shares of stock we current hold

	for (vector<double>::iterator cday = cdays.begin()+10; cday != cdays.end(); cday++) {
	    // Main loop

	    double dcp = *cday; // day closing price
	    if (dcp < sma && !buyed) {
		// we can buy some stock
		buy(Property, dcp, cshare);
		buyed = true;
	    } 
	    else if (dcp > sma && buyed) {
		// we can sell all stock
		sell(Property, dcp, cshare);
		buyed = false;
	    }

	    suma -= *(cday-offset);
	    suma += *(cday);

	    sma = suma / offset;
	}

	if (buyed) Property += cshare * *(cdays.end()-1);
	cout << endl;
	cout << "New Balance: " << setprecision(10) << Property << endl;
    }


private:
    string csvdata; // the string stream where fetch the csv data
    vector<double> days;     // Sequencial container recorded all open price
    vector<double> cdays;    // Sequencial container recorded all close price

    vector<double> mons;     // Recorded all smas each month
    vector<double> biasmons; // each month only one number for print
    int ybegin;
    int mbegin;
    int dbegin;

    double Property = 100000; // the wealth I have

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
	    // close = stof((line))
	    days.push_back(open);

	    int n_s = 0;
	    bool read = false;
	    string sclose = "";

	    // read close
	    for (string::iterator sit = line.begin()+11; sit != line.end(); sit++) {
		if (*sit == ',') {
		    n_s++;
		    if (n_s == 3) {
			read = true;
		    } else if (read && n_s > 3) {
			break;
		    }
		}

		else if (read) {
		    sclose += *sit;
		}
	    }
	    cdays.push_back(stof(sclose));
	}

    }

    void buy(double& property, double dcp, int& cshare) {
	// Use all money buy stock as much as I can
	cshare = property / dcp; // see how many shares we can buy
	double cost = cshare * dcp;
	property -= cost;
	cout << "Buy " << cshare << " shares of stock, Account balance: $" << setprecision(10) << property << endl;
    }

    void sell(double& property, double dcp, int& cshare) {
	// Sell all stocks that I currently hold
	double earned = cshare * dcp;
	cshare = 0;
	property += earned;
	cout << "Sell all stocks, Earned $" << setprecision(10) << earned << ", Account balance: $" << setprecision(10) << property << endl;
	cout << endl;
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
