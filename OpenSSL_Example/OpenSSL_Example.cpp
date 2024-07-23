// OpenSSL_Example.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include <fstream>
#include <Encryptor.hpp>
#include <algorithm>

using namespace std;

//Demo encrypto and decrypto
void MessageEncrypt(Encryptor& enc, const string& msg) {
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

int main(int argc, char* argv[])
{
    vector<string> cmds, params;
    bool is_file = false, is_encrypt = true;
    Encryptor enc;
    
    for (int i = 1; i < argc; i++) {
        if (*argv[i] == '-') {
            cmds.emplace_back(argv[i]+1);
            if (strcmp(argv[i], "-f") == 0)
                is_file = true;
            if (strcmp(argv[i], "-dec") == 0)
                is_encrypt = false;
            if (strncmp(argv[i], "-key=", 5) == 0) {
                auto start = (uint8_t*)argv[i] + 5;
                enc.SetKey(vector<uint8_t>(start, start+ strlen(argv[i] + 5)));
            }
        }
        else {
            params.emplace_back(argv[i]);
        }
    }
    if (is_file && params.size() > 1) {
        ifstream ifs(params[0], ios::ate);
        auto input_size = ifs.tellg();
        auto st = clock();
        auto s = is_encrypt ? enc.Encrypt(params[0], params[1]) : enc.Decrypt(params[0], params[1]);
        auto et = clock();
        if (s) {
            ifstream ifs(params[1], ios::ate);
            cout << (is_encrypt ? "Encrypt" : "Decrypt") << " file " << params[0] << " to " << params[1] << " successfully" << endl;
            cout << params[0] << " size : " << input_size << endl;
            cout << params[1] << " size : " << ifs.tellg() << endl;
            cout << "  time cost " << et - st << "ms" << endl;
        }
        else
            cout << (is_encrypt ? "Encryption" : "Decryption") << " failure" << endl;
    }
    else if (params.size() > 0) {
        auto st = clock();
        auto res = is_encrypt ? enc.Encrypt(params.front()) : enc.Decrypt(params.front());
        auto et = clock();
        cout << (is_encrypt ? "Encryption" : "Decryption") << endl;
        cout << "  input:" << params.front() << endl;
        cout << "  output:" << res << endl;
        cout << "  time cost:" << et - st << " ms" << endl;
    }
    else {
        vector<string> cases = {
                    "123456", "ABCDEFGHIJKLMNOPQRSTUVWXYZ","中文測試訊息",
                    "PC_CPU_ABCDEFGHIJKLMNOPQRSTUVWXYZ,;%!^@1234567890#$*&/?abcdefghijklmnopqrstuvwxyz"
        };
        for (auto& i : cases)
            MessageEncrypt(enc, i);
    }
    //cout << "incorrect arguments" << endl;
    system("pause");
}
