/*
AES encryption example, Hashing is simpler.
compile with g++ cryptoexample.cpp -static -std=c++11 -municode -lcrypt32 -lbcrypt -o crypttest.exe

*/


#include <iostream>
#include <string>
#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>
#include <utility/BaseWindow.h>
#include <utility/System.Cryptography.h>


int wWinMain(HINSTANCE, HINSTANCE, LPWSTR, INT)
{
  std::string data = "this is some data we are going to encrypt with AES";
  std::string pass = "PassKey";
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 md(seed);
  std::vector<byte> iv;
  iv.resize(16);
  
  for(int i = 0; i < 16; i++)
  {
    iv[i] = (unsigned char)(md());
  }
  
  System::Crypto::Symmetric::CBCryptAES aes;
  aes.SetIv(iv);
  aes.SetKey(pass);
  std::vector<byte> cleanData(data.begin(),data.end());
  auto encrypted = aes.Encrypt(cleanData);
  auto decrypted = aes.Decrypt(encrypted);
  
  std::cout << "Original Data "  << std::endl << data << std::endl;
  std::cout << "Encrypted Data " << std::endl;
  
  for(auto e : encrypted)
  {
        std::cout << std::setw(2) <<  std::hex << (int)e;
        std::cout << " ";
  }
  
  std::cout << std::endl;
  std::cout << "Decrypted Data "  << std::endl;
  
  for(auto e : decrypted)
  {
        std::cout << std::hex << e;
  }
  
  std::cout << std::endl;
  return 0;
}