/**
 * The LocalFiles class is used to add, remove, update or view all files;
 * @author Shahzore Khan (250957012)
 * @since 2023-03-14
 */

#include "LocalFiles.h"


using namespace std;
/**
 * The constructor for the LocalFiles class
 */
LocalFiles::LocalFiles() {}
/**
 * The deconstructor for the LocalFiles Class class
 */
LocalFiles::~LocalFiles() {}

/**
 * @brief Creates a new file and writes content to it
 * This function prompts the user to enter a filename and the content to be written to the file.
 * It then creates a new file with the given filename and writes the content to it.
 * @return null
*/
void LocalFiles::createFile(){
    string filename, content;
    ofstream outfile;
    
    cout << "Enter the name of the file to create: ";
    cin >> filename;
    outfile.open(filename.c_str()); //creates file
    cout << "Enter the content to write to the file: ";
    cin.ignore();
    getline(cin, content);
    outfile << content << endl;     //stores content to file
    outfile.close();
    cout << "File created successfully." << endl;
}

/**
 * @brief Updates an existing file with additional content.
 * Prompts the user to enter the name of the file to update, and the content
 * to append to the file. Opens the file in append mode, and writes the new
 * content to the end of the file. Finally, closes the file and prints a
 * success message.
 * @return null
*/
void LocalFiles::updateFile() {
    string filename, content;
    ofstream outfile2;
    
    cout << "Enter the name of the file to update: ";
    cin >> filename;
    outfile2.open(filename.c_str(), ios::app);  //opens file to update
    cout << "Enter the content to append to the file: ";
    cin.ignore();
    getline(cin, content);
    outfile2 << content << endl;        //updates content 
    outfile2.close();
    cout << "File updated successfully." << endl;
}

/**
 * @brief Deletes the file with the given filename.
 * This function prompts the user to enter the filename of the file to delete.
 * If the file exists, it is deleted; otherwise, an error message is displayed.
 * @return null
*/
void LocalFiles::deleteFile() {
    string filename;
    cout << "Enter the name of the file to delete: ";
    cin >> filename;
    if (remove(filename.c_str()) != 0)
    {
        cout << "Error deleting file" << endl;
    }
    else
    {
        cout << "File successfully deleted" << endl;
    }
}

/**
 * @brief Displays a list of files in the current working directory.
 * This function opens the current working directory and iterates through each file, displaying the name of each regular file.
 * If there is an error opening the directory, an error message will be displayed.
*/
void LocalFiles::viewFiles() {
    DIR *dir; //create directory object
    struct dirent *ent;
    if ((dir = opendir(".")) != NULL)
    {
        cout << "Files in the working directory:" << endl;
        while ((ent = readdir(dir)) != NULL)    //while loop iterates through each file
        {
            if (ent->d_type == DT_REG)
            {
                cout << ent->d_name << endl;
            }
        }
        closedir(dir);
    }
    else
    {
        perror("Error opening directory");
    }
}





