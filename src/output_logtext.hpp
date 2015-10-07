#pragma once

#ifndef JUBEAT_ONLINE_OUTPUT_LOGTEXT_H
#define JUBEAT_ONLINE_OUTPUT_LOGTEXT_H

#include <iostream>
#include <string>
#include <fstream>
#include <mutex>

namespace jubeat_online {

#define OUTPUT_LOGTEXT_FILENAME "DebugLog.txt"

	class OutputLogtext {
	private:
		
		static std::ofstream file;
		static std::mutex mtx_;

	public:
		
		static int Output(const char* str, const char * from = NULL);
		static int OutputS(const std::string str, const std::string from = "");
	
	};
}

#endif