// OpenSSL_Example.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include <fstream>
#include <Encryptor.hpp>

using namespace std;

//Demo encrypto and decrypto
void MessageEncrypt(const string& msg) {
    Encryptor enc;
    auto st = clock();
    auto cipher = enc.Encrypt(msg);
    auto et = clock();
    cout << "Encrypt" << endl;
    cout << "  msg:" << msg << endl;
    cout << "  msg len:" << msg.size() << endl;
    cout << "  cipher:" << cipher << endl;
    cout << "  cipher len:" << cipher.length() << endl;
    cout << "  time cost:" << (et - st) << "ms" << endl;
    st = clock();
    auto res = enc.Decrypt(cipher);
    et = clock();
    cout << "Decrypt" << endl;
    cout << "  msg:" << res << endl;
    cout << "  msg len:" << res.size() << endl;
    cout << "  time cost:" << (et - st) << "ms" << endl;
}

void ShowHelp() {

}

int main(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++) {
        cout << argv[i] << endl;
    }
    Encryptor enc;
    string msg_file = "msg_en.txt";
    string cipher_file = "cipher_en.txt";
    auto st = clock();
    if (enc.Encrypt(msg_file, cipher_file, false)) {
        auto et = clock();
        cout << "Encrypt file " << msg_file << " to " << cipher_file << " successfully, cost " << et - st << "ms" << endl;
    }
    
    st = clock();
    if (enc.Decrypt(cipher_file, msg_file, false)) {
        auto et = clock();
        cout << "Decrypt file " << cipher_file << " to " << msg_file << " successfully, cost " << et - st << "ms" << endl;
    }
    

    vector<string> cases = {
        "123456", "ABCDEFGHIJKLMNOPQRSTUVWXYZ","中文測試訊息",
        "PC_CPU_ABCDEFGHIJKLMNOPQRSTUVWXYZ,;%!^@1234567890#$*&/?abcdefghijklmnopqrstuvwxyz"
    };
    for(auto& i : cases)
        MessageEncrypt(i);
    system("pause");
}
