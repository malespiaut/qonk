// ReadConf.h
// Class for reading named values from configuration files
// Troels Kofoed Jacobsen, Mar 19 2007, tkjacobsen@gmail.com
// Licensed under GPLv2
#ifndef READCONF_H
#define READCONF_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cctype>
#include <sstream>

class Config {
public:
	Config(char*);
	
	std::string getValue(char*);
	std::string getValue(std::string);
	bool getValueBOOL(char*);
	int getValueINT(char*);	
	
	void printVec();
		
private:
	std::vector<std::string> data;
	
};

bool fileExists(char*);
void createFile(char*,std::vector<std::string>);
bool str2bool(const std::string&);
int str2int(const std::string&);

#endif
