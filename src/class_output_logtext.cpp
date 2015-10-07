#include "include/output_logtext.hpp"

std::ofstream jubeat_online::OutputLogtext::file;
std::mutex jubeat_online::OutputLogtext::mtx_;

int jubeat_online::OutputLogtext::Output(const char * str,const char * from)
{
#ifdef _DEBUG
	std::lock_guard<std::mutex> lock(mtx_);
	file.open(OUTPUT_LOGTEXT_FILENAME, std::ios::app);

	if (file.fail() || file.bad()) {
		std::cout << "OutputLogtext>o—Í‚ª‹‘”Û‚³‚ê‚Ü‚µ‚½\n";
		return -1;
	}

	if (from == NULL) {
		file << ">" << str << "\n";
		std::cout << ">" << str << "\n";
	}
	else {
		file << from << ">" << str << "\n";
		std::cout << from << ">" << str << "\n";
	}

	file.close();
#endif
	return 0;
}

int jubeat_online::OutputLogtext::OutputS(const std::string str, const std::string from)
{
	Output(str.c_str(), from.c_str());
	return 0;
}

