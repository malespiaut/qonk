#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/stat.h>

#include "readconf.h"

Config *Config::instance = 0;

Config&
Config::getInstance() {
	if (!instance) {
		instance = new Config("qonk.settings");
	}

	return *instance;
}

Config::Config( const std::string& configFileName ){
	std::string pathname = "";
#ifdef WINDOWS
	// Do nothing. Falls back to application directory.
#elif MAC_OSX
	// Get from some mysterious directory ...
#else
	// GNU/LINUX/BSD/POSIX/...
	if(getenv("HOME")!=NULL) {
		pathname = getenv("HOME");
		pathname += "/.qonk/";
		mkdir(pathname.c_str(), 0755);
        }
#endif

	fileName = pathname + configFileName;
}

bool
Config::load() {
	if( !fileExists(fileName))
		return false;

	std::ifstream file( fileName.c_str() );
	while (!file.eof()){
		std::string line;
		getline( file, line );

		std::string::size_type loc = line.find( "=", 0 );
		if( line[0] != '#' )
		if( loc != std::string::npos ) {
			std::string parameter = stripWhiteSpace( line.substr( 0, loc ) );
			std::string value = stripWhiteSpace( line.substr( loc + 1 ) );
			data[parameter] = value;
		}
	}
	file.close();

	return true;
}

bool
Config::save() {
	std::ofstream outfile( fileName.c_str() );
	for( std::map< std::string, std::string >::const_iterator i = data.begin(); i != data.end(); i++ ) {
		std::string temp = i->first + "=" + i->second;
		outfile.write( temp.data(), temp.size() );
		outfile.write( "\n", 1 );
	}
	outfile.close();

	return true;
}

std::string 
Config::getValue( const std::string& parameter, std::string defaultValue ) const {
	std::map< std::string, std::string >::const_iterator i = data.find( parameter );
	if( i != data.end() )
		return i->second;
	else
		return defaultValue;
}

void
Config::setValue( const std::string& parameter, std::string value) {
	data[parameter] = value;
}

bool 
Config::getValueAsBool( const std::string& parameter, bool defaultValue ) const {
	std::map< std::string, std::string >::const_iterator i = data.find( parameter );
	if( i != data.end() )
		return fromString<bool>(i->second);
	else
		return defaultValue;
}

void
Config::setValue( const std::string& parameter, bool value) {
	data[parameter] = toString<bool>(value);
}

int 
Config::getValueAsInt( const std::string& parameter, int defaultValue ) const {
	std::map< std::string, std::string >::const_iterator i = data.find( parameter );
	if( i != data.end() )
        {
		return fromString<int>(i->second);
        }
	else
		return defaultValue;
}

void
Config::setValue( const std::string& parameter, int value) {
	data[parameter] = toString<int>(value);
}

void 
Config::print() const {
	for( std::map< std::string, std::string >::const_iterator i = data.begin(); i != data.end(); i++ )
		std::cout << "'" << i->first << "' => '" << i->second << "'" << std::endl;
}

template <typename T>
T
Config::fromString( const std::string& s) {
	std::istringstream iss(s);

	T target;

	iss >> target;

	return target;
}

template <typename T>
std::string
Config::toString( const T& t) {
	std::ostringstream oss;

	oss << t;

	return oss.str();
}

bool 
fileExists( const std::string& fileName ) {
	std::ifstream infile( fileName.c_str() );
	bool result = infile.good();
	infile.close();
	return result;
}

bool 
str2bool( const std::string& s ) {
	if (s == std::string("true")) {
		return true;
	} else if (s == std::string("false")){
		return false;
	} else {
		std::cerr << "Invalid bool type";
	}
}

std::string 
stripWhiteSpace( const std::string& original ) {
	int begin = original.find_first_not_of( " " );
	int end = original.find_last_not_of( " " );
	return original.substr( begin, end - begin + 1 );
}
