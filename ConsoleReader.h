#ifndef ConsoleReader_H
#define ConsoleReader_H
//#include "stdafx.h"
#include <iostream>

#include <string>
#include <vector>
using namespace std;
class ConsoleReader {
public:
	ConsoleReader() {
		
	}
	~ConsoleReader() {


	}

	bool commandRead(string value) {
		std::vector<string> commands;
		commands = splitString(value);

		std::vector<string>::iterator firstRecord = commands.begin();
		string command = *firstRecord;

		if (command.compare("exit")== 0 || command.compare("Exit") == 0) {
			//cout << "exit state" << endl;
			return false;
		}
		else if (command.compare("spawn") == 0 || command.compare("Spawn") == 0) {
			spawn(commands);
		}
		else if (command.compare("boundBox") == 0 || command.compare("boundBox") == 0) {
			boundBox(commands);
		}


			//cout << "First:>"+command<<"<" << endl;

		/*
		for (std::vector<string>::iterator it = commands.begin(); it != commands.end(); ++it){
			std::cout << ' ' << *it;
		}
		*/

		//temp
		return true;
	}


private:
	/*Split input*/
	std::vector<string> splitString(string value) {

		string delimiter = " ";
		std::vector<string> commands;
		bool process = true;


		while (process) {

			if (value == "" || value == " ") {
				process = false;
			}
			if (value.find(delimiter) == string::npos) {
				commands.push_back(value);
				process = false;
			}
			else {
				commands.push_back(value.substr(0, value.find(delimiter)));
				value = value.substr(value.find(delimiter) + 1, value.length());
			}

		}

		return commands;
	}
	/******************************************/
	void spawn(std::vector<string> commands) {
		//cout << "Spawn init " << endl;
	}
	void boundBox (std::vector<string> commands) {
		//cout << "bound start init " << endl;
	}
};
#endif