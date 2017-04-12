/*!
 *@brief	3D���f���̕`��
 */
#define MAX_LIGHTNUM 4
bool Texflg;							//�e�N�X�`��
bool Spec;	//�X�y�L�����t���O

float4x4 g_rotationMatrix;				//��]�s��B�@������]�����邽�߂ɕK�v�ɂȂ�B���C�e�B���O����Ȃ�K�{�B
float4x4 g_worldMatrix;					//���[���h�s��B
float4x4 g_viewMatrix;					//�r���[�s��B
float4x4 g_projectionMatrix;			//�v���W�F�N�V�����s��B

int g_LightNum;									//���C�g�̐�
float4	g_diffuseLightDirection[MAX_LIGHTNUM];	//�f�B�t���[�Y���C�g�̕����B
float4	g_diffuseLightColor[MAX_LIGHTNUM];		//�f�B�t���[�Y���C�g�̃J���[�B
float4	g_ambientLight;								//�����B

float4 g_cameraPos;						//�J�����̈ʒu

float4  g_Color;		//�J���[
float4  g_blendColor;

texture g_Texture;				//�e�N�X�`���B
sampler g_TextureSampler = 
sampler_state
{
	Texture = <g_Texture>;
    MipFilter = NONE;
    MinFilter = NONE;
    MagFilter = NONE;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

//���_���\����

struct VS_INPUT{
	float4	pos		: POSITION;
	float2	uv		: TEXCOORD0;
	float3	normal	: NORMAL;
};

struct VS_OUTPUT{
	float4	pos		: POSITION;
	float2	uv		: TEXCOORD0;
	float3	normal	: TEXCOORD1;
	float4  world	: TEXCOORD2;
	float4	wvp		: TEXCOORD3;
};


/*!
 *@brief	���_�V�F�[�_�[�B
 */
VS_OUTPUT VSMain(VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	float4 pos;
	pos = mul( In.pos, g_worldMatrix );		//���f���̃��[�J����Ԃ��烏�[���h��Ԃɕϊ��B
	
	Out.world = pos;
	
	pos = mul( pos, g_viewMatrix );			//���[���h��Ԃ���r���[��Ԃɕϊ��B
	pos = mul( pos, g_projectionMatrix );	//�r���[��Ԃ���ˉe��Ԃɕϊ��B

	Out.pos = pos;
	Out.wvp = Out.pos;
	Out.uv = In.uv;
	Out.normal = mul(normalize(In.normal), g_rotationMatrix);	//�@�����񂷁B
	
	return Out;
}

struct PS_OUTPUT {
	//�F
	float4 color	: COLOR0;
	//�[�x
	float4 depth	: COLOR1;
	//�P�x
	float4 brightness	: COLOR2;
};

/*!
 *@brief	�s�N�Z���V�F�[�_�[�B
 */
PS_OUTPUT PSMain(VS_OUTPUT In)
{
	PS_OUTPUT Out = (PS_OUTPUT)0;
	//�e�N�X�`���̗L��
	if (Texflg){
		//uv�ɒ���t��
		Out.color = tex2D(g_TextureSampler, In.uv);
	}
	else
	{
		//�}�e���A���̃J���[�ɂ���B
		Out.color = g_Color;
	}

	Out.color *= g_blendColor;

	//�f�t���[�Y���C�g���v�Z�B
	{
		float4 dif = 0.0f;
		for (int i = 0; i < g_LightNum; i++)
		{
			//0.0f�����Ȃ�0.0f��Ԃ�
			dif.xyz += max(0.0f, dot(In.normal.xyz, -g_diffuseLightDirection[i].xyz))
				* g_diffuseLightColor[i].xyz;
		}

		dif += g_ambientLight;

		Out.color.xyz *= dif.xyz;
	}

	//�X�y�L�����[���C�g
	if(Spec)
	{
		float3 spec = 0.0f;
		float3 toEyeDir = normalize(g_cameraPos.xyz - In.world);
		float3 R = -toEyeDir + 2.0f * dot(In.normal, toEyeDir) * In.normal;

		for (int i = 0; i < g_LightNum; i++)
		{
			//�X�y�L�����������v�Z����B
			//���˃x�N�g�����v�Z�B
			float3 L = -g_diffuseLightDirection[i].xyz;
			spec += g_diffuseLightColor[i] * pow(max(0.0f, dot(L, R)), 10) * g_diffuseLightColor[i].w * 100;	//�X�y�L�������x�B
		}
		Out.color.xyz += spec.xyz;
	}
	//�[�x�v�Z
	//�[�x�͎ˉe�ϊ��ς݂̒��_�� Z / W �ŎZ�o�ł���
	Out.depth = In.wvp.z / In.wvp.w;
	Out.brightness = float4(1, 1, 1, 1);

	return Out;
}

////////////////////////////
//�e�N�j�b�N

//���ʂɕ`�悷��p
technique NormalRender
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMain();
	}
}

technique PreRender
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMain();
	}
}