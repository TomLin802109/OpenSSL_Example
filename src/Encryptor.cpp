#include <Encryptor.hpp>
#include <openssl/evp.h>
#include <fstream>
#include <iostream>

using namespace std;

Encryptor::Encryptor() {
	_key = (unsigned char*)"w96j0 ej;32/ 2u04y3ej3zp4u.3vu04ej/ n ";
	_iv = (unsigned char*)"tjg3vu;4xu;4fwjefijnadjf";
}
Encryptor::Encryptor(const std::string& key, const std::string& iv) 
	: _key((unsigned char*)key.c_str()), _iv((unsigned char*)iv.c_str()) {}

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

bool Encryptor::Encrypt(const std::string& src_file, const std::string& out_file, bool binary) {
	//auto i_mode = binary ? ios::binary | ios::ate : ios::ate;
	//auto o_mode = binary ? ios::binary | ios::out : ios::out;
	//read source file
	ifstream ifs(src_file, ios::ate);
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
	ofstream ofs(out_file);
	if (!ofs.is_open())
		return false;
	ofs.write(cipher.c_str(), cipher.size());
	ofs.close();

	//check output file size
	ifs = ifstream(out_file, ios::ate);
	auto out_size = ifs.tellg();
	return out_size > 0;
}

bool Encryptor::Decrypt(const std::string& src_file, const std::string& out_file, bool binary) {
	/*auto i_mode = binary ? ios::binary | ios::ate : ios::ate;
	auto o_mode = binary ? ios::binary | ios::out : ios::out;*/
	//read source file
	ifstream ifs(src_file, ios::ate);
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
	ofs.open(out_file);
	ofs.write(msg.c_str(), msg.size());
	ofs.close();

	//check output file size
	ifs = ifstream(out_file, ios::ate);
	auto out_size = ifs.tellg();
	return out_size > 0;
}

void Encryptor::SetKey(const std::string& key) {
	_key = (unsigned char*)key.c_str();
}
void Encryptor::SetInitVec(const std::string& iv) {
	_iv = (unsigned char*)iv.c_str();
}