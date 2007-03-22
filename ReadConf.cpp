#include "ReadConf.h"

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::isspace;
using std::cout;
using std::cerr;
using std::endl;

Config::Config( const std::string& configFileName ){
	ifstream file( configFileName.c_str() );
	while (!file.eof()){
		string line;
		getline( file, line );
		string::size_type loc = line.find( "=", 0 );
		if( line[0] != '#' )
		if( loc != string::npos ) {
			string parameter = stripWhiteSpace( line.substr( 0, loc ) );
			string value = stripWhiteSpace( line.substr( loc + 1 ) );
			data[ parameter ] = value;
		}
	}
	file.close();
	std::cout << "config file read" << std::endl;
}

string 
Config::getValue( const std::string& parameter ) const {
	std::map< std::string, std::string >::const_iterator i = data.find( parameter );
	if( i != NULL )
		return i->second;
	else
		return "";
}

bool 
Config::getValueBOOL( const std::string& parameter ) const {
	string s = getValue(parameter);
	return str2bool(s);
}

int 
Config::getValueINT( const std::string& parameter ) const {
	string s = getValue(parameter);
	return str2int(s);
}

void 
Config::print() const {
	for( std::map< std::string, std::string >::const_iterator i = data.begin(); i != data.end(); i++ )
		std::cout << "'" << i->first << "' => '" << i->second << "'" << std::endl;
}

bool 
fileExists( const std::string& fileName ) {
	ifstream infile( fileName.c_str() );
	bool result = infile.good();
	infile.close();
	return result;
}

void 
createFile( const std::string& fileName, std::vector<string> content ){
	if( !fileExists( fileName ) ){
		ofstream outfile( fileName.c_str() );
		for( std::vector< std::string >::const_iterator iter = content.begin(); iter != content.end(); iter++ ) {
			outfile.write( (*iter).data(), (*iter).size() );
			outfile.write( "\n", 1 );
		}
		outfile.close();
	}
}

bool 
str2bool( const string& s ) {
	if (s == string("true")) {
		return true;
	} else if (s == string("false")){
		return false;
	} else {
		cerr << "Invalid bool type";
	}
}

int 
str2int(const string& s) {
	std::istringstream i(s);
	int x;
	i>>x;
	return x;
}

std::string 
stripWhiteSpace( const std::string& original ) {
	int begin = original.find_first_not_of( " " );
	int end = original.find_last_not_of( " " );
	return original.substr( begin, end - begin + 1 );
}
