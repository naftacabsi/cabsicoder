#include <iostream>
#include <fstream>
#include <string>
#include <openssl/aes.h>

class Message {
public:
    std::string text;
};

std::string encryptMessage(const Message& message, const std::string& key) {
    AES_KEY aesKey;
    if (AES_set_encrypt_key(reinterpret_cast<const unsigned char*>(key.c_str()), 128, &aesKey) != 0) {
        std::cerr << "Failed to set AES encrypt key" << std::endl;
        return "";
    }

    const size_t blocksize = AES_BLOCK_SIZE;
    size_t encryptedLength = ((message.text.length() + blocksize - 1) / blocksize) * blocksize;
    unsigned char* encryptedText = new unsigned char[encryptedLength];
    memset(encryptedText, 0, encryptedLength);

    AES_encrypt(reinterpret_cast<const unsigned char*>(message.text.c_str()), encryptedText, &aesKey);

    std::string encryptedMessage(reinterpret_cast<char*>(encryptedText), encryptedLength);
    delete[] encryptedText;

    return encryptedMessage;
}

int main() {
    std::string text;
    std::cout << "Enter the text to encrypt: ";
    std::getline(std::cin, text);

    std::string key;
    std::cout << "Enter the AES key (must be 16, 24, or 32 characters long): ";
    std::getline(std::cin, key);

    Message message;
    message.text = text;

    std::string encryptedMessage = encryptMessage(message, key);

    std::ofstream outputFile("encrypted.txt", std::ios::binary);
    if (!outputFile) {
        std::cerr << "Failed to open encrypted.txt for writing" << std::endl;
        return 1;
    }

    outputFile.write(encryptedMessage.c_str(), encryptedMessage.length());
    outputFile.close();

    std::cout << "Message encrypted successfully. Encrypted text saved in encrypted.txt" << std::endl;

    return 0;
}