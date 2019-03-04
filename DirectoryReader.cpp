#include "DirectoryReader.h"
#ifdef WIN32
#include "pch.h"
#include <Windows.h>
#include <tchar.h>
#elif __linux__
#include <dirent.h>
#endif
#include <iostream>

namespace DmsLog {

	vector<string>  DirectoryReader::collectFileNames(wstring pattern) {
		std::vector<std::string> ans;
#ifdef __linux__
        string tmpPattern (pattern.begin(), pattern.end());
		std::cout<<" pattern " << tmpPattern<<std::endl;
		DIR *dirp = opendir(".");
		while (dirp)
		{
			errno = 0;
			dirent *dp = nullptr;
			if ((dp = readdir(dirp)) != nullptr)
			{
				std::string fname(dp->d_name);
				if (patternMatch (fname, tmpPattern)) {
					ans.push_back(fname);
				}
			}
			else
			{
				if (errno == 0)
				{
					closedir(dirp);
					break;
				}
				closedir(dirp);
				break;
			}
		}
#elif _WIN32
		ans = get_all_files_names_within_folder(pattern);
#endif

		return ans;
	}

	bool DirectoryReader::patternMatch(string name, string pattern) {
		if (name.size() < pattern.size()) return false;
		if (pattern == "*") return true;
		else {
			int i = 0;
			for ( ;i < pattern.size(); ++ i) {
				if (pattern[i] == '*') break;
			}
			string first = pattern.substr(0, i);
			string second = i+1>=pattern.size() ? "" :  pattern.substr (i+1);
			if (name.substr (0,i) == first && name.substr (name.size() - second.size() ,second.size()) == second ) { return true;}
			else return false;
		}
	}

	/// abcdefg /// len = 7, size = 3

#ifdef _WIN32
	vector<string> DirectoryReader::get_all_files_names_within_folder(wstring pattern)
	{
		vector<string> names;
		WIN32_FIND_DATA fd;
		HANDLE hFind = ::FindFirstFile(pattern.c_str(), &fd);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					wstring ws(fd.cFileName);
					string fname(ws.begin(), ws.end());
					names.push_back(fname);
				}
			} while (::FindNextFile(hFind, &fd));
			::FindClose(hFind);
		}
		return names;
	}
#endif

}