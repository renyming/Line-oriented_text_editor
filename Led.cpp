#include "Led.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

Led::Led(string file) :
	filename_{ file },
	currentline_{ 0 },
	lastline_{ 0 }
{}

Led::~Led() {}

void Led::run() {

	bool file_existed = false;

	if (filename_ == "")
		file_existed = false;
	else {
		//reads in file if it's existed
		file_existed = ReadFile(filename_);
	}

	//display header info
	if (file_existed) {
		cout << "\"" << filename_ << "\"" << " " << lastline_ << " lines" << endl;
	}
	else {
		if (filename_ == "") {
			cout << "\"?\"";
		}
		else {
			cout << "\"" + filename_ + "\"";
		}
		cout << " " << "[New File]" << endl;
	}

	cout << "Entering command mode.\n";

	//exit flag
	bool exit_flag = false;
	while (!exit_flag) {
		cout << "? ";
		string command_line;
		getline(cin, command_line);
		Command cmd(currentline_, lastline_);
		if (!cmd.parse(command_line)) {
			cout << "Invalid command\n";
			continue;
		}

		int n1 = cmd.getFirstNumber();
		int n2 = cmd.getSecondNumber();
		char command = cmd.getCommand();

		//check range
		if (!(n1 >= 0 && n1 <= n2 && n2 <= lastline_)) {
			cout << "Invalid range\n";
			continue;
		}

		switch (command)
		{
		case'a':
			Append(n1);
			break;
		case 'i':
			Insert(n1);
			break;
		case 'v':
			PasteBelow(n1);
			break;
		case 'u':
			PasteAbove(n1);
			break;
		case 'd':
			Delete(n1, n2);
			break;
		case 'x':
			Cut(n1, n2);
			break;
		case 'r':
			Replace(n1, n2);
			break;
		case 'j':
			Join(n1, n2);
			break;
		case 'p':
			Print(n1, n2);
			break;
		case 'c':
			Change(n1, n2);
			break;
		case '-':
			Up(n1);
			break;
		case '+':
			Down(n1);
			break;
		case 'g':
			Goto(n1);
			break;
		case 'w':
			WriteToFile(file_existed);
			break;
		case 'q':
			Exit(file_existed);
			exit_flag = true;
			break;
		}
		cin.clear();
		clearerr(stdin);
	}
}

bool Led::ReadFile(const std::string filename) {
	ifstream fin;
	fin.open(filename);
	if (fin) {
		string line;
		int n = 0;
		while (getline(fin, line)) {
			buffer_.push_back(line);
			++n;
		}
		currentline_ = n;
		lastline_ = n;
		return true;
	}
	else {
		return false;
	}
}

void Led::Insert(const int n1) {
	if (lastline_ == 0) {
		Append(0);
	}
	else {
		Append(n1 - 1);
	}
}

void Led::Append(const int n1) {
	std::stringstream user_input;
	user_input << cin.rdbuf();
	string line;

	list<string>::iterator it;

	if (!buffer_.empty()) {
		it = buffer_.begin();
		advance(it, n1);
	}
	currentline_ = n1;
	while (getline(user_input, line)) {
		if (lastline_ != 0) {
			buffer_.insert(it, line);
			++currentline_;
		}
		else {
			buffer_.push_back(line);
			it = buffer_.begin();
			++it;
			currentline_ = 1;
		}
		++lastline_;
	}
}

void Led::Print(const int n1, const int n2) const {
	if (isEmpty()) return;

	list<string>::const_iterator it;
	list<string>::const_iterator it1;
	list<string>::const_iterator it2;
	it1 = buffer_.begin();
	it2 = buffer_.begin();
	advance(it1, n1 - 1);
	advance(it2, n2);
	int n = n1;

	for (it = it1; it != it2; ++it) {
		if (n != currentline_) {
			cout << n << ": ";
		}
		else {
			cout << n << "> ";
		}
		cout << *it << endl;
		++n;
	}

}

void Led::Goto(const int n1) {
	if (isEmpty()) return;
	currentline_ = n1;
}

bool Led::isEmpty() const {
	if (buffer_.empty()) {
		cout << "Empty buffer" << endl;
		return true;
	}
	else {
		return false;
	}
}

void Led::Delete(const int n1, const int n2) {
	list<string>::iterator range_begin;
	list<string>::iterator range_end;
	range_begin = buffer_.begin();
	range_end = buffer_.begin();
	advance(range_begin, n1 - 1);
	advance(range_end, n2);
	//update last line
	lastline_ -= n2 - n1 + 1;
	//set current address
	if (range_end == buffer_.end())
		currentline_ = n1 - 1;
	else
		currentline_ = n1;

	buffer_.erase(range_begin, range_end);
}

void Led::Cut(const int n1, const int n2) {
	list<string>::iterator range_begin;
	list<string>::iterator range_end;
	range_begin = buffer_.begin();
	range_end = buffer_.begin();
	advance(range_begin, n1 - 1);
	advance(range_end, n2);
	//update last line
	lastline_ -= n2 - n1 + 1;
	//set current address
	if (range_end == buffer_.end())
		currentline_ = n1 - 1;
	else
		currentline_ = n1;

	clipboard_.clear();
	clipboard_.insert(clipboard_.begin(), range_begin, range_end);
	buffer_.erase(range_begin, range_end);
	//if (buffer_.empty()) {
	//	currentline_ = 0;
	//	lastline_ = 0;
	//}
	//cout << "CL:" << currentline_ << endl;
	//cout << "LL:" << lastline_ << endl;
}

void Led::PasteBelow(const int n1) {
	list<string>::iterator range_begin;
	range_begin = buffer_.begin();
	advance(range_begin, n1);
	buffer_.insert(range_begin, clipboard_.begin(), clipboard_.end());
	//update address info
	currentline_ = n1 + clipboard_.size();
	lastline_ += clipboard_.size();
}

void Led::PasteAbove(const int n1) {
	PasteBelow(n1 - 1);
}

void Led::Replace(const int n1, const int n2) {
	Delete(n1, n2);
	Insert(n1);
}

void Led::Join(const int n1, const int n2) {
	list<string>::iterator first = buffer_.begin();
	list<string>::iterator range_begin = buffer_.begin();
	list<string>::iterator range_end = buffer_.begin();
	advance(first, n1 - 1);
	advance(range_begin, n1);
	advance(range_end, n2);
	for (list<string>::iterator it = range_begin; it != range_end; ++it)
		(*first) += ' ' + (*it);
	Delete(n1 + 1, n2);
	currentline_ = n1;
}

void Led::Change(const int n1, const int n2) {
	string find_str;
	string to_str;

	cout << "Change what? ";
	getline(cin, find_str);

	cout << setw(13) << "To what? ";
	getline(cin, to_str);
	string::size_type len_to_str = to_str.size();
	string::size_type len_find_str = find_str.size();

	//counter for occurrence
	int cnt = 0;

	list<string>::iterator range_begin;
	list<string>::iterator range_end;
	range_begin = buffer_.begin();
	range_end = buffer_.begin();
	advance(range_begin, n1 - 1);
	advance(range_end, n2);

	int line_last_occurence = currentline_;
	int tmp_line = n1;

	for (list<string>::iterator it = range_begin; it != range_end; ++it) {
		string::size_type pos = (*it).find(find_str);
		while (pos != string::npos) {
			(*it).replace(pos, len_find_str, to_str);
			++cnt;
			pos = (*it).find(find_str, pos + len_to_str);
			line_last_occurence = tmp_line;
		}
		++tmp_line;
	}
	//update address info: last occurence been replaced
	currentline_ = line_last_occurence;
	cout << "Changed " << cnt << " occurrence(s)" << endl;
}

void Led::Up(const int n1) {

	list<string>::iterator current = buffer_.begin();
	list<string>::iterator dest = buffer_.begin();
	//set to current line
	advance(current, currentline_ - 1);

	//set destination line
	//if doesn't exceed the top of file
	if (currentline_ - n1 > 0) {
		advance(dest, currentline_ - n1 - 1);
		currentline_ = currentline_ - n1;
		buffer_.insert(dest, (*current));
		buffer_.erase(current);
	}
	else {
		currentline_ = 1;
		cout << "Top of file reached\n";
	}
}

void Led::Down(const int n1) {

	list<string>::iterator current = buffer_.begin();
	list<string>::iterator dest = buffer_.begin();
	//set to current line
	advance(current, currentline_ - 1);

	//set destination line
	//if doesn't exceed the end of file
	if (currentline_ + n1 <= lastline_) {
		advance(dest, currentline_ + n1);
		currentline_ = currentline_ + n1;
		buffer_.insert(dest, (*current));
		buffer_.erase(current);
	}
	else {
		currentline_ = lastline_;
		cout << "End of file reached\n";
	}
}

void Led::WriteToFile(bool& file_existed) {
	//file does not exist
	if (!file_existed && filename_ == "") {
		cout << "Enter a file name: ";
		getline(cin, filename_);
	}
	ofstream fout(filename_);
	//output error
	while (!fout) {
		cout << "Write to file error" << endl;
		//an incorrect non-existing file name, gives user opportunity to correct
		if (!file_existed) {
			cout << "Please try to enter a valid file name: ";
			getline(cin, filename_);
			fout.open(filename_);
		}
		else {
			return;
		}
	}

	for (list<string>::iterator it = buffer_.begin(); it != buffer_.end(); ++it) {
		fout << (*it) << endl;
	}
	fout.close();
	//writes to file successfully, update file status by reference
	file_existed = true;
	cout << buffer_.size() << " lines written to file: \"" << filename_ << "\"" << endl;
}

void Led::Exit(bool& file_existed) {
	bool valid_input = false;
	string user_input = "";
	while (!valid_input) {
		cout << "Save changes to a file (y/n)? ";
		getline(cin, user_input);
		if (user_input == "y" || user_input == "n") {
			valid_input = true;
			break;
		}
		cout << "bad answer: " << user_input << endl;
		cout << "Enter y for yes and n for no." << endl;
	}
	if (user_input == "y")
		WriteToFile(file_existed);
	cout << "Bye" << endl;
}