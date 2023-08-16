/**
 * The Reminder class is used to add, remove or view all reminders;
 * it uses the linux build in cron system to save reminders on the linux system.
 * @author Shahzore Khan (250957012)
 * @since 2023-03-14
 */


#include "Reminder.h"

using namespace std;

/**
 * The constructor for the Reminder class
 */
Reminder::Reminder() {}

/**
 * The deconstructor for the Reminder class
 */
Reminder::~Reminder() {}

// A map to hold reminders for each day
map<string, vector<ReminderStruct>> reminders;

/**
 * This function reads the reminders from the reminders.txt file and displays them on the console
 * @return all the reminder for the user to view
 */
void Reminder::viewReminder() {
    std::ifstream infile("reminders.txt");
    cout << "All reminders:" << endl;
    string line;
    while (getline(infile, line)) {
        // Extract the date and time information from the cronjob
        istringstream iss(line);
        string minute, hour, day, month, weekday, command;
        iss >> minute >> hour >> day >> month >> weekday >> command;

        // Convert the date and time information to a time_t value
        tm t;
        t.tm_sec = 0;
        t.tm_min = stoi(minute);
        t.tm_hour = stoi(hour);
        t.tm_mday = stoi(day);
        t.tm_mon = stoi(month) - 1;
        t.tm_year = 2023 - 1900;  // assuming the year is always 2023
        t.tm_isdst = -1;  // use local daylight saving time setting
        time_t time = mktime(&t);

        // Extract the reminder message from the cronjob
        size_t start = line.find("\"") + 1;
        size_t end = line.find_last_of("\"");
        string message = line.substr(start, end - start);

        // Format the date and time as a string and print the reminder message
        stringstream ss;
        ss << message << " (" << put_time(localtime(&time), "%c") << ")";
        cout << "------------------------" << endl;
        cout << ss.str() << endl;
        cout << "------------------------" << endl;
    }
    infile.close();
}

/**
 * This function removes a reminder from the cron job list using by allowing the user 
 * to select the index of the reminder to remove
 */
void Reminder::removeReminder(){
    // Remove a reminder
    ifstream infile("reminders.txt");
    vector<string> lines;
    string line;
    int index = 1;
    while (getline(infile, line)) {
        cout << setw(4) << index << ". " << line << endl;
        lines.push_back(line);
        index++;
    }
    infile.close();

    cout << endl << "Enter the index of the reminder to remove (q to quit): ";
    string input;
    getline(cin, input);
    if (input == "q") {
        return;
    }
    int selection = stoi(input);
    if (selection < 1 || selection > lines.size()) {
        cout << "Invalid selection." << endl;
        return;
    }

    // Remove the selected reminder
    lines.erase(lines.begin() + selection - 1);

    // Rewrite the remaining reminders to the file
    ofstream outfile("reminders.txt", ios::trunc);
    for (string& line : lines) {
        outfile << line << endl;
    }
    outfile.close();

    //send the text files content to cron jobs
    system("./reminder.sh");

    cout << "Reminder removed." << endl;
}

/**
 * This function prompts the user to enter a date, time, and message for the new reminder. The reminder
 * is added to the appropriate date in the reminders map and written to the reminders.txt file. The
 * reminder.sh shell script is executed to update the system's cron job scheduler.
 */
void Reminder::addReminder() {
    while (true) {
        // Prompt the user for a date and time to schedule a reminder
        string date, time, message;
        int hour, minute;
        char ampm;

        // Get a valid date from the user
        while (true) {
            cout << "Enter a date in the format 'month(01 - 12)/day(1 - 31)': ";
            getline(cin, date);
            int month, day;
            if (sscanf(date.c_str(), "%d/%d", &month, &day) != 2) {
                cout << "Error: Invalid date format." << endl;
                continue;
            }
            if (month < 1 || month > 12 || day < 1 || day > 31) {
                cout << "Error: Invalid date." << endl;
                continue;
            }
            break;
        }

        // Get a valid time from the user
        while (true) {
            cout << "Enter a time in the format 'hour:minutes AM/PM': ";
            getline(cin, time);
            if (sscanf(time.c_str(), "%d:%d %c", &hour, &minute, &ampm) != 3) {
                cout << "Error: Invalid time format." << endl;
                continue;
            }
            if (hour < 1 || hour > 12 || minute < 0 || minute > 59) {
                cout << "Error: Invalid time." << endl;
                continue;
            }
            if (ampm == 'p' || ampm == 'P') {
                if (hour != 12) {
                    hour += 12;
                }
            } else if (ampm == 'a' || ampm == 'A') {
                if (hour == 12) {
                    hour = 0;
                }
            } else {
                cout << "Error: Invalid time format." << endl;
                continue;
            }
            break;
        }



        // Get the reminder message from the user
        cout << "Enter a reminder message: ";
        getline(cin, message);

        // Add the reminder to the appropriate date
        ReminderStruct reminder = { message, hour, minute };
        reminders[date].push_back(reminder);

        // Ask the user if they want to add another reminder
        cout << "Reminder added. Add another? (y/n) ";
        string response;
        getline(cin, response);
        if (response != "y") {
            break;
        }
    }

    // Write all reminders to a file
    writeRemindersToFile("reminders.txt");

    // Send the text file's content to cron jobs
    system("./reminder.sh");
}


/**
 * @brief Write reminders to a file
 * This function writes all the reminders to a file with the given filename.
 * The function appends to the file if it already exists, and creates a new file if it doesn't.
 * @param filename The name of the file to write reminders to
 * @return void
 */
void Reminder::writeRemindersToFile(string filename) {
    string path = getenv("PWD");
    path += "/" + filename;
    ofstream outfile(path, ios::app);
    for (auto it = reminders.begin(); it != reminders.end(); it++) {
        vector<ReminderStruct> dateReminders = it->second;
        for (int i = 0; i < dateReminders.size(); i++) {
            ReminderStruct reminder = dateReminders[i];
            int month = stoi(it->first.substr(0, 2));
            int day = stoi(it->first.substr(3));
            
            string outputFilePath = getenv("PWD");
            outputFilePath += "/output.txt";
            outfile << reminder.minute << " " << reminder.hour << " " << day << " " << month  << " * " << "echo \"" << "Reminder: " << reminder.message << "\"" << " >> " << outputFilePath << endl;

        }
    }
    outfile.close();
}
