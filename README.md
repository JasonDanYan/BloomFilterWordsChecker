How to run the program

Put dic.txt and input.txt in the project folder;
Compile Project and run in VS c++ enviroment

What has been tested

Input Correctness: punctuation, English Character, Lowercase and uppercase;
Output test: print function return value;
Extreme Condition: long string(>256 char), large input file( > 60m);
Logic Check: change the size of bloom filter;
Boundary Check:  long words (array), long sentences (array), large size of dictionary(hash map);
Computing: cpu (most for load files and check_filter), low RAM usage (not a large dictionary) .
