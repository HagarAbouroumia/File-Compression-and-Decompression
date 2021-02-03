
#include <stdlib.h>
#include <string.h>
#include <list>
#include <iostream>
#include <fstream>

using namespace std;
void write_char_to_file(list<unsigned char> bytes, string filename);

list<unsigned char> read_char_from_file(string filename)
{
    list<unsigned char> output_list;
    ifstream file;
    file.open(filename);
    if (!file.is_open())
    {
        cout << "File does not exist" << endl;
        exit(0);
    }
    char c;
    while (file.get(c))
    {
        unsigned char cc = c;
        output_list.push_back(cc);
    }
    file.close();
    return output_list;
}

void write_char_to_file(list<unsigned char> bytes, string filename)
{
    ofstream myfile;
    myfile.open(filename);
    if (!myfile.is_open())
    {
        cout << "File does not exist" << endl;
        exit(0);
    }
    unsigned char byte;
    while (!bytes.empty())
    {
        byte = bytes.front();
        bytes.pop_front();
        myfile << byte;
    }
    myfile.close();
}