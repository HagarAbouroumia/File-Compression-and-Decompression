
#include <list>
#include <iostream>
using namespace std;
void convert_ascii_characters_to_bits(unsigned char character);
list<unsigned char> convert_bits_to_bytes(list<unsigned char> bits);

list<unsigned char> convert_bytes_to_bites(list<unsigned char> bytes)
{
    list<unsigned char> output_bites;
    unsigned char byte;
    unsigned char comparator = 128;
    int no_of_bites = 0;
    while (!bytes.empty())
    {
        byte = bytes.front();
        bytes.pop_front();
        while (no_of_bites < 8)
        {
            if ((byte & comparator) == 0)
                output_bites.push_back(0);
            else
                output_bites.push_back(1);
            byte <<= 1;
            no_of_bites++;
        }
        no_of_bites = 0;
    }
    return output_bites;
}

void convert_ascii_characters_to_bits(unsigned char character) {
    unsigned char byte = 128;
    int no_of_bits = 0;
    while (no_of_bits < 8) {
        if ((byte & character) == 0)
            cout << 0;
        else
            cout << 1;
        character <<= 1u;
        no_of_bits++;

    }
}


list<unsigned char> convert_bits_to_bytes(list<unsigned char> bits)
{
    list<unsigned char> output_bytes;
    unsigned char byte = 0;
    int no_of_bits = 0;
    unsigned char bit;

    while (!bits.empty())
    {
        bit = bits.front();
        bits.pop_front();

        byte <<= 1u;
        byte |= bit;
        no_of_bits++;

        if (no_of_bits > 7)
        {
            output_bytes.push_back(byte);
            byte = 0;
            no_of_bits = 0;
        }
    }
    return output_bytes;
}
