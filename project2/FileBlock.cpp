#include "FileBlock.h"
#include <cstring>

FileBlock::FileBlock() {
    id = 0;
    checksum = 0;
    occupied = false;
    for(int i = 0; i < 500; i++) {
        payload[i] = 0;
    }
}

void FileBlock::setData(unsigned int blockId, const char* data) {
    id = blockId;
    occupied = true;
    // zero out the payload first
    for(int i = 0; i < 500; i++) {
        payload[i] = 0;
    }
    // copy the data
    int i = 0;
    while(data[i] != '\0') {
        payload[i] = data[i];
        i++;
    }
    // compute checksum
    checksum = computeChecksum();
}

void FileBlock::corruptData(const char* data) {
    // zero everything
    for(int i = 0; i < 500; i++) {
        payload[i] = 0;
    }
    // copy new data
    int i = 0;
    while(data[i] != '\0') {
        payload[i] = data[i];
        i++;
    }
    // dont recompute checksum
}

/*
CITATION:
Used ChatGPT (chat.openai.com) to understand casting chars to 
unsigned char to avoid negative values affecting the sum. 
The actual implementation and logic structure are my own work.
*/
int FileBlock::computeChecksum() {
    int sum = 0;
    for(int i = 0; i < 500; i++) {
        sum = sum + (int)((unsigned char)payload[i]);
    }
    int result = sum % 500;
    return result;
}

bool FileBlock::validateChecksum() {
    int newChecksum = computeChecksum();
    if(newChecksum == checksum) {
        return true;
    } else {
        return false;
    }
}

unsigned int FileBlock::getId() {
    return id;
}

int FileBlock::getChecksum() {
    return checksum;
}

bool FileBlock::isOccupied() {
    return occupied;
}

void FileBlock::clear() {
    id = 0;
    checksum = 0;
    occupied = false;
    for(int i = 0; i < 500; i++) {
        payload[i] = 0;
    }
}