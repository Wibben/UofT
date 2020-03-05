#pragma once

#include <vector>
#include <unordered_map>

/* TrieNodes are used to build tries, a search tree where strings can be stored
 * and words or prefixes of words can be found efficiently.
 * This class allows for insertion of strings, specifically street names along
 * with their street ids. Then a prefix of a street name can be given and street
 * ids of corresponding street names are returned.
 */
class TrieNode{
public:
    bool is_root;                       //Is this the root node
    bool is_end_of_word;                //Is this the last letter of a word
    
    //All street ids that contain the sequence of letters up to this node as a
    //prefix of the street name
    std::vector<unsigned> street_ids;   
    
    //All child nodes with the next letter of the word being stored as the key 
    //and another TrieNode as the value
    std::unordered_map<char,TrieNode*> Trie_children;   
    
    //Constructors
    TrieNode();
    TrieNode(bool, bool);
    //Inserts a street name and associated street id into the trie
    void insert(std::string, unsigned);
    //Returns street ids whose names contain the input prefix string
    std::vector<unsigned> find_prefix_in_street_names(std::string);
    //Deallocates memory used in trie
    void clear_TrieNode();
    
};