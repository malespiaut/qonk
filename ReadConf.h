// ReadConf.h
// Class for reading named values from configuration files
// Troels Kofoed Jacobsen, Mar 19 2007, tkjacobsen@gmail.com
// Licensed under GPLv2
#ifndef READCONF_H
#define READCONF_H

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <cctype>
#include <sstream>

class Config {
public:
	Config( const std::string& fileName );
	
	std::string getValue( const std::string& parameter ) const;
	bool getValueBOOL( const std::string& parameter ) const;
	int getValueINT( const std::string& parameter ) const;	
	
	void print() const;
		
private:
	std::map< std::string, std::string > data;
};

bool fileExists( const std::string& );
void createFile( const std::string&, std::vector< std::string > );
bool str2bool(const std::string& );
int str2int(const std::string& );
std::string stripWhiteSpace( const std::string& );

#endif
