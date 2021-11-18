#include <string>
#include <iostream>

using namespace::std;

int main() {
    string s = "2019-09-12,107.999001,108.135002,107.540001,108.021004,108.021004,102892";

    int n_s = 0;
    bool read = false;
    string sclose = "";
    for (string::iterator sit = s.begin()+11; sit != s.end(); sit++) {
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

    cout << sclose << endl;
}
