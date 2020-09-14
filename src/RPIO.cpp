#include "../includes/RPIO.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <limits>
#include <memory>
#include "../includes/RPEngine.h"
using namespace rp;

RawFile* RPIO::ReadFileRaw(std::string path)
{
    std::ifstream ifs = std::ifstream(path, std::ios::binary);
    ifs.ignore(std::numeric_limits<std::streamsize>::max());  
    std::streamsize length = ifs.gcount();
    ifs.clear();
    ifs.seekg(0, std::ios_base::beg);
    
    if(length <= 0)
    {
        //Error
        //for now we just cout it
        std::cout << "File is less than 0 or 0 bytes in size!" << std::endl;
        char* buffer = new char[5]{'E','R','R','O','R'};
        return new RawFile(buffer,5, path);
    }
    else
    {
        char* buffer = new char[length];
        ifs.read(buffer,length);
        ifs.close();
        return new RawFile(buffer,length, path); 
    }
    

    
}
Scene* RPIO::LoadScene(std::string path)
{
    //Not a good way - Should just have a static logger...
    EngineLogger el = EngineLogger();
    //signed char* cannot find 0xFF. We have to work around it. 
    unsigned char* objectEnd = 0;
    unsigned char* objectStart = 0;
    RawFile* sceneFile = RPIO::ReadFileRaw(path);
    //Check if header is sane
    char* expectedHeader = new char[3]{0x52,0x50,0x53};
    int ver = 0;
    Scene* s = new Scene();
    ver = (ver << sceneFile->buffer[3] << 8 | sceneFile->buffer[4]);
    switch(ver)
    {
        case 1:
        {
            //Default version
            objectEnd = new unsigned char[2]{0x0F,0xF0};
            objectStart = new unsigned char[2]{0xF0,0x0F};
            
            //sanity check
            uint32_t tes1 = 0;
            uint32_t tes2 = 0;
            tes1 = (tes1 << sceneFile->buffer[0] << 16 | sceneFile->buffer[1] << 8 | sceneFile->buffer[2]);
            tes2 = (tes2 << expectedHeader[0] << 16 | sceneFile->buffer[1] << 8 | expectedHeader[2]);
            if(tes1 == tes2)
            {
                el.Log("LoadScene - INFO: File is sane");
                DoubleArray<int>* pairs = new DoubleArray<int>();
                ArrPair<unsigned char*> patterns = ArrPair<unsigned char*>(objectStart,objectEnd);
                LoadPairs(*pairs,sceneFile,patterns);
                std::string name = "";
                UIBase* tmpBase = 0;
                SDL_Rect* tmpRect = 0;
                el.Log("pairs found: " + std::to_string(pairs->length()));
                if(pairs->length() > 0)
                {
                for(int i = 0; i < pairs->length();i++)
                {
                    for(int u = 0; u < pairs->at(i).item2-pairs->at(i).item1;u++)
                    {
                    tmpBase = new UIBase();
                    tmpRect = new SDL_Rect();
                    int nameLengh = 0;
                    std::cout << "Searching, starting at index " << pairs->at(i).item1+u << std::endl;
                    if(sceneFile->buffer[pairs->at(i).item1+u] == (char)0xAA/* && sceneFile.buffer[i+1] == (char)0x01*/)
                    {
                        switch(sceneFile->buffer[pairs->at(i).item1+u+1])
                        {
                            default:
                                
                            break;
                            
                            case (char)0x01:
                                nameLengh = sceneFile->buffer[pairs->at(i).item1+u+2];
                                for(int c = 0; c < nameLengh;c++)
                                {
                                    name += sceneFile->buffer[pairs->at(i).item1+u+3+c];
                                }
                                tmpBase->SetName(name);
                            break;
                            
                            case (char)0x02:
                                    tmpRect->x = (sceneFile->buffer[pairs->at(i).item1+u+2] << 8 | sceneFile->buffer[pairs->at(i).item1+u+3]);
                            break;
                            
                            case (char)0x03:
                                    tmpRect->y = (sceneFile->buffer[pairs->at(i).item1+u+2] << 8 | sceneFile->buffer[pairs->at(i).item1+u+3]);
                            break;
                            case (char)0x04:
                                    tmpRect->w = (sceneFile->buffer[pairs->at(i).item1+u+2] << 8 | sceneFile->buffer[pairs->at(i).item1+u+3]);
                            break;
                            
                            case (char)0x05:
                                    tmpRect->h = (sceneFile->buffer[pairs->at(i).item1+u+2] << 8 | sceneFile->buffer[pairs->at(i).item1+u+3]);
                            break;
                            
                            case (char)0x06:
                                    std::string uiPath = "";
                                    for(int c = 0; c < sceneFile->buffer[pairs->at(i).item1+u+2];c++)
                                    {
                                        uiPath += sceneFile->buffer[pairs->at(i).item1+u+3+c];
                                    }
                                    tmpBase->SetGraphic(new UIGraphic(uiPath));
                            break;
                        }
                    }
                }
                s->AddObject(tmpBase);
                }
                
            }
                else
                {
                    el.Log("No pairs was found - Scenefile is corrupted or the IO controller encountered a read error");
                    UIBase* b = new UIBase();
                    b->SetName("Scene Load Error");
                    SDL_Rect* r = new SDL_Rect();
                    r->w = 300;
                    r->h = 300;
                    b->SetRect(r);
                    b->SetGraphic("Resources/textures/error.png");
                    s->AddObject(b);
                }
            }
            else
            {
               el.Log("LoadScene - ERROR: File is insane(unexpected value in header)"); 
            }
        break;
        }
        case 2:
            
        break;
        
        default:
            el.Log("LoadScene - Error: Scene file version unexpected, corrupted file?");
        break;
    }
    if(objectEnd != 0)
    {
        delete[] objectEnd;
        delete[] objectStart;
    }
    return s;
}
void RPIO::LoadPairs(DoubleArray<int>& dvec, RawFile* file, ArrPair<unsigned char*> filePatterns)
{
    int objStart = -1;
    int objEnd = -1;
    for(int i = 0; i < file->length-1;i++)
    {
        char* workBytes = new char[2];
        workBytes[0] = file->buffer[i];
        workBytes[1] = file->buffer[i+1];
        unsigned char* compArr = CppTools::CharArrToUnsignedArr(workBytes,2);
        if(CTools::compareArray<unsigned char*>(&compArr,&filePatterns.item1,2))
        {
            objStart = i;
        }
        else if(CTools::compareArray<unsigned char*>(&compArr,&filePatterns.item2,2))
        {
            objEnd = i;
        }
        if(objStart != -1 && objEnd != -1)
        {
            dvec.Add(objStart,objEnd);
            objStart = -1;
            objEnd = -1;
        }
        delete[] workBytes;
        delete[] compArr;
        
    }
}
