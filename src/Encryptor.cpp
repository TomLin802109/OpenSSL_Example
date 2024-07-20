#include <Encryptor.hpp>
#include <openssl/evp.h>
#include <fstream>
#include <iostream>

using namespace std;

Encryptor::Encryptor() {
	_key = (unsigned char*)"w96j0 ej;32/ 2u04y3ej3zp4u.3vu04ej/ n ";
	_iv = (unsigned char*)"tjg3vu;4xu;4fwjefijnadjf";
}

std::string Encryptor::Encrypt(const std::string& msg) {
	string cipher;
	vector<unsigned char> buf(max(msg.size(), (size_t)256));
	int len = 0;
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_EncryptInit(ctx, EVP_aes_256_cbc(), _key, _iv);
	EVP_EncryptUpdate(ctx, buf.data(), &len, (unsigned char*)msg.c_str(), static_cast<int>(msg.size()));
	cipher.append((char*)buf.data(), len);
	EVP_EncryptFinal(ctx, buf.data(), &len);
	cipher.append((char*)buf.data(), len);
	return cipher;
}

std::string Encryptor::Decrypt(const std::string& cipher) {
	string msg;
	vector<unsigned char> buf(cipher.size());
	int len = 0;
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_DecryptInit(ctx, EVP_aes_256_cbc(), _key, _iv);
	EVP_DecryptUpdate(ctx, buf.data(), &len, (unsigned char*)cipher.c_str(), static_cast<int>(cipher.size()));
	msg.append((char*)buf.data(), len);
	EVP_DecryptFinal(ctx, buf.data(), &len);
	msg.append((char*)buf.data(), len);
	return msg;
}

bool Encryptor::Encrypt(const std::string& src_file, const std::string& out_file) {
	ifstream ifs;
	ifs.open(src_file);
	ifs.seekg(0, ios::end);
	size_t file_size = ifs.tellg();
	ifs.seekg(0);
	cout << "file size:" << file_size<< endl;
	if (file_size <= 0)
		return false;
	string ctx(file_size, ' ');
	ifs.read(&ctx[0], file_size);
	auto cipher = Encrypt(ctx);
	ifs.close();
	ofstream ofs;
	ofs.open(out_file);
	ofs.write(cipher.c_str(), cipher.size());
	ofs.close();
	return true;
}

bool Encryptor::Decrypt(const std::string& src_file, const std::string& out_file) {
	ifstream ifs;
	ifs.open(src_file);
	ifs.seekg(0, ios::end);
	size_t file_size = ifs.tellg();
	ifs.seekg(0);
	cout << "file size:" << file_size << endl;
	if (file_size <= 0)
		return false;
	string ctx(file_size, ' ');
	ifs.read(&ctx[0], file_size);
	auto msg = Decrypt(ctx);
	ifs.close();
	ofstream ofs;
	ofs.open(out_file);
	ofs.write(msg.c_str(), msg.size());
	ofs.close();
	return true;
}