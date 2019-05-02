// Implementation of the TreeDB class
#include "TreeDB.h"
#include <iostream>

// Constructor
TreeDB::TreeDB()
{
    root = NULL;
    probesCount = 0;
}

// Destructor
TreeDB::~TreeDB()
{
    clear(); // Remove all entries
}

// Insert a new entry into the database
// true - entry successful, false - entry exists
bool TreeDB::insert(DBentry* newEntry)
{
    if(root == NULL) { // Empty tree
        root = new TreeNode(newEntry);
        return true;
    }
    // Find the right place and do the insertion
    return traverseAndInsert(root,newEntry);
}

// Traverse the tree for the correct place to insert
bool TreeDB::traverseAndInsert(TreeNode* current, DBentry* newEntry)
{
    if(current->getEntry()->getName() == newEntry->getName()) { // Entry already exists
        delete newEntry;
        return false;
    }
    
    // Decide whether to travel down the left or right path
    if(newEntry->getName() > current->getEntry()->getName()) { //Go right
        // Insert if right child is NULL, if not keep going
        if(current->getRight() == NULL) {
            // Create a new right child node
            current->setRight(new TreeNode(newEntry));
            return true;
        } else return traverseAndInsert(current->getRight(),newEntry);
    } else { // Go left
        // Insert if left child is NULL, if not keep going
        if(current->getLeft() == NULL) {
            // Create a new left child node
            current->setLeft(new TreeNode(newEntry));
            return true;
        } else return traverseAndInsert(current->getLeft(),newEntry);
    }
}

// Searches database for an entry equal to name
// returns NULL if entry not found
DBentry* TreeDB::find(string name)
{
    TreeNode* found = traverseAndFind(root,name,1);
    return (found == NULL) ? NULL:found->getEntry();
}

// Traverse the tree to find the entry
// Also records number of probes required to find entry
TreeNode* TreeDB::traverseAndFind(TreeNode* current, string name, int probes)
{
    // If current node is NULL, that means the entry wasn't found
    // if current node's entry equals name, the entry is found
    // either way return current node's entry
    if(current == NULL || current->getEntry()->getName() == name) {
        probesCount = probes; // Set probesCount
        return current;
    }
    
    // Decide whether to travel down the left or right path
    if(name > current->getEntry()->getName()) { // Go right
        return traverseAndFind(current->getRight(),name,probes+1);
    } else { // Go left
        return traverseAndFind(current->getLeft(),name,probes+1);
    }
}

// Deletes entry from database
// true - deletion successful, false - entry not found
bool TreeDB::remove(string name)
{
    // Find the node to remove
    TreeNode* toRemove = traverseAndFind(root,name,1);
    
    // If node not found return false
    if(toRemove == NULL) return false;
    
    // If node is root node
    if(toRemove == root) {
        if(root->getLeft() != NULL && root->getRight() != NULL) { // Two children
            TreeNode* leftMax = maximum(root->getLeft());
            
            // If leftMax is root's left child
            if(root->getLeft() == leftMax) {
                // Replace root with leftMax
                leftMax->setRight(root->getRight());
                root = leftMax;
            } else { // leftMax has a parent that is not root
                TreeNode* maxParent = traverseAndFindParent(root,leftMax->getEntry()->getName());
                // leftMax will always be its parent's right child
                // leftMax will never have a right child, but it may have a left child
                if(leftMax->getLeft() != NULL) { // Has a left child
                    maxParent->setRight(leftMax->getLeft());
                } else { // Has no children
                    maxParent->setRight(NULL); 
                }
                // Replace root with leftMax
                leftMax->setLeft(root->getLeft());
                leftMax->setRight(root->getRight());
                root = leftMax;
            }
        } else if(root->getLeft() != NULL) { // Only has left child
            root = root->getLeft();
        } else if(root->getRight() != NULL) { // Only has right child
            root = root->getRight();
        } else { // No children
            root = NULL;
        }
    } else { // If node is not root node there is a parent
        TreeNode* parentNode = traverseAndFindParent(root,name);
        // Figure oput if toRemove is its parent's left or right child
        bool isLeftChild = (toRemove->getEntry()->getName() < parentNode->getEntry()->getName());
        
        if(toRemove->getLeft() != NULL && toRemove->getRight() != NULL) { // Two children
            TreeNode* leftMax = maximum(toRemove->getLeft());
            
            // If leftMax is toRemove's left child
            if(toRemove->getLeft() == leftMax) {
                // Replace corresponding child of the parent with leftMax
                leftMax->setRight(toRemove->getRight());
                if(isLeftChild) parentNode->setLeft(leftMax);
                else parentNode->setRight(leftMax);
            } else { // leftMax has a parent that is not toRemove
                TreeNode* maxParent = traverseAndFindParent(root,leftMax->getEntry()->getName());
                // leftMax will always be its parent's right child
                // leftMax will never have a right child, but it may have a left child
                if(leftMax->getLeft() != NULL) { // Has a left child
                    maxParent->setRight(leftMax->getLeft());
                } else { // Has no children
                    maxParent->setRight(NULL); 
                }
                // Replace corresponding child of the parent with leftMax
                leftMax->setLeft(toRemove->getLeft());
                leftMax->setRight(toRemove->getRight());
                if(isLeftChild) parentNode->setLeft(leftMax);
                else parentNode->setRight(leftMax);
            }
        } else if(toRemove->getLeft() != NULL) { // Only has left child
            // Replace corresponding child of the parent with toRemove's left child
            if(isLeftChild) parentNode->setLeft(toRemove->getLeft());
            else parentNode->setRight(toRemove->getLeft());
        } else if(toRemove->getRight() != NULL) { // Only has right child
            // Replace corresponding child of the parent with toRemove's right child
            if(isLeftChild) parentNode->setLeft(toRemove->getRight());
            else parentNode->setRight(toRemove->getRight());
        } else { // No children
            // Replace corresponding child of the parent with NULL
            if(isLeftChild) parentNode->setLeft(NULL);
            else parentNode->setRight(NULL);
        }
    }
    
    // Delete the node to be deleted
    delete toRemove;
    return true;
}

// Traverse the tree to find the parent node of an entry
// Unlike traverseAndFind() the node will always exist, also don't need to count probes
TreeNode* TreeDB::traverseAndFindParent(TreeNode* current, string name)
{
    // if one of current node's children's entry equals name, the node is found
    if((current->getLeft() != NULL && current->getLeft()->getEntry()->getName() == name) ||
       (current->getRight() != NULL && current->getRight()->getEntry()->getName() == name)) {
        return current;
    }
    
    // Decide whether to travel down the left or right path
    if(name > current->getEntry()->getName()) { // Go right
        return traverseAndFindParent(current->getRight(),name);
    } else { // Go left
        return traverseAndFindParent(current->getLeft(),name);
    }
}

// Returns the maximum of the subtree with root current
TreeNode* TreeDB::maximum(TreeNode* current)
{
    // Keep going right until there is no right child
    if(current->getRight() != NULL) return maximum(current->getRight());
    else return current;
}

// Deletes all entries in database
void TreeDB::clear()
{
    traverseAndDelete(root);
    root = NULL;
}

// Traverse the tree to delete all nodes
void TreeDB::traverseAndDelete(TreeNode* current)
{
    // Do nothing if current node is null
    if(current == NULL) return;
    else {
        // Delete right subtree, left subtree, then the current node
        traverseAndDelete(current->getRight());
        traverseAndDelete(current->getLeft());
        delete current; // Current will always be a leaf at this point
    }
}

// Prints number of probes stored in probesCount
void TreeDB::printProbes() const
{
    cout << probesCount << endl;
}

// Prints total number of active entries in database
void TreeDB::countActive() const
{
    cout << traverseAndCountActive(root) << endl;
}

// Traverse the tree to count all active nodes
int TreeDB::traverseAndCountActive(TreeNode* current) const
{
    // Stop traversal if current node is null
    if(current == NULL) return 0;
    // Count the current node if active and go through the right and left subtrees
    return (current->getEntry()->getActive() ? 1:0) + 
            traverseAndCountActive(current->getRight()) +
            traverseAndCountActive(current->getRight());
}

// Prints all the nodes in the entire tree
ostream& operator<< (ostream& out, const TreeDB& rhs)
{
    if(rhs.root != NULL) out << rhs.root;
    return out;
}

// Complementary to the override for TreeDB in printing all nodes
ostream& operator<< (ostream& out, TreeNode* rhs)
{
    // To print in ascending order use in-order traversal
    if(rhs->getLeft() != NULL) out << rhs->getLeft();
    out << *(rhs->getEntry());
    if(rhs->getRight() != NULL) out << rhs->getRight();
    return out;
}