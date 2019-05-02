// Implementation of the TreeNode class
#include "TreeNode.h"

// Constructor
TreeNode::TreeNode(DBentry* _entryPtr)
{
    entryPtr = _entryPtr;
    left = NULL;
    right = NULL;
}

// Destructor
TreeNode::~TreeNode()
{
    delete entryPtr;
    left = NULL;
    right = NULL;
}

// Setters
void TreeNode::setLeft(TreeNode* newLeft)   { left = newLeft; }
void TreeNode::setRight(TreeNode* newRight) { right = newRight; }

// Getters
TreeNode* TreeNode::getLeft() const         { return left; }
TreeNode* TreeNode::getRight() const        { return right; }
DBentry* TreeNode::getEntry() const         { return entryPtr; }
