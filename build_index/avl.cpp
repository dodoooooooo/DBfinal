#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm> // for std::remove, std::sort

#define READ_FILE_CUT_SIZE 3
#define DATA_COUNTER 466

using namespace std;

// Function to split the input data
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

// Function to remove newlines from a string
void remove_newlines(string& s) {
    s.erase(remove(s.begin(), s.end(), '\r'), s.end());
    s.erase(remove(s.begin(), s.end(), '\n'), s.end());
}

// DataEntry struct representing each entry
struct DataEntry {
    string student_id;
    string course_code;
    string path_info;
};

// Function to compare DataEntry by student_id and path_info
bool compareByStudentIDAndPathInfo(const DataEntry &a, const DataEntry &b) {
    if (a.student_id == b.student_id) {
        return a.path_info < b.path_info;
    }
    return a.student_id < b.student_id;
}

int main() {
    ifstream reads_file;
    vector<DataEntry> data_entries;

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
            cout << "�ɮ׶}�ҥ���\n";
            exit(1);
        } else {
            string read_file_string;
            getline(reads_file, read_file_string); // Skip the header line

            while (getline(reads_file, read_file_string)) {
                string datacut[READ_FILE_CUT_SIZE];
                cut_input_data(read_file_string, datacut, READ_FILE_CUT_SIZE);
                remove_newlines(datacut[2]);

                DataEntry entry;
                entry.student_id = datacut[0];
                entry.course_code = datacut[1];
                entry.path_info = data_locate_str;
                data_entries.push_back(entry);
            }

            reads_file.close();
        }
    }

    // Sort data_entries by student_id and path_info
    sort(data_entries.begin(), data_entries.end(), compareByStudentIDAndPathInfo);

    // Write the sorted data to the output file
    ofstream output_file_student_id("../cut_index/uncut/student_id1.txt");

    if (!output_file_student_id.is_open()) {
        cout << "�L�k�}�ҿ�X�ɮ�\n";
        exit(1);
    }

    for (const auto& entry : data_entries) {
        output_file_student_id << entry.student_id << " " << entry.path_info << "\n";
    }

    output_file_student_id.close();

    return 0;
}