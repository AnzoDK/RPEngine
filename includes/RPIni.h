#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include <limits>
#include <memory>
#include <any> //cpp 17
#include <variant>
/* Part of RPEngine - Simple InI parser */

namespace rp{
struct KeyValue
{
    public:
        KeyValue(std::string _value, std::string _name)
        {
            value = _value;
            name = _name;
        }
        std::string value;
        std::string name;
    private:
};

struct Key
{
    public:
        Key(std::string _name)
        {
            name = _name;
            subkeys = std::vector<KeyValue>();
        }
        void AddKey(KeyValue key)
        {
            subkeys.push_back(key);
        }
        std::string GetSubKey(std::string keyName)
        {
            for(unsigned int i = 0; i < subkeys.size();i++)
            {
                if(subkeys.at(i).name == keyName)
                {
                    return subkeys.at(i).value;
                    break;
                }
            }
            return std::to_string(-2);
        }
        std::string name;
    private:
        std::vector<KeyValue> subkeys;
};

struct Ini
{
    public:
        Ini(std::string _path)
        {
            path = _path;
            keys = std::vector<Key*>();
            Parse();
        }
        void Parse()
        {
          std::ifstream in = std::ifstream(path);
          in.ignore( std::numeric_limits<std::streamsize>::max() );
          std::streamsize length = in.gcount();
          in.clear();
          in.seekg( 0, std::ios_base::beg );
          
          char* buffer = new char[length];
          in.read(buffer,length);
          in.close();
          
          bool processed = 0;
          bool readAsKey = 0;
          Key* latestKey = nullptr;
          int counter = 0;
          int start = -1;
          int end = -1;
          bool onComment = 0;
          int commentStart = -1;
          while(!processed)
          {
            if(buffer[counter] == 0x3B)
            {
                onComment = 1;
                commentStart = counter;
                //end = counter;
            }
            if(onComment && buffer[counter] == 0x0a)
            {
                end = counter;
            }
            if(readAsKey && !onComment)
            {
                //Definently a strange way to do this - Wastes a lot of cycles
                if(buffer[counter] == 0x0a && counter != end+1 && buffer[counter+1] != 0x3B && !onComment)
                {
                    int newLen = counter - end;
                    if(commentStart != -1)
                    {
                        newLen -= (counter - commentStart);
                        commentStart = -1;
                    }
                    char* buff = new char[newLen];
                    int equal = 0;
                    for(int i = 0; i < newLen; i++)
                    {
                        buff[i] = buffer[end+i];
                        if(buff[i] == 0x3D)
                        {
                            equal = i;
                        }
                    }
                    std::string valName = "";
                    for(int i = 0; i < equal-2;i++)
                    {
                        valName += buff[i+2];
                    }
                    std::string valString = "";
                    for(int i = equal; i < newLen-1;i++)
                    {
                        valString += buff[i+1];
                    }
                    delete[] buff;
                    KeyValue keyval = KeyValue(valString,valName);
                    latestKey->AddKey(keyval);
                    end = counter-1;
                    
                }
            }
            if(buffer[counter] == 0x5b && !onComment)
            {
                start = counter;
                readAsKey = 0;
                end = -1;
            }
            if(buffer[counter] == 0x5D && !onComment)
            {
                end = counter;
            }
            if(start != -1 && end != -1 && !readAsKey)
            {
                int len = end - start;
                std::string keyname = "";
                for(int i = 0; i < len-1; i++)
                {
                    keyname += buffer[start+i+1];
                }
                Key* nKey = new Key(keyname);
                latestKey = nKey;
                keys.push_back(nKey);
                start = -1;
                readAsKey = 1;
            }
            if(counter == length-1)
            {
                processed = 1;
                break;
            }
            if(buffer[counter] == 0x0a && onComment)
            {
                onComment = 0;
                commentStart = -1;
                
            }
            counter++;
          }
          delete[] buffer;
          
        }
        void key(std::string _key)
        {
            
        }
        ~Ini(){keys.clear();}
        Key* GetKey(std::string name)
        {
            for(int i = 0; i < keys.size();i++)
            {
                if(keys.at(i)->name == name)
                {
                    return keys.at(i);
                    break;
                }
            }
            return new Key("Invalid");
        }
    private:
        std::string path;
        std::vector<Key*> keys;
};
};
