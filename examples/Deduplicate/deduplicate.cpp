/*
De-duplicator, compile with a c++ 11 standards compliant compiler, currently only supported entirely by
g++ from the mingw64 project using specificity the mingw-builds 64 bit structured exception handling(seh) win32 threads
tool-chain.
http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/4.9.1/threads-win32/seh/x86_64-4.9.1-release-win32-seh-rt_v3-rev1.7z

Libraries used:
Boost file system(boost.org) (use until c++ 11 (boost file system will be included in c++ 14's std library)
Utility ( https://github.com/kd-boss/Utility )  while there's other libraries for SHA256, this is just a very thin
wrapper over the windows api, and as such introduces almost NO overhead at runtime. Even more importantly, it's header
only, so no compiling required, just include it in code and link to the appropriate windows dll's (bcrypt, crypt32).

if using mingw:
1) make sure the tool chain is added to the PATH environment variable.
2) make sure boost header files are installed in %mingwfolder%/x86_64-w64-mingw32/include/boost
3) make sure boost libraries files are installed in %mingwfolder%/x86_64-w64-mingw32/libs
4) rename boost lib's to remove the compiler version and library version from the file name
ex:
debug build - libboost_whatever-mt-49-d-1_56.lib to libboost_whatever-d.lib
release build - libboost_whatever-mt-49-1_56.lib to libboost_whatever.lib

5) make sure Utility headers are installed in %mingwfolder%/x86_64-w64-mingw32/include/utility
6) compile with the following:
g++ deduplicate.cpp -static -std=c++11 -O3 -llibboost_filesystem -llibboost_system -lcrypt32 -lbcrypt -o deduplicate.exe

This utility works very well with non system folders (ie music, movie, pictures , general file folders), but do not run
the output of this utility on any system folders or installed program locations.
*/

// standard library headers
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>

// My Utility library
#include <utility/System.Cryptography.h>

// boost headers
#include <boost/filesystem.hpp>

template <typename T> bool increment_stub(T &t)
{
    try
    {
        t.operator++();
        return true;
    }
    catch (std::exception &e)
    {
        t.no_push();
        std::cout << e.what() << std::endl;
        return false;
    }
}

// increment function for boost file system iterators, hides the while loop, templates are used to keep typing down to a
// minimum.
template <typename T> void increment(T &t, T &u)
{
    while (!increment_stub(t) && t != u)
    {
    }
}

// drag in boost.
using namespace boost;
using namespace boost::filesystem;

void PrintUsage()
{
    std::string message =
        R"~(
                            Deduplicate! version 1.5
About:
finds duplicate files and generates a bat file to delete the duplicates

Usage:
deduplicate <directory>
                            - then -
de-dup.bat
             - if you need to restory the directory -
re-dup.bat

Usage Example:
deduplicate C:\Users\JohnDoe\Documents

Corner Cases:
If a <directory> has any spaces in it (ex: %HOMEPATH%\My Documents) surround it with double quotes.
But do not end <directory> with a '\' when doing so, if you must end <directory> with a '\', use 2
ex:
deluplicate "C:\Some Dir\\" )~";
    std::cout << message << std::endl;
}

int main(int argc, char *argv[])
{
    try // gracefully handle all errors.
    {
        if (argc != 2)
        {
            PrintUsage();
            return 0;
        }
        std::cout << "Starting in: " << argv[1] << std::endl;

        // persistent variables.
        std::map<std::string, std::string> uniques;
        std::fstream outfile;
        std::fstream hashes;
        filesystem::recursive_directory_iterator it, end;
        std::vector<unsigned char> buffer;
        std::size_t iread;
        std::pair<std::map<std::string, std::string>::iterator, bool> res;
        std::string hash;
        System::ByteVector ret;

        // initiate persistent variables.
        buffer.resize(2 * 1024 * 1024); // 2MB
        outfile.open("de-dupe.bat", std::ios_base::out);
        hashes.open("re-dupe.bat", std::ios_base::out);
        it = filesystem::recursive_directory_iterator(argv[1]);

        while (it != end)
        {
            try // every one of these operations -can- throw. Handle it all within the loop to keep prevent halting
                // until the file system iterator is at the end.
            {
                if (filesystem::is_regular_file(it->path()))
                {
                    // construct our temporaries.
                    System::Crypto::Hash::CBCryptSHA256 crypt;
                    std::ifstream file;

                    // open the current file
                    file.open(it->path().string(), std::ios_base::binary);

                    // read whole file, in 2MB chunks, adding the chunks to the incremental hash.
                    while ((iread = file.readsome(reinterpret_cast<char *>(&buffer[0]), buffer.size())))
                    {
                        crypt.AddDataToHash(System::ByteVector(&buffer[0], &buffer[iread - 1]));
                    }

                    // obtain the completed SHA256
                    ret = crypt.Finalise();

                    // convert it to a hexadecimal string.
                    hash = System::Crypto::BinaryToStringA(System::Crypto::CryptStringType::HexNoCrLf, ret);

                    // remove the space characters from the hexadecimal string.
                    hash.erase(std::remove(std::begin(hash), std::end(hash), ' '), hash.end());

                    // try to add it to the std::map.
                    res = uniques.emplace(hash, it->path().string());

                    // if emplacement into the std::map fails, we have a duplicate
                    if (res.second == false)
                    {
                        std::cout << "found duplicate: " << std::endl
                                  << res.first->second << std::endl
                                  << it->path().string() << std::endl
                                  << std::endl;
                        hashes << "copy " << res.first->second << " " << it->path().string() << std::endl;
                        outfile << "del " << it->path().string() << std::endl;
                    }
                }
                // increment the file system iterator.
                increment(it, end);
            }
            catch (std::exception &e)
            {
                // display the error.
                std::cout << e.what() << std::endl;

                // increment the file system iterator.
                increment(it, end);
            }
        }
        std::cout << "Finished!" << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }
}
