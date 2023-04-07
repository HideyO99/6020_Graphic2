#include "cTextureFromFile.h"

cTextureFromFile::cTextureFromFile()
{
	m_bIsCubeMap = false;
}

cTextureFromFile::~cTextureFromFile()
{
}

bool cTextureFromFile::createNewTextureFromFile(std::string fileName, std::string fileNameFullPath, bool bGenerateMIPMap)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileNameFullPath.c_str(), 0);
	FIBITMAP* bitmap = FreeImage_Load(format, fileNameFullPath.c_str(), 0);
	FIBITMAP* bitmap32 = FreeImage_ConvertTo32Bits(bitmap);

	// Get the image dimensions
	int width = FreeImage_GetWidth(bitmap32);
	int height = FreeImage_GetHeight(bitmap32);

	// Get a pointer to the image data
	BYTE* data = FreeImage_GetBits(bitmap32);

	this->m_fileNameFullPath = fileNameFullPath;
	this->m_textureName = fileName;

	this->m_textureNumber = 0;
	glGenTextures(1, &(this->m_textureNumber));
	glBindTexture(GL_TEXTURE_2D, this->m_textureNumber);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)data);

	// Free the FreeImage bitmaps
	FreeImage_Unload(bitmap);
	FreeImage_Unload(bitmap32);

	if (bGenerateMIPMap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);		// OpenGL 4.0
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	return true;
}

GLuint cTextureFromFile::getTextureNumber(void)
{
	return this->m_textureNumber;
}

bool cTextureFromFile::createNewCubeTextureFromBMPFiles(std::string cubeMapName, std::string posX_fileName, std::string negX_fileName, std::string posY_fileName, std::string negY_fileName, std::string posZ_fileName, std::string negZ_fileName, bool bIsSeamless)
{
	bool bReturnVal = true;



	// Pick a texture number...
	glGenTextures(1, &(this->m_textureNumber));
	// Worked?
	//if ((glGetError() & GL_INVALID_VALUE) == GL_INVALID_VALUE)
	//{
	//	bReturnVal = false;
	//	return false;
	//}

	glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_textureNumber);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE /*GL_REPEAT*/);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE /*GL_REPEAT*/);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE /*GL_REPEAT*/);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// IF YOU DON'T GENERATE MIP MAPS, then you can use LINEAR filtering
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

	if (bIsSeamless)
	{
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);	// OpenGL 4.1, maybe
	}


	// Positive X image...
	// Assume all the images are the same size. If not, then it will screw up
	{
		FIBITMAP* buff = this->loadEachCube(posX_fileName);
		glTexStorage2D(GL_TEXTURE_CUBE_MAP,
			10, // Mipmap levels
			GL_RGBA8,	// Internal format
			this->m_numberOfColumns,	// width (pixels)
			this->m_numberOfRows);		// height (pixels)


		// Positive X image...
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			0,   // Level
			0, 0, // Offset
			this->m_numberOfColumns,	// width
			this->m_numberOfRows,		// height
			GL_BGRA,
			GL_UNSIGNED_BYTE,
			this->m_ImgData);
		this->clearLoadImg(buff);
	}
	


	// Negative X image...
	{
		FIBITMAP* buff = this->loadEachCube(negX_fileName);

		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, 0, this->m_numberOfColumns, this->m_numberOfRows, GL_BGRA, GL_UNSIGNED_BYTE, this->m_ImgData);
		this->clearLoadImg(buff);

	}


	// Positive Y image...
	{
		FIBITMAP* buff = this->loadEachCube(posY_fileName);

		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, 0, this->m_numberOfColumns, this->m_numberOfRows, GL_BGRA, GL_UNSIGNED_BYTE, this->m_ImgData);
		this->clearLoadImg(buff);
	}

	// Negative Y image...
	{
		FIBITMAP* buff = this->loadEachCube(negY_fileName);
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, 0, this->m_numberOfColumns, this->m_numberOfRows, GL_BGRA, GL_UNSIGNED_BYTE, this->m_ImgData);
		this->clearLoadImg(buff);
	}


	// Positive Z image...
	{
		FIBITMAP* buff = this->loadEachCube(posZ_fileName);
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, 0, this->m_numberOfColumns, this->m_numberOfRows, GL_BGRA, GL_UNSIGNED_BYTE, this->m_ImgData);
		this->clearLoadImg(buff);
	}


	// Negative Z image...
	{
		FIBITMAP* buff = this->loadEachCube(negZ_fileName);
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, 0, this->m_numberOfColumns, this->m_numberOfRows, GL_BGRA, GL_UNSIGNED_BYTE, this->m_ImgData);
		this->clearLoadImg(buff);
	}


	this->m_textureName = cubeMapName;

	this->m_bIsCubeMap = true;

	return bReturnVal;
}

void cTextureFromFile::clearLoadImg(FIBITMAP* buff)
{
	FreeImage_Unload(buff);
}

FIBITMAP* cTextureFromFile::loadEachCube(std::string file)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(file.c_str(), 0);
	FIBITMAP* bitmap = FreeImage_Load(format, file.c_str(), 0);
	FIBITMAP* bitmap32 = FreeImage_ConvertTo32Bits(bitmap);

	// Get the image dimensions
	this->m_numberOfColumns = FreeImage_GetWidth(bitmap32);
	this->m_numberOfRows = FreeImage_GetHeight(bitmap32);

	// Get a pointer to the image data
	this->m_ImgData = FreeImage_GetBits(bitmap32);

	return bitmap32;
}
