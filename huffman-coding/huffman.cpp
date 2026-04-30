#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

struct Node {
    string ch;
    int freq;
    Node* left;
    Node* right;

    Node(string c, int f) {
        ch = c;
        freq = f;
        left = nullptr;
        right = nullptr;
    }

    Node(Node* l, Node* r) {
        left = l; right = r;
        freq = l->freq + r->freq;
        ch = "";
    }
};

struct NodeCompare {
    bool operator()(Node* a, Node* b) {
        if (a->freq > b->freq)
            return true;
        else
            return false;
    }
};

void getCodes(Node* root, string s, unordered_map<char, string>& codes) {
    if(root == nullptr)
        return;
    if(root->left == nullptr && root->right == nullptr) {
        codes[root->ch[0]] = s;
        return;
    }
    getCodes(root->left, s + "0", codes);
    getCodes(root->right, s + "1", codes);
}


int main() {

    string txt = "This is an example text for Huffman coding. Huffman coding is a compression algorithm that uses variable-length codes for encoding characters, based on their frequencies. The most common characters have shorter codes, while less common characters have longer codes. This method helps in reducing the overall size of the text.";

    unordered_map<char, int> freq;
    for (int i = 0; i < txt.size(); i++) {
        freq[txt[i]]++;
    }

    priority_queue<Node*, vector<Node*>, NodeCompare> pq;
    for (auto it = freq.begin(); it != freq.end(); it++) {
        pq.push(new Node(string(1, it->first), it->second));
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        Node* parent = new Node(left, right);
        pq.push(parent);
    }

    Node* root = pq.top();

    unordered_map<char, string> huffCodes;
    getCodes(root, "", huffCodes);

    cout << "Huffman Codes: " << endl;
    for (auto it = huffCodes.begin(); it != huffCodes.end(); it++) {
        cout << it->first << " -> " << it->second << endl;
    }

    string encoded = "";
    for (int i = 0; i < txt.size(); i++) {
        encoded += huffCodes[txt[i]];
    }
    cout << endl;
    cout << "Encoded Text: " << endl;
    cout << encoded << endl;

    return 0;
}
