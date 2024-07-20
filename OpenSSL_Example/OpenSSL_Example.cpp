// OpenSSL_Example.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
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

int main()
{
    vector<string> cases = {
        "123456", "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
        "PC_CPU_ABCDEFGHIJKLMNOPQRSTUVWXYZ,;%!^@1234567890#$*&/?abcdefghijklmnopqrstuvwxyz"
    };
    for(auto& i : cases)
        MessageEncrypt(i);
    system("pause");
}
