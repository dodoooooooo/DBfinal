#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <filesystem> // C++17中的檔案系統操作

using namespace std;
namespace fs = std::filesystem;

void splitFile(const string& inputFileName, const string& outputDir, int chunkSize) {
    ifstream inputFile(inputFileName);
    if (!inputFile) {
        cerr << "Error opening input file: " << inputFileName << endl;
        return;
    }

    fs::create_directory(outputDir); // 創建輸出目錄，如果不存在的話

    vector<string> lines;
    string line;
    int currentSize = 0;
    int chunkNumber = 1;

    while (getline(inputFile, line)) {
        lines.push_back(line);
        currentSize += line.size() + 1; // 計算加上換行符號後的行大小

        if (currentSize >= chunkSize) {
            stringstream outputFileName;
            outputFileName << outputDir << "/" << setfill('0') << setw(4) << chunkNumber << ".txt";
            ofstream outputFile(outputFileName.str());

            for (const auto& l : lines) {
                outputFile << l << '\n'; // 寫入每行文字到檔案中，包括換行符號
            }

            outputFile.close();

            lines.clear();
            currentSize = 0;
            ++chunkNumber;
        }
    }

    // 處理剩餘的行
    if (!lines.empty()) {
        stringstream outputFileName;
        outputFileName << outputDir << "/" << setfill('0') << setw(4) << chunkNumber << ".txt";
        ofstream outputFile(outputFileName.str());

        for (const auto& l : lines) {
            outputFile << l << '\n'; // 寫入每行文字到檔案中，包括換行符號
        }

        outputFile.close();
    }

    inputFile.close();

    cout << "File " << inputFileName << " split into " << chunkNumber << " chunks." << endl;
}

int main() {
    string inputFileName = "../cut_index/uncut/course_id1.txt";  // 輸入檔案名稱
    string outputDir = "../cut_index/cut_/index_cid"; // 輸出目錄
    int chunkSize = 2000;  // 每個檔案的大小限制 2KB

    splitFile(inputFileName, outputDir, chunkSize);

    inputFileName = "../cut_index/uncut/student_id1.txt";
    outputDir = "../cut_index/cut_/index_sid"; // 輸出目錄
    splitFile(inputFileName, outputDir, chunkSize);

    inputFileName = "../cut_index/uncut/course_name.txt";
    outputDir = "../cut_index/cut_/index_cname"; // 輸出目錄
    splitFile(inputFileName, outputDir, chunkSize);

    return 0;
}