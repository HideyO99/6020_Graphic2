#include "cTextureManager.h"

bool cTextureManager::create2DTextureFromBMP(std::string textureFileName, bool genMIPMap)
{
	std::string fileToLoadFullPath = this->m_basePath + "/" + textureFileName;


	cTextureFromBMP* pTempTexture = new cTextureFromBMP();
	if (!pTempTexture->createNewTextureFromBMPFile2(textureFileName, fileToLoadFullPath, genMIPMap))
	{
		this->m_appendErrorString("Can't load ");
		this->m_appendErrorString(fileToLoadFullPath);
		this->m_appendErrorString("\n");
		return false;
	}

	// Texture is loaded OK

	this->m_map_TexNameToTexture[textureFileName] = pTempTexture;

	return true;
}

std::string cTextureManager::PickRandomTexture()
{
	if (this->m_map_TexNameToTexture.empty())
	{
		// There are no textures loaded, yet.
		return "";
	}

	int textureIndexRand = rand() % (this->m_map_TexNameToTexture.size() + 1);
	if (textureIndexRand >= this->m_map_TexNameToTexture.size())
	{
		textureIndexRand = 0;
	}

	std::map< std::string, cTextureFromBMP* >::iterator itTex = this->m_map_TexNameToTexture.begin();
	for (unsigned int count = 0; count != textureIndexRand; count++, itTex++)
	{
	}

	return itTex->second->getTextureName();
}

bool cTextureManager::createCubeTextureFromBMP(std::string cubeMapName, std::string posX_filename, std::string negX_filename, std::string posY_filename, std::string negY_filename, std::string posZ_filename, std::string negZ_filename, bool isSeamless, std::string& error)
{
	std::string posX_fileName_FullPath = this->m_basePath + "/" + posX_filename;
	std::string negX_fileName_FullPath = this->m_basePath + "/" + negX_filename;
	std::string posY_fileName_FullPath = this->m_basePath + "/" + posY_filename;
	std::string negY_fileName_FullPath = this->m_basePath + "/" + negY_filename;
	std::string posZ_fileName_FullPath = this->m_basePath + "/" + posZ_filename;
	std::string negZ_fileName_FullPath = this->m_basePath + "/" + negZ_filename;

	GLenum errorEnum;
	std::string errorDetails;
	cTextureFromBMP* pTempTexture = new cTextureFromBMP();
	if (!pTempTexture->createNewCubeTextureFromBMPFiles(
		cubeMapName,
		posX_fileName_FullPath, negX_fileName_FullPath,
		posY_fileName_FullPath, negY_fileName_FullPath,
		posZ_fileName_FullPath, negZ_fileName_FullPath,
		isSeamless, errorEnum, error, errorDetails))
	{
		this->m_appendErrorString("Can't load ");
		this->m_appendErrorString(cubeMapName);
		this->m_appendErrorString(" because:\n");
		this->m_appendErrorString(error);
		this->m_appendErrorString("\n");
		this->m_appendErrorString(errorDetails);
		error += ("\n" + errorDetails);
		return false;
	}//if ( ! pTempTexture->CreateNewCubeTextureFromBMPFiles()

	// Texture is loaded OK
	//this->m_nextTextureUnitOffset++;

	this->m_map_TexNameToTexture[cubeMapName] = pTempTexture;

	return true;
}

bool cTextureManager::createCubeTextureFromFreeImgLib(std::string cubeMapName, std::string posX_filename, std::string negX_filename, std::string posY_filename, std::string negY_filename, std::string posZ_filename, std::string negZ_filename, bool isSeamless, std::string& error)
{
	std::string posX_fileName_FullPath = this->m_basePath + "/" + posX_filename;
	std::string negX_fileName_FullPath = this->m_basePath + "/" + negX_filename;
	std::string posY_fileName_FullPath = this->m_basePath + "/" + posY_filename;
	std::string negY_fileName_FullPath = this->m_basePath + "/" + negY_filename;
	std::string posZ_fileName_FullPath = this->m_basePath + "/" + posZ_filename;
	std::string negZ_fileName_FullPath = this->m_basePath + "/" + negZ_filename;

	cTextureFromFile* pTempTexture = new cTextureFromFile();
	bool result = pTempTexture->createNewCubeTextureFromBMPFiles(cubeMapName,
		posX_fileName_FullPath, negX_fileName_FullPath,
		posY_fileName_FullPath, negY_fileName_FullPath,
		posZ_fileName_FullPath, negZ_fileName_FullPath,
		isSeamless);

	if (result)
	{
		this->m_map_TexNameToTextureID[cubeMapName] = pTempTexture;
		return true;
	}

	return false;
}

GLuint cTextureManager::getTexttureID(std::string textureFileName)
{
	std::map< std::string, cTextureFromFile* >::iterator itTexture
		= this->m_map_TexNameToTextureID.find(textureFileName);
	// Found it?
	if (itTexture == this->m_map_TexNameToTextureID.end())
	{
		return 0;
	}
	// Reutrn texture number (from OpenGL genTexture)
	return itTexture->second->getTextureNumber();
}

GLuint cTextureManager::getTexttureID_BMP(std::string textureFileName)
{
	std::map< std::string, cTextureFromFile* >::iterator itTexture
		= this->m_map_TexNameToTextureID.find(textureFileName);
	// Found it?
	if (itTexture == this->m_map_TexNameToTextureID.end())
	{
		return 0;
	}
	// Reutrn texture number (from OpenGL genTexture)
	return itTexture->second->getTextureNumber();
}

void cTextureManager::setBasePath(std::string basePath)
{
	this->m_basePath = basePath;
}

bool cTextureManager::create2DTextureFromFreeImgLib(std::string textureFileName, bool genMIPMap)
{
	std::string fileToLoadFullPath = this->m_basePath + "/" + textureFileName;

	cTextureFromFile* pTempTexture = new cTextureFromFile();
	bool result = pTempTexture->createNewTextureFromFile(textureFileName, fileToLoadFullPath, genMIPMap);
	if (result)
	{
		this->m_map_TexNameToTextureID[textureFileName] = pTempTexture;
		return true;
	}
	return true;
}

bool cTextureManager::isNotExistTexture(std::string texName)
{
	std::map< std::string, cTextureFromFile* >::iterator it = m_map_TexNameToTextureID.find(texName);
	if (it == m_map_TexNameToTextureID.end())
	{
		return true;
	}
	return false;
}

void cTextureManager::m_appendErrorString(std::string nextErrorText)
{
	std::stringstream ss;
	ss << this->m_lastError << nextErrorText;
	this->m_lastError = ss.str();
}

void cTextureManager::m_appendErrorStringLine(std::string nextErrorTextLine)
{
	std::stringstream ss;
	ss << this->m_lastError << std::endl;
	ss << nextErrorTextLine << std::endl;
	this->m_lastError = ss.str();
}
