#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>

using namespace std;

// 將輸入數據分割成兩個部分
void cutInputData(const string& input, string arr[]) {
    istringstream iss(input);
    iss >> arr[0];
    getline(iss, arr[1]);
}

// 將數據行分割成三個部分
void cutData(const string& input, string arr[]) {
    stringstream ss(input);
    string token;
    int index = 0;
    while (getline(ss, token, ',')) {
        if (index < 3) {
            arr[index++] = token;
        } else {
            break;
        }
    }
}

int counter = 0;
set<string> seenResults; // 用於追蹤已輸出的結果

// 從數據檔案中搜索並顯示匹配結果
void searchInDataFile(const string& filePath, const string& key, int keyIndex, int outputIndex1, int outputIndex2) {
    ifstream dataFile(filePath.c_str());
    string line;
    // 檢查文件是否成功打開
    if (!dataFile.is_open()) {
        cerr << "Failed to open data file: " << filePath << endl;
        return;
    }
    while (getline(dataFile, line)) {
        string parts[3];
        cutData(line, parts);

        if (parts[keyIndex] == key) {
            string result = parts[outputIndex1] + " " + parts[outputIndex2];
            if (seenResults.find(result) == seenResults.end()) { // 檢查結果是否已經輸出過
                counter++;
                cout << result << endl;
                seenResults.insert(result); // 將結果加入追蹤集合中
            }
        }
    }
    dataFile.close();
}

// 搜索索引檔案
void searchIndex(const string& searchKey, const string& indexDir, int keyIndex, int outputIndex1, int outputIndex2) {
    int fileCounter = 0;
    bool searchSuccess = false;
    ifstream indexFile;
    while (true) {
        stringstream ss;
        ss << indexDir << setw(4) << setfill('0') << ++fileCounter << ".txt";
        string indexFilePath = ss.str();

        indexFile.open(indexFilePath.c_str());
        if (indexFile.fail()) {
            break;
        }

        string line;
        while (getline(indexFile, line)) {
            string indexParts[3];
            cutInputData(line, indexParts);

            if (indexParts[0] == searchKey) {
                searchInDataFile(indexParts[1].substr(1), searchKey, keyIndex, outputIndex1, outputIndex2);
                searchSuccess = true;
            }
        }
        indexFile.close();
    }

    if (!searchSuccess) {
        cout << "Data not found" << endl;
    }
}

// 根據 CID 搜索
void searchByCID(const string& cid) {
    searchIndex(cid, "../cut_index/cut_/index_cid/", 1, 0, 2);
}

// 根據課程名稱搜索
void searchByName(const string& name) {
    searchIndex(name, "../cut_index/cut_/index_cname/", 2, 0, 1);
}

// 根據 SID 搜索
void searchBySID(const string& sid) {
    searchIndex(sid, "../cut_index/cut_/index_sid/", 0, 1, 2);
}

int main() {
    cout << "Enter a student ID, course ID, or course name to search (type 'exit' to quit): ";
    string input;
    while (getline(cin, input) && input != "exit") {
        if (input.empty()) {
            cout << "Input cannot be empty. Please try again: ";
            continue;
        }
        seenResults.clear(); // 每次搜索前清空追蹤集合
        if (input[0] == 'D' || input[0] == 'd') { // 假設學號以"D"或"d"開頭
            searchBySID(input);
            cout << "total: " << counter << endl;
            counter = 0;
        } else if (isdigit(input[0])) { // 假設輸入以數字開頭，認為是課程代碼
            searchByCID(input);
            cout << "total: " << counter << endl;
            counter = 0;
        } else { // 其他情況認為是課程名稱
            searchByName(input);
            cout << "total: " << counter << endl;
            counter = 0;
        }
        cout << "Enter another student ID, course ID, or course name to search (type 'exit' to quit): ";
    }
    return 0;
}
