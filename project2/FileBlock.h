#ifndef FILEBLOCK_H
#define FILEBLOCK_H

class FileBlock {
private:
    unsigned int id;
    char payload[500];
    int checksum;
    bool occupied;

public:
    FileBlock();
    
    void setData(unsigned int blockId, const char* data);
    void corruptData(const char* data);
    int computeChecksum();
    bool validateChecksum();
    
    unsigned int getId();
    int getChecksum();
    bool isOccupied();
    void clear();
};

#endif