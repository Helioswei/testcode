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

int main() {
    Cipher::Ptr pCipher4 = CipherFactory::defaultFactory().createCipher(RSAKey("./public.pub"));
    return 0
    RSAKey key2(pub);
    CipherFactory factory2;
    Cipher::Ptr pCipher2 = factory2.createCipher(key2);
    std::string enc1 = pCipher2->encryptString(val2,Cipher::ENC_BASE64);
    cout << enc1 << endl; 

    return 0;
    //RSAKey key1(RSAKey::KL_1024, RSAKey::EXP_LARGE);
    RSAKey key1(RSAKey::KL_1024, RSAKey::EXP_SMALL);
    //生成公钥和私钥
    string pubFile1("public.pub");
    string privFile1("private.priv");

    //rsa加密解密
    //Cipher::Ptr pCipher = CipherFactory::defaultFactory().createCipher(
    //    RSAKey(RSAKey::KL_1024, RSAKey::EXP_SMALL));
    CipherFactory factory;
    Cipher::Ptr pCipher = factory.createCipher(key1);
    //RSA_PADDING_PKCS1
    key1.save(pubFile1, privFile1);

    
    std::string val("1111");
    //一般加密解密后用
    std::string enc = pCipher->encryptString(val,Cipher::ENC_BASE64);
    std::string dec = pCipher->decryptString(enc, Cipher::ENC_BASE64);

    std::cout << "加密后： " << enc << std::endl;
    std::cout << "解密后: " << dec << std::endl;
    return 0;

    std::string msg("Test this sign message");
    RSAKey key(RSAKey::KL_2048, RSAKey::EXP_LARGE);
    string pubFile("public.pub");
    string privFile("private.priv");
    key.save(pubFile, privFile);
    Poco::Crypto::RSADigestEngine eng(key);
    eng.update(msg.c_str(), static_cast<unsigned>(msg.length()));
    const Poco::DigestEngine::Digest& sig = eng.signature();
    std::string hexDig = Poco::DigestEngine::digestToHex(sig);
    cout << hexDig << endl;

    // verify
    std::ostringstream strPub;
    key.save(&strPub);
    std::string pubKey = strPub.str();
    std::istringstream iPub(pubKey);
    RSAKey keyPub(&iPub);
    Poco::Crypto::RSADigestEngine eng2(keyPub);
    eng2.update(msg.c_str(), static_cast<unsigned>(msg.length()));
    // const Poco::DigestEngine::Digest& a = eng2.signature();
    // std::string text = Poco::DigestEngine::digestToHex(a);
    // cout << text << endl;
    // assertTrue(eng2.verify(sig));

    return 0;
}

