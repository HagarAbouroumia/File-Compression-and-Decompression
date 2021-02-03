#include "Huffman.h"
#include "print_outputs.h"
#include "file.h"
using namespace std::chrono;
void decompress(string filename);
void compress(string filename)
{
    list<unsigned char> message;
    unordered_map<unsigned char, int> message_frequencies;
    priority_queue<Node *, vector<Node *>, CompareValue> huffman_tree;
    Node *root;
    unordered_map<unsigned char, list<unsigned char>> new_codes;
    list<unsigned char> unwanted;
    list<unsigned char> combined_bites;
    list<unsigned char> compressed_file_bytes;
    auto start = high_resolution_clock::now();
    message = read_char_from_file(filename);
    message_frequencies = get_frequencies_of_chars(message);
    huffman_tree = create_leaf_nodes(message_frequencies);
    root = get_root_of_huffman_tree(huffman_tree);
    new_codes = get_new_codes_from_huffman_tree(root, new_codes, unwanted);
    combined_bites = get_encoded_huffman_tree_bits(root, combined_bites);
    combined_bites = encode_message_bites(message, new_codes, combined_bites);
    combined_bites = add_one_padding_to_combined_bites(combined_bites);
    compressed_file_bytes = convert_bits_to_bytes(combined_bites);
    write_char_to_file(compressed_file_bytes, filename);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "___________________________" << endl;
    cout << "Compression Ratio: " << (float)compressed_file_bytes.size() / (float)message.size() << endl;
    cout << "___________________________\n\n";
    print_codes_table(new_codes);
    cout << "\nExecution Time for Compressing the file is " << duration.count() / 1000 << " seconds\n";
}

void decompress(string filename)
{
    list<unsigned char> compressed_file_bytes;
    list<unsigned char> compressed_file_bits;
    Node *root;
    list<unsigned char> output_message;
    auto start = high_resolution_clock::now();
    compressed_file_bytes = read_char_from_file(filename);
    compressed_file_bits = convert_bytes_to_bites(compressed_file_bytes);
    compressed_file_bits = remove_paddings(compressed_file_bits);
    root = rebuild_huffman_tree(&compressed_file_bits);
    output_message = get_decoded_message(compressed_file_bits, root);
    write_char_to_file(output_message, filename);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "\nExecution Time for Decompressing the file is " << duration.count() / 1000 << " seconds\n";
}

int main()
{
    string filename;
    int choice;

    cout << "Enter file name\n";
    cin >> filename;
    cout << "Choose 1 to Compress the file\nChoose 2 to Decompress the file\n";
    cin >> choice;
    if (choice == 1)
        compress(filename);
    else if (choice == 2)
        decompress(filename);
}
