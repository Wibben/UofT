// Implementation of the DBentry class
#include "DBentry.h"
#include <fstream>

// Constructors
DBentry::DBentry()
{
    name = "";
    IPaddress = 0;
    active = false;
}

DBentry::DBentry(string _name, unsigned int _IPaddress, bool _active)
{
    name = _name;
    IPaddress = _IPaddress;
    active = _active;
}

// Destructor
DBentry::~DBentry()
{
    // No action required as none of the data members are pointers
}

// Setters
void DBentry::setName(string _name)                 { name = _name; }
void DBentry::setIPaddress(unsigned int _IPaddress) { IPaddress = _IPaddress; }
void DBentry::setActive(bool _active)               { active = _active; }

// Getters
string DBentry::getName() const                     { return name; }
unsigned int DBentry::getIPaddress() const          { return IPaddress; }
bool DBentry::getActive() const                     { return active; }

// Prints the entry
// name : IPaddress : active  followed by newline
// if entry is NULL print error
ostream& operator<< (ostream& out, const DBentry& rhs)
{
    out << rhs.name << " : " << rhs.IPaddress << " : " << (rhs.active ? "active":"inactive") << endl;
    return out;
}
