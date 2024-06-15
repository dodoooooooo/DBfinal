#include <iostream>
#include <sstream>

using namespace std;

int main(){
    string input;
    cout << "輸入學號或課程代碼：";
    cin >> input;
    if (input[0] == 'D'){
        cout << "ok\n";
    }
    return 0;
}