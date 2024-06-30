/**
 * This file represents the header for the Reminder class
 * @author Shahzore Khan (251124330)
 * @since 2023-03-14
 */

#ifndef REMINDERS_H
#define REMINDERS_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib> // for system() function

#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <ctime>

/**
* @brief Structure to hold information about a reminder
*
* This structure holds the message and time information for a reminder.
*/
struct ReminderStruct {
    std::string message;
    int hour;
    int minute;
};

// A map to hold reminders for each day
extern std::map<std::string, std::vector<ReminderStruct>> reminders;

/**
 * @brief A class for managing reminders.
*/
class Reminder{
    public:
        Reminder();
        ~Reminder();
        void addReminder();
        void writeRemindersToFile(std::string filename);
        void viewReminder();
        void removeReminder();
};

#endif /* REMINDERS_H */
