//
//  main.cpp skeleton
//  lab3
//
//  Created by Tarek Abdelrahman on 2018-08-25.
//  Copyright © 2018 Tarek Abdelrahman. All rights reserved.
//

#define NOT_LAST_ARG true
#define LAST_ARG false

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#include "globals.h"
//#include "shape.h"

// This is the shape array, to be dynamically allocated
shape** shapesArray;

// The number of shapes in the database, to be incremented 
// every time a shape is successfully created
int shapeCount = 0;

// The value of the argument to the maxShapes command
int max_shapes;

void setMaxShape(stringstream& line);
void createShape(stringstream& line);
void moveShape(stringstream& line);
void rotateShape(stringstream& line);
void drawShape(stringstream& line);
void deleteShape(stringstream& line);

bool parseArg(stringstream& line, int& val, bool notLastArg);
bool parseArgWithMax(stringstream& line, int& val, bool notLastArg, int maxVal);

bool numOfArgError(stringstream& line, bool notLastArg);
bool validShapeNameError(string name);
bool shapeNameExistsError(string name);
bool validShapeTypeError(string type);

int findShape(string name);
bool isStringInArray(string val, string* array, int length);

int main() {

    string line;
    string command;
    
    cout << "> ";         // Prompt for input
    getline(cin, line);   // Get a line from standard input

    while (!cin.eof()) {
        // Put the line in a linestream for parsing
        // Making a new sstream for each line so the flags are cleared
        stringstream lineStream (line);

        // Read from string stream into the command
        // The only way this can fail is if the eof is encountered
        lineStream >> command;
        lineStream >> std::ws; // Remove leading whitespaces

        // Check for the command and act accordingly
        if(command!="all" && isStringInArray(command,keyWordsList,NUM_KEYWORDS)) {
            if(lineStream.peek() == -1) { // Catch the case where the user just inputs the command
                cout << "Error: too few arguments" << endl;
            } else { // Runs appropriate command
                if(command == "maxShapes") setMaxShape(lineStream);
                else if(command == "create") createShape(lineStream);
                else if(command == "move") moveShape(lineStream);
                else if(command == "rotate") rotateShape(lineStream);
                else if(command == "draw") drawShape(lineStream);
                else if(command == "delete") deleteShape(lineStream);
            }
        } else cout << "Error: invalid command" << endl;

        // Once the command has been processed, prompt for the
        // next command
        cout << "> ";          // Prompt for input
        getline(cin, line);   // Get the command line
        
    }  // End input loop until EOF.
    
    return 0;
}

/**
 * --------------------Functions for executing the commands--------------------
 * The only time when stringstream will fail for strings is at eof,
 * therefore name and type will never yield invalid argument 
 * (it will be caught by a number of arguments error in the previous arguments)
 */

// Set max_shapes and allocate memory
// maxShapes value, assumed to never error
void setMaxShape(stringstream& line)
{
    line >> max_shapes;
    
    // Allocate memory and set every entry as NULL
    shapesArray = (shape**)malloc(max_shapes*sizeof(shape*));
    for(int i=0; i<max_shapes; i++)
        shapesArray[i] = NULL;
    
    cout << "New database: max shapes is " << max_shapes << endl;
}

// Creates a shape and puts it into array
// create name type x_loc y_loc x_sz y_sz
void createShape(stringstream& line)
{
    string name,type,dummy;
    int x_loc,y_loc,x_sz,y_sz;
    
    // ----------Parsing inputs----------
    // Shape name
    line >> name >> std::ws;
    // Validate shape name, type, and if number of arguments is incorrect
    if(validShapeNameError(name) || shapeNameExistsError(name) || numOfArgError(line,NOT_LAST_ARG)) return;
    
    // Shape type
    line >> type >> std::ws;
    // Check that shape type is valid and if number of arguments is incorrect
    if(validShapeTypeError(type) || numOfArgError(line,NOT_LAST_ARG)) return;
    
    if(!parseArg(line,x_loc,NOT_LAST_ARG)) return; // x location
    if(!parseArg(line,y_loc,NOT_LAST_ARG)) return; // y location
    if(!parseArg(line,x_sz,NOT_LAST_ARG)) return; // x size
    if(!parseArg(line,y_sz,LAST_ARG)) return; // y size
    // ----------End Parsing inputs----------
    
    // Make sure shapeArray is not full
    if(shapeCount == max_shapes) {
        cout << "Error: shape array is full" << endl;
        return;
    }
    
    // Add to shape array and increment counter
    shapesArray[shapeCount++] = new shape(name,type,x_loc,y_loc,x_sz,y_sz);
    cout << "Created " << name << ": " << type << " " 
         << x_loc << " " << y_loc << " "
         << x_sz << " " << y_sz << endl;
}

// Moves a shape to a specified location
// move name x_loc y_loc
void moveShape(stringstream& line)
{
    string name;
    int x_loc,y_loc;
    int id; // Index of shape
    
    // ----------Parsing inputs----------
    // Shape name
    line >> name >> std::ws;
    if(validShapeNameError(name)) return; // Validate shape name
    id = findShape(name); // Find index of shape if valid
    if(id==-1 || numOfArgError(line,NOT_LAST_ARG)) return; // Also check if number of arguments is incorrect
    
    if(!parseArg(line,x_loc,NOT_LAST_ARG)) return; // x location
    if(!parseArg(line,y_loc,LAST_ARG)) return; // y location
    // ----------End Parsing inputs----------
    
    // Set the shape to the new location
    shapesArray[id]->setXlocation(x_loc);
    shapesArray[id]->setYlocation(y_loc);
    cout << "Moved " << name << " to " << x_loc << " " << y_loc << endl;
}

// Rotates shape by a specified angle (deg)
// rotate name angle
void rotateShape(stringstream& line)
{
    string name;
    int angle;
    int id; // Index of shape
    
    // ----------Parsing inputs----------
    // Shape name
    line >> name >> std::ws;
    if(validShapeNameError(name)) return; // Validate shape name
    id = findShape(name); // Find index of shape if valid
    if(id==-1 || numOfArgError(line,NOT_LAST_ARG)) return; // Also check if number of arguments is incorrect
    
    if(!parseArgWithMax(line,angle,LAST_ARG,360)) return; // Parse angle
    // ----------End Parsing inputs----------
    
    // Set the rotation to the new angle
    shapesArray[id]->setRotate(angle);
    cout << "Rotated " << name << " by " << angle << " degrees" << endl;
}

// Draws the shapes
// draw name, if name = all, draw all
void drawShape(stringstream& line)
{
    string name;
    
    // Shape name, only 1 input
    line >> name >> std::ws;
    
    if(name == "all") { // Draw all shapes
        if(numOfArgError(line,LAST_ARG)) return; // Check number of arguments
        
        cout << "Drew all shapes" << endl;
        
        // Draw everything
        for(int i=0; i<shapeCount; i++) {
            if(shapesArray[i] != NULL) {
                shapesArray[i]->draw();
            }
        }
    } else { // Draw a single shape
        int id; // Index of array
        
        // More validation
        if(validShapeNameError(name)) return; // Validate shape name
        id = findShape(name); // Find index of shape if valid
        if(id==-1 || numOfArgError(line,LAST_ARG)) return; // Also check if number of arguments is incorrect
         
        // Draw shape
        cout << "Drew " << name << endl;
        shapesArray[id]->draw();
    }
}

// Deletes the specified shape
// delete name, if name = all, delete all
void deleteShape(stringstream& line)
{
    string name;
    
    // Shape name, only 1 argument
    line >> name >> std::ws;
    
    if(name == "all") { // Delete all shapes
        if(numOfArgError(line,LAST_ARG)) return; // Check number of arguments
                
        cout << "Deleted: all shapes" << endl;
        
        // Delete everything
        for(int i=0; i<shapeCount; i++) {
            if(shapesArray[i] != NULL) {
                free(shapesArray[i]); // Free the allocated memory
                shapesArray[i] = NULL;
            }
        }
    } else { // Delete a single shape
        int id; // Index of array
        
        // More validation
        if(validShapeNameError(name)) return; // Validate shape name
        id = findShape(name); // Find index of shape if valid
        if(id==-1 || numOfArgError(line,LAST_ARG)) return; // Also check if number of arguments is incorrect
        
        // Delete shape and assign pointer to NULL
        cout << "Deleted shape " << name << endl;
        free(shapesArray[id]); // Free allocated memory
        shapesArray[id] = NULL;
    }
}

/**
 * --------------------Number parsing functions--------------------
 * Reads in a integer value and validates it (already established that
 * strings will always read in properly and will only error at eof)
 * String inputs are also unique enough that they are either only used
 * once (type) or can't really be put into functions (name)
 */

// Parses the argument to a int type
// true - successful, false - unsuccessful
bool parseArg(stringstream& line, int& val, bool notLastArg)
{
    line >> val;
    line >> std::ws; // Remove leading whitespaces for peek
    if(line.fail() || line.peek()=='.') { // Check if input failed(string) or if the next char is a .(floating point)
        cout << "Error: invalid argument" << endl;
        return false;
    } else if(val < 0) { // For most ints also must make sure the value is >= 0
        cout << "Error: invalid value" << endl;
        return false;
    } else if(numOfArgError(line,notLastArg)) { // Check if number of arguments is incorrect
        return false;
    }
    
    return true;
}

// Parsing int but also with a max limit, for rotation
// true - successful, false - unsuccessful
bool parseArgWithMax(stringstream& line, int& val, bool notLastArg, int maxVal)
{
    line >> val;
    line >> std::ws; // Remove leading whitespaces for peek
    if(line.fail()) { // Check if input failed
        cout << "Error: invalid argument" << endl;
        return false;
    } else if(val < 0 || val > maxVal) { // For most ints also must make sure the value is >= 0
        cout << "Error: invalid value" << endl;
        return false;
    } else if(numOfArgError(line,notLastArg)) { // Check if number of arguments is incorrect
        return false;
    }
    
    return true;
}

/**
 * --------------------Other helper functions--------------------
 * Mostly error checks
 */

// Checks if there is an error with the number of arguments
// true - there is an error, false - no error
bool numOfArgError(stringstream& line, bool notLastArg)
{
    if(notLastArg && line.peek() == -1) { // Check if there are more arguments
        cout << "Error: too few arguments" << endl;
        return true;
    } else if(!notLastArg && line.peek() != -1) { // Check if it's the end of the line
        cout << "Error: too many arguments" << endl;
        return true;
    }
    
    return false;
}


// Check if the shape name is a keyword/shape type
// true - there is an error, false - no error
bool validShapeNameError(string name)
{
    if(isStringInArray(name,keyWordsList,NUM_KEYWORDS) || isStringInArray(name,shapeTypesList,NUM_TYPES)) {
        cout << "Error: invalid shape name" << endl;
        return true;
    }
    return false;
}

// Check if there is already a shape with the same name
// true - there is an error, false - no error
bool shapeNameExistsError(string name)
{
    for(int i=0; i<shapeCount; i++) {
        if(shapesArray[i]!=NULL && name == shapesArray[i]->getName()) {
            cout << "Error: shape " << name << " exists" << endl;
            return true;
        }
    }
    return false;
}

// Check if the shape type is valid
// true - there is an error, false - no error
bool validShapeTypeError(string type)
{
    if(!isStringInArray(type,shapeTypesList,NUM_TYPES)) {
        cout << "Error: invalid shape type" << endl;
        return true;
    }
    return false;
}

// Finds the index of the shape in the array
int findShape(string name)
{
    // Go through array for shape name
    for(int i=0; i<shapeCount; i++) {
        if(shapesArray[i]!=NULL && name == shapesArray[i]->getName()) {
            return i;
        }
    }
    
    // Shape name DNE if it reaches here
    cout << "Error: shape " << name << " not found" << endl;
    return -1;
}

// Check if a string is a part of an array
bool isStringInArray(string val, string* array, int length)
{
    for(int i=0; i<length; i++) // Go through the array to see if string exists
        if(array[i]==val) return true;
    return false;
}
