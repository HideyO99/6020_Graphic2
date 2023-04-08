#version 420

in vec4 fColor;		// color;			
in vec4 fNormal;
in vec4 fVertWorldLocation;	// using only matModel
in vec4 fUVx2;
in vec4 fTangent;
in vec4 fBinormal;

#define MAX_LIGHT_SOURCE 10
out vec4 pixelOutputColor;
out vec4 FBO_vertexNormal;	
out vec4 FBO_vertexWorldPos;
out vec4 FBO_vertexSpecular;
out vec4 FBO_vertexRefraction;

uniform bool bUseRGBA_Color;
uniform bool bIsFlameObject;

uniform bool bUseDiscardTexture;

//FBO
uniform sampler2D sampler_FBO_vertexMaterialColour;
uniform sampler2D sampler_FBO_vertexNormal;	
uniform sampler2D sampler_FBO_vertexWorldPos;
uniform sampler2D sampler_FBO_vertexSpecular;
uniform sampler2D sampler_FBO_vertexRefraction;

uniform vec2 FBO_width_height;					// x = width, y = height
uniform vec2 screen_width_height;					// x = width, y = height
uniform bool bFullScreen;
uniform float blurAmount;
uniform vec3 focusPlane;
const int MAX_KERNEL_1D_SIZE = 20;
uniform bool bMirror;
uniform bool bRipple;
uniform float iTime;
uniform float lastTurnOn;
uniform float lastTurnOff;
uniform bool bTV;
uniform bool bTurnOn;

uniform vec4 debugColour;

struct sLight
{
	vec4 position;			
	vec4 diffuse;	
	vec4 specular;	    // rgb = highlight colour, w = power
	vec4 attenuation;	// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 direction;	    // Spot, directional lights
	vec2 angle;	    	// x = inner angle, y = outer angle
	int type;
	                    // 0 = pointlight
					    // 1 = spot light
					    // 2 = directional light
	int turnON;			// 0 for off, 1 for on	    
};

uniform vec4 eyeLocation;
uniform vec4 RGBA_Color;
uniform bool bDoNotLight;
uniform sLight Light[MAX_LIGHT_SOURCE];

uniform vec4 specularColour;			// RGB object hightlight COLOUR
										// For most material, this is white (1,1,1)
										// For metals google "metal specular hightlight colour"
										// For plastic, it's the same colour as the diffuse
										// W value is the "specular power" or "Shininess" 
										// Starts at 1, and goes to 10,000s
										//	1 = not shiny 
										// 10 = "meh" shiny

uniform sampler2D texture0;		
uniform sampler2D texture1;		
uniform sampler2D texture2;		
uniform sampler2D texture3;		
uniform sampler2D texture4;		
uniform sampler2D texture5;		
uniform sampler2D texture6;		
uniform sampler2D texture7;		

uniform vec4 texRatio_0_3;		// x = texture0, y = texture1, etc. 0 to 1
uniform vec4 texRatio_4_7;		// 0 to 1

uniform samplerCube skyboxTexture;
// When true, applies the skybox texture
uniform bool bIsSkyboxObject;

// HACK: colour the island
uniform bool bIsIlandModel;

uniform bool bStaticScreen;

//function
vec3 GaussianBlurCalculation(int numElement);
float gauss(float x, float sigma);
vec4 LightContribute(vec3 vMaterialColor, vec3 vNormal, vec3 vWorldPos, vec4 vSpecular, vec4 refraction);
vec4 LightCalculation(vec2 FragCoord);
vec4 RippleEffect(vec2 fragCoord);
vec4 noise(vec2 fragCoord);
float WarmUpTimer();

void main()
{
	//////////////////////////////////////////////////////////////////////////
	//	2nd pass															//
	//////////////////////////////////////////////////////////////////////////
	if(bFullScreen)
	{
		float screen_width = screen_width_height.x;
		float screen_height = screen_width_height.y;
		vec2 textCoords;
		if(bMirror)
		{
			textCoords = vec2( -gl_FragCoord.x / screen_width, gl_FragCoord.y  / screen_height );
		}
		else
		{
			textCoords = vec2( gl_FragCoord.x / screen_width, gl_FragCoord.y  / screen_height );
		}
		vec4 vertexColour = texture( sampler_FBO_vertexMaterialColour, textCoords );
		vec4 vertexNormal = texture( sampler_FBO_vertexNormal, textCoords );
		vec4 vertexWorldPosition = texture( sampler_FBO_vertexWorldPos, textCoords );
		vec4 vertexSpecular = texture( sampler_FBO_vertexSpecular, textCoords );
		vec4 vertexRefraction = texture( sampler_FBO_vertexRefraction, textCoords );

		if(blurAmount == 0.f)
		{
			if(!bRipple)
			{
				if(vertexNormal == vec4(1.f))
				{
					pixelOutputColor = vertexColour;
					//return;
				}
				else
				{
					pixelOutputColor = LightContribute(vertexColour.rgb, vertexNormal.xyz, vertexWorldPosition.xyz, vertexSpecular, vertexRefraction);
					//pixelOutputColor = LightContribute(vertexColour.rgb, vertexNormal.xyz, vertexWorldPosition.xyz, vertexSpecular, vec4(0));
				}
			}
			else
			{
				pixelOutputColor = RippleEffect(textCoords);
			}
		}
		if(blurAmount>0)
		{
			int GaussianElementNum = int(blurAmount * float(MAX_KERNEL_1D_SIZE));
			GaussianElementNum = clamp(GaussianElementNum, 0, MAX_KERNEL_1D_SIZE);
			pixelOutputColor.rgb = GaussianBlurCalculation(GaussianElementNum);
//			vec3 ambient = 0.15* pixelOutputColor.rgb;
//			pixelOutputColor.rgb += ambient;
			pixelOutputColor.a = 1.f;
		}

		if(bStaticScreen)
		{
			pixelOutputColor = noise(textCoords);
			pixelOutputColor.a = 1.f;
		}
		if(bTV)
		{
			pixelOutputColor.a = WarmUpTimer();
			
		}
		//pixelOutputColor = vec4(0);//vertexRefraction*vertexRefraction;
		//
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//	1st pass															//
	//////////////////////////////////////////////////////////////////////////
	pixelOutputColor = vec4(0.0f);
	FBO_vertexNormal = vec4(0.0f);
	FBO_vertexWorldPos = vec4(0.0f);
	FBO_vertexSpecular = vec4(0.0f);



	if (bIsSkyboxObject)
	{
		vec3 cubeMapColor = texture( skyboxTexture, fNormal.xyz ).rgb;
		pixelOutputColor.rgb = cubeMapColor.rgb;
		pixelOutputColor.a = 1.0f;

		FBO_vertexNormal = vec4(1.f);
		
		return;
	}
	
	if (bIsFlameObject)
	{
		// DON'T light. Apply the texture. Use colour as alpha
		vec3 flameColor = texture( texture0, fUVx2.st ).rgb;	
		
		pixelOutputColor.rgb = flameColor;
		
		// Set the alpha transparency based on the colour.
		float RGBcolorSum = pixelOutputColor.r + pixelOutputColor.g + pixelOutputColor.b;
		pixelOutputColor.a = max( ((RGBcolorSum - 0.1f) / 3.0f), 0.0f);
	
		FBO_vertexNormal = vec4(fNormal.rgb, 0.f);
		// Exit early so bypasses lighting
		return;
	}

	if ( bUseDiscardTexture )
	{	
		// Compare the colour in the texture07 black and white texture
		// If it's 'black enough' then don't draw the pixel
		// NOTE: I'm only sampling from the red 
		// (since it's black and white, all channels would be the same)
		float greyscalevalue = texture( texture7, fUVx2.st ).r;
		
		// Here, 0.5 is "black enough" 
		if ( greyscalevalue < 0.5f )
		{
			discard;
		}
	}

	vec3 materialColor = vec3(0.f);
	float alphaTransparency = RGBA_Color.w;
	if(bUseRGBA_Color)
	{
		pixelOutputColor = RGBA_Color;
		//pixelOutputColor = vec4(materialColor.rgb,alphaTransparency);
		FBO_vertexNormal = vec4(fNormal.xyz,1.f);
//		FBO_vertexWorldPos = vec4(fVertWorldLocation.xyz, 1.f);
		return;
	}
	else
	{
		vec3 textColour0 = texture( texture0, fUVx2.st ).rgb;		
		vec3 textColour1 = texture( texture1, fUVx2.st ).rgb;	
		vec3 textColour2 = texture( texture2, fUVx2.st ).rgb;	
		vec3 textColour3 = texture( texture3, fUVx2.st ).rgb;	
			
			
		materialColor =  (textColour0.rgb * texRatio_0_3.x) 
						+ (textColour2.rgb * texRatio_0_3.z) 
						+ (textColour1.rgb * texRatio_0_3.y) 
						+ (textColour3.rgb * texRatio_0_3.w);
	}
	pixelOutputColor = vec4(materialColor.rgb,alphaTransparency);


	FBO_vertexWorldPos = vec4(fVertWorldLocation.xyz, 1.f);
	if ( bDoNotLight )
	{
		pixelOutputColor = vec4(materialColor.rgb, 1.f);
		FBO_vertexNormal = vec4(1.f);

		return;
	}
	else
	{
		FBO_vertexNormal = vec4(fNormal.rgb, 1.f);
		pixelOutputColor = vec4(materialColor.rgb, 1.f);

	}
	if(bIsIlandModel)
	{
			vec4 pixelOutput_tmp = vec4(0.f);

				// Make the objects 'refractive' (like a see through glass or water or diamond...)
				vec3 R_eyeVector = normalize(eyeLocation.xyz - fVertWorldLocation.xyz);
				// genType reflect(	genType IncidentVector, genType Normal);
				// (index of refraction for diamond is 2.417 according to wikipedia)
				// (index of refraction for water is 1.333 according to wikipedia)
				vec3 STU_Vector = refract(R_eyeVector, fNormal.xyz, 1.0f/2.417f);
				//vec3 STU_Vector = refract(R_eyeVector, vertexNormal.xyz, 1.0f/1.333f);

				vec3 cubeMapColour = texture( skyboxTexture, STU_Vector.xyz ).rgb;
				//pixelOutputColor.rgb *= 0.00001f;
				//pixelOutputColor.rgb += cubeMapColour.rgb;
				pixelOutput_tmp.rgb *=0.00001f;
				pixelOutput_tmp.rgb += cubeMapColour.rgb;
				FBO_vertexRefraction = vec4(R_eyeVector,1.f);
	}
	else
	{
		FBO_vertexRefraction = vec4(0.f);
	}

	FBO_vertexSpecular = specularColour;
	
}



float gauss(float x, float sigma) {
    float coeff = 1.0 / (sqrt(2.0 * 3.14159) * sigma);
    float exponent = -(x * x) / (2.0 * sigma * sigma);
    return coeff * exp(exponent);
}

vec4 LightContribute(vec3 vMaterialColor, vec3 vNormal, vec3 vWorldPos, vec4 vSpecular, vec4 refraction)
{
	// calculateLightContribute
	vec4 finalObjectColor = vec4( 0.0f, 0.0f, 0.0f, 1.0f );

    vec3 normal = normalize(vNormal.xyz);
	for(int i = 0; i < MAX_LIGHT_SOURCE; i++)
	{
		if(Light[i].turnON == 0)
		{
			continue;
		}
		
		if( Light[i].type == 2)
		{
			vec3 lightContrib = Light[i].diffuse.rgb;
			float dotProd = dot(-Light[i].direction.xyz, normal);//normalize(normal.xyz));
			dotProd = max( 0.0f, dotProd);
			lightContrib *= dotProd;
			finalObjectColor.rgb += (vMaterialColor.rgb * Light[i].diffuse.rgb * lightContrib);
			continue;
		}
		
		vec3 vLightToVertex = Light[i].position.xyz - vWorldPos.xyz;
		float distanceToLight = length(vLightToVertex);
		vec3 lightVector = normalize(vLightToVertex);
		float dotProduct = dot(lightVector, vNormal.xyz);
		dotProduct = max( 0.0f, dotProduct );
		vec3 lightDiffuseContrib = dotProduct * Light[i].diffuse.rgb;
		
		vec3 lightSpecularContrib = vec3(0.0f);
//		if(Light[i].type == 0)
//		{
			vec3 reflectVector = reflect( -lightVector, normal);//normalize(normal.xyz) );
			
			vec3 eyeVector = normalize(eyeLocation.xyz - vWorldPos.xyz);
			float objectSpecularPower = vSpecular.w;
			lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower ) * (vSpecular.rgb * Light[i].specular.rgb);
			
			float atten = 1.0f /( Light[i].attenuation.x + Light[i].attenuation.y * distanceToLight +	Light[i].attenuation.z * distanceToLight*distanceToLight );  
			
			lightDiffuseContrib *= atten;
			lightSpecularContrib *= atten;
//		}
		
		if(Light[i].type == 1)
		{
			vec3 vertexToLight = normalize(vWorldPos.xyz - Light[i].position.xyz);
			
			float currentLightRayAngle = dot( vertexToLight.xyz, Light[i].direction.xyz );
			
			currentLightRayAngle = max(0.0f, currentLightRayAngle);
			float outerConeAngleCos = cos(radians(Light[i].angle.y));
			float innerConeAngleCos = cos(radians(Light[i].angle.x));
			
			if ( currentLightRayAngle < outerConeAngleCos )
			{
				// Nope. so it's in the dark
				lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
				lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
			}
			else if ( currentLightRayAngle < innerConeAngleCos )
			{
	
				float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) / 
									(innerConeAngleCos - outerConeAngleCos);
									
				lightDiffuseContrib *= penumbraRatio;
				lightSpecularContrib *= penumbraRatio;
			}		
		}
		
		finalObjectColor.rgb += (vMaterialColor.rgb * lightDiffuseContrib.rgb) + (vSpecular.rgb  * lightSpecularContrib.rgb );
	}

	if(refraction!=vec4(0.f))
	{
		// (index of refraction for diamond is 2.417 according to wikipedia)
		// (index of refraction for water is 1.333 according to wikipedia)
		vec3 STU_Vector = refract(refraction.xyz, vNormal.xyz, 1.0f/1.333f);
		vec3 cubeMapColour = texture( skyboxTexture, STU_Vector.xyz ).rgb;
		vec4 pixelOutput_tmp = vec4(0.f);
		pixelOutput_tmp.rgb *=0.00001f;
		pixelOutput_tmp.rgb += cubeMapColour.rgb;
		vec3 ambient = vMaterialColor * 0.15;
		finalObjectColor.rgb += ambient;
		finalObjectColor.rgb += pixelOutput_tmp.rgb;
		
		return vec4(finalObjectColor.rgb,1.f);
	}else{
		vec3 ambient = vMaterialColor * 0.15;
		finalObjectColor.rgb += ambient;

		return vec4(finalObjectColor.rgb,1.f);
		//finalObjectColor.a = 1.0f;
	}

	return finalObjectColor;
}

vec3 GaussianBlurCalculation(int numElement)
{
	float screen_width = screen_width_height.x;
	float screen_height = screen_width_height.y;
	vec3 pixelColor = vec3(0.f);
	float totalGaussianWeightUsed = 0.0f;
	float recticle = pow((gl_FragCoord.x -(screen_width/2)),2) + pow((gl_FragCoord.y-(screen_height/2)),2); 
	float radius = pow((screen_height/4),2);
	float thick = pow((screen_height/4)+10,2);
	if((recticle < thick)&& (recticle>radius))
	{
		return vec3(0.f);
	}
	else if(recticle < radius)
	{
		//don't blur
		vec2 texcoord = vec2((gl_FragCoord.x ) , (gl_FragCoord.y));
		if(bMirror)
		{
			texcoord = vec2((-gl_FragCoord.x ) , (gl_FragCoord.y));
		}

		pixelColor = LightCalculation(texcoord).rgb;
		
		return pixelColor;
	}
	else
	{
		for(int i = 0; i <= numElement ; i++)
		{
			//cal Light
			
			vec4 calPixel_R;
			vec4 calPixel_L;
			vec4 calPixel_U;
			vec4 calPixel_D;

			if(bMirror)
			{
				calPixel_R = LightCalculation(vec2( -gl_FragCoord.x + i, gl_FragCoord.y));
				calPixel_L = LightCalculation(vec2( -gl_FragCoord.x - i, gl_FragCoord.y));
				calPixel_U = LightCalculation(vec2( gl_FragCoord.x	, gl_FragCoord.y + 1));
				calPixel_D = LightCalculation(vec2( gl_FragCoord.x	, gl_FragCoord.y - 1));
			}
			else
			{
				calPixel_R = LightCalculation(vec2( gl_FragCoord.x + i, gl_FragCoord.y));
				calPixel_L = LightCalculation(vec2( gl_FragCoord.x - i, gl_FragCoord.y));
				calPixel_U = LightCalculation(vec2( gl_FragCoord.x	, gl_FragCoord.y + 1));
				calPixel_D = LightCalculation(vec2( gl_FragCoord.x	, gl_FragCoord.y - 1));
			}
			float blurWeight = gauss(i,numElement);
			calPixel_R.rgb *= blurWeight;
			calPixel_L.rgb *= blurWeight;
			calPixel_U.rgb *= blurWeight;
			calPixel_D.rgb *= blurWeight;
					
					// We used 4 weights here
			totalGaussianWeightUsed += (blurWeight * 4);
					
			pixelColor.rgb += calPixel_R.rgb + calPixel_L.rgb + calPixel_U.rgb + calPixel_D.rgb;
		}
		pixelColor.rgb /= totalGaussianWeightUsed;

		return pixelColor;
	}
}

vec4 LightCalculation(vec2 FragCoord)
{
	vec4 calPixel = vec4(0.f);
	float screen_width = screen_width_height.x;
	float screen_height = screen_width_height.y;
	vec4 pixelColor		= texture( sampler_FBO_vertexMaterialColour,	vec2((FragCoord.x)	/screen_width , (FragCoord.y)/screen_height));
	vec4 pixelNormal	= texture( sampler_FBO_vertexNormal,			vec2((FragCoord.x)	/screen_width , (FragCoord.y)/screen_height)); 
	vec4 pixelWorldPos	= texture( sampler_FBO_vertexWorldPos,			vec2((FragCoord.x)	/screen_width , (FragCoord.y)/screen_height));
	vec4 pixelSpec		= texture( sampler_FBO_vertexSpecular,			vec2((FragCoord.x)	/screen_width , (FragCoord.y)/screen_height)); 
	vec4 pixelrefract	= texture( sampler_FBO_vertexRefraction,		vec2((FragCoord.x)	/screen_width , (FragCoord.y)/screen_height));

	if(pixelNormal == vec4(1.f))
	{
		calPixel = pixelColor;
	}
	else
	{
		calPixel = LightContribute(pixelColor.rgb,pixelNormal.rgb,pixelWorldPos.rgb,pixelSpec,pixelrefract);
	}
	return calPixel;
}

vec4 RippleEffect(vec2 fragCoord)
{
    vec2 cp = -1.0 + 2.0 * fragCoord ;
    float cl = length(cp);
    vec2 uv = fragCoord  + (cp / cl) * cos(cl * 12.0 - iTime * 4.0) * 0.02;
    //vec2 uv = fragCoord / resolution + (cp / cl) * cos(cl * 12.0 - 0 * 4.0) * 0.02;
    vec3 col = texture(sampler_FBO_vertexMaterialColour, uv).xyz;
	vec3 vertexNormal = texture( sampler_FBO_vertexNormal, uv ).xyz;
	vec3 vertexWorldPosition = texture( sampler_FBO_vertexWorldPos, uv ).xyz;
	vec4 vertexSpecular = texture( sampler_FBO_vertexSpecular, uv );
	vec4 vertexRefraction = texture( sampler_FBO_vertexRefraction, uv );
	vec4 calPixel = LightContribute(col, vertexNormal, vertexWorldPosition, vertexSpecular, vertexRefraction);
	return calPixel;
}

vec4 noise(vec2 fragCoord)
{
	float G = fract((iTime*0.01));
	float cx  = fragCoord.x*G;
    float cy  = fragCoord.y*G;
	vec3 color = vec3(fract(23.0*fract(2.0/fract(fract(cx*2.4/cy*23.0+pow(abs(cy/22.4),3.3))*fract(cx*iTime/pow(abs(cy),0.050))))));
	return vec4(color,1.f);
}

float WarmUpTimer()
{
	if(bTurnOn)
	{
		float timer = 5;
		float diff = iTime-lastTurnOn;
		//float result = mix(0,timer,diff) / timer;
		float result = diff / timer;
		result = clamp(result,0,1);
		return result;
	}
	else
	{
		float timer = 0.5;
		float diff = iTime-lastTurnOff;
		//float result = mix(0,timer,diff) / timer;
		float result = diff / timer;
		result = 1 - clamp(result,0,1);
		return result;
	}
}