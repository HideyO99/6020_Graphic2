#pragma once
#include "../Texture/cTextureFromBMP.h"
class cAI_Map :
    public cTextureFromBMP
{
public:
    cAI_Map();
    ~cAI_Map();
    cAI_Map(std::string fileName);

    bool setColorAt(C24BitBMPpixel pixelColor, unsigned int row, unsigned int column);
    C24BitBMPpixel* getBMPBuffer();

    C24BitBMPpixel* theMapBuff;
    unsigned long numRows;
    unsigned long numColumns;

};

