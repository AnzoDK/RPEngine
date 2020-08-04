#include "../includes/RPPng.h"

PngChunk::PngChunk(unsigned int _length, uint8_t* type, uint8_t* data, uint8_t* _CRC)
{
    length = _length;
    chunkType = type;
    chunkData = data;
    CRC = _CRC;
    
    
}
PngChunk::~PngChunk()
{
  delete[] chunkType;
  delete[] chunkData;
  delete[] CRC;
}


PngFile::PngFile(std::string _path)
{
    chunks = std::vector<PngChunk*>();
    path = _path;
    std::ifstream file = std::ifstream(path,std::ios::binary | std::ios::ate);
    bufferSize = file.tellg();
    char* ch_buffer = new char[bufferSize];
    buffer = new uint8_t[bufferSize];
    file.read(ch_buffer,bufferSize);
    file.close();
    for(int i = 0; i < bufferSize;i++)
    {
        buffer[i] = ch_buffer[i];
    }
    delete[] ch_buffer;
    if(SanityCheck())
    {
        //File is valid PNG
        valid = 1;
        GenerateChunkVector();
    }
    else
    {
      //std::cout << "Invalid PNG File Loading Requested! - Setting Valid to 0" << std::endl;
      delete[] buffer;
      valid = 0;
      return;
    }
}
PngFile::~PngFile()
{
    for(unsigned int i = 0; i < chunks.size();i++)
    {
        delete(chunks.at(i));
    }
}


int PngFile::IsValid()
{
    return valid;
}
int PngFile::SanityCheck()
{
    char* expectedHeader = new char[8];
    expectedHeader[0] = 0x89;
    expectedHeader[1] = 0x50;
    expectedHeader[2] = 0x4E;
    expectedHeader[3] = 0x47;
    expectedHeader[4] = 0x0D;
    expectedHeader[5] = 0x0A;
    expectedHeader[6] = 0x1A;
    expectedHeader[7] = 0x0A;
    char* header = new char[8];
    for(int i = 0;i < 8;i++)
    {
        header[i] = buffer[i];
    }
    if(CTools::compareArray(expectedHeader,header,8))
    {
        return 1;
    }
    return 0;
}
void PngFile::GenerateChunkVector()
{
    //We have to keep track of the 3 critical chunks IHDR - IDAT - IEND
    bool done = 0;
    //The chunk "length" property is not counting itself - thus we add 4 bytes to our offeset in the end
    int extraOffset = 4;
    int headerOffset = 8;
    PngChunk* tmpChunk;
    int counter = 0;
    int newChunkOffset = 0;
    while(!done)
    {
        if(counter == 0)
        {
            uint8_t* IHDR = new uint8_t[4];
            IHDR[0] = 0x49;
            IHDR[1] = 0x48;
            IHDR[2] = 0x44;
            IHDR[3] = 0x52;
            //First chunk must be the IHDR chunk - FIND IT!
            bool found = 0;
            int foundOffset = 0;
            int c = 0;
            while(!found)
            {
                uint8_t* tmpData = new uint8_t[4];
                for(unsigned int i = 0; i < 4; i++)
                {
                    tmpData[i] = buffer[i+c];
                }
                if(CTools::compareArray(IHDR,tmpData,4))
                {
                  found = 1;
                  foundOffset = c;
                }
              c++;
              delete[] tmpData;
            }
            delete[] IHDR;
        }
        //Locate length
        uint8_t* lengthBytes = new uint8_t[4];
        for(int i = 0; i < 4;i++)
        {
            lengthBytes[i] = buffer[newChunkOffset+i];
        }
        unsigned int length = 0;
        length = (lengthBytes[0] << 24) | (lengthBytes[1] << 16) | (lengthBytes[2] << 8) | lengthBytes[3];
        delete[] lengthBytes;
        //Locate ChunkType
        uint8_t* chunkTypeBytes = new uint8_t[4];
        uint8_t* chunkDataBytes = new uint8_t[length];
        uint8_t* CRCBytes = new uint8_t[4];
        for(int i = 0; i < 4;i++)
        {
            chunkTypeBytes[i] = buffer[newChunkOffset+i+extraOffset];
        }
        int ChunkDataOffeset = newChunkOffset+4+extraOffset;
        for(int i = 0; i < length;i++)
        {
            chunkTypeBytes[i] = buffer[ChunkDataOffeset+i];
        }
        int CRCOffset = ChunkDataOffeset+length;
        for(int i = 0; i < 4;i++)
        {
            chunkTypeBytes[i] = buffer[CRCOffset+i];
        }
        newChunkOffset = CRCOffset+4;
        tmpChunk = new PngChunk(length, chunkTypeBytes, chunkDataBytes,CRCBytes);
        chunks.push_back(tmpChunk);
        
        
        
        counter++;
        
    }
}



