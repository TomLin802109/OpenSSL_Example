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
	Encryptor(const std::string& key, const std::string& iv);

	/// <summary>
	/// Encrypt a message
	/// </summary>
	/// <param name="msg">source message</param>
	/// <returns>cipher</returns>
	std::string Encrypt(const std::string& msg);
	/// <summary>
	/// Decrypt a cipher
	/// </summary>
	/// <param name="cipher"></param>
	/// <returns>origin message</returns>
	std::string Decrypt(const std::string& cipher);
	bool Encrypt(const std::string& src_file, const std::string& out_file, bool binary);
	bool Decrypt(const std::string& src_file, const std::string& out_file, bool binary);
	void SetKey(const std::string& key);
	void SetInitVec(const std::string& iv);
private:
	unsigned char* _key;
	unsigned char* _iv;
};
#endif // ENCRYPTOR_HPP