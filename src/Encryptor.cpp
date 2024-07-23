#include <Encryptor.hpp>
#include <openssl/evp.h>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

Encryptor::Encryptor() {
	_key = vector<uint8_t>(32, 'q');
	_iv = vector<uint8_t>(32, 'r');
}
Encryptor::Encryptor(const std::vector<unsigned char>& key, const std::vector<unsigned char>& iv)
	: _key(key), _iv(iv) {}

std::string Encryptor::Encrypt(const std::string& msg) {
	string cipher;
	vector<unsigned char> buf(max(msg.size(), (size_t)256));
	int len = 0;
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_EncryptInit(ctx, EVP_aes_256_cbc(), _key.data(), _iv.data());
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
	EVP_DecryptInit(ctx, EVP_aes_256_cbc(), _key.data(), _iv.data());
	EVP_DecryptUpdate(ctx, buf.data(), &len, (unsigned char*)cipher.c_str(), static_cast<int>(cipher.size()));
	msg.append((char*)buf.data(), len);
	EVP_DecryptFinal(ctx, buf.data(), &len);
	msg.append((char*)buf.data(), len);
	return msg;
}

bool Encryptor::Encrypt(const std::string& src_file, const std::string& out_file) {
	//read source file
	ifstream ifs(src_file, ios::binary | ios::ate);
	if (!ifs.is_open())
		return false;
	size_t file_size = ifs.tellg();
	if (file_size <= 0)
		return false;
	ifs.seekg(0);
	string ctx(file_size, ' ');
	ifs.read(&ctx[0], file_size);
	ifs.close();

	auto cipher = Encrypt(ctx);
	
	//write cipher to file
	ofstream ofs(out_file, ios::binary | ios::out);
	if (!ofs.is_open())
		return false;
	ofs.write(cipher.c_str(), cipher.size());
	ofs.close();
	//check output file size
	ifs = ifstream(out_file, ios::binary | ios::ate);
	auto out_size = ifs.tellg();
	return out_size > 0;
}

bool Encryptor::Decrypt(const std::string& src_file, const std::string& out_file) {
	//read source file
	ifstream ifs(src_file, ios::binary | ios::ate);
	if (!ifs.is_open())
		return false;
	size_t file_size = ifs.tellg();
	if (file_size <= 0)
		return false;
	ifs.seekg(0);
	string ctx(file_size, ' ');
	ifs.read(&ctx[0], file_size);
	ifs.close();

	auto msg = Decrypt(ctx);

	//write cipher to file
	ofstream ofs;
	ofs.open(out_file, ios::binary);
	ofs.write(msg.c_str(), msg.size());
	ofs.close();

	//check output file size
	ifs = ifstream(out_file, ios::binary | ios::ate);
	auto out_size = ifs.tellg();
	return out_size > 0;
}

void Encryptor::SetKey(const std::vector<unsigned char>& key) {
	_key = key;
}
void Encryptor::SetInitVec(const std::vector<unsigned char>& iv) {
	_iv = iv;
}

std::vector<unsigned char> Encryptor::Encrypt(const std::vector<unsigned char>& data) {
	vector<unsigned char> res;
	vector<unsigned char> buf(max(data.size(), (size_t)256));
	int len = 0;
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_EncryptInit(ctx, EVP_aes_256_cbc(), _key.data(), _iv.data());
	EVP_EncryptUpdate(ctx, buf.data(), &len, data.data(), static_cast<int>(data.size()));
	res.insert(res.end(), buf.begin(), buf.begin() + len);
	EVP_EncryptFinal(ctx, buf.data(), &len);
	res.insert(res.end(), buf.begin(), buf.begin() + len);
	return res;
}
std::vector<unsigned char> Encryptor::Decrypt(const std::vector<unsigned char>& data) {
	vector<unsigned char> res;
	vector<unsigned char> buf(data.size());
	int len = 0;
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_DecryptInit(ctx, EVP_aes_256_cbc(), _key.data(), _iv.data());
	EVP_DecryptUpdate(ctx, buf.data(), &len, data.data(), static_cast<int>(data.size()));
	res.insert(res.end(), buf.begin(), buf.begin() + len);
	EVP_DecryptFinal(ctx, buf.data(), &len);
	res.insert(res.end(), buf.begin(), buf.begin() + len);
	return res;
}