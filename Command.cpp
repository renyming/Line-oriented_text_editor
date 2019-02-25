#include "Command.h"
#include <regex>
#include <iomanip>

using namespace std;

Command::Command(int currentline, int lastline) :
	currentline_{ currentline },
	lastline_{ lastline },
	cmdstr{ "" },
	comma_{ false },
	cmd_{ '\0' },
	first_{ 0 },
	second_{ 0 }
{}

Command::~Command() {}

bool Command::parse(string input) {
	cmdstr = input;
	//use regular expression to validate command string
	regex reg("^(\\$|\\.|\\d*)(,*)(\\$|\\.|\\d*)([a|i|v|u|d|x|r|j|p|c|\\-|\\+|g|w|q|\\*]*)$");

	//match with regex
	if (regex_match(input, reg)) {

		//extract substring
		smatch m;
		regex_search(input, m, reg);
		/*
		m[1]: fist address
		m[2]: comma
		m[3]: second address
		m[4]: command
		*/

		string tmp_cmd;

		//empty cmd_
		if (input.size() == 0) {
			first_ = 1;
			second_ = 1;
			cmd_ = '+';
			return true;
		}

		//illegal when user input address is "0"
		//however, address "0" is used in some Led member functions
		if (m[1] == "0" || m[3] == "0")
			return false;

		//deal with single ".", "$", +, - and *
		if (input.size() == 1) {
			char single_cmd = input.c_str()[0];
			switch (single_cmd)
			{
			case '.':
				first_ = currentline_;
				second_ = currentline_;
				cmd_ = 'p';
				return true;
			case '$':
				first_ = lastline_;
				second_ = lastline_;
				cmd_ = 'g';
				return true;
			case '+':
			case '-':
				first_ = 1;
				second_ = 1;
				cmd_ = single_cmd;
				return true;
			case '*':
				first_ = 1;
				second_ = lastline_;
				cmd_ = 'p';
				return true;
			}
		}

		int tmp_first = 0;
		int tmp_second = 0;

		//has first address
		if (m[1] != "")
		{
			if (m[1] == ".")
				tmp_first = currentline_;
			else if (m[1] == "$")
				tmp_first = lastline_;
			else
				tmp_first = stoi(m[1]);
		}
		else
			tmp_first = currentline_;

		//has comma
		if (m[2] != "") {
			comma_ = true;
		}

		//has second address
		if (m[3] != "")
			if (m[3] == ".")
				tmp_second = currentline_;
			else if (m[3] == "$")
				tmp_second = lastline_;
			else
				tmp_second = stoi(m[3]);
		else {
			//has comma: x,z=>x,.z; x,=>x,.p
			if (comma_)
				tmp_second = currentline_;
			//no comma: xz=>x,xz; x=>x,xg
			else
				tmp_second = tmp_first;
		}

		first_ = tmp_first;
		second_ = tmp_second;

		//has command
		if (m[4] != "")
			tmp_cmd = m[4];
		else {
			//has comma
			if (comma_)
				tmp_cmd = "p";
			else
				//x=>x,xg
				tmp_cmd = "g";
		}

		cmd_ = tmp_cmd.c_str()[0];

		return true;
	}
	else {
		return false;
	}
}

int Command::getFirstNumber() const {
	return first_;
}

int Command::getSecondNumber() const {
	return second_;
}

char Command::getCommand() const {
	return cmd_;
}

std::ostream& operator<<(std::ostream& out, const Command& cmd) {
	out << setw(4) << cmd.first_ << setw(4) << cmd.comma_ << setw(4) << cmd.second_ << setw(4) << cmd.cmd_;
	return out;
}