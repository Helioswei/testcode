#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Poco/Crypto/Cipher.h"
#include "Poco/Crypto/CipherFactory.h"
#include "Poco/Crypto/RSAKey.h"
#include "Poco/Crypto/RSADigestEngine.h"

using Poco::Crypto::Cipher;
using Poco::Crypto::CipherFactory;
using Poco::Crypto::RSAKey;
using namespace std;

//自动生成证书，存在当前目录下
void generatePriPubKey() {
    //生成公钥和私钥
    RSAKey key(RSAKey::KL_1024, RSAKey::EXP_SMALL);
    string pubFile("public.pub");
    string privFile("private.priv");
    key.save(pubFile, privFile);
}

// 
int main() {
    //读取公钥进行加密
    Cipher::Ptr pCipher =
        CipherFactory::defaultFactory().createCipher(RSAKey("./public.pub"));
    std::string val("1111");
    //一般加密解密后用
    std::string enc = pCipher->encryptString(val, Cipher::ENC_BASE64);
    cout << enc << endl;
    //读取私钥进行解密
    Cipher::Ptr pCipher1 = CipherFactory::defaultFactory().createCipher(
        RSAKey("", "./private.priv"));
    std::string dec = pCipher1->decryptString(enc, Cipher::ENC_BASE64);
    cout << dec << endl;
    // std::string dec = pCipher->decryptString(enc, Cipher::ENC_BASE64);
    return 0;

    // std::string msg("Test this sign message");
    // RSAKey key(RSAKey::KL_2048, RSAKey::EXP_LARGE);
    // string pubFile("public.pub");
    // string privFile("private.priv");
    // key.save(pubFile, privFile);
    // Poco::Crypto::RSADigestEngine eng(key);
    // eng.update(msg.c_str(), static_cast<unsigned>(msg.length()));
    // const Poco::DigestEngine::Digest& sig = eng.signature();
    // std::string hexDig = Poco::DigestEngine::digestToHex(sig);
    // cout << hexDig << endl;
}

