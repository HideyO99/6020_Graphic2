#include "cAI_Map.h"

cAI_Map::cAI_Map()
{
}

cAI_Map::~cAI_Map()
{
}

cAI_Map::cAI_Map(std::string fileName)
{
	LoadBMP2(fileName);
	theMapBuff = m_p_theImages;
	numRows = m_numberOfRows;
	numColumns = m_numberOfColumns;
}

bool cAI_Map::setColorAt(C24BitBMPpixel pixelColor, unsigned int row, unsigned int column)
{
	if (row >= this->m_numberOfRows)
	{
		row = this->m_numberOfRows - 1;
	}
	if (column >= this->m_numberOfColumns)
	{
		column = this->m_numberOfColumns - 1;
	}

	this->m_p_theImages[row * this->m_Width_or_Columns + column] = pixelColor;
	return true;
}

C24BitBMPpixel* cAI_Map::getBMPBuffer()
{
	return m_p_theImages;
}
