/*!
 * @brief	�X�L�����f���V�F�[�_�[�B(4�{�[���X�L�j���O)
 */
#define MAX_LIGHTNUM 4
bool Texflg;							//�e�N�X�`��
bool Spec;								//�X�y�L����
bool ReceiveShadow;						//�e���ʂ�

float4 g_cameraPos;						//�J�����̈ʒu

//�X�L���s��B
#define MAX_MATRICES  50
float4x3    g_mWorldMatrixArray[MAX_MATRICES] : WORLDMATRIXARRAY;

float4x4 g_worldMatrix;					//���[���h�s��B
float4x4 g_viewMatrix;					//�r���[�s��B
float4x4 g_projectionMatrix;			//�v���W�F�N�V�����s��B
float4x4 g_rotationMatrix;				//��]�s��B�@������]�����邽�߂ɕK�v�ɂȂ�B���C�e�B���O����Ȃ�K�{�B

float	g_numBone;			//���̐��B

int g_LightNum;									//���C�g�̐�
float4	g_diffuseLightDirection[MAX_LIGHTNUM];	//�f�B�t���[�Y���C�g�̕����B
float4	g_diffuseLightColor[MAX_LIGHTNUM];		//�f�B�t���[�Y���C�g�̃J���[�B
float4	g_ambientLight;								//�����B

float4  g_diffuseMaterial : COLOR0;					//�}�e���A���J���[
float4  g_blendcolor;//������F

float4x4 g_LVP;					//���C�g����݂��r���[�v���W�F�N�V�����s��

float2 g_TexelSize;				//�e�N�Z���T�C�Y(�[�x�o�b�t�@��)

float4  g_Textureblendcolor;	//�e�N�X�`���ɍ�����F
texture g_diffuseTexture;		//�f�B�t���[�Y�e�N�X�`���B
sampler g_diffuseTextureSampler = 
sampler_state
{
	Texture = <g_diffuseTexture>;
    MipFilter = NONE;
    MinFilter = NONE;
    MagFilter = NONE;
    AddressU = Wrap;
	AddressV = Wrap;
};

texture g_Depth;				//���C���J�������猩���[�x�e�N�X�`��
sampler g_DepthSampler =
sampler_state
{
	Texture = <g_Depth>;
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

texture g_Shadow;				//�e�e�N�X�`��
sampler g_ShadowSampler_0 =
sampler_state
{
	Texture = <g_Shadow>;
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};


/*!
 * @brief	���͒��_
 */
struct VS_INPUT
{
    float4  _Pos             : POSITION;
    float4  _BlendWeights    : BLENDWEIGHT;
    float4  _BlendIndices    : BLENDINDICES;
    float3  _Normal          : NORMAL;
	float4	_Color			: COLOR0;
    float2  _UV            : TEXCOORD0;
};

/*!
 * @brief	�o�͒��_�B
 */
struct VS_OUTPUT
{
	float4  _Pos    : POSITION;
	float4	_Color	: COLOR0;
    float3  _Normal	: TEXCOORD0;
    float2  _UV		: TEXCOORD1;
	float4  _World	: TEXCOORD2;	//���[���h���W
	float4  _LVP	: TEXCOORD3;	//���C�g(�e�p�J����)����݂��s��
	float4  _WVP	: TEXCOORD4;	//�J�������猩���s��
};

VS_OUTPUT VSMain( VS_INPUT In )
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	
	//�u�����h����{�[���̃C���f�b�N�X�B
	int4 IndexVector = D3DCOLORtoUBYTE4(In._BlendIndices);
	
	//�u�����h���[�g�B
	float BlendWeightsArray[4] = (float[4])In._BlendWeights;
    int   IndexArray[4]        = (int[4])IndexVector;
    float LastWeight = 0.0f;
    float3 pos = 0.0f;
    float3 normal = 0.0f;
	//�{�[�������[�v?
    for (int iBone = 0; iBone < g_numBone-1; iBone++)
    {
        LastWeight = LastWeight + BlendWeightsArray[iBone];
        
        pos += mul(In._Pos, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
        normal += mul(In._Normal, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
    }
    LastWeight = 1.0f - LastWeight; 

    //�H�H
	pos += (mul(In._Pos, g_mWorldMatrixArray[IndexArray[g_numBone-1]]) * LastWeight);
    normal += (mul(In._Normal, g_mWorldMatrixArray[IndexArray[g_numBone-1]]) * LastWeight);

	o._World = float4(pos.xyz, 1.0f);
	o._LVP = mul(o._World, g_LVP);				//���C�g�̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������

	//���[���h�s��
	//_Pos = mul(float4(_Pos.xyz, 1.0f), g_worldMatrix);
	//�r���[�s��
	pos = mul(float4(pos.xyz, 1.0f), g_viewMatrix);
	//�v���W�F�N�V�����s��
	o._WVP = o._Pos = mul(float4(pos.xyz, 1.0f), g_projectionMatrix);

	//�@��
	//o._Normal = mul(normal, g_rotationMatrix);	//�@�����񂷁B
	o._Normal = normal;

    o._UV = In._UV;
	o._Color = In._Color;

	return o;
}

/*!
 * @brief	�s�N�Z���V�F�[�_�[�B
 */
float4 PSMain( VS_OUTPUT In ):COLOR0
{
	float4 color = (float4)0;	//�ŏI�I�ɏo�͂���J���[
	float4 diff = (float4)0;	//���b�V���̃}�e���A��
	float3 normal = normalize(In._Normal);
	//�J���[
	if (Texflg)
	{
		diff = tex2D(g_diffuseTextureSampler, In._UV) * g_Textureblendcolor;
	}
	else
	{
		diff = g_diffuseMaterial;
	}
	diff *= g_blendcolor;
	color = diff;

	float4 light = 0.0f;

	//�G�b�W���o
	if(false)
	{
		float4 WVP = In._WVP;
		WVP.xyz /= WVP.w;		//�[�x�v�Z
		float2 shadowMapUV = float2(0.5f, -0.5f) * WVP.xy + float2(0.5f, 0.5f);
		//�ߖT4�s�N�Z���̐F
		float4 depth,depth1, depth2, depth3, depth4;
		depth = tex2D(g_ShadowSampler_0, shadowMapUV);	//��
		depth1 = tex2D(g_ShadowSampler_0, float2(shadowMapUV.x + g_TexelSize.x, shadowMapUV.y));	//�E
		depth2 = tex2D(g_ShadowSampler_0, float2(shadowMapUV.x - g_TexelSize.x, shadowMapUV.y));	//��
		depth3 = tex2D(g_ShadowSampler_0, float2(shadowMapUV.x, shadowMapUV.y + g_TexelSize.y));	//��
		depth4 = tex2D(g_ShadowSampler_0, float2(shadowMapUV.x, shadowMapUV.y - g_TexelSize.y));	//��

		//���ق��������l�𒴂����Ȃ�
		if (abs(depth.x - depth1.x) > 0.15f ||
			abs(depth.x - depth2.x) > 0.15f ||
			abs(depth.x - depth3.x) > 0.15f ||
			abs(depth.x - depth4.x) > 0.15f)
		{
			return 1;
		}
	}

	//�f�t���[�Y���C�g���v�Z�B
	for (int i = 0; i < g_LightNum; i++)
	{
		//0.0f�����Ȃ�0.0f��Ԃ�
		light.xyz += max(0.0f, -dot(normal, g_diffuseLightDirection[i].xyz))* g_diffuseLightColor[i].xyz;
		light.a = 1.0f;
	}

	//�X�y�L�����[���C�g
	if(true)
	{
		float3 spec = 0.0f;
		float3 toEyeDir = normalize(g_cameraPos.xyz - In._World);
		float3 R = -toEyeDir + 2.0f * dot(normal, toEyeDir) * normal;
		for (int i = 0; i < g_LightNum; i++)
		{
			//�X�y�L�����������v�Z����B
			//���˃x�N�g�����v�Z�B
			float3 L = -g_diffuseLightDirection[i].xyz;
			spec += g_diffuseLightColor[i] * pow(max(0.0f, dot(L, R)), 2) * g_diffuseLightColor[i].w;	//�X�y�L�������x�B
		}

		light.xyz += spec.xyz;
	}

	//�e
	if (ReceiveShadow)
	{
		sampler texSampler[3];
		texSampler[0] = g_ShadowSampler_0;
		texSampler[1] = g_ShadowSampler_0;
		texSampler[2] = g_ShadowSampler_0;

		for (int i = 0; i < 1; i++) {
			float4 posInLVP = In._LVP;
			posInLVP.xyz /= posInLVP.w;
			//uv���W�ɕϊ��B
			float2 shadowMapUV = float2(0.5f, -0.5f) * posInLVP.xy + float2(0.5f, 0.5f);
			float2 shadow_val = 1.0f;

			if (shadowMapUV.x < 0.99f && shadowMapUV.y < 0.99f && shadowMapUV.x > 0.01f && shadowMapUV.y > 0.01f) {
				shadow_val = tex2D(texSampler[i], shadowMapUV).rg;
				float depth = min(posInLVP.z, 1.0f);
				//�o���A���X�V���h�E�}�b�v�̃t���O
				if (true) {
					if (depth > shadow_val.r) {
						// ��^2
						float depth_sq = shadow_val.r * shadow_val.r;
						float variance = max(shadow_val.g - depth_sq, 0.0006f);
						float md = depth - shadow_val.r;
						float P = variance / (variance + md * md);
						light.rgb *= pow(P, 5.0f);
					}
				}
				else {
					//�e�Ȃ�
					if (depth > shadow_val.r + 0.006f) {
						//�F����
						light.rgb *= 0.5f;
					}
				}
				break;
			}
		}
	}
	//
	color.rgb *= light.rgb + g_ambientLight.rgb;
	return color;
}

//�{�`��
technique NormalRender
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMain();
	}
}


//////////////////////////////////////////////////////////////
//�e�`��p

struct VS_ShadowIN {
	float4  pos             : POSITION;
	float4  _BlendWeights    : BLENDWEIGHT;
	float4  _BlendIndices    : BLENDINDICES;
};

struct VS_ShadowOUT {
	float4	pos		: POSITION;
	float4	shadow		: TEXCOORD0;
};

VS_ShadowOUT VSShadow(VS_ShadowIN In)
{
	VS_ShadowOUT Out = (VS_ShadowOUT)0;

	//�u�����h����{�[���̃C���f�b�N�X�B
	int4 IndexVector = D3DCOLORtoUBYTE4(In._BlendIndices);

	//�u�����h���[�g�B
	float BlendWeightsArray[4] = (float[4])In._BlendWeights;
	int   IndexArray[4] = (int[4])IndexVector;
	float LastWeight = 0.0f;
	float3 pos = 0.0f;
	//�{�[�������[�v?
	for (int iBone = 0; iBone < g_numBone - 1; iBone++)
	{
		LastWeight = LastWeight + BlendWeightsArray[iBone];

		pos += mul(In.pos, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
	}
	LastWeight = 1.0f - LastWeight;

	//�e�s���������
	pos += (mul(In.pos, g_mWorldMatrixArray[IndexArray[g_numBone - 1]]) * LastWeight);	//�Ō�̂�H
	pos = mul(float4(pos.xyz, 1.0f), g_viewMatrix);
	float4 P = mul(float4(pos.xyz, 1.0f), g_projectionMatrix);
	Out.pos = P;
	Out.shadow = P;

	return Out;
}

float4 PSShadow(VS_ShadowOUT In) : COLOR0	//�����_�[�^�[�Q�b�g0�ɏo��
{
	//�[�x
	float4 depth = (float4)0;

	//�[�x�͎ˉe�ϊ��ς݂̒��_�� Z / W �ŎZ�o�ł���
	depth = In.shadow.z / In.shadow.w;

	return float4(depth.xyz, 1.0f);
}

technique Shadow
{
	pass p0
	{
		VertexShader = compile vs_2_0 VSShadow();
		PixelShader = compile ps_2_0 PSShadow();
	}
}