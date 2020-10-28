#include <iostream>
#include <windows.h>
#include <vector>
#include <direct.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <regex>

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
    bool foundmatch = false;
    try {
        std::string contains_str = contains;
        std::regex re(contains_str);
        foundmatch = std::regex_search(str, re);
    }
    catch (std::regex_error&) {
        // Syntax error in the regular expression
    }
    return foundmatch;
}

//************************************
// @function:    GetFilenames
// @brief:  遍历目录中的子文件
// @arg[in] dir: 父目录
// @arg[in] filenames: 文件名列表（绝对目录）
// @arg[in] contains: 包含的字符串
// @arg[in] sub_path: 遍历子文件夹
// @return: int 文件数目
//************************************
inline int GetFilenames(const std::string& dir,
    std::vector<std::string> &filenames,
    const std::vector<std::string> &contains = std::vector<std::string>(),
    bool sub_path = true)
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
            if (contains.empty())
            {
                filenames.push_back(iter->path().string());
            }
            else
            {
                for (const auto & contain : contains)
                {
                    if (StringContains(filename, contain.c_str()))
                    {
                        filenames.push_back(iter->path().string());
                        break;
                    }
                }
            }
        }
        if (boost::filesystem::is_directory(iter->status()) && sub_path)
        {
            GetFilenames(iter->path().string(), filenames, contains);
        }
    }
    return static_cast<int>(filenames.size());
}

inline int GetDirectorys(const std::string& dir,
    std::vector<std::string> &paths,
    const std::vector<std::string> &contains = std::vector<std::string>())
{
    boost::filesystem::path path(dir);
    if (!boost::filesystem::exists(path))
    {
        return -1;
    }
    if (boost::filesystem::is_regular_file(path))
    {
        paths.push_back(dir);
        return 0;
    }
    boost::filesystem::directory_iterator end_iter;
    for (boost::filesystem::directory_iterator iter(path); iter != end_iter; ++iter)
    {
        if (boost::filesystem::is_directory(iter->status()))
        {
            std::string path = iter->path().string();
            if (contains.empty())
            {
                paths.push_back(iter->path().string());
            }
            else
            {
                for (const auto & contain : contains)
                {
                    if (StringContains(path, contain.c_str()))
                    {
                        paths.push_back(iter->path().string());
                        break;
                    }
                }
            }
            GetDirectorys(iter->path().string(), paths, contains);
        }

    }
    return static_cast<int>(paths.size());
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
    nPos = str_copy.find(old_sub_str, nPos);
    while (nPos != std::string::npos)
    {
        str_copy.replace(nPos, nsrclen, new_sub_str);
        nPos += ndstlen;
        nPos = str_copy.find(old_sub_str, nPos);
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

//文件名父文件夹下添加加子目录
inline std::string Convert2SubpathParent(std::string filename_input, std::string subpath_name)
{
    using namespace boost::filesystem;
    path file_path(filename_input);
    path parent_path = file_path.parent_path();
    path parent_path_parent = parent_path.parent_path();
    path sub_directory_path = parent_path_parent / subpath_name;
    path parent_path_name = parent_path.filename();
    if (!exists(sub_directory_path))
        create_directories(sub_directory_path);
    path new_parent_path = sub_directory_path / parent_path_name;
    if (!exists(new_parent_path))
        create_directories(new_parent_path);
    path new_file_path = new_parent_path / file_path.filename();
    return new_file_path.string();
}


//************************************
// @function:    GetFilename
// @brief:  获取全路径下的文件名
// @arg[in] filename_input: 全路径
// @return: std::string 文件名 
//************************************
inline std::string GetFilename(std::string filename_input)
{
    boost::filesystem::path path_temp(filename_input);
    return path_temp.filename().string();
}

//************************************
// @function:    GetFilename
// @brief:  获取全路径下的文件名（不包含后缀)
// @arg[in] filename_input: 全路径
// @return: std::string 无后缀的文件名 
//************************************
inline std::string GetFilenameWithoutSuffix(std::string filename_input)
{
    boost::filesystem::path path_temp(filename_input);
    return path_temp.stem().string();
}

//************************************
// @function:    GetFileSuffix
// @brief:  获取文件的后缀（带.)
// @arg[in] filename_input:文件路径
// @return: std::string 后缀名
//************************************
inline std::string GetFileSuffix(std::string filename_input)
{
    boost::filesystem::path path_temp(filename_input);
    return path_temp.extension().string();
}


//************************************
// @function:    ConcatPath
// @brief:  拼接路径
// @arg[in] file_path: 文件夹路径
// @arg[in] filename: 文件名
// @return: std::string 拼接后的全路径
//************************************
inline std::string ConcatPath(std::string file_path, std::string filename)
{
    boost::filesystem::path path1(file_path);
    boost::filesystem::path path2 = path1 / filename;
    return path2.string();
}

//************************************
// @function:    CreateSupportSuffix
// @brief:  创建Opencv支持的图片类型
// @return: std::vector<std::string> string_contains
//************************************
inline std::vector<std::string> CreateSupportSuffix()
{
    std::vector<std::string> string_contains;
    string_contains.emplace_back(".bmp");
    string_contains.emplace_back(".dib");
    string_contains.emplace_back(".jpeg");
    string_contains.emplace_back(".jpg");
    string_contains.emplace_back(".jpe");
    string_contains.emplace_back(".jp2");
    string_contains.emplace_back(".png");
    string_contains.emplace_back(".webp");
    string_contains.emplace_back(".pbm");
    string_contains.emplace_back(".pgm");
    string_contains.emplace_back(".ppm");
    string_contains.emplace_back(".pxm");
    string_contains.emplace_back(".pnm");
    string_contains.emplace_back(".pfm");
    string_contains.emplace_back(".sr");
    string_contains.emplace_back(".ras");
    string_contains.emplace_back(".tiff");
    string_contains.emplace_back(".tif");
    string_contains.emplace_back(".exr");
    string_contains.emplace_back(".hdr");
    string_contains.emplace_back(".pic");
    return string_contains;
}

//************************************
// @function:    CopyDirectory
// @brief:  拷贝目录中内容
// @arg[in] strSourceDir: 待拷贝的目录
// @arg[in] strDestDir: 目标路径 
// @return: bool
//************************************
inline bool CopyDirectoryFiles(const std::string &strSourceDir, const std::string &strDestDir)
{
    boost::filesystem::recursive_directory_iterator end; //设置遍历结束标志，用recursive_directory_iterator即可循环的遍历目录
    boost::system::error_code ec;
    for (boost::filesystem::recursive_directory_iterator pos(strSourceDir); pos != end; ++pos)
    {
        std::string strAppPath = boost::filesystem::path(*pos).string();
        std::string strRestorePath;
        //replace_first_copy在algorithm/string头文件中，在strAppPath中查找strSourceDir字符串，找到则用strDestDir替换，替换后的字符串保存在一个输出迭代器中
        boost::replace_first_copy(std::back_inserter(strRestorePath), strAppPath, strSourceDir, strDestDir);
        if (!boost::filesystem::exists(boost::filesystem::path(strRestorePath).parent_path()))
        {
            boost::filesystem::create_directories(boost::filesystem::path(strRestorePath).parent_path(), ec);
        }
        //目录为空时，创建路径
        if (boost::filesystem::is_directory(*pos))
        {
            boost::filesystem::create_directories(boost::filesystem::path(strRestorePath), ec);
        }

        boost::filesystem::copy_file(strAppPath, strRestorePath, boost::filesystem::copy_option::overwrite_if_exists, ec);
    }
    if (ec)
    {
        return false;
    }
    return true;
}
