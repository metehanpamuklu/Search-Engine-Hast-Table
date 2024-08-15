// Created by Metehan Pamuklu
#include <iostream>
#include<string>
#include "AvlSearchTree.h"
#include "HashTable.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
using namespace std;

struct DocumentItem {
    string documentName;
    int count;
};
struct WordItem {
    string word;
    vector<DocumentItem> documentList;
};

bool isAlphaphtic(const string str) {
    for (char c : str) {
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
            return false;
        }

    }
    return true;
}
string toLower(string& str) {
    string res = "";
    for (char c : str) {
        res += tolower(c);

    }
    return res;
}

bool checking_for_tree(vector<string>& query, AVLSearchTree<string, WordItem*>& avlTree) {
    for (const string& query : query)
    {
        if (avlTree.find(query) == "not here")
        {
            return false;
        }
    }
    return true;
}
bool checking_for_docs(vector<string>& queries, const string& doc_name, AVLSearchTree<string, WordItem*>& myTree) {
    for (const string& query : queries) {
        WordItem* wordItem = myTree.getValue(query);
        if (wordItem != nullptr) {
            bool found = false;
            for (const DocumentItem& doc : wordItem->documentList) {
                if (doc.documentName == doc_name) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
        else {
            return false;
        }
    }
    return true;
}
//same check functions for hash table
bool checking_for_hash_table(vector<string>& queries, HashTable<string, WordItem*>& hTable) {
    string notFound;
    for (const string& query : queries)
    {
        if (hTable.find(query).key == notFound)
        {
            return false;
        }
    }
    return true;
}
bool doc_has_queries(vector<string>& queries, HashTable<string, WordItem*>& hTable, string fName) {
    for (const string& query : queries) {
        WordItem* wordItem = hTable.find(query).value;
        if (wordItem != nullptr) {
            bool found = false;
            for (const DocumentItem& doc : wordItem->documentList) {
                if (doc.documentName == fName) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
        else {
            return false;
        }
    }
    return true;
}
//replacing non alphabetic chars with space in order split them
string replaceNonAlpha(const string& line) {
    string processedLine;
    for (char c : line) {
        if (isalpha(c)) {
            processedLine += c;
        }
        else {
            processedLine += ' '; // Use space as separator
        }
    }
    return processedLine;
}



int main() {
    string notFound;
    string file;
    int size = 53;
    HashTable<string, WordItem*> table(notFound, size);
    AVLSearchTree<string, WordItem*> avlTree;//creating a AvlsearchTree   
    int file_count;
    vector<string> file_names;//hold them for printing
    cout << "Enter number of input files: ";
    cin >> file_count;
    for (int i = 0; i < file_count; i++) {
        cout << "Enter " << i + 1 << ". " << "file name: ";
        cin >> file;
        file_names.push_back(file);
    }
    
    for (int i = 0; i < file_names.size(); i++) {
        string file_2 = file_names[i];
        ifstream file1(file_2);
        if (!file1.is_open()) {
            cout << "Failed to open" << file_2 << endl;
        }


        string words;//key for worditem
        DocumentItem doc_1;//struct of document vector in word item
        WordItem* word_1;
        string line;
        while (getline(file1, line)) {
            // Process the line
            string processedLine = replaceNonAlpha(line);

            // Extract words from the processed line
            istringstream iss(processedLine);
            string word;
            while (iss >> word) {
                // Convert word to lowercase for case-insensitive processing
                word = toLower(word);

                // Update data structures (AVL tree and hash table)

                // **AVL Tree:**
                if (avlTree.find(word) == "not here") {
                    // Word not found in AVL tree
                    WordItem* wordItem = new WordItem;
                    wordItem->word = word;
                    DocumentItem doc;
                    doc.count = 1;
                    doc.documentName = file_2;
                    wordItem->documentList.push_back(doc);
                    avlTree.insert(word, wordItem);
                }
                else {
                    // Word found in AVL tree
                    WordItem* wordItem = avlTree.getValue(word);
                    bool foundDocument = false;
                    for (DocumentItem& document : wordItem->documentList) {
                        if (document.documentName == file_2) {
                            document.count++;
                            foundDocument = true;
                            break;
                        }
                    }
                    if (!foundDocument) {
                        // Document not found in wordItem's document list
                        DocumentItem doc;
                        doc.count = 1;
                        doc.documentName = file_2;
                        wordItem->documentList.push_back(doc);
                    }
                }

                // **Hash Table:**
                if (table.find(word).key == notFound) {
                    // Word not found in hash table
                    WordItem* wordItem = new WordItem;
                    wordItem->word = word;
                    DocumentItem doc;
                    doc.count = 1;
                    doc.documentName = file_2;
                    wordItem->documentList.push_back(doc);
                    table.insert(word, wordItem);
                }
                else {
                    // Word found in hash table
                    WordItem* wordItem = table.find(word).value;
                    bool foundDocument = false;
                    for (DocumentItem& document : wordItem->documentList) {
                        if (document.documentName == file_2) {
                            document.count++;
                            foundDocument = true;
                            break;
                        }
                    }
                    if (!foundDocument) {
                        // Document not found in wordItem's document list
                        DocumentItem doc;
                        doc.count = 1;
                        doc.documentName = file_2;
                        wordItem->documentList.push_back(doc);
                    }
                }
            }
        }
    }




    cout << endl << endl << "After preprocessing, the unique word count is " << table.getTableSize() << ". Current load ratio is " << table.getLoadFactor() << endl;

    // queries finding words in avltree
    string quer;
    cout << "Enter queried words in one line: ";
    cin.ignore();
    getline(cin, quer);
    while (quer != "ENDOFINPUT") {
        string processedQuery = replaceNonAlpha(quer);
        istringstream iss(processedQuery);
        string word;
        vector<string> query_list;
        while (iss >> word)
        {
            if (word != "REMOVE")
                word = toLower(word);
            query_list.push_back(word);//making vector to find words in avltree 
        }

        int k = 20;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < k; i++)
        {
            // QueryDocuments(with BST);
            if (checking_for_tree(query_list, avlTree)) {//to see if word exist in tree        
                for (const string& file : file_names) {//making a for loop to reach words document by document to print like wanted
                    if (checking_for_docs(query_list, file, avlTree)) {//if word in document
                        if (i == 19) {
                            cout << "in Document" << " " << file << ",";
                        }
                        
                        for (const string& word : query_list) {//reaching every word in query 
                            {
                                WordItem* wordItem = avlTree.getValue(word);
                                vector<DocumentItem> doc = wordItem->documentList;
                                int index = 0;
                                for (size_t i = 0; i < doc.size(); ++i) {
                                    if (doc[i].documentName == file) {
                                        index = i;//loop for finding correct index of in the document vector in worditem pointer in avltree
                                        break;
                                    }
                                }
                                DocumentItem doc_1 = doc[index];
                                if (i == 19) {
                                    cout << word << " found " << doc_1.count << " times.";
                                    if (word != query_list[query_list.size() - 1]) {
                                        cout << ", ";
                                    }
                                }
                            }
                        }
                        if (i == 19) {
                            cout << endl;
                        }
                    }
                }
            }
            else {
                if (i == 19) {
                    cout << "No document contains the given query" << endl;
                }
            }

        }
        auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < k; i++)
        {
            // QueryDocuments (with hashtable);
            // Search in hash table
            if (checking_for_hash_table(query_list, table)) {
                for (const string& file : file_names) {
                    if (doc_has_queries(query_list, table, file)) {
                        if (i == 19) {
                            cout << "in Document" << " " << file << ",";
                        }
                        for (const string& word : query_list) {
                            WordItem* wordItem = table.find(word).value;
                            vector<DocumentItem> doc = wordItem->documentList;
                            int index = 0;
                            for (size_t i = 0; i < doc.size(); ++i) {
                                if (doc[i].documentName == file) {
                                    index = i;
                                    break;
                                }
                            }
                            DocumentItem doc_1 = doc[index];
                            if (i == 19) {
                                cout << word << " found " << doc_1.count << " times.";
                                if (word != query_list[query_list.size() - 1]) {
                                    cout << ", ";
                                }
                            }
                        }
                        if (i == 19) {
                            cout << endl;
                        }
                    }
                }
            }
            else {
                if (i == 19) {
                    cout << "No document contains the given query" << endl;
                }
            }
        }

        auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);
        cout << "\nTime: " << BSTTime.count() / k;
        cout << "\nTime: " << HTTime.count() / k << "\n";
        cout << "Speed Up: " << double(BSTTime.count() / k) / double(HTTime.count() / k);
        cout << endl;
        cout << "Enter queried words in one line: ";
        getline(cin, quer);

    }
    return 0;
}
