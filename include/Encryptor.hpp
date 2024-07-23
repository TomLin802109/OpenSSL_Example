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

#ifdef USE_SWIG
class Encryptor;
#else
class ENCRYPTION_API Encryptor;
#endif // USE_SWIG
extern "C" {
	class Encryptor
	{
	public:
		Encryptor();
		Encryptor(const std::vector<unsigned char>& key, const std::vector<unsigned char>& iv);

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

		std::vector<unsigned char> Encrypt(const std::vector<unsigned char>& data);
		std::vector<unsigned char> Decrypt(const std::vector<unsigned char>& data);

		bool Encrypt(const std::string& src_file, const std::string& out_file);
		bool Decrypt(const std::string& src_file, const std::string& out_file);


		void SetKey(const std::vector<unsigned char>& key);
		void SetInitVec(const std::vector<unsigned char>& iv);
	private:
		std::vector<unsigned char> _key;
		std::vector<unsigned char> _iv;
	};
}

#endif // ENCRYPTOR_HPP