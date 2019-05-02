// Interface which prompts the user to input commands to operate TreeDB
// Assumed that all inputs are correct and no error checking required
#include <iostream>
#include <sstream>
#include "TreeDB.h"

using namespace std;

// Helper function declarations
void insertEntry(stringstream& line, TreeDB& database);
void findEntry(stringstream& line, TreeDB& database);
void removeEntry(stringstream& line, TreeDB& database);
void printAll(TreeDB& database);
void printProbes(stringstream& line, TreeDB& database);
void removeAll(TreeDB& database);
void countActive(TreeDB& database);
void updateStatus(stringstream& line, TreeDB& database);

int main() 
{
    // Database
    TreeDB database;
    // User inputs
    string line,command;
    
    // User prompt
    cout << "> ";
    getline(cin,line);
    
    while(!cin.eof()) {
        // Convert to stringstream for parsing
        stringstream lineStream(line);
        lineStream >> command;
        
        if(command=="insert") insertEntry(lineStream,database);
        else if(command=="find") findEntry(lineStream,database);
        else if(command=="remove") removeEntry(lineStream,database);
        else if(command=="printall") printAll(database);
        else if(command=="printprobes") printProbes(lineStream,database);
        else if(command=="removeall") removeAll(database);
        else if(command=="countactive") countActive(database);
        else if(command=="updatestatus") updateStatus(lineStream,database);
            
        // Clears and prompts again
        lineStream.clear();
        cout << "> ";
        getline(cin,line);
    }
    
    return 0;
}

// Insert command
// insert name IPaddress status
void insertEntry(stringstream& line, TreeDB& database)
{
    // Get inputs
    string name,status;
    unsigned int IPaddress;
    line >> name >> IPaddress >> status;
    
    // Do the insertion and output the correct message
    DBentry* toInsert = new DBentry(name,IPaddress,status=="active");
    
    if(database.insert(toInsert)) cout << "Success" << endl;
    else cout << "Error: entry already exists" << endl;
}

// Find command
// find name
void findEntry(stringstream& line, TreeDB& database)
{
    // Get inputs
    string name;
    line >> name;
    
    // Do the find and outputs the correct message
    DBentry* found = database.find(name);
    
    if(found == NULL) cout << "Error: entry does not exist" << endl;
    else cout << *found;
}

// Remove Entry command
// removeEntry name
void removeEntry(stringstream& line, TreeDB& database)
{
    // Get inputs
    string name;
    line >> name;
    
    // Do the removal and output the correct message
    if(database.remove(name)) cout << "Success" << endl;
    else cout << "Error: entry does not exist" << endl;
}

// Print All command
// printall
void printAll(TreeDB& database)
{
    // Print the database
    cout << database;
}

// Print Probes command
// printprobes name
void printProbes(stringstream& line, TreeDB& database)
{
    // Get inputs
    string name;
    line >> name;
    
    // Do the find and output the correct message
    DBentry* found = database.find(name);
    
    if(found == NULL) cout << "Error: entry does not exist" << endl;
    else database.printProbes();
}

// Remove All command
// removeall
void removeAll(TreeDB& database)
{
    // Clears database and output message
    database.clear();
    cout << "Success" << endl;
}

// Count Active command
// countactive
void countActive(TreeDB& database)
{
    // Counts active nodes in database
    database.countActive();
}

// Update Status command
// updatestatus name status
void updateStatus(stringstream& line, TreeDB& database)
{
    // Get inputs
    string name,status;
    line >> name >> status;
    
    // Find the entry and update its status if it exists, then output correct message
    DBentry* found = database.find(name);
    
    if(found == NULL) cout << "Error: entry does not exist" << endl;
    else {
        found->setActive(status=="active");
        cout << "Success" << endl;
    }
}