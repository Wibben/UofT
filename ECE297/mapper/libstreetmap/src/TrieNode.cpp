#include "TrieNode.h"

//Contructors
TrieNode::TrieNode() {
    is_root = false;
    is_end_of_word = false;
}

TrieNode::TrieNode(bool root, bool end) {
    is_root = root;
    is_end_of_word = end;
}

//Inserts a street name and associated street id into the trie
void TrieNode::insert(std::string street_name, unsigned street_id) {
    TrieNode* curr = this;

    //Loop through characters of street name input
    for (unsigned i = 0; i < street_name.size(); i++) {
        //If none of the keys of the TrieNode's children include the current
        //character then make a new TrieNode with the current character as its key
        if (curr->Trie_children.find(street_name[i]) == curr->Trie_children.end()) {
            TrieNode* TrieNode_child = new TrieNode;
            curr->Trie_children[street_name[i]] = TrieNode_child;
        }
        
        //Make the current TrieNode the child node with the current character as
        //its key
        curr = curr->Trie_children[street_name[i]];
        //The sequence of characters up to this node are a prefix of this street
        //name so add the street id to the vector of street ids for this node
        curr->street_ids.push_back(street_id);

    }

    curr->is_end_of_word = true;    //This node has the last letter of the word

}

//Returns street ids whose names contain the input prefix string
std::vector<unsigned> TrieNode::find_prefix_in_street_names(std::string prefix) {
    TrieNode* curr = this;
    std::vector<unsigned> no_matching_street_ids;
    
    //Loop through characters of the input prefix
    for (unsigned i = 0; i < prefix.size(); i++) {
        //If none of the child nodes have the current character as a key, then
        //return an empty vector because this prefix is not in any stored string
        if (curr->Trie_children.find(prefix[i]) == curr->Trie_children.end()) {
            return no_matching_street_ids;
        } else {
            //Make the current TrieNode the child node with the character as its key
            curr = curr->Trie_children[prefix[i]];
        }
    }
    
    return curr->street_ids;
}

//Deallocates memory used in trie
void TrieNode::clear_TrieNode() {
    
    //If trie is empty, then there is nothing to clear
    if (this->is_root && this->Trie_children.empty()) {
        return;
    }
    else {
        //Loop through all child nodes
        for (auto it : this->Trie_children) {
            //If the child node has no children, it can be safely deleted
            if (it.second->Trie_children.empty()) {
                delete it.second;
            }
            //Call this function on the child node so its children can be deleted
            else {
                it.second->clear_TrieNode();
            }
        }
        this->Trie_children.clear();    //Clear unordered map
        if (!(this->is_root)) {
            //After finally deleting all children, this TrieNode can be deleted
            delete this;
        }
    }
}