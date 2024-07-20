#pragma once
#ifdef ENCRYPTION_EXPORTS
#define ENCRYPTION_API __declspec(dllexport)
#else
#define ENCRYPTION_API __declspec(dllimport)
#endif

#ifndef ENCRYPTOR_HPP
#define ENCRYPTOR_HPP

#include <vector>
#include <string>

class ENCRYPTION_API Encryptor;

class Encryptor
{
public:
	Encryptor();
	std::string Encrypt(const std::string& msg);
	std::string Decrypt(const std::string& cipher);
	bool Encrypt(const std::string& src_file, const std::string& out_file);
	bool Decrypt(const std::string& src_file, const std::string& out_file);
private:
	unsigned char* _key;
	unsigned char* _iv;
};
#endif // ENCRYPTOR_HPP