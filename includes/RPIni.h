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
enum keyType {String=0,Int=1};
template<typename T>
struct KeyValue
{
    public:
        KeyValue(T _value, std::string _name,keyType _t)
        {
            value = _value;
            name = _name;
            t = _t;
        }
        void GetValue(void* ptr)
        {
            &ptr = value;
        }
        T value;
        std::string name;
    private:
        keyType t;
};

struct Key
{
    public:
        Key(std::string _name)
        {
            name = _name;
            stringsubkeys = std::vector<KeyValue<std::string>>();
            intsubkeys = std::vector<KeyValue<int>>();
        }
        void AddKey(KeyValue<int> key)
        {
            intsubkeys.push_back(key);
        }
        void AddKey(KeyValue<std::string> key)
        {
            stringsubkeys.push_back(key);
        }
        std::string GetSubKey(std::string keyName)
        {
            for(int i = 0; i < intsubkeys.size();i++)
            {
                if(intsubkeys.at(i).name == keyName)
                {
                    return std::to_string(intsubkeys.at(i).value);
                }
            }
            for(int i = 0; i < stringsubkeys.size();i++)
            {
                if(stringsubkeys.at(i).name == keyName)
                {
                    return stringsubkeys.at(i).value;
                }
            }
            return std::to_string(-2);
        }
        std::string name;
    private:
        std::vector<KeyValue<int>> intsubkeys;
        std::vector<KeyValue<std::string>> stringsubkeys;
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
          int start = 0;
          int end = 0;
          while(!processed)
          {
            
            if(readAsKey)
            {
                if(buffer[counter] == 0x0a)
                {
                    int newLen = counter - end;
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
                    for(int i = 0; i < equal-1;i++)
                    {
                        valName += buff[i];
                    }
                    std::string valString = "";
                    for(int i = equal; i < newLen;i++)
                    {
                        valString += buff[i];
                    }
                    delete[] buff;
                    char* val;
                    strtol(valString.c_str(),&val,10);
                    if(*val)
                    {
                       KeyValue<std::string> keyval = KeyValue<std::string>(valString,valName,keyType::String);
                       latestKey->AddKey(keyval);
                    }
                    else
                    {
                        KeyValue<int> keyval = KeyValue<int>(atoi(valString.c_str()),valName,keyType::Int);
                        latestKey->AddKey(keyval);
                    }
                    
                }
            }
            if(buffer[counter] == '[')
            {
                start = buffer[counter];
                readAsKey = 0;
            }
            if(buffer[counter] == ']')
            {
                end = counter;
            }
            if(start != 0 && end != 0)
            {
                int len = end - start;
                std::string keyname = "";
                for(int i = 0; i < len; i++)
                {
                    keyname += buffer[start+i];
                }
                Key* nKey = new Key(keyname);
                latestKey = nKey;
                keys.push_back(nKey);
                start = 0;
                end++;
                readAsKey = 1;
                counter++;
            }
            if(counter == length-1)
            {
                processed = 1;
                break;
            }
            counter++;
          }
          
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
            return nullptr;
        }
    private:
        std::string path;
        std::vector<Key*> keys;
};
};
