#include <iostream>
#include <fstream>
#include <sstream>
#include <SFML/System.hpp>
#include "log.hpp"
#include "info.hpp"

using namespace std;
namespace OUT {
	bool initiated=false, running=false, debugBool=true;    //change this if you don't want the debug output
	ofstream logfile;
	ifstream checkexist;

	void logFileFlusher() { //thread that flushes the log every 30 seconds
		while(running) {
			sf::sleep(sf::seconds(30));
			logfile << flush;
		}
	}
	sf::Thread logFileFlusherThread(&logFileFlusher);
	void init() {
		running=true;
		int logNumber=-1;
		ostringstream fileName;
		do {
			logNumber++;
			fileName.str(std::string());
			fileName << "log/OUT." << logNumber << ".log";  //find the new log for this round (add numbers until we find one we can use)
		} while(checkFileExistance(fileName.str()));
		logfile.open(fileName.str().c_str());   //.c_str turns a string into a char[]
		logFileFlusherThread.launch();
	}
	void deinit() {
		running=false;
		logFileFlusherThread.terminate();   //stop the thread in its place
		logfile << flush;   //flush once more
		logfile.close();
	}
}
	void log(string message) {
		cout << "INFO:     " << message << endl;
		OUT::logfile << "INFO:     " << message << "\n";
	}
	void debug(float num) { debug(tostr(num)); }   //quick access for numbers
	void debug(string message) {
		if(!OUT::debugBool) return;
		cout << "DEBUG:    " << message << endl;
		OUT::logfile << "DEBUG:    " << message << "\n";
	}
	void warn(string message) {

		cout << "WARNING:  " << message << endl;
		OUT::logfile << "WARNING:  " << message << "\n";
	}
	void crit(string message) {
		cout << "CRITICAL: " << message << endl;
		OUT::logfile << "CRITICAL: " << message << "\n";
	}
	bool checkFileExistance(string filename) {
		OUT::checkexist.open(filename.c_str());
		bool returner = OUT::checkexist;    //true if the files exists
		OUT::checkexist.close();
		return returner;
	}
