/*****************************************************************************
    Copyright: Orbbec
    File name: main.cpp
    Description: 若目录不存在，则一层层创建文件夹
    Author: liuchang
    Date: 19-01-30
*****************************************************************************/

#include <iostream>
#include <windows.h>
#include <vector>
#include <direct.h>
#pragma warning(disable:4267)

//************************************
// @function:    dirExists
// @brief:  判断目录是否存在
// @arg[in] dirName_in: 输入的目录名
// @return: bool 存在true
//************************************
inline bool dirExists(const std::string &dirName_in)
{
    DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;  //something is wrong with your path!

    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
        return true;   // this is a directory!

    return false;    // this is not a directory!
}

//************************************
// @function:    getParentDirectory
// @brief:  获得目录的父目录
// @arg[in] directory: 目录
// @return: std::string 父目录
//************************************
inline std::string getParentDirectory(const std::string &directory)
{
    int index = directory.find_last_of("/");
    if (index < 0)
        index = directory.find_last_of("\\");
    if (index < 0)
        return "";
    return directory.substr(0, index);
}

//************************************
// @function:    getParentDirectorys
// @brief:  获得目录的父目录列表
// @arg[in] directory: 目录
// @return: std::vector<std::string> 父目录列表
//************************************
inline std::vector<std::string> getParentDirectorys(std::string directory)
{
    std::vector<std::string> dirs;

    //判断directory最后一列是否为目录，若包含.则认为不是目录
    int idx_point = directory.find_last_of(".");
    int idx = directory.find_last_of("\\");
    if (idx < 0)
    {
        idx = directory.find_last_of("/");
    }
    if (idx_point > idx)
    {
        directory = directory.substr(0, idx);
    }

    do
    {
        dirs.push_back(directory);
        directory = getParentDirectory(directory);
    } while (!directory.empty());
    return dirs;
}

//************************************
// @function:    MakeParentDir
// @brief:  创建目录，若输入路径为文件，则创建所有父目录；
//          若输入路径为目录，则创建目录及其父目录。
// @arg[in] directory: 目录
// @return: bool true 创建成功 false 创建失败
//************************************
bool MakeParentDir(std::string directory)
{
    std::vector<std::string> dirs = getParentDirectorys(directory);

    //若文件夹不存在则创建文件夹
    for (int i = dirs.size() - 1; i >= 0; i--)
    {
        directory = dirs[i];
        if (!dirExists(directory))
        {
            int ret = _mkdir(directory.c_str());
            if (ret < 0)
            {
                return false;
            }
        }
    }
    return true;
}

int main(int argc, char** argv)
{
    std::string directory = argv[1];

    bool ret = MakeParentDir(directory);

    return 0;
}
