//this is like queries in the DBs
//dic will not access consequently so it is good to store in the Hashmap
//in this task I only need to focus on the RAM. No extra Network, Disk consumptions.
//dic data is related static so I don't need to consider write-amplify but I need to amplify reading
//filter will reduce access frequency of the hashmap
//In this task, File size <20 M, so, I don't need to consider how to divide files to pieces then load into RAM also I didn't need combine the results. 
//but if dic file and input file is large enough. For example, 1TB. I need to consider how to orgnize file systems, index and partition and may need to consider the hashmap size (depends on the RAM size)
//another solution is using index tree because the longest words is 256 so the max depth of the tree is 256 

#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <math.h>
#include <unordered_map>
#include <algorithm>

using namespace std;

/*
* define bloom filter
*/
//bloom filter size
#define FILTER_SIZE 500
const int filter_size = FILTER_SIZE;
bool hash_tag[filter_size] = { false };

//hash map for dictionary
unordered_map<string, uint64_t> dic_map;

//hash function 1
int hashf1(string s, int filter_size)
{
    uint64_t hash = 0;
    for (int i = 0; i < s.size(); i++)
    {
        hash = (hash + ((int)s[i]));
        hash %= filter_size;
    }

    return hash;
}

// hash function 2 
int hashf2(string s, int filter_size)
{
    uint64_t hash = 1;
    for (int i = 0; i < s.size(); i++)
    {
        hash = hash + pow(19, i) * s[i];
        hash = hash % filter_size;
    }
    return hash % filter_size;
}

// hash function 3 
int hashf3(string s, int filter_size)
{
    uint64_t hash = 7;
    for (int i = 0; i < s.size(); i++)
    {
        hash = (hash * 31 + s[i]) % filter_size;
    }
    return hash % filter_size;
}

// hash function 4 
int hashf4(string s, int filter_size)
{
    uint64_t hash = 3;
    int p = 7;
    for (int i = 0; i < s.size(); i++) {
        hash += hash * 7 + s[0] * pow(p, i);
        hash = hash % filter_size;
    }
    return hash;
}


// check operation. return true when all four bit are true in the bloom filter 
/*
* Need a double-check when true
*/
bool check_Filter(bool* bitarray, int filter_size, string s)
{
    int a = hashf1(s, filter_size);
    int b = hashf2(s, filter_size);
    int c = hashf3(s, filter_size);
    int d = hashf4(s, filter_size);


    if (bitarray[a] && bitarray[b] && bitarray
        && bitarray[d])
        return true;
    else
        return false;
}

// insert operation 
void insert_Filter(bool* bitarray, int filter_size, string s)
{
    // check if the element in already present or not 
    if (check_Filter(bitarray, filter_size, s))
        ;
    else
    {
        //add to the hash map, set all 4 position to true
        int a = hashf1(s, filter_size);
        int b = hashf2(s, filter_size);
        int c = hashf3(s, filter_size);
        int d = hashf4(s, filter_size);

        bitarray[a] = true;
        bitarray[b] = true;
        bitarray[c] = true;
        bitarray[d] = true;
    }
}

/*
initial dictionary hashmap with dictionary files
initial bloom filter
*/

void initial_dic_filter_map(const string& dic_file) {
    ifstream rfile;
    rfile.open(dic_file);
    string line;
    while (std::getline(rfile, line)) {
        transform(line.begin(), line.end(), line.begin(), ::tolower);
        //set initial count value to 0 for the given key
        dic_map[line] = 0;
        insert_Filter(hash_tag, filter_size, line);
    }
}

//only left eng characters
void delete_punctuation(string &str)
{
    for (int i = 0; i < str.size(); i++)       
        if (str[i] < 65 || (str[i] > 90 && str[i] < 97) || str[i] > 122)
            str[i] = ' ';
}

//get matched count
void update_dic_map(const string in_file)
{
    ifstream rfile;
    rfile.open(in_file);
    string line;

    while (std::getline(rfile, line)) {
        delete_punctuation(line);
        stringstream iss(line);
        string word;
        while (iss >> word)
        {
            //lowercase
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            //when return true, need a double-check in the dictionary hashmap
            if (check_Filter(hash_tag, filter_size, word))
                if (dic_map.find(word) != dic_map.end())
                    dic_map[word]++;
        }
    }
}

void main()
{
    string input_file_name = "input.txt";
    string dic_filename = "dic.txt";
    //initial dictionary
    if (dic_filename != "")
        initial_dic_filter_map(dic_filename);

    //update count value in the hashmap
    if (input_file_name != "") 
       update_dic_map(input_file_name);

    
    auto print_key_value = [](const auto& key, const auto& value)
    {
            std::cout << key << "   " << value << "\n";
    };

    std::cout << "Predefined Word      " << "Match Count \n";
    //print results
    for (const std::pair<const std::string, int>& n : dic_map)
        print_key_value(n.first, n.second);
}

