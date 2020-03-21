#include <iostream>
#include <windows.h>
#include <vector>
#include <direct.h>
#include <boost/filesystem.hpp>
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
inline bool MakeParentDir(std::string directory)
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

///************************************
// @function:    StringContains
// @brief:  字符串包含
// @arg[in] str: 字符串
// @arg[in] contains: 包含子字符
// @return: bool true false
///************************************
inline bool StringContains(const std::string &str, const char* contains)
{
	std::string::size_type position;
	position = str.find(contains);
	if (position != str.npos)
		return true;
	return false;
}
///************************************
// @function:    GetFilenames
// @brief:  遍历目录中的子文件
// @arg[in] dir: 父目录
// @arg[out] filenames: 文件名列表（绝对目录）
// @return: int 文件数目
///************************************
inline int GetFilenames(const std::string& dir, std::vector<std::string>& filenames)
{
	boost::filesystem::path path(dir);
	if (!boost::filesystem::exists(path))
	{
		return -1;
	}
	if (boost::filesystem::is_regular_file(path))
	{
		filenames.push_back(dir);
		return 0;
	}
	boost::filesystem::directory_iterator end_iter;
	for (boost::filesystem::directory_iterator iter(path); iter != end_iter; ++iter)
	{
		if (boost::filesystem::is_regular_file(iter->status()))
		{
			std::string filename = iter->path().string();
			if (StringContains(filename, ".bmp"))
				filenames.push_back(iter->path().string());
		}
		if (boost::filesystem::is_directory(iter->status()))
		{
			GetFilenames(iter->path().string(), filenames);
		}
	}
	return static_cast<int>(filenames.size());
}

//************************************
// @function:    StringReplace
// @brief:  替换字符串
// @arg[in] str: 待替换的完整字符串
// @arg[in] old0: 待替换的子串
// @arg[in] new0: 替换为xxx
// @return: std::string 替换后的字符串
//************************************
inline std::string StringReplace(const std::string &str, const std::string old_sub_str, const std::string new_sub_str)
{
	std::string str_copy = str;
	std::string::size_type nPos = 0;
	std::string::size_type nsrclen = old_sub_str.size();
	std::string::size_type ndstlen = new_sub_str.size();
	while (nPos = str_copy.find(old_sub_str, nPos))
	{
		if (nPos == std::string::npos) break;
		str_copy.replace(nPos, nsrclen, new_sub_str);
		nPos += ndstlen;
	}
	return str_copy;
}

//文件名加子目录
inline std::string Convert2Subpath(std::string filename_input, std::string subpath_name)
{
	using namespace boost::filesystem;
	path file_path(filename_input);
	path parent_path = file_path.parent_path();
	path sub_directory_path = parent_path / subpath_name;
	if (!exists(sub_directory_path))
		create_directories(sub_directory_path);
	path new_file_path = sub_directory_path / file_path.filename();
	return new_file_path.string();
}
