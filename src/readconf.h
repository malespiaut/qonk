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
	static Config& getInstance();

	bool load();
	bool save();
	
	std::string getValue( const std::string& parameter, std::string defaultValue = "") const;
	void setValue (const std::string &, std::string value);

	bool getValueAsBool( const std::string& parameter, bool defaultValue = false ) const;
	void setValue (const std::string &, bool value);

	int getValueAsInt( const std::string& parameter, int defaultValue = 0 ) const;	
	void setValue (const std::string &, int value);
	
	void print() const;
		
private:
	Config( const std::string& fileName );

	template <typename T> static T fromString(const std::string&);

	template <typename T> static std::string toString(const T&);

	std::map< std::string, std::string > data;

	std::string fileName;

	static Config *instance;
};

bool fileExists( const std::string& );
bool str2bool(const std::string& );
std::string stripWhiteSpace( const std::string& );

#endif
