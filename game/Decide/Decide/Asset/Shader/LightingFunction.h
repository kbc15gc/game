//���C�e�B���O�̌v�Z�Ɏg���֐����܂Ƃ߂��w�b�_�[
#define NUM_DIFFUSE_LIGHT 4					//�f�B�t���[�Y���C�g�̐��B

//�X�y�L�����}�b�v
texture g_speculerMap;
sampler g_speculerMapSampler = 
sampler_state
{
	Texture = <g_speculerMap>;
    MipFilter = NONE;
    MinFilter = NONE;
    MagFilter = NONE;
    AddressU = Wrap;
	AddressV = Wrap;
};

struct SLight{
	float3	diffuseLightDir[NUM_DIFFUSE_LIGHT];		//�f�B�t���[�Y���C�g�̌����B
	float4  diffuseLightColor[NUM_DIFFUSE_LIGHT];	//�f�B�t���[�Y���C�g�̃J���[�B
	float3	limLightDir;							//�������C�g�̕����B
	float4	limLightColor;							//�������C�g�̐F�B
	float4  pointLightPosition;						//�|�C���g���C�g�̍��W�B
	float4  pointLightColor;						//�|�C���g���C�g�̃J���[�B
	float3  ambient;								//�A���r�G���g���C�g�B
	float3  emission;								//���Ȕ����B
};

SLight	g_light;		//!<���C�g

float4 g_MapFlg;		//�ǂ�ȃ}�b�v���g�����̃t���O
float4 g_EffectFlg;	//x�͓��e�Ay�̓X�y�L����

int g_LightNum;										//���C�g�̐�
float4	g_diffuseLightDirection[NUM_DIFFUSE_LIGHT];	//�f�B�t���[�Y���C�g�̕����B
float4	g_diffuseLightColor[NUM_DIFFUSE_LIGHT];		//�f�B�t���[�Y���C�g�̃J���[�B
float4	g_ambientLight;								//�����B

float4	g_cameraPos;	//!<�J�����̍��W�B
float3	g_cameraDir;	//!<�J���������B

//�f�B�t���[�Y���C�g���v�Z�B	
float4 DiffuseLight( float3 normal )
{
	float4 color = 0.0f;
	for (int i = 0; i < g_LightNum; i++)
	{
		color += max(0.0f, -dot(normal, g_diffuseLightDirection[i])) * g_diffuseLightColor[i];
	}
	
	color.a = 1.0f;
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
	float specPow = tex2D(g_speculerMapSampler, uv);
	float3 spec = 0.0f;
	float3 toEyeDir = normalize( g_cameraPos.xyz - worldPos );
	float3 R = -toEyeDir + 2.0f * dot(normal, toEyeDir) * normal;
	for( int i = 0; i < NUM_DIFFUSE_LIGHT; i++ ){
		//�X�y�L�����������v�Z����B
		//���˃x�N�g�����v�Z�B
		float3 L = -g_diffuseLightDirection[i].xyz;
		spec += g_diffuseLightColor[i] * pow(max(0.0f, dot(L,R)), 2 ) * g_diffuseLightColor[i].w;	//�X�y�L�������x�B
	}
	return spec * specPow;
}
/*!
 * @brief	�|�C���g���C�g���v�Z�B
 */
float3 PointLight( float3 normal, float3 worldPos, int lim )
{
	float3 lightDir = worldPos - g_light.pointLightPosition.xyz;
	float len = length(lightDir) / g_light.pointLightColor.w;
	lightDir = normalize(lightDir);
	float3 color = max( 0.0f, -dot(normal, lightDir)) * g_light.pointLightColor.xyz;
	//�����ɔ���Ⴕ�Č���
	color /= max( 1.0f, (len*len) );

	return color;
}
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