# Encryption
This example used OpenSSL ver. 3.3.1, download the installer [here](https://slproweb.com/products/Win32OpenSSL.html).

Message Encrypt and Decrypt Example:
```C++
    #include <Encryptor.hpp>
    
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
```