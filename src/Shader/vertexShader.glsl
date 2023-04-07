#version 420

layout (location = 0) in vec4 vPosition ;
layout (location = 1) in vec4 vUVx2;
layout (location = 2) in vec4 vNormal ;			
layout (location = 3) in vec4 vBoneID;
layout (location = 4) in vec4 vBoneWeight;
layout (location = 5) in vec4 vTangent;
layout (location = 6) in vec4 vBiNormal;
layout (location = 7) in vec4 vColour ;

// Going "out" to the fragment shader
out vec4 fColor;		
out vec4 fNormal;
out vec4 fVertWorldLocation;	// using only matModel
out vec4 fUVx2;
out vec4 fTangent;
out vec4 fBinormal;

//uniform
uniform mat4 mModel;
uniform mat4 mModelInverseTranspose;		// mModel with Only Rotation;
uniform mat4 mView;
uniform mat4 mProjection;

//moving texture
uniform float time;
uniform bool bMovingTexture;

uniform mat4 BoneMatrices[30];
uniform mat4 BoneRotationMatrices[30];
uniform bool hasBone;

void main()
{
    vec3 vertPosition = vPosition.xyz;
    //vec3 vertNormal = vNormal.xyz;
    mat4 mMVP = mProjection * mView * mModel;

    if(hasBone)
    {
        mat4 boneTransform = BoneMatrices[int(vBoneID[0])] * vBoneWeight[0];
	    boneTransform += BoneMatrices[int(vBoneID[1])] * vBoneWeight[1];
	    boneTransform += BoneMatrices[int(vBoneID[2])] * vBoneWeight[2];
	    boneTransform += BoneMatrices[int(vBoneID[3])] * vBoneWeight[3];
	    vec4 position = boneTransform * vPosition;
        gl_Position = mMVP * position; 
        fColor = vec4(vBoneID[0],0,0,1.f);
    }
    else
    {
        gl_Position = mMVP * vec4(vertPosition, 1.0); 
        fColor = vColour;
    }
    fVertWorldLocation.xyz = (mModel * vec4(vertPosition, 1.0f)).xyz;
	fVertWorldLocation.w = 1.0f;

    fNormal.xyz = normalize(mModelInverseTranspose * vec4(vNormal.xyz, 1.0f)).xyz;
	fNormal.w = 1.0f;
    //fNormal = vec4(vNormal.xyz, 1.0);
    //fColor = vColour;
    fUVx2 = vUVx2;
    fTangent = vTangent;
    fBinormal = vBiNormal;
    if(bMovingTexture)
	{
		fUVx2.st = fUVx2.st+ vec2(time,time);
	}
}