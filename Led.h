#ifndef LED_H
#define LED_H

#include "Command.h"
#include <list>
#include <vector>
#include <string>
#include <sstream>
/**
 * Line-oriented text editor, depends on Command class to parse command line
 */
class Led {
public:
    /**
     * Constructor
     * @param filename text file name for input
     */
	Led(std::string filename);

	/**
	 * Destructor
	 */
	~Led();

	/**
	 * Starts editing session by reading in text file whose file name is provided in constructor
	 */
	void run();
private:
	std::string filename_;
	std::list<std::string> buffer_;
	std::vector<std::string> clipboard_;
	int currentline_;
	int lastline_;

	/**
	 * Auxiliary function. Reads text file line-by-line into buffer list.
	 * @param filename text file name to open
	 * @return
	 */
	bool ReadFile(const std::string filename);

	/**
	 * Switches into input mode, and inserts text before line n1
	 * @param n1 line number to insert before
	 */
	void Insert(const int n1);

	/**
	 * Switches into input mode, and appends text after line n1
	 * @param n1 line number to append after
	 */
	void Append(const int n1);

	/**
	 * Prints out the content of lines between n1 and n2
	 * @param n1 start line to be printed
	 * @param n2 last line to be printed
	 */
	void Print(const int n1, const int n2) const;

	/**
	 * Set current line to n1
	 * @param n1 line number to be set as current line
	 */
	void Goto(const int n1);

	/**
	 * Auxiliary function. Detects if buffer is empty, and prints out buffer empty info if it's true
	 * @return empty status of buffer
	 */
	bool isEmpty() const;

	/**
	 * Cuts the lines between n1 and n2 into the clipboard
	 * @param n1 start line to be cut
	 * @param n2 last line to be cut
	 */
	void Cut(const int n1, const int n2);

	/**
	 * Pastes text from the clipboard into the buffer below line n1
	 * @param n1 line number to paste below
	 */
	void PasteBelow(const int n1);

	/**
	 * Pastes text from the clipboard into the buffer above line n1
	 * @param n1 line number to paste above
	 */
	void PasteAbove(const int n1);

	/**
	 * Deletes lines from n1 to n2 from buffer
	 * @param n1 start line to be deleted
	 * @param n2 last line to be deleted
	 */
	void Delete(const int n1, const int n2);

	/**
	 * Switches into input mode and replaces the lines between n1 and n2 with new text
	 * @param n1 start line to be replaced
	 * @param n2 last line to be replaced
	 */
	void Replace(const int n1, const int n2);

	/**
	 * Concatenates lines from n1 to n2 into n1, with a white space between each previous lines
	 * @param n1 start line to be joined
	 * @param n2 last line to be joined
	 */
	void Join(const int n1, const int n2);

	/**
	 * Changes substring into new substring, and sets the current line to the last line that has been changed
	 * @param n1 start line to be changed
	 * @param n2 last line to be changed
	 */
	void Change(const int n1, const int n2);

	/**
	 * Moves current line up by n1 lines. If there is less than n1 above, prints out "top of line reached"
	 * @param n1 number of lines to move current line up by
	 */
	void Up(const int n1);

	/**
	 * Moves current line down by n1 lines. If there is less than n1 below, prints out "end of line reached"
	 * @param n1 number of lines to move current line down by
	 */
	void Down(const int n1);

	/**
	 * Writes buffer into file. If it's a non-existed new file, prompts to ask for file name; if it's a new file and
	 * file name is invalid, gives user another opportunity to enter a valid file name
	 * @param file_existed reference to a bool flag indicates whether the file existed or not
	 */
	void WriteToFile(bool& file_existed);

	/**
	 * Prompts to ask user whether to save current buffer to file before exit the program
	 * @param file_existed reference to a bool flag indicates whether the file existed or not
	 */
	void Exit(bool& file_existed);
};

#endif // !LED_H

