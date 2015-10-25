#pragma once

#ifndef JUBEAT_ONLINE_INI_FILEREADER_H_
#define JUBEAT_ONLINE_INI_FILEREADER_H_

#include <string>
#include <fstream>

namespace jubeat_online {

	struct INIFileReaderElements{
		std::string section_;
		std::string key_;
		std::string value_;
		INIFileReaderElements * next;
	};

	class INIFileReader {
	public:

		INIFileReader();
		INIFileReader(const std::string filename);

		int LoadFile(const std::string filename);
		int ReloadFile();

		std::string getValue(const std::string section, const std::string key, const std::string def_str = "") const;
		int RewriteValue(const std::string section, const std::string key, std::string value);

		void Delete();

		~INIFileReader() {}
	
	private:
		//unsigned int element_num_;

		INIFileReaderElements *data_;

		std::fstream file_;

		std::string filename_;

		bool is_load_;

		void Delete(INIFileReaderElements * d);

	};

};


#endif
