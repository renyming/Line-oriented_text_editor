#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <iostream>

/**
 * Command class to validate, and parse command line into first number, second number and command char
 */
class Command {
	
	/**
	 * Overrides << operator to enable "cout<<cmd" for debug purpose
	 * @param out reference of ostream
	 * @param cmd reference of Command object
	 * @return reference of ostream
	 */
	friend std::ostream& operator<<(std::ostream& out, const Command& cmd);

public:

	/**
	 * Constructor
	 * @param currentline current line number in integer
	 * @param lastline last line number in integer
	 */
	Command(int currentline, int lastline);
	
	/**
	 * Destructor
	 */
	~Command();
	
	/**
	 * Validates and parses command line
	 * @param input command line string
	 * @return the validity of command
	 */
	bool parse(std::string input);
	
	/**
	 * Getter for first number in command
	 * @return first number in command
	 */
	int getFirstNumber() const;
	
	/**
	 * Getter for second number in command
	 * @return second number in command
	 */
	int getSecondNumber() const;
	
	/**
	 * Getter for command
	 * @return command in char type
	 */
	char getCommand() const;
private:
	std::string cmdstr;
	int currentline_;
	int lastline_;
	bool comma_;
	char cmd_;
	int first_;
	int second_;
};

#endif // !COMMAND_H
