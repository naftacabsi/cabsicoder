#include <iostream>
#include <fstream>
#include <string>
#include <openssl/aes.h>

std::string decryptMessage(const std::string& encryptedMessage, const std::string& key) {
    AES_KEY aesKey;
    if (AES_set_decrypt_key(reinterpret_cast<const unsigned char*>(key.c_str()), 128, &aesKey) != 0) {
        std::cerr << "Failed to set AES decrypt key" << std::endl;
        return "";
    }

    const size_t blocksize = AES_BLOCK_SIZE;
    size_t decryptedLength = ((encryptedMessage.length() + blocksize - 1) / blocksize) * blocksize;
    unsigned char* decryptedText = new unsigned char[decryptedLength];
    memset(decryptedText, 0, decryptedLength);

    AES_decrypt(reinterpret_cast<const unsigned char*>(encryptedMessage.c_str()), decryptedText, &aesKey);

    std::string decryptedMessage(reinterpret_cast<char*>(decryptedText));
    delete[] decryptedText;

    return decryptedMessage;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid number of arguments. Usage: decryptor <input_file>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1], std::ios::binary);
    if (!inputFile) {
        std::cerr << "Failed to open input file" << std::endl;
        return 1;
    }

    std::string encryptedMessage;
    inputFile.seekg(0, std::ios::end);
    encryptedMessage.resize(inputFile.tellg());
    inputFile.seekg(0, std::ios::beg);
    inputFile.read(&encryptedMessage[0], encryptedMessage.size());
    inputFile.close();

    std::string key;
    std::cout << "Enter the AES key (must be 16, 24, or 32 characters long): ";
    std::getline(std::cin, key);

    std::string decryptedMessage = decryptMessage(encryptedMessage, key);

    std::ofstream outputFile("decrypted.txt");
    if (!outputFile) {
        std::cerr << "Failed to open decrypted.txt for writing" << std::endl;
        return 1;
    }

    outputFile << decryptedMessage;
    outputFile.close();

    std::cout << "Message decrypted successfully. Decrypted text saved in decrypted.txt" << std::endl;

    return 0;
}