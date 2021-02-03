

#include <bits/stdc++.h>
#include <iostream>
#include <unordered_map>
#include "bits.h"

using namespace std;
void print_ascii_characters_in_bits(unsigned char character);
void print_codes_table(unordered_map<unsigned char, list<unsigned char>> huffman_codes);


void print_list(list<unsigned char> print)
{
    while (!print.empty())
    {
        cout << (int)print.front();
        print.pop_front();
    }
}

void print_ascii_characters_in_bits(unsigned char character)
{
    unsigned char byte = 128;
    int no_of_bits = 0;
    while (no_of_bits < 8)
    {
        if ((byte & character) == 0)
            cout << 0;
        else
            cout << 1;
        character <<= 1u;
        no_of_bits++;
    }
}

void print_codes_table(unordered_map<unsigned char, list<unsigned char>> huffman_codes)
{
    list<unsigned char> temp;
    cout << "Byte\tCode\t\tNewCode" << endl;
    cout << "----\t----\t\t-------" << endl;
    for (auto itr = huffman_codes.begin(); itr != huffman_codes.end(); itr++)
    {
        temp = itr->second;
        cout << (int)itr->first << "\t";
        print_ascii_characters_in_bits(itr->first);
        cout << "\t";
        print_list(temp);
        cout << endl;
    }
}
