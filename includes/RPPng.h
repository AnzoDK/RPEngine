#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "RPCtools.h"

struct PngChunk
{
  PngChunk(unsigned int _length, uint8_t* type, uint8_t* data, uint8_t* _CRC);
  ~PngChunk();
  unsigned int length;
  uint8_t* chunkType;
  uint8_t* chunkData;
  uint8_t* CRC;
  
  
};

class PngFile
{
    public:
        PngFile(std::string path);
        ~PngFile();
        int IsValid();
        
    private:
        void GenerateChunkVector();
        uint8_t* buffer;
        std::string path;
        long unsigned int bufferSize;
        int SanityCheck();
        bool valid = 0;
        std::vector<PngChunk*> chunks;
        int bitdepth;
        int width;
        int height;
        uint8_t colorType;
        uint8_t compressionMethod;
        uint8_t filterMethod;
        uint8_t interlaceMethod;
        
};
