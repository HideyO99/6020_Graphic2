#pragma once
#include "../globalOpenGL.h"
#include <string>
#include <FreeImage/FreeImage.h>


class cTextureFromFile
{
public:
	cTextureFromFile();
	~cTextureFromFile();

	bool createNewTextureFromFile(std::string fileName, std::string fileNameFullPath, bool bGenerateMIPMap);
	GLuint getTextureNumber(void);

	bool createNewCubeTextureFromBMPFiles(std::string cubeMapName, std::string posX_fileName, std::string negX_fileName, std::string posY_fileName, std::string negY_fileName, std::string posZ_fileName, std::string negZ_fileName, bool bIsSeamless);
	void clearLoadImg(FIBITMAP* buff);
private:
	FIBITMAP* loadEachCube(std::string file);
	GLuint m_textureNumber;				// The texture number
	std::string m_textureName;
	std::string m_fileNameFullPath;
	bool m_bIsCubeMap;
	unsigned long	m_numberOfColumns;
	unsigned long	m_numberOfRows;
	BYTE* m_ImgData;
};
