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

// 函數：切割字串 s 成為大小為 size 的陣列 arr
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

// 移除字串中的換行符號
void remove_newlines(string& s) {
    s.erase(remove(s.begin(), s.end(), '\r'), s.end());
    s.erase(remove(s.begin(), s.end(), '\n'), s.end());
}

// 資料結構：每筆資料的結構
struct DataEntry {
    string student_id;      // 學號
    string course_code;     // 課程代碼
    string course_name;     // 課程名稱
    string path_info;       // 存放路徑資訊
};

int main() {
    ifstream reads_file;

    vector<DataEntry> data_entries;

    int data_counter = 1;
    string data_locate = "../data_big5/";

    int x = DATA_COUNTER;

    // 讀取多個數據檔案，總共 DATA_COUNTER 個
    while (x--) {
        stringstream ss;
        ss << data_locate << setw(4) << setfill('0') << data_counter;
        string data_locate_str = ss.str();
        data_counter++;

        // 打開檔案
        reads_file.open(data_locate_str);

        if (reads_file.fail()) {
            cout << "檔案開啟失敗\n";
            exit(1);
        } else {
            string read_file_string;
            getline(reads_file, read_file_string); // 跳過第一行標題

            // 讀取每行資料，直到檔案結尾
            while (getline(reads_file, read_file_string)) {
                string datacut[READ_FILE_CUT_SIZE];
                cut_input_data(read_file_string, datacut, READ_FILE_CUT_SIZE);
                DataEntry entry;
                entry.student_id = datacut[0];           // 學號
                entry.course_code = datacut[1];          // 課程代碼
                entry.course_name = datacut[2];          // 課程名稱
                remove_newlines(entry.course_name);      // 移除課程名稱中的換行符號
                entry.path_info = data_locate_str;       // 存放路徑資訊
                data_entries.push_back(entry);
            }

            // 關閉檔案
            reads_file.close();
        }
    }

    // 分別輸出至三個不同的檔案：student_id.txt、course_id.txt、course_name.txt
    ofstream output_file_student_id("../cut_index/uncut/student_id.txt");
    ofstream output_file_course_id("../cut_index/uncut/course_id.txt");
    ofstream output_file_course_name("../cut_index/uncut/course_name.txt");

    // 確認檔案是否成功開啟
    if (!output_file_student_id.is_open() || !output_file_course_id.is_open() || !output_file_course_name.is_open()) {
        cout << "無法開啟輸出檔案\n";
        exit(1);
    }

    // 寫入至輸出檔案
    for (const auto& entry : data_entries) {
        output_file_student_id << entry.student_id << " " << entry.path_info << "\n";
        output_file_course_id << entry.course_code << " " << entry.path_info << "\n";
        output_file_course_name << entry.course_name << " " << entry.path_info << "\n";
    }

    // 關閉檔案
    output_file_student_id.close();
    output_file_course_id.close();
    output_file_course_name.close();

    return 0;
}
