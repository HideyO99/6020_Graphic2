#pragma once

typedef struct sFloat4 {
	float x, y, z, w;
} sFloat4;

typedef struct sUint4 {
	unsigned int x, y, z, w;
} sUint4;

typedef struct sInt4 {
	int x, y, z, w;
} sInt4;

typedef struct sVertex_RGBA_XYZ_N_UV_T_BiN_Bones
{
	sFloat4 Pos;
	sFloat4 Normal;
	sFloat4 RGBA;
	sFloat4 TexUVx2;
	sFloat4 Tangent;
	sFloat4 BiNormal;
	sFloat4 BoneID;
	sFloat4 BoneWeight;
}sVertex_RGBA_XYZ_N_UV_T_BiN_Bones;