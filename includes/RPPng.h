#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "RPCtools.h"

//TODO Rethink Chunks - https://vivaxyblog.github.io/2019/11/07/decode-a-png-image-with-javascript.html

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
        //uint8_t* GetBuffer();
        std::string GetPath();
    private:
        //void DecodeScanlines(int idatoffset, int idatlength);
        //int AlphaCalc(int alpha, int background, int foreground);
        //void GenerateChunkVector();
        //int LocateChunk(uint8_t* bytes);
        //void IHDRAnalyze(int offset);
        //uint8_t* buffer;
        std::string path;
        //long unsigned int bufferSize;
        int SanityCheck();
        bool valid = 0;
        std::vector<PngChunk*> chunks;
        //int bitdepth;
        //int width;
        //int height;
        //uint8_t colorType;
        //uint8_t compressionMethod;
        //uint8_t filterMethod;
        //uint8_t interlaceMethod;
        
};
