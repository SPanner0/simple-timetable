#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <fstream>
#include <sstream>
#include <iomanip>
using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::setw;
using std::setfill;


// Declare struct to store data from the file to be manipulated locally
struct subject
{
    string code;
    string day;
    int hour;
    int minute;
    bool added;     // State unsaved added state in file
    bool deleted;   // State unsaved deleted state in file
};

vector<subject> sessions;   // Vector to store subject info in struct
int minHour = 8;    // Minimum hour supported by the program (larger ranges look messed up on smaller terminals)
int maxHour = 17;   // Minimum hour supported by the program
const int colW = 10; // Width of each column on timetable
string validDays[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};  // List of weekdays supported by the program

// Const chars used to draw timetable
const char v = '|';
const char h = '-';
const char c = '+';

// Main functions
int eventHandler();
void loadFile(vector<subject> &sessions);
int addSession();
int deleteSession();
int editSession();
int printSessions();
int saveChanges();
int askToSave();
int printTimetable();

// Support/Utility functions for the main functions
bool validDay(string day);
void drawCell(string filling);
void drawBorder(int numOfCols);
void drawHeader(int colW, vector<int> &timeVec, int numOfCols);
void drawSessionRows(int numOfCols);
string sessionFromCoords(string day, int col);
bool checkForConflict(int sessionIndex);


// Driver function
int main()
{   
    try
    {
        // Load file unto struct variable
        loadFile(sessions);

        cout << "Welcome to the timetable console application made by Shawn, Luqman, and Ahmed!\n";

        // Main loop
        while (true)
        {
            // Clear input stream before proceeding
            cin.clear();

            // Calls event handler and closes the loop when 1 is returned
            if (eventHandler() == 1)
            {
                break;
            }

            cout << "\nPress Enter to continue..."; // Pause program until user enters something (or just hits enter) after each command 
            cin.get();
            cin.clear();
            cin.ignore(10000, '\n');
        }

        cout << "\nExiting timetable program..." << std::endl;

        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}


// Handle user input and call the appropriate functions
int eventHandler()
{
    // Print a menu and ask user for input
    cout << "\nPlease select an option by entering a number corresponding to the option you want below:\n\n";
    cout << "1) List Classes\n2) Add Class\n3) Delete Class\n4) Edit Class\n5) Save Changes to File\n6) Print Timetable\n7) Exit Program\n";
    cout << "Enter here: ";

    int selection;
    cin >> selection;

    cout << '\n';

    // Check if input is valid or is within bounds
    if (cin.fail()) 
    {
        cout << "Invalid option entered." << '\n';
        cin.clear();
        cin.ignore(10000, '\n');
        return -1;
    }
    else
    {
        if (selection == 1)
        {
            // List all sessions
            return printSessions();
        }
        else if (selection == 2)
        {
            // To add a session unto list
            return addSession();
        }
        else if (selection == 3)
        {
            // Erase a session from the list
            return deleteSession();
        }
        else if (selection == 4)
        {
            // Edit a session
            return editSession();
        }
        else if (selection == 5)
        {
            // Save changes that were made locally
            return saveChanges();
        }
        else if (selection == 6)
        {
            // Print timetable to terminal
            return printTimetable();
        }
        else if (selection == 7)
        {
            // Return 1 to terminate the program
            return askToSave();
        }
        else 
        {
            // Integer input out of bounds
            cout << "Input out of bounds. Please enter a number based on the options provided in the menu.\n";
            cin.ignore(10000, '\n');
            return -1;
        }
    }
}


// Read data from text file and store the data in a vector of structs to be manipulated locally
void loadFile(vector<subject> &sessions)
{
    string line;
    std::ifstream dataFile("timetable-data.txt");

    // Iterate through each line of the text file
    while (getline(dataFile, line))
    {
        subject session;
        std::stringstream ss(line);
        string data;
        int i = 0;

        while (getline(ss, data, '|'))
        {
            // Assign each data read from the file to their respective struct attribute
            std::stringstream ls(data);
            
            if (i == 0) ls >> session.code;
            else if (i == 1) ls >> session.day;
            else if (i == 2) ls >> session.hour;
            else if (i == 3) ls >> session.minute;
            else break;

            if (ls.fail())
            {
                throw std::runtime_error("Invalid data detected in file. Please either fix the data or delete the file entirely (Warning: This will delete ALL your saved data.)");
            }

            i++;
        }
        if (session.code.length() > colW - 1) throw std::runtime_error("Invalid class name detected. Name might be too long. You can edit it from the text file.");
        if (!validDay(session.day)) throw std::runtime_error("One of the classes has an invalid day of the week.");
        if (session.hour < minHour || session.hour > maxHour) throw std::runtime_error("One of the classes has an hour value outside the bounds supported by the program. Note that the program reads time in a 24 hour format.");
        if (session.minute != 0 && session.minute != 30) throw std::runtime_error("One of the classes has an invalid minute; only 0 and 30 minutes are supported.");
        session.added = false;          // Indicate that the session is not an unsaved change
        session.deleted = false;
        sessions.push_back(session);    // Add to list of structs
    }
    dataFile.close();
}


// Adds a session onto the list of sessions
int addSession()
{
    // Declare variables to store session attributes
    subject session;
    string code;
    string day;
    int hour;
    int minute;

    cin.ignore(10000, '\n');

    // Class code
    cout << "\nEnter the code for your subject (max " << colW - 1 << " characters): ";
    getline(cin, code);
    if (code.length() > (colW - 1))
    {
        cout << "Code entered is too long, please try with a shorter code.\n";
        cin.ignore(10000, '\n');
        return -1;
    }

    cout << "\nWhat day of the week will this class be on?: ";
    getline(cin, day);

    // Day of the week
    if (!validDay(day))
    {
        cout << "Invalid day of the week entered. Valid entries are: Monday, Tuesday, Wednesday, Thursday, Friday.\n";
        cin.clear();
        cin.ignore(10000, '\n');
        return -1;
    }

    // Hour the class will be at
    cout << "\nAt what hour (24 hour format) will this class be held at?: ";
    cin >> hour;
    if (cin.fail() || hour < minHour || hour > maxHour)
    {
        cout << "Invalid value for hour entered.\n";
        cin.clear();
        cin.ignore(10000, '\n');
        return -1;
    }

    // Minute the class will be at
    cout << "\nAt what minute (0 or 30) of the hour will this class be held at?: ";
    cin >> minute;
    if (cin.fail() || (minute != 0 && minute != 30))
    {
        cout << "Invalid value for minute entered. Only 0 and 30 minutes are supported.\n";
        cin.clear();
        cin.ignore(10000, '\n');
        return -1;
    }

    // Create new session and add to the list of sessions
    session = {code, day, hour, minute, true, false};
    sessions.push_back(session);
    cout << "\nClass added successfully!\n";

    return 0;
}


// Delete a session from the list of sessions structs
int deleteSession()
{
    printSessions();
    cout << "Select a class to delete from the list by specifying its number on the list: ";
    int selectDel;
    cin >> selectDel;
    if (cin.fail() || selectDel < 1 || selectDel > sessions.size()) // Check if int input and if input within bounds
    {
        cout << "Invalid selection entered.\n";
        cin.clear();
        cin.ignore(10000, '\n');
        return -1;
    }
    cout << "\nClass deleted successfully!\n";

    sessions[selectDel - 1].added = false;
    sessions[selectDel - 1].deleted = true;
    return 0;
}


// Edit a session. In reality, simply deletes and makes a new one
int editSession()
{
    if (deleteSession() == -1) return -1;
    if (addSession() == -1) return -1;
    return 0;
}


// Print all sessions onto terminal
int printSessions()
{
    cout << "Here are the list of classes you have saved, a + means that class is added but unsaved and a '-' means that class is deleted but unsaved: \n";
    for (int i = 0; i < sessions.size(); i++)
    {
        if (sessions[i].added) cout << '+';         // Append '+' if unsaved added
        else if (sessions[i].deleted) cout << '-';  // Append '-' if unsaved deleted
        else cout << ' ';                           // Whitespace if neither for prettier formatting
        cout << i + 1 << ") " <<  sessions[i].code << ", " << sessions[i].day << ", " << sessions[i].hour << ':';
        if (sessions[i].minute == 0) cout << "00";  // Print 0 minute as "00" instead of "0"
        else cout << sessions[i].minute;            // "30" stays the same
        if (!sessions[i].deleted && checkForConflict(i)) cout << " (CONFLICT!)";   // Print CONFLICT at end if conflict detected
        cout << '\n';
    }

    return 0;
}


// Check if day of the week entered by user is valid
bool validDay(string day)
{
    int numOfValidDays = *(&validDays + 1) - validDays;

    for (int i = 0; i < numOfValidDays; i++)
    {
        if (day == validDays[i]) return true;
    }
    
    return false;
}


// Write and save all changes done locally to text file
int saveChanges()
{
    std::ofstream saveFile;
    saveFile.open("timetable-data.txt");

    for (int i = 0; i < sessions.size(); i++)   // Iterate through the list of sessions
    {
        subject &s = sessions[i];
        if (!s.deleted) // If session not deleted, write it to file
        {
            saveFile << s.code << '|' << s.day << '|' << s.hour << '|' << s.minute << '\n';
            sessions[i].added = false;  // Set added to false to indicate that change has been recorded to file
        }
    }

    saveFile.close();

    sessions.clear();
    loadFile(sessions);
    

    cout << "Changed saved successfully!\n";
    return 0;
}


// Print data in timetable format to the terminal
int printTimetable()
{
    vector<int> timeVec;                                            // Vector to store time slots for timetable
    for (int i = minHour; i < maxHour; i++) timeVec.push_back(i);   // Fill the timeVec with time slots
    int numOfCols = timeVec.size() * 2 + 1;                         // Number of columns the timetable will have

    drawHeader(colW, timeVec, numOfCols);
    drawSessionRows(numOfCols);

    return 0;
}


// Draw a cell of the timetable containing a data "|EXAMPLE  "
void drawCell(string filling)
{
    cout << v << std::left << setw(colW - 1) << setfill(' ') << filling;
}


// Draw top and bottom border for the timetable "-------"
void drawBorder(int numOfCols)
{
    for (int i = 0; i < numOfCols; i++)
    {
        cout << h << setfill(h) << setw(colW);
    }
    cout << h << '\n' << setfill(' ');
}


// Draw header for the timetable
void drawHeader(int colW, vector<int> &timeVec, int numOfCols)
{
    drawBorder(numOfCols);
    cout << v << setw(colW) << setfill(' ');
    // Iterate through number of columns based on the number of time slots
    for (int i = 0; i < timeVec.size(); i++)
    {
        string dotStr = std::to_string(timeVec[i]) + ":00";     // Create time string, e.g. "12:00"
        string halfStr = std::to_string(timeVec[i]) + ":30";    // Same as above except with half hours, e.g. "12:30"
        drawCell(dotStr);                                       // Draw whole hours e.g. 12:00
        drawCell(halfStr);                                      // Draw half hours e.g. 12:30
    }
    cout << v << '\n';
    drawBorder(numOfCols);
}


// Draw the rows containing the classes
void drawSessionRows(int numOfCols)
{
    for (int i = 0; i < *(&validDays + 1) - validDays; i++)
    {
        drawCell(validDays[i]); // Draw cell for day of the week for current row
        for (int j = 0; j < numOfCols - 1; j++)
        {
            drawCell(sessionFromCoords(validDays[i], j));   // Pass found session based on day of week and column number to draw the cell
        }
        cout << v << '\n';
        drawBorder(numOfCols);  // Draw border before the next row
    }
}


// Find whether or not there is a conflicting class and return the appropriate class code based on the row (day of week) and column provided by parent
string sessionFromCoords(string day, int col)
{
    int dupe = 0;   // Counts duplicates (1 means there is only one class taking up that timeslot)
    string foundSession;
    for (int i = 0; i < sessions.size(); i++)
    {
        /* 
        Check if:
        1. col is even
        2. the current day of the week being iterated matches the day of the week assigned to the session
        3. session's hour converted based on this graph (https://www.desmos.com/calculator/ugkfcq1pqn), where n is minHour matches the column index
        4. minute set to 0

        Flexible based on the minimum hour supported by the program
        */
        if (col % 2 == 0 && sessions[i].day == day && sessions[i].hour == (col / 2) + minHour && sessions[i].minute == 0 && !sessions[i].deleted)
        {
            foundSession = sessions[i].code;
            dupe++;
        }
        // Same as above but checks if odd and if minute is set to 30
        else if (col % 2 == 1 && sessions[i].day == day && sessions[i].hour == (col / 2) + minHour && sessions[i].minute == 30 && !sessions[i].deleted)
        {
            foundSession = sessions[i].code;
            dupe++;
        }
    }

    if (dupe == 0) return " ";          // Return whitespace if nothing found
    if (dupe == 1) return foundSession; // Return the thing if thing found
    if (dupe > 1) return "CONFLICT";    // Return conflict if more than one session is found in a timeslot
    return " ";
}


// Ask the user whether or not they want to save the file (called when user wants to close program)
int askToSave()
{
    char choice;
    for (int i = 0; i < sessions.size(); i++)   // Iterate through list of sessions
    {
        if (sessions[i].added || sessions[i].deleted)   // If either added or deleted is detected, ask to save changes
        {
            cout << "Unsaved changes detected: \n";
            printSessions();
            cout << "Would you like to save changes (y/n)?: ";
            
            cin >> choice;

            if (choice == 'y' || choice == 'Y')         // Save changes and return 1 to close program
            {
                saveChanges();
                return 1;
            }
            else if (choice == 'n' || choice == 'N')    // Return 1 without doing anything
            {
                return 1;
            }
            else                                        // Invalid option, return -1 and don't close loop
            {
                cout << "Invalid option entered!\n";
                cin.clear();
                cin.ignore(10000, '\n');
                return -1;
            }
        }
    }

    return 1;
}


// Check for conflict with the given session index
bool checkForConflict(int sessionIndex)
{
    subject sessionBeingChecked = sessions[sessionIndex];   // Current session being checked, pulled from the list

    for (int i = 0; i < sessions.size(); i++)   // Iterate through list of sessions
    {
        if (i != sessionIndex && sessionBeingChecked.day == sessions[i].day && sessionBeingChecked.hour == sessions[i].hour && sessionBeingChecked.minute == sessions[i].minute && !sessions[i].deleted)
        {
            return true;    // Skips the index being checked. If day, hour, and minute matches + not unsaved deleted return true
        }
    }

    return false;   // No dupe found; return false
}