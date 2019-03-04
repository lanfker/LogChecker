#pragma once

#include <string> 
#include <vector> 

#ifndef DIRECTORY_READER_H
#define DIRECTORY_READER_H
using namespace std;


namespace DmsLog {
	class DirectoryReader {
	private:
		vector<string> get_all_files_names_within_folder(wstring pattern);
		bool patternMatch (string name, string pattern);

	public:
		vector<string> collectFileNames(wstring pattern);
	};
}
#endif
