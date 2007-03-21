#include "ReadConf.h"

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::isspace;
using std::cout;
using std::cerr;
using std::endl;

Config::Config(char* configfile){
 	ifstream file;
 	file.open(configfile);
 	while (!file.eof()){
 		string s;
 		getline(file,s);
 		if (s[0]!='#')
 			data.push_back(s);
 	}
 	file.close();
}

string Config::getValue(string entry){
	for (vector<string>::const_iterator i = data.begin(); i!=data.end();i++){
		string::const_iterator b=(*i).begin(), e=(*i).end(), iter=b;
		int entryBegin, entryEnd, valueBegin, valueEnd, j=0;
		while (isspace(*iter)){
			iter++;
			j++;
		}
		entryBegin = j;
		while (!isspace(*iter) && *iter != '='){
			iter++;
			j++;	
		}
		entryEnd = j;
		if ((*i).substr(entryBegin,entryEnd-entryBegin)==entry){
			while (isspace(*iter) || *iter == '='){
				iter++;
				j++;	
			}
			valueBegin = j;
			while (!isspace(*iter) && iter!=e){
				iter++;
				j++;	
			}
			valueEnd = j;
			return (*i).substr(valueBegin,valueEnd-valueBegin);	
		}
	}
}

string Config::getValue(char* entry){
	string entryS(entry); //convert to string
	return getValue(entryS);
}

bool Config::getValueBOOL(char* entry){
	string s = getValue(entry);
	return str2bool(s);
}
int Config::getValueINT(char* entry){
	string s = getValue(entry);
	return str2int(s);
}

void Config::printVec(){
	for (vector<string>::const_iterator i = data.begin(); i!=data.end();i++)
		cout << *i << endl;
}

bool fileExists(char* file) {
	ifstream infile;
	infile.open (file);
	bool fE=infile.good();
	infile.close();
	return fE;
}

void createFile(char* file,std::vector<string> content){
	 	if (!fileExists(file)){
	 		ofstream outfile;
			outfile.open (file);
			for (vector<string>::const_iterator iter = content.begin();iter!=content.end();iter++){
				outfile.write ((*iter).data(),(*iter).size());
				outfile.write ("\n",1);
			}
			outfile.close();
		}
}

bool str2bool(const string& s){
	if (s == string("true")){
		return true;
	} else if (s == string("false")){
		return false;
	} else {
		cerr << "Invalid bool type";
	}
}

int str2int(const string& s)
 {
   std::istringstream i(s);
   int x;
   i>>x;
   return x;
 }

