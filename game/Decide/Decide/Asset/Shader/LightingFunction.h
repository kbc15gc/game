//���C�e�B���O�̌v�Z�Ɏg���֐����܂Ƃ߂��w�b�_�[
#define NUM_DIFFUSE_LIGHT 4					//�f�B�t���[�Y���C�g�̐��B

//�X�y�L�����}�b�v
texture g_speculerMap;
sampler g_speculerMapSampler = 
sampler_state
{
	Texture = <g_speculerMap>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = Wrap;
	AddressV = Wrap;
};

//�X�y�L�����}�b�v
texture g_NormalMap;
sampler g_NormalMapSampler =
sampler_state
{
	Texture = <g_NormalMap>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

float4 g_MapFlg;		//�ǂ�ȃ}�b�v���g�����̃t���O
float4 g_EffectFlg;	//x�͓��e�Ay�̓X�y�L����



int g_LightNum;										//���C�g�̐�
float4	g_diffuseLightDirection[NUM_DIFFUSE_LIGHT];	//�f�B�t���[�Y���C�g�̕����B
float4	g_diffuseLightColor[NUM_DIFFUSE_LIGHT];		//�f�B�t���[�Y���C�g�̃J���[�B
float4	g_ambientLight;								//�����B

float4	g_cameraPos;	//!<�J�����̍��W�B
float3	g_cameraDir;	//!<�J���������B

float g_Alpha; //�A���t�@�̂������l.

#define NUM_CHARA_DIFFLIGHT 4
/**
* �L�����N�^�[���C�g�N���X.
*/
struct CharacterLightS
{
	float4	DiffuseDir[NUM_CHARA_DIFFLIGHT];	//�f�B�t���[�Y���C�g�̕���.
	float4	DiffuseColor[NUM_CHARA_DIFFLIGHT];		//�f�B�t���[�Y���C�g�̃J���[.
	float4	Ambient;								//����.
	int LightCount;
};

CharacterLightS g_CharaLight;
float4 g_CharaLightParam; // x:�L��.

/*!
* @brief	��C�U���p�����[�^�B
*/
struct SAtmosphericScatteringParam
{
	float3 v3LightPos;
	float3 v3LightDirection;
	float3 v3InvWavelength;	// 1 / pow(wavelength, 4) for the red, green, and blue channels
	float fCameraHeight;		// The camera's current height
	float fCameraHeight2;		// fCameraHeight^2
	float fOuterRadius;		// The outer (atmosphere) radius
	float fOuterRadius2;		// fOuterRadius^2
	float fInnerRadius;		// The inner (planetary) radius
	float fInnerRadius2;		// fInnerRadius^2
	float fKrESun;				// Kr * ESun
	float fKmESun;				// Km * ESun
	float fKr4PI;				// Kr * 4 * PI
	float fKm4PI;				// Km * 4 * PI
	float fScale;				// 1 / (fOuterRadius - fInnerRadius)
	float fScaleOverScaleDepth;// fScale / fScaleDepth
	float g;
	float g2;
};

SAtmosphericScatteringParam g_atmosParam;

int g_atmosFlag;

// The scale depth (the altitude at which the average atmospheric density is found)
const float fScaleDepth = 0.25;
const float fInvScaleDepth = 4;

const int nSamples = 2;
const float fSamples = 2.0f;

const int AtomosphereFuncNone = 0;						//��C�����V�~�����[�V�����Ȃ��B
const int AtomosphereFuncObjectFromAtomosphere = 1;		//�I�u�W�F�N�g���C�����猩���ꍇ�̑�C�����V�~�����[�V�����B
const int AtomosphereFuncSkyFromAtomosphere = 2;		//����C�����猩���ꍇ�̑�C�����V�~�����[�V�����B

float3 CalcNormal(float3 InNormal, float3 InTangent, float2 InUV)
{
	float3 retNormal = InNormal;
	if (g_MapFlg.x)
	{
		//�@���}�b�v����
		float3 normal = tex2D(g_NormalMapSampler, InUV);
		float3 biNormal = normalize(cross(InTangent, InNormal));
		normal = (normal * 2.0f) - 1.0f;
		normal = InTangent * normal.x + biNormal * normal.y + InNormal * normal.z;
		retNormal = normal;
	}
	return retNormal;
}

//�f�B�t���[�Y���C�g���v�Z�B	
float4 DiffuseLight( float3 normal)
{
	float4 color = 0.0f;
	
	for (int i = 0; i < g_LightNum; i++)
	{
		color += max(0.0f, -dot(normal, g_diffuseLightDirection[i])) * g_diffuseLightColor[i];
	}

	if (g_atmosFlag == AtomosphereFuncObjectFromAtomosphere)
	{
		//��C�������ݒ肳��Ă���ꍇ��0�Ԗڂ̃��C�g�𑾗z���Ƃ���B
		float t = max(0.0f, dot(float3(0.0f, -1.0f, 0.0f), g_diffuseLightDirection[0]));
		t *= 2.0f;
		t = min(1.0f, pow(t, 2.0f));
		color.xyz *= t;
	}

	color.w = 1.0f;
	
	return color;
}

/**
* ������̌v�Z.
*/
float3 CalcMoonLight(float3 normal, float3 worldPos, float2 uv)
{
	float3 color = 0.0f;
	color += max(0.0f, -dot(normal, g_diffuseLightDirection[1])) * g_diffuseLightColor[1];

	if (g_MapFlg.y)
	{
		float3 spec = 0.0f;
		float specPow = tex2D(g_speculerMapSampler, uv);

		float3 toEyeDir = normalize(g_cameraPos.xyz - worldPos);
		float3 R = -toEyeDir + 2.0f * dot(normal, toEyeDir) * normal;

		//�X�y�L�����������v�Z����B
		//���˃x�N�g�����v�Z�B
		float3 L = -g_diffuseLightDirection[1].xyz;
		spec += g_diffuseLightColor[1] * pow(max(0.0f, dot(L, R)), 2) * g_diffuseLightColor[1].w;	//�X�y�L�������x�B

		color += spec * specPow;
	}

	return color;
}

/**
* �L�����N�^�[���C�g���v�Z.
*/
float3 CalcCharaLight(float3 normal)
{
	float3 color = 0.0f;
	if (g_CharaLightParam.x)
	{
		for (int i = 0; i < g_CharaLight.LightCount; i++)
		{
			float3 dir = g_CharaLight.DiffuseDir[i].xyz;
			color.xyz += max(0.0f, -dot(normal, dir)) * g_CharaLight.DiffuseColor[i].xyz;
		}
	}
	return color;
}

/*!
 *@brief	�������C�g���v�Z�B
 */
float3 CalcLimLight( float3 normal, float3 lightDir, float3 limColor)
{
	float lim = 0.0f;
	float baselim = 1.0f - abs( dot(normal, g_cameraDir ) );
	lim += baselim * max( 0.0f, -dot(g_cameraDir, lightDir));
	lim = pow(lim, 1.5f);
	return limColor * lim;
}
/*!
 *@brief	�X�y�L�������C�g���v�Z�B
 */
float3 SpecLight(float3 normal, float3 worldPos, float2 uv)
{
	float3 spec = 0.0f;
	float3 toEyeDir = normalize(g_cameraPos.xyz - worldPos);
	float3 R = -toEyeDir + 2.0f * dot(normal, toEyeDir) * normal;
	for (int i = 0; i < g_LightNum; i++) {
		//�X�y�L�����������v�Z����B
		//���˃x�N�g�����v�Z�B
		float3 L = -g_diffuseLightDirection[i].xyz;
		spec += g_diffuseLightColor[i] * pow(max(0.0f, dot(L, R)), 2) * g_diffuseLightColor[i].w;	//�X�y�L�������x�B
	}
	//spec += g_ambientLight.xyz * pow(max(0.0f, dot(normal, R)), 2) * 10.0f;	//�X�y�L�������x�B

	if (g_MapFlg.y)
	{
		float specPow = tex2D(g_speculerMapSampler, uv);
		spec *= specPow;
	}
	return spec;
}

/*!
*@brief	�X�y�L�������C�g���v�Z�B
*/
float3 CalcCharaSpecLight(float3 normal, float3 worldPos, float2 uv)
{
	float3 spec = 0.0f;
	float3 toEyeDir = normalize(g_cameraPos.xyz - worldPos);
	float3 R = -toEyeDir + 2.0f * dot(normal, toEyeDir) * normal;
	for (int i = 0; i < g_CharaLight.LightCount; i++) {
		//�X�y�L�����������v�Z����B
		//���˃x�N�g�����v�Z�B
		float3 L = -g_CharaLight.DiffuseDir[i].xyz;
		spec += g_CharaLight.DiffuseColor[i] * pow(max(0.0f, dot(L, R)), 2) * g_CharaLight.DiffuseColor[i].w;	//�X�y�L�������x�B
	}
	//spec += g_ambientLight.xyz * pow(max(0.0f, dot(normal, R)), 2) * 10.0f;	//�X�y�L�������x�B

	if (g_MapFlg.y)
	{
		float specPow = tex2D(g_speculerMapSampler, uv);
		spec *= specPow;
	}
	return spec;
}
/*!
 * @brief	�|�C���g���C�g���v�Z�B
 */
//float3 PointLight( float3 normal, float3 worldPos, int lim )
//{
//	float3 lightDir = worldPos - g_light.pointLightPosition.xyz;
//	float len = length(lightDir) / g_light.pointLightColor.w;
//	lightDir = normalize(lightDir);
//	float3 color = max( 0.0f, -dot(normal, lightDir)) * g_light.pointLightColor.xyz;
//	//�����ɔ���Ⴕ�Č���
//	color /= max( 1.0f, (len*len) );
//
//	return color;
//}
/*!
 * @brief	�A���t�@�ɖ��ߍ��ދP�x���v�Z�B
 */
float CalcLuminance( float3 color )
{
	float luminance = dot( color.xyz, float3(0.2125f, 0.7154f, 0.0721f) );
	if(luminance > 1.0f ){
		luminance = 1.0f / luminance;
	}else{
		luminance = 0.0f;
	}
	return luminance;
}

// �t�H�O�p�����[�^ 
// x:�t�H�O��������n�߂鋗��.
// y:�t�H�O�������肫�鋗��.
// z:�t�H�O�̎��.
float4 g_fogParam;
// �t�H�O�̐F.
float4 g_fogColor;

/**
* �t�H�O���v�Z.
*/
float3 CalcFog(float3 worldPos, float3 color)
{
	float3 retColor = color;
	float night = 1.0f;
	if(g_fogParam.w)
	{
		night = dot(g_atmosParam.v3LightDirection, float3(0.0f, -1.0f, 0.0f));
		night = max(0.0f, min(1.0f, night + 0.2f));
	}
	if (g_fogParam.z > 1.9f)
	{
		//�����t�H�O.
		float h = max(0.0f, worldPos.y - g_fogParam.y);
		float t = min(1.0f, h / g_fogParam.x);
		retColor = lerp(g_fogColor.xyz, color.xyz, t * night);
	}
	else if (g_fogParam.z > 0.0f)
	{
		//�����t�H�O.
		float z = length(worldPos - g_cameraPos);
		z = max(0.0f, z - g_fogParam.x);
		float t = z / g_fogParam.y;
		retColor = lerp(color, g_fogColor.xyz, t * night);
	}
	return retColor;
}


#define DP_SIZE 8

// �f�B�U�p�^�[��.
const float g_DitherPattern[DP_SIZE][DP_SIZE] =
{
	//	{ 1, 1, 1, 1, 1, 1, 1, 1 },
	//	{ 2, 1, 10, 1, 4, 1, 12, 1 } ,
	//	{ 1, 1, 1, 1, 1, 1, 1, 1 },
	//	{ 14, 1, 6, 1, 16, 1, 8, 1 } ,
	//	{ 1, 1, 1, 1, 1, 1, 1, 1 },
	//	{ 5, 1, 13, 1, 3, 1, 11, 1 } ,
	//	{ 1, 1, 1, 1, 1, 1, 1, 1 },
	//	{ 17, 1, 9, 1, 15, 1, 7, 1 },
	{ 1, 33, 9, 41, 3, 35, 11, 43 },
	{ 49, 17, 57, 25, 51, 19, 59, 27 } ,
	{ 13, 45, 5, 37, 15, 47, 7, 39 },
	{ 61, 29, 53, 21, 63, 31, 55, 23 } ,
	{ 4, 36, 12, 44, 2, 34, 10, 42 },
	{ 52, 20, 60, 28, 50, 18, 58, 26 } ,
	{ 16, 48, 8, 40, 14, 46, 6, 33 },
	{ 64, 32, 56, 24, 62, 30, 54, 22 },
};

/**
* �f�B�U�����O�p�����[�^
* x : �t���O.
* y : �f�B�U�W��.
*/
float4 g_DitherParam;

//�|���肫��Œ�l.
const float DitherMinLen = 1.5f;
//�|����n�߂�ō��l.
const float DitherMaxLen = 4.5f;

/**
* �f�B�U�����O.
*/
void CalcDither(float4 WVP,float3 World)
{
	if (g_DitherParam.x > 0.0f)
	{
		float dither = g_DitherParam.y;

		if (g_DitherParam.x > 1.0f)
		{
			//�J����������W�ւ̃x�N�g��.
			float CameraToPosLen = length(World - g_cameraPos.xyz);

			CameraToPosLen -= DitherMinLen;
			//�Œ�l~�ō��l�ɃN�����v.
			CameraToPosLen = min(DitherMaxLen - DitherMinLen, CameraToPosLen);
			CameraToPosLen - max(0.0f, CameraToPosLen);
			//���K��.
			CameraToPosLen /= (DitherMaxLen - DitherMinLen);

			// �f�B�U�W��.
			// 0 ~ 65.
			// �f�B�U�W�������傫���l�̂Ƃ��낪�c��.
			dither = max(dither, (1.0f - CameraToPosLen) * 65.0f);
		}

		if (dither <= 0.0f)
		{
			return;
		}

		float2 f2TextureUV = 0.0f;
		f2TextureUV.x = WVP.x / WVP.w;
		f2TextureUV.y = WVP.y / WVP.w;

		f2TextureUV.x *= 0.5;
		f2TextureUV.y *= -0.5;
		f2TextureUV += 0.5;

		f2TextureUV.x *= g_DitherParam.z / 5;
		f2TextureUV.y *= g_DitherParam.w / 5;

		int2 uv = fmod(f2TextureUV, DP_SIZE);

		clip(g_DitherPattern[uv.x][uv.y] - dither);
	}
}

// The scale equation calculated by Vernier's Graphical Analysis
float scale(float fCos)
{
	float x = 1.0 - fCos;
	return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}
// Returns the near intersection point of a line and a sphere
float getNearIntersection(float3 v3Pos, float3 v3Ray, float fDistance2, float fRadius2)
{
	float B = 2.0 * dot(v3Pos, v3Ray);
	float C = fDistance2 - fRadius2;
	float fDet = max(0.0, B*B - 4.0 * C);
	return 0.5 * (-B - sqrt(fDet));
}
// Calculates the Mie phase function
float getMiePhase(float fCos, float fCos2, float g, float g2)
{
	return 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos2) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);
}

// Calculates the Rayleigh phase function
float getRayleighPhase(float fCos2)
{
	//return 1.0;
	return 0.75 + 0.75*fCos2;
}

/*!
* @brief	��C�����������������̃~�[�����ƃ��C���[�������v�Z�B
*/
void CalcMieAndRayleighColorsSkyFromAtomosphere(out float4 mieColor, out float4 rayColor, out float3 posToCameraDir, float3 worldPos)
{
	//	worldPos.y += g_atmosParam.fInnerRadius;
	float3 cameraPos = g_cameraPos.xyz;
	cameraPos *= 0.001f;
	cameraPos.y += g_atmosParam.fInnerRadius;
	mieColor = 0.0f;
	rayColor = 0.0f;
	worldPos *= 0.001f;
	worldPos.y += g_atmosParam.fInnerRadius;
	// Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
	float3 v3Ray = worldPos - cameraPos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;

	// Calculate the closest intersection of the ray with the outer atmosphere (which is the near point of the ray passing through the atmosphere)
	//	float fNear = getNearIntersection(g_cameraPos.xyz, v3Ray, g_atmosParam.fCameraHeight2, g_atmosParam.fOuterRadius2);

	float3 v3Start = cameraPos;
	float fHeight = length(v3Start);
	float fDepth = exp(g_atmosParam.fScaleOverScaleDepth * (g_atmosParam.fInnerRadius - g_atmosParam.fCameraHeight));
	float fStartAngle = dot(v3Ray, v3Start) / fHeight;
	float fStartOffset = fDepth*scale(fStartAngle);

	// Initialize the scattering loop variables
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * g_atmosParam.fScale;
	float3 v3SampleRay = v3Ray * fSampleLength;
	float3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

	// Now loop through the sample rays
	float3 v3FrontColor = float3(0.0, 0.0, 0.0);
	for (int i = 0; i<nSamples; i++)
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(g_atmosParam.fScaleOverScaleDepth * (g_atmosParam.fInnerRadius - fHeight));
		float fLightAngle = dot(g_atmosParam.v3LightDirection, v3SamplePoint) / fHeight;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
		float fScatter = (fStartOffset + fDepth*(scale(fLightAngle) - scale(fCameraAngle)));
		float3 v3Attenuate = exp(-fScatter * (g_atmosParam.v3InvWavelength * g_atmosParam.fKr4PI + g_atmosParam.fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}

	// Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader

	mieColor.rgb = v3FrontColor * g_atmosParam.fKmESun;
	rayColor.rgb = v3FrontColor * (g_atmosParam.v3InvWavelength * g_atmosParam.fKrESun);
	posToCameraDir = cameraPos - worldPos;
}
/*!
* @brief	��C��������I�u�W�F�N�g���������̃~�[�����ƃ��C���[�������v�Z�B
*/
void CalcMieAndRayleighColorsObjectFromAtomosphere(out float4 mieColor, out float4 rayColor, out float3 posToCameraDir, float3 worldPos)
{
	//	worldPos.y += g_atmosParam.fInnerRadius;
	float3 cameraPos = g_cameraPos.xyz;
	cameraPos *= 0.001f; //�P�ʂ�m����km�ɕύX�B
	cameraPos.y += g_atmosParam.fInnerRadius;
	mieColor = 0.0f;
	rayColor = 0.0f;
	worldPos *= 0.001f; //�P�ʂ�m����km�ɕύX�B
	worldPos.y += g_atmosParam.fInnerRadius;
	worldPos.y = min(worldPos.y, cameraPos.y);
	// Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
	float3 v3Ray = worldPos - cameraPos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;
	
	// Calculate the closest intersection of the ray with the outer atmosphere (which is the near point of the ray passing through the atmosphere)
	//	float fNear = getNearIntersection(g_cameraPos.xyz, v3Ray, g_atmosParam.fCameraHeight2, g_atmosParam.fOuterRadius2);

	float3 v3Start = cameraPos;
	float fDepth = exp((g_atmosParam.fInnerRadius - g_atmosParam.fCameraHeight) / fScaleDepth);
	float fCameraAngle = dot(-v3Ray, worldPos) / length(worldPos);
	float fLightAngle = dot(g_atmosParam.v3LightDirection, worldPos) / length(worldPos);
	float fCameraScale = scale(fCameraAngle);
	float fLightScale = scale(fLightAngle);
	float fCameraOffset = fDepth*fCameraScale;
	float fTemp = (fLightScale + fCameraScale);

	// Initialize the scattering loop variables
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * g_atmosParam.fScale;
	float3 v3SampleRay = v3Ray * fSampleLength;
	float3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

	// Now loop through the sample rays
	float3 v3FrontColor = float3(0.0, 0.0, 0.0);
	float3 v3Attenuate;
	for (int i = 0; i<nSamples; i++)
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(g_atmosParam.fScaleOverScaleDepth * (g_atmosParam.fInnerRadius - fHeight));
		float fScatter = fDepth*fTemp - fCameraOffset;
		v3Attenuate = exp(-fScatter * (g_atmosParam.v3InvWavelength * g_atmosParam.fKr4PI + g_atmosParam.fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}

	// Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader

	mieColor.rgb = min(float3( 1.0f, 1.0f, 1.0f), v3Attenuate);
	rayColor.rgb = min(float3( 0.6f, 0.6f, 0.6f), v3FrontColor * (g_atmosParam.v3InvWavelength * g_atmosParam.fKrESun + g_atmosParam.fKmESun));
	posToCameraDir = cameraPos - worldPos;
}
/*!
*@brief	��C�����V�~�����[�V�����B
*@param[in]	In		���͒��_�B
*@param[out]	Pos		���[���h���W�̊i�[��B
*@param[out]	Normal	���[���h�@���̊i�[��B
*@param[out]	Tangent	���[���h�ڃx�N�g���̊i�[��B
*/

void CalcMieAndRayleighColors(out float4 mieColor, out float4 rayColor, out float3 posToCameraDir, float3 worldPos)
{
	if (g_atmosFlag == AtomosphereFuncObjectFromAtomosphere) {
		CalcMieAndRayleighColorsObjectFromAtomosphere(mieColor, rayColor, posToCameraDir, worldPos);
	}
	else if (g_atmosFlag == AtomosphereFuncSkyFromAtomosphere) {
		CalcMieAndRayleighColorsSkyFromAtomosphere(mieColor, rayColor, posToCameraDir, worldPos);
	}
	else {
		mieColor = 0.0f;
		rayColor = 0.0f;
	}

}