#include "cFBO.h"

cFBO::cFBO()
{
	ID = 0;

	vertexMaterialColorID = 0;
	vertexNormalID = 0;
	vertexWorldPositionID = 0;
	vertexSpecularID = 0;
	vertexRefractionID = 0;
	depthTextureID = 0;
	width = -1;
	height = -1;
	
}

cFBO::~cFBO()
{
}

bool cFBO::init(int width, int height, std::string& error)
{
	this->width = width;
	this->height = height;

	GLfloat black[] = { 0.f,0.f,0.f,0.f };

	glCreateFramebuffers(1, &(this->ID));
	glBindFramebuffer(GL_FRAMEBUFFER, this->ID);

	//color buffer(texture)
	glGenTextures(1, &(this->vertexMaterialColorID));
	glBindTexture(GL_TEXTURE_2D, this->vertexMaterialColorID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, this->width, this->height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR,black);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);


	//vertex normal buffer(texture)
	glGenTextures(1, &(this->vertexNormalID));
	glBindTexture(GL_TEXTURE_2D, this->vertexNormalID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, this->width, this->height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, black);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//vertex world position
	glGenTextures(1, &(this->vertexWorldPositionID));
	glBindTexture(GL_TEXTURE_2D, this->vertexWorldPositionID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, this->width, this->height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, black);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//vertex specular
	glGenTextures(1, &(this->vertexSpecularID));
	glBindTexture(GL_TEXTURE_2D, this->vertexSpecularID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, this->width, this->height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, black);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//vertex refraction
	glGenTextures(1, &(this->vertexRefractionID));
	glBindTexture(GL_TEXTURE_2D, this->vertexRefractionID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, this->width, this->height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, black);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//vertex CubeMap
	glGenTextures(1, &(this->vertexCubeMapID));
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->vertexCubeMapID);
	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(targets[i], 0, GL_RGB, 2048, 2048,0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	}
	//glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGBA32F, 2048, 2048);
	//result = glGetError();
	//for (int i = 0; i < 6; i++)
	//{
	//	glTexSubImage2D(targets[i], 0, 0, 0, 2048, 2048, GL_RGBA, GL_UNSIGNED_BYTE, &(this->img));
	//	result = glGetError();
	//}
	//glTexStorage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA32F, 2048, 2048);
	//result = glGetError();
	//glTexStorage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA32F, 2048, 2048);
	//result = glGetError();
	//glTexStorage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA32F, 2048, 2048);
	//result = glGetError();
	//glTexStorage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA32F, 2048, 2048);
	//result = glGetError();
	//glTexStorage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA32F, 2048, 2048);
	//result = glGetError();

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//depth buffer
	glGenTextures(1, &(this->depthTextureID));
	glBindTexture(GL_TEXTURE_2D, this->depthTextureID);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, this->width, this->height);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->vertexMaterialColorID,0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, this->vertexNormalID, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, this->vertexWorldPositionID, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, this->vertexSpecularID, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, this->vertexRefractionID, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, targets[0], this->vertexCubeMapID, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, this->depthTextureID, 0);

	glDrawBuffers(6, draw_bufer);
	//check FrameBuffer status is ready
	bool bFBOReadyStatus = false;
	
	switch (glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{
	case GL_FRAMEBUFFER_COMPLETE:
		bFBOReadyStatus = true;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		error = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
		bFBOReadyStatus = false;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		error = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
		bFBOReadyStatus = false;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		error = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
		bFBOReadyStatus = false;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		error = "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
		bFBOReadyStatus = false;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		error = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
		bFBOReadyStatus = false;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		error = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
		bFBOReadyStatus = false;
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		error = "GL_FRAMEBUFFER_UNSUPPORTED";
		bFBOReadyStatus = false;
		break;
	case GL_FRAMEBUFFER_UNDEFINED:
		error = "GL_FRAMEBUFFER_UNDEFINED";
		bFBOReadyStatus = false;
		break;
	default:
		bFBOReadyStatus = false;
		break;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return bFBOReadyStatus;
}

void cFBO::shutdown()
{
	glDeleteTextures(1, &(this->vertexMaterialColorID));
	glDeleteTextures(1, &(this->vertexNormalID));
	glDeleteTextures(1, &(this->vertexWorldPositionID));
	glDeleteTextures(1, &(this->vertexSpecularID));
	glDeleteTextures(1, &(this->vertexRefractionID));
	glDeleteTextures(1, &(this->vertexCubeMapID));
	glDeleteTextures(1, &(this->depthTextureID));
	glDeleteFramebuffers(1, &(this->ID));
}

bool cFBO::reset(int width, int height, std::string& error)
{
	this->shutdown();
	bool result = this->init(width, height, error);
	return result;
}

void cFBO::clearBuffer(bool clearColor, bool clearDepth)
{
	if (clearColor)
	{
		this->clearColorBuffer(0);
		this->clearColorBuffer(1);
		this->clearColorBuffer(2);
		this->clearColorBuffer(3);
		this->clearColorBuffer(4);
		this->clearColorBuffer(5);
	}
	if (clearDepth)
	{
		this->clearDepthBuffer(0);
	}
	this->clearStencilBuffer();
}

void cFBO::clearColorBuffer(int bufferIndex)
{
	glViewport(0, 0, this->width, this->height);
	GLfloat black[] = { 0.f,0.f,0.f,1.f };
	glClearBufferfv(GL_COLOR, bufferIndex, black);
}

void cFBO::clearDepthBuffer(int bufferIndex)
{
	GLfloat one = 1.f;
	glClearBufferfv(GL_DEPTH, bufferIndex, &one);

	GLfloat vertexWorld[] = { 0.f,0.f,0.f,0.f };
	glClearBufferfv(GL_COLOR, 1, vertexWorld);
}

void cFBO::clearStencilBuffer()
{
	glStencilMask(0xff);

	glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.f, 0);
}

int cFBO::getMaxColorAttachment()
{
	int maxColorAttach = 0;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttach);

	return maxColorAttach;
}

int cFBO::getMaxDrawBuffer()
{
	int maxDrawBuffer = 0;
	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffer);
	return maxDrawBuffer;
}
