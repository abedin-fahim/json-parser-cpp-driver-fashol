#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>
#include <stdexcept>

class Encryption {
public:
    std::string encrypt(const std::string& data, const std::string& key);
    std::string decrypt(const std::string& data, const std::string& key);
};

#endif // ENCRYPTION_H
