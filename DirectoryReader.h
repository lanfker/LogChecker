#pragma once

#include <string> 
#include <vector> 

#ifndef DIRECTORY_READER_H
#define DIRECTORY_READER_H
using namespace std;


namespace DmsLog {
	class DirectoryReader {
	private:
        /**
         * This method targets windows platform where I use windows.h and tchar.h to read file names from a directory. 
         * The returned file names should match the pattern specified by parameter pattern
         * The default directory is the current work directory. 
         *
         * \param pattern The file name pattern to match. Linux implementation only support a single *
         *
         * \return A list of file names in the CWD that matches the pattern of the specified.
         */
		vector<string> get_all_files_names_within_folder(wstring pattern);

        /**
         * This method targets on Linux to implement a simple pattern matching mechanism.
         *
         * \param pattern The file name pattern to match. Linux implementation only support a single *
         * \param name The file name to be matched
         *
         * \return true if matches, false if unable to match.
         */
		bool patternMatch (string name, string pattern);

	public:
        /**
         * A public interface to collect file names in the CWD that matches the specified pattern
         *
         * \param pattern The file name pattern to match. Linux implementation only support a single *
         *
         * \return A list of file names in the CWD that matches the pattern of the specified.
         */
		vector<string> collectFileNames(wstring pattern);
	};
}
#endif
