#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include "../include/encryption.h"

using json = nlohmann::json;

bool storeJsonData(const std::string& fileName, const json& data, bool minimize = false, bool encrypt = false, const std::string& key = "") {
    // Dump JSON with or without minimization
    std::string jsonStr = minimize ? data.dump() : data.dump(4);
    
    if (encrypt && !key.empty()) {
        try {
            // Make sure the key is exactly 32 bytes
            std::string paddedKey = key;
            if (paddedKey.length() < 32) {
                paddedKey.append(32 - paddedKey.length(), '0');
            } else if (paddedKey.length() > 32) {
                paddedKey = paddedKey.substr(0, 32);
            }
            
            Encryption enc;
            jsonStr = enc.encrypt(jsonStr, paddedKey);
        } catch (const std::exception& e) {
            std::cerr << "Encryption failed: " << e.what() << std::endl;
            return false;
        }
    }
    
    std::ofstream outFile(fileName, std::ios::out | std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return false;
    }
    
    outFile << jsonStr;
    outFile.close();
    return true;
}

json parseJsonInput(const std::string& input) {
    // First try to read as a file
    std::ifstream inFile(input, std::ios::binary);
    if (inFile.is_open()) {
        try {
            json data;
            inFile >> data;
            inFile.close();
            return data;
        } catch (json::parse_error& e) {
            inFile.close();
            // If file parsing fails, try parsing as a JSON string
            return json::parse(input);
        }
    }
    
    // If not a file, try parsing as a JSON string
    try {
        return json::parse(input);
    } catch (json::parse_error& e) {
        throw std::runtime_error("Input is neither a valid JSON file nor a valid JSON string");
    }
}

// Replace checkFileStatus with getFileContent
std::string getFileContent(const std::string& fileName, const std::string& key = "") {
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + fileName);
    }
    
    // Read the entire file into a string
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();
    
    if (content.empty()) {
        throw std::runtime_error("File is empty: " + fileName);
    }
    
    // Check if content appears to be encrypted
    bool seemsEncrypted = !content.empty() && !isprint(content[0]);
    if (seemsEncrypted) {
        if (key.empty()) {
            throw std::runtime_error("File is encrypted, encryption key required");
        }
        // Decrypt content
        try {
            std::string paddedKey = key;
            if (paddedKey.length() < 32) {
                paddedKey.append(32 - paddedKey.length(), '0');
            } else if (paddedKey.length() > 32) {
                paddedKey = paddedKey.substr(0, 32);
            }
            
            Encryption enc;
            content = enc.decrypt(content, paddedKey);
        } catch (const std::exception& e) {
            throw std::runtime_error("Decryption failed: " + std::string(e.what()));
        }
    }
    
    // Try to parse and format the JSON
    try {
        json j = json::parse(content);
        return j.dump(4); // Pretty print with 4 spaces
    } catch (json::parse_error& e) {
        // If parsing fails, return the raw content
        return content;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./main <json_input> <output_file> [--minimize] [--encrypt <encryption_key>]" << std::endl;
        std::cerr << "   or: ./main --read <file_path> [--key <encryption_key>]" << std::endl;
        return 1;
    }

    std::string firstArg = argv[1];
    if (firstArg == "--read") {
        if (argc < 3) {
            std::cerr << "Error: No file specified for --read command" << std::endl;
            return 1;
        }
        try {
            std::string fileName = argv[2];
            std::string key;
            
            // Check for encryption key
            if (argc > 4 && std::string(argv[3]) == "--key") {
                key = argv[4];
            }
            
            // Read and output file content
            std::string content = getFileContent(fileName, key);
            std::cout << content;
            return 0;
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
    } else {
        // Handle the original JSON processing logic
        if (argc < 3) {
            std::cerr << "Usage: ./main <json_input> <output_file> [--minimize] [--encrypt <encryption_key>]" << std::endl;
            return 1;
        }

        bool shouldEncrypt = false;
        bool shouldMinimize = false;
        std::string encryptionKey;
        
        // Parse command line arguments
        for (int i = 3; i < argc; i++) {
            std::string arg = argv[i];
            if (arg == "--minimize") {
                shouldMinimize = true;
            } else if (arg == "--encrypt" && i + 1 < argc) {
                shouldEncrypt = true;
                encryptionKey = argv[++i];
            }
        }

        // Validate encryption key if encryption is requested
        if (shouldEncrypt && encryptionKey.empty()) {
            std::cerr << "Encryption key must be provided when using --encrypt" << std::endl;
            return 1;
        }

        json inputData;
        try {
            inputData = parseJsonInput(argv[1]);
        } catch (const std::exception& e) {
            std::cerr << "Error processing JSON input: " << e.what() << std::endl;
            return 1;
        }

        std::string outputFileName = argv[2];

        // Store the data with optional encryption and minification
        if (storeJsonData(outputFileName, inputData, shouldMinimize, shouldEncrypt, encryptionKey)) {
            std::cout << "Data successfully stored in " << outputFileName 
                    << (shouldMinimize ? " (minimized)" : "")
                    << (shouldEncrypt ? " (encrypted)" : "") << std::endl;
        } else {
            std::cerr << "Failed to store data." << std::endl;
            return 1;
        }
    }

    return 0;
}
