// LogChecker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#ifdef WIN32
#include "pch.h"
#include <Windows.h>
#include <tchar.h>
#endif

#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <string>
#include "DirectoryReader.h"
#include "LogValidator.h"
using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 3) {
        cout << endl<<"Usage:\n\tLogChecker \"filepattern\" \"CAN ID separated by space\""<<endl << endl;
        return 1;
    }
    //convert const char* to std::string
    wstring pattern(argv[1], argv[1] + strlen(argv[1]));
    DmsLog::DirectoryReader dreader;
    vector<string> files = dreader.collectFileNames (pattern);

    string idstring (argv[2], argv[2] + strlen(argv[2]));
    istringstream iss(idstring);
    int id = 0;
    vector<int> idList{};
    while (iss>>std::hex>>id) { idList.push_back (id); }

    for (const auto name : files) {
        cout<<" --------------------------Log file name "<< name <<endl;
        DmsLog::LogValidator logvalidator(name);
        logvalidator.setIdList(idList);
        logvalidator.processLogs();
    }
    //getchar();
    return 0;
}
