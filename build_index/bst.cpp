#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm> // for std::remove

#define READ_FILE_CUT_SIZE 3
#define DATA_COUNTER 466

using namespace std;

void cut_input_data(string s, string arr[], int size) {
    size_t start = 0;
    size_t end = s.find(",");
    int index = 0;

    while (end != string::npos && index < size) {
        arr[index++] = s.substr(start, end - start);
        start = end + 1;
        end = s.find(",", start);
    }

    if (index < size) {
        arr[index] = s.substr(start);
    }
}

void remove_newlines(string& s) {
    s.erase(remove(s.begin(), s.end(), '\r'), s.end());
    s.erase(remove(s.begin(), s.end(), '\n'), s.end());
}

struct DataEntry {
    string course_code;     // 課程代碼
    vector<string> path_info; // 存放路徑資訊的向量
    DataEntry* left;
    DataEntry* right;
};

DataEntry* newNode(string course_code, string path_info) {
    DataEntry* temp = new DataEntry;
    temp->course_code = course_code;
    temp->path_info.push_back(path_info);
    temp->left = temp->right = nullptr;
    return temp;
}

DataEntry* insert(DataEntry* node, string course_code, string path_info) {
    if (node == nullptr) return newNode(course_code, path_info);

    if (course_code < node->course_code)
        node->left = insert(node->left, course_code, path_info);
    else if (course_code > node->course_code)
        node->right = insert(node->right, course_code, path_info);
    else {
        // 檢查該路徑是否已經存在於vector中
        if (find(node->path_info.begin(), node->path_info.end(), path_info) == node->path_info.end()) {
            node->path_info.push_back(path_info);
        }
    }

    return node;
}

void inOrder(DataEntry* root, ofstream& output_file_course_id) {
    if (root != nullptr) {
        inOrder(root->left, output_file_course_id);
        for (const auto& path : root->path_info) {
            output_file_course_id << root->course_code << " " << path << "\n";
        }
        inOrder(root->right, output_file_course_id);
    }
}

void deleteTree(DataEntry* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

int main() {
    ifstream reads_file;
    DataEntry* root = nullptr;

    int data_counter = 1;
    string data_locate = "../data_big5/";

    int x = DATA_COUNTER;

    while (x--) {
        stringstream ss;
        ss << data_locate << setw(4) << setfill('0') << data_counter;
        string data_locate_str = ss.str();
        data_counter++;

        reads_file.open(data_locate_str);

        if (reads_file.fail()) {
            cout << "檔案開啟失敗\n";
            exit(1);
        } else {
            string read_file_string;
            getline(reads_file, read_file_string); // 跳過第一行標題

            while (getline(reads_file, read_file_string)) {
                string datacut[READ_FILE_CUT_SIZE];
                cut_input_data(read_file_string, datacut, READ_FILE_CUT_SIZE);
                remove_newlines(datacut[2]);
                root = insert(root, datacut[1], data_locate_str);
            }

            reads_file.close();
        }
    }

    ofstream output_file_course_id("../cut_index/uncut/course_id1.txt");

    if (!output_file_course_id.is_open()) {
        cout << "無法開啟輸出檔案\n";
        exit(1);
    }

    inOrder(root, output_file_course_id);

    output_file_course_id.close();
    deleteTree(root); // 釋放BST記憶體

    return 0;
}