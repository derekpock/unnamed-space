#ifndef __Main_Log
#define __Main_Log

namespace OUT {
	extern bool initiated, debugBool;	//true if its ready, false otherwise; change the debugBool to false or true for debug output
	void init();	//must be called before logging is to occur, only call once (called my main_main)
	void deinit();	//cleanup only
}
	void log(std::string);	//basic out
	void debug(std::string);	//rather verbose output for means of debugging, only activated if OUT::debugBool is true
	void debug(float);      //easy use of debug with a number instead of manually doing std::to_string()
	void warn(std::string);	//more serious out, not neccessarily devastating but not normal
	void crit(std::string);	//something that would cause the program to fail if it's not fixed...out
	bool checkFileExistance(std::string);	//true if it does, false otherwise - relative to working directory, absolute capability

#endif
