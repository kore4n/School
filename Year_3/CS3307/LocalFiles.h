/**
 * This file represents the header for the LocalFiles class
 * @author Shahzore Khan (251124330)
 * @since 2023-03-14
 */


#ifndef LOCALFILES_HPP
#define LOCALFILES_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>

/**
 * @brief A class for managing local files.
*/
class LocalFiles{
    public:
        LocalFiles();
        ~LocalFiles();
        void createFile();
        void updateFile();
        void deleteFile();
        void viewFiles();
};

#endif