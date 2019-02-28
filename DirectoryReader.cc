#include "DirectoryReader.h"
namespace DmsLog {
    DirectoryReader::DirectoryReader (const string& pattern) : namePattern{pattern} {}

    vector<string>  DirectoryReader::collectFileNames() const {
        vector<string> ans;
        DIR *dirp = opendir(".");
        while (dirp)
        {
            errno = 0;
            dirent *dp = nullptr;
            if ((dp = readdir(dirp)) != nullptr)
            {
                std::string fname(dp->d_name);
                if (fname.size() >= namePattern.size() && fname.substr(0, namePattern.size()) == namePattern)
                {
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
        return ans;
    }
}