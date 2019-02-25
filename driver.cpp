//Driver program to test the Led class implemented in assignment 2
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;
#include "Led.h"

int main(int argc, char* argv[])
{
	string filename; //an empty filename
	switch (argc) {	//determine the filename
	case 1: //no file name
		break;
	case 2: filename = argv[1];
		break;
	default: cout << ("too many arguments - all discarded") << endl;
		break;
	}
	Led editor(filename);  //create a Led named editor
	editor.run();
	return 0;
}
