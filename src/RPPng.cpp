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
    /*std::ifstream file = std::ifstream(path,std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    bufferSize = size;
    file.seekg(0, std::ios::beg);
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
        //GenerateChunkVector();
        valid = 1;
    }
    else
    {
      //std::cout << "Invalid PNG File Loading Requested! - Setting Valid to 0" << std::endl;
      delete[] buffer;
      valid = 0;
      return;
    }*/
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
    //The chunk "length" property is not counting itself - thus we add 4 bytes to our offeset in the end - When indexing for chunks like IHDR, we need to
    //Remember that it is the 4 bytes before the chunktype i.e IHDR
    int extraOffset = 4;
    int headerOffset = 8;
    PngChunk* tmpChunk;
    bool scanlineFilterByte = 0;
    int IHDROffset,IDATOffset,IENDOffset;
    unsigned int IHDRLength,IDATLength,IENDLength;
            uint8_t* IHDR = new uint8_t[4];
            IHDR[0] = 0x49;
            IHDR[1] = 0x48;
            IHDR[2] = 0x44;
            IHDR[3] = 0x52;
            
            uint8_t* IDAT = new uint8_t[4];
            IDAT[0] = 0x49;
            IDAT[1] = 0x44;
            IDAT[2] = 0x41;
            IDAT[3] = 0x54;
            
            uint8_t* IEND = new uint8_t[4];
            IEND[0] = 0x49;
            IEND[1] = 0x45;
            IEND[2] = 0x4E;
            IEND[3] = 0x44;
            //First chunk must be the IHDR chunk - FIND IT!
            
            IHDROffset = LocateChunk(IHDR);
            IDATOffset = LocateChunk(IDAT);
            IENDOffset = LocateChunk(IEND);
            
            
            delete[] IHDR;
            delete[] IDAT;
            delete[] IEND;
            
            if(IHDROffset != 0 && IDATOffset != 0 && IENDOffset != 0)
            {
                //Valid PNG file !!!
                
                //The byte offset is due to our memory address for the IHDR IDAT and IEND all being at the I - Thus to get the data after the letters we have to offset it by 4, which is the amount of bytes making up the chunktype
                int byteOffset = 4;
                IHDRAnalyze(IHDROffset+byteOffset);
            }
            else
            {
               //Invalid File
               valid = 0;
               return; 
            }
            //Locate length
            uint8_t* IHDRLengthBytes = new uint8_t[4];
            uint8_t* IDATLengthBytes = new uint8_t[4];
            uint8_t* IENDLengthBytes = new uint8_t[4];
            for(int i = 0; i < 4;i++)
            {
                IHDRLengthBytes[i] = buffer[IHDROffset-4+i];
                IDATLengthBytes[i] = buffer[IDATOffset-4+i];
                IENDLengthBytes[i] = buffer[IENDOffset-4+i];
            }
            IHDRLength = (IHDRLengthBytes[0] << 24) | (IHDRLengthBytes[1] << 16) | (IHDRLengthBytes[2] << 8) | IHDRLengthBytes[3];
            IDATLength = (IDATLengthBytes[0] << 24) | (IDATLengthBytes[1] << 16) | (IDATLengthBytes[2] << 8) | IDATLengthBytes[3];
            IENDLength = (IENDLengthBytes[0] << 24) | (IENDLengthBytes[1] << 16) | (IENDLengthBytes[2] << 8) | IENDLengthBytes[3];
            delete[] IHDRLengthBytes;
            delete[] IDATLengthBytes;
            delete[] IENDLengthBytes;
            //Locate ChunkType
            uint8_t* IDATDataBytes = new uint8_t[IDATLength];
            for(int i = 0; i < IDATLength;i++)
            {
                IDATDataBytes[i] = buffer[IDATOffset+4+i];
            }
            
            
}
int PngFile::LocateChunk(uint8_t* bytes)
{
    bool found = 0;
    int foundOffset = 0;
    unsigned long int c = 0;

    while(!found)
    {
        if(c > bufferSize)
        {
            //could not be found
            break;
        }
        uint8_t* tmpData = new uint8_t[4];
        for(unsigned int i = 0; i < 4; i++)
        {
            tmpData[i] = buffer[i+c];
        }
        if(CTools::compareArray(bytes,tmpData,4))
        {
            found = 1;
            foundOffset = c;
        }
        c++;
        delete[] tmpData;
    }
    return foundOffset;
}
void PngFile::IHDRAnalyze(int offset)
{
    uint8_t* widthBytes = new uint8_t[4];
    uint8_t* heightBytes = new uint8_t[4];
    uint8_t bitDepthByte = buffer[offset+8];
    uint8_t colorTypehByte = buffer[offset+9];
    uint8_t compressionByte = buffer[offset+10];
    uint8_t filterByte = buffer[offset+11];
    uint8_t interlaceByte = buffer[offset+12];
    
    for(int i = 0; i < 4;i++)
    {
        widthBytes[i] = buffer[offset+i];
        heightBytes[i] = buffer[offset+4+i];
    }
    
    unsigned int _width = 0;
    unsigned int _height = 0;
    _width = (widthBytes[0] << 24) | (widthBytes[1] << 16) | (widthBytes[2] << 8) | widthBytes[3];
    _height = (heightBytes[0] << 24) | (heightBytes[1] << 16) | (heightBytes[2] << 8) | heightBytes[3];
    width = _width;
    height = _height;
    
    bitdepth = bitDepthByte;
    colorType = colorTypehByte;
    compressionMethod = compressionByte;
    filterMethod = filterByte;
    interlaceMethod = interlaceByte;
    delete[] widthBytes;
    delete[] heightBytes;
}
uint8_t* PngFile::GetBuffer()
{
    return buffer;
}
int PngFile::AlphaCalc(int alpha, int background, int foreground)
{
    return alpha * foreground + (1-alpha) * background;
}
void PngFile::DecodeScanlines(int IDAToffset, int IDATLength)
{
    //As of now there is only one fitler mode (mode 0) - Therefore we just set scanlineFilterByte to 1
    int scanlineFilterByte = 1;
    int scanlineSize = 0;
    int maxAlpha = 0;
    switch(filterMethod)
    {
        case 0:
            //none
        break;
    }
    switch(interlaceMethod)
    {
        case 0:
            //Not interlaced
        break;
    }
    switch(colorType)
    {
        default:
            //Undefined
        break;
                
        case 0:
            //Greyscale sample
        break;
                
        case 2:
            //Normal RGB triple
        break;
                
        case 3:
            //PLTE Index
        break;
                
        case 4:
            //Greyscale sample followed by alpha sample
            maxAlpha = 2^bitdepth;
        break;
                
        case 6:
            //RGB triple followed by alpha sample
            maxAlpha = 2^bitdepth;
        break;
    }
    switch(compressionMethod)
    {
        default:
            //Undefined
        break;
                
        case 0:
            //zlib deflation
                    
        break;
    }
    scanlineSize = width*bitdepth;
}
std::string PngFile::GetPath()
{
    return path;
}






