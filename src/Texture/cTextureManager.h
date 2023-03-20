#pragma once

#include <string>
#include <map>
#include <sstream>
#include "../globalOpenGL.h"
#include "cTextureFromBMP.h"
#include "cTextureFromFile.h"



class cTextureManager
{
public:
	bool create2DTextureFromBMP(std::string textureFileName, bool genMIPMap = true);

	std::string PickRandomTexture();

	bool createCubeTextureFromBMP(std::string cubeMapName,
		std::string posX_filename, std::string negX_filename,
		std::string posY_filename, std::string negY_filename,
		std::string posZ_filename, std::string negZ_filename,
		bool isSeamless, std::string& error);

	bool createCubeTextureFromFreeImgLib(std::string cubeMapName,
		std::string posX_filename, std::string negX_filename,
		std::string posY_filename, std::string negY_filename,
		std::string posZ_filename, std::string negZ_filename,
		bool isSeamless, std::string& error);

	GLuint getTexttureID(std::string textureFileName);
	GLuint getTexttureID_BMP(std::string textureFileName);
	void setBasePath(std::string basePath);

	bool create2DTextureFromFreeImgLib(std::string textureFileName, bool genMIPMap = true);
	bool isExistTexture(std::string texName);

private:
	std::string m_basePath;
	std::string m_lastError;
	void m_appendErrorString(std::string nextErrorText);
	void m_appendErrorStringLine(std::string nextErrorTextLine);

	std::map< std::string, cTextureFromBMP* > m_map_TexNameToTexture;
	std::map< std::string, cTextureFromFile* > m_map_TexNameToTextureID;
};

