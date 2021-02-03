#include <bits/stdc++.h>
#include <queue>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <tgmath.h>
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <vector>

using namespace std;

class Node
{
public:
    int data = 0;
    Node *left;
    Node *right;
    unsigned char letter{};
};

struct CompareValue
{
    bool operator()(Node *p1, Node *p2)
    {
        return (p1->data > p2->data);
    }
};

unordered_map<unsigned char, int> get_frequencies_of_chars(list<unsigned char> message);
priority_queue<Node *, vector<Node *>, CompareValue> create_leaf_nodes(unordered_map<unsigned char, int> message_frequencies);
Node *get_root_of_huffman_tree(priority_queue<Node *, vector<Node *>, CompareValue> huffman_tree);
unordered_map<unsigned char, list<unsigned char>>
save_new_code(unsigned char letter, unordered_map<unsigned char, list<unsigned char>> new_codes,
              list<unsigned char> single_code);
unordered_map<unsigned char, list<unsigned char>> get_new_codes_from_huffman_tree(Node *root, unordered_map<unsigned char, list<unsigned char>> new_codes, list<unsigned char> single_code);

list<unsigned char> convert_asci_characters_to_bits(unsigned char character, list<unsigned char> huffman_tree_codes);
list<unsigned char> get_encoded_huffman_tree_bits(Node *root, list<unsigned char> huffman_tree_codes);
list<unsigned char> encode_message_bites(list<unsigned char> message, unordered_map<unsigned char, list<unsigned char>> new_codes, list<unsigned char> combined_bites);
list<unsigned char> add_one_padding_to_combined_bites(list<unsigned char> combined_bites);
list<unsigned char> remove_paddings(list<unsigned char> temp);
list<unsigned char> get_leaf_value(list<unsigned char> asci);
Node *rebuild_huffman_tree(list<unsigned char> *temp);
list<unsigned char> get_decoded_message(list<unsigned char> message_decoded_bits, Node *root);
unsigned char decode_compressed_message(Node *root, list<unsigned char> *message_decoded_bits);

unordered_map<unsigned char, int> get_frequencies_of_chars(list<unsigned char> message)
{
    unordered_map<unsigned char, int> message_frequencies;
    list<unsigned char> temp_message = message;

    while (!temp_message.empty())
    {
        unsigned char character = temp_message.back();
        temp_message.pop_back();
        auto it = message_frequencies.find(character);
        if (it != message_frequencies.end())
            it->second = it->second + 1;
        else
            message_frequencies[character] = 1;
    }
    return message_frequencies;
}

priority_queue<Node *, vector<Node *>, CompareValue> create_leaf_nodes(unordered_map<unsigned char, int> message_frequencies)
{
    priority_queue<Node *, vector<Node *>, CompareValue> huffman_tree;
    unordered_map<unsigned char, int>::iterator itr;
    for (itr = message_frequencies.begin(); itr != message_frequencies.end(); itr++)
    {
        Node *temp = new Node();
        temp->left = nullptr;
        temp->right = nullptr;
        temp->letter = itr->first;
        temp->data = itr->second;
        huffman_tree.push(temp);
    }

    return huffman_tree;
}

Node *get_root_of_huffman_tree(priority_queue<Node *, vector<Node *>, CompareValue> huffman_tree)
{

    while (huffman_tree.size() > 1)
    {
        Node *left = huffman_tree.top();
        huffman_tree.pop();
        Node *right = huffman_tree.top();
        huffman_tree.pop();
        Node *sum = new Node();
        sum->data = right->data + left->data;
        sum->left = left;
        sum->right = right;
        huffman_tree.push(sum);
    }
    Node *root = huffman_tree.top();
    huffman_tree.pop();
    return root;
}

unordered_map<unsigned char, list<unsigned char>>
save_new_code(unsigned char letter, unordered_map<unsigned char, list<unsigned char>> new_codes,
              list<unsigned char> single_code)
{

    list<unsigned char> temp_single_code_to_save;
    list<unsigned char> temp_single_code = single_code;

    while (!temp_single_code.empty())
    {
        temp_single_code_to_save.push_back(temp_single_code.front());
        temp_single_code.pop_front();
    }
    new_codes[letter] = temp_single_code_to_save;

    return new_codes;
}

unordered_map<unsigned char, list<unsigned char>> get_new_codes_from_huffman_tree(Node *root, unordered_map<unsigned char, list<unsigned char>> new_codes, list<unsigned char> single_code)
{
    unsigned char zero = 0;
    unsigned char one = 1;

    if (root->left != nullptr)
    {
        single_code.push_back(zero);
        new_codes = get_new_codes_from_huffman_tree(root->left, new_codes, single_code);
        single_code.pop_back();
    }

    if (root->right != nullptr)
    {
        single_code.push_back(one);
        new_codes = get_new_codes_from_huffman_tree(root->right, new_codes, single_code);
        single_code.pop_back();
    }

    if (root->left == nullptr && root->right == nullptr)
    {
        new_codes = save_new_code(root->letter, new_codes, single_code); // save the new code for each character
    }
    return new_codes;
}

list<unsigned char> convert_asci_characters_to_bits(unsigned char character, list<unsigned char> huffman_tree_codes)
{
    unsigned char byte = 128;
    int no_of_bits = 0;
    while (no_of_bits < 8)
    {
        if ((byte & character) == 0)
            huffman_tree_codes.push_back(0);
        else
            huffman_tree_codes.push_back(1);
        character <<= 1;
        no_of_bits++;
    }
    return huffman_tree_codes;
}

list<unsigned char> get_encoded_huffman_tree_bits(Node *root, list<unsigned char> huffman_tree_codes)
{

    unsigned char zero = 0;
    unsigned char one = 1;

    if (root->left == nullptr && root->right == nullptr)
    {
        huffman_tree_codes.push_back(one);
        huffman_tree_codes = convert_asci_characters_to_bits(root->letter, huffman_tree_codes);
    }
    else
    {
        huffman_tree_codes.push_back(zero);
        huffman_tree_codes = get_encoded_huffman_tree_bits(root->left, huffman_tree_codes);
        huffman_tree_codes = get_encoded_huffman_tree_bits(root->right, huffman_tree_codes);
    }
    return huffman_tree_codes;
}

list<unsigned char>
encode_message_bites(list<unsigned char> message, unordered_map<unsigned char, list<unsigned char>> new_codes,
                     list<unsigned char> combined_bites)
{
    list<unsigned char> temp_message = message;
    unsigned char bit;
    list<unsigned char> encryption_code;
    while (!temp_message.empty())
    {
        unsigned char character = temp_message.front();
        temp_message.pop_front();
        encryption_code = new_codes[character];
        while (!encryption_code.empty())
        {
            bit = encryption_code.front();
            combined_bites.push_back(bit);
            encryption_code.pop_front();
        }
    }

    return combined_bites;
}

list<unsigned char> add_one_padding_to_combined_bites(list<unsigned char> combined_bites)
{
    unsigned char one = 1;

    while ((combined_bites.size() % 8) != 0)
    {
        combined_bites.push_front(one);
    }
    return combined_bites;
}

list<unsigned char> remove_paddings(list<unsigned char> temp)
{
    while (temp.front() == 1)
        temp.pop_front();
    return temp;
}

list<unsigned char> get_leaf_value(list<unsigned char> asci)
{
    int no_of_bits = 0;
    unsigned char asci_output = 0;
    unsigned char asci_bit;
    unsigned char comparator = 128;

    while (no_of_bits < 8)
    {
        asci_bit = asci.front();
        asci.pop_front();

        if (asci_bit == 1)
            asci_output |= comparator;

        comparator >>= 1;
        no_of_bits++;
    }
    asci.push_front(asci_output);

    return asci;
}

Node *rebuild_huffman_tree(list<unsigned char> *temp)
{
    unsigned char character;
    if ((*temp).front() == 1)
    {
        (*temp).pop_front();
        Node *leaf = new Node();
        leaf->left = nullptr;
        leaf->right = nullptr;
        (*temp) = get_leaf_value(*temp);
        character = (*temp).front();
        (*temp).pop_front();
        leaf->letter = character;
        return leaf;
    }
    else
    {
        (*temp).pop_front();
        Node *leftChild = rebuild_huffman_tree(temp);
        Node *rightChild = rebuild_huffman_tree(temp);
        Node *parent = new Node();
        parent->left = leftChild;
        parent->right = rightChild;
        return parent;
    }
}

list<unsigned char> get_decoded_message(list<unsigned char> message_decoded_bits, Node *root)
{
    Node *temp = root;
    list<unsigned char> output_message;
    unsigned char character;
    while (!message_decoded_bits.empty())
    {

        character = decode_compressed_message(temp, &message_decoded_bits);
        output_message.push_back(character);
    }
    return output_message;
}

unsigned char decode_compressed_message(Node *root, list<unsigned char> *message_decoded_bits)
{

    if (root->right == nullptr && root->left == nullptr)
    {
        //cout << root->letter << endl;
        return root->letter;
    }
    int bit = (*message_decoded_bits).front();
    (*message_decoded_bits).pop_front();
    if (bit == 0)
        decode_compressed_message(root->left, message_decoded_bits);
    else
        decode_compressed_message(root->right, message_decoded_bits);
}
