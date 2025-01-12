#include "../include/encryption.h"

std::string Encryption::encrypt(const std::string& data, const std::string& key) {
    if (key.length() != 32) {
        throw std::runtime_error("Key must be exactly 32 bytes");
    }
    
    std::string result = data;
    for (size_t i = 0; i < data.length(); ++i) {
        result[i] = data[i] ^ key[i % key.length()];
    }
    return result;
}

std::string Encryption::decrypt(const std::string& data, const std::string& key) {
    return encrypt(data, key);
}
