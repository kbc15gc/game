/**
* �u���[�N���X�̎���.
*/
#include"fbstdafx.h"
#include"Blur.h"
#include "_Object\Vertex.h"
#include "EffectManager.h"

/**
* �쐬.
*
* @param w		����.
* @param h		�c��.
* @param tex	�e�N�X�`���[.	.
*/
void Blur::Create(int w, int h,TEXTURE* tex)
{
	//�\�[�X�e�N�X�`����ݒ�.
	_SrcTexture = tex;

	D3DSURFACE_DESC desc;
	_SrcTexture->pTexture->GetLevelDesc(0, &desc);

	//�T�C�Y���R�s�[
	_SrcTextureSize[0] = w;
	_SrcTextureSize[1] = h;

	//�u���[�T�C�Y���쐬
	int  size[2][2]
	{
		{ w >> 1, h },
		{ w >> 1, h >> 1 },
	};

	//�����_�����O�^�[�Q�b�g�̍쐬
	for (int i = 0; i < 2; i++)
	{
		_BlurRT[i].Create(Vector2(size[i][0], size[i][1]), desc.Format, D3DFMT_D16);
	}

	//�G�t�F�N�g�̃��[�h
	_Effect = EffectManager::LoadEffect("Blur.fx");

	InitVertex();
}

void Blur::Create(int w, int h, _D3DFORMAT fmt)
{
	//�T�C�Y���R�s�[
	_SrcTextureSize[0] = w;
	_SrcTextureSize[1] = h;

	//�u���[�T�C�Y���쐬
	int  size[2][2]
	{
		{ w >> 1, h },
		{ w >> 1, h >> 1 },
	};

	//�����_�����O�^�[�Q�b�g�̍쐬
	for (int i = 0; i < 2; i++)
	{
		_BlurRT[i].Create(Vector2(size[i][0], size[i][1]), fmt, D3DFMT_D16);
	}

	//�G�t�F�N�g�̃��[�h
	_Effect = EffectManager::LoadEffect("Blur.fx");

	InitVertex();
}

/**
* �`��.
*/
void Blur::Render()
{
	//X�u���[�B
	{
		//�����_�����O�^�[�Q�b�g���u���[�ɐݒ�
		INSTANCE(RenderTargetManager)->SetRT(0,&_BlurRT[0]);

		float size[2] = {
			static_cast<float>(_SrcTextureSize[0]),
			static_cast<float>(_SrcTextureSize[1])
		};

		float texelOffset[] = {
			0.5f / _BlurRT[0].texture->Size.x,
			0.5f / _BlurRT[0].texture->Size.y
		};

		_Effect->SetTechnique("XBlur");

		_Effect->Begin(0, D3DXFX_DONOTSAVESTATE);
		_Effect->BeginPass(0);

		_Effect->SetTexture("g_Blur", _SrcTexture->pTexture);
		_Effect->SetValue("g_TexSize", size, sizeof(size));
		_Effect->SetValue("g_texelOffset", texelOffset, sizeof(texelOffset));

		_Effect->CommitChanges();

		//�v���~�e�B�u�̕`��
		_Vertex->DrawPrimitive();
		
		_Effect->EndPass();
		_Effect->End();

	}
	//Y�u���[�B
	{
		//�����_�����O�^�[�Q�b�g���u���[�ɐݒ�
		INSTANCE(RenderTargetManager)->SetRT(0, &_BlurRT[1]);
		

		float size[2] =
		{
			static_cast<float>(_BlurRT[1].texture->Size.x),
			static_cast<float>(_BlurRT[1].texture->Size.y)
		};

		float texelOffset[] = {
			0.5f / _BlurRT[0].texture->Size.x,
			0.5f / _BlurRT[0].texture->Size.y
		};

		_Effect->SetTechnique("YBlur");

		_Effect->Begin(0, D3DXFX_DONOTSAVESTATE);
		_Effect->BeginPass(0);
		
		_Effect->SetTexture("g_Blur", _BlurRT[0].texture->pTexture);
		_Effect->SetValue("g_TexSize", size, sizeof(size));
		_Effect->SetValue("g_texelOffset", texelOffset, sizeof(texelOffset));

		_Effect->CommitChanges();

		//�v���~�e�B�u�̕`��
		_Vertex->DrawPrimitive();
		
		_Effect->EndPass();
		_Effect->End();

	}
}

void Blur::InitVertex()
{
	if (_Vertex == nullptr)
	{
		_Vertex = new Vertex();

		//�|�W�V������`
		VERTEX_POSITION position[] = {
			{ -1.0f, -1.0f, 0.0f, 1.0f },//����
			{ -1.0f, 1.0f, 0.0f, 1.0f },//����
			{ 1.0f, -1.0f, 0.0f, 1.0f },//�E��
			{ 1.0f, 1.0f, 0.0f, 1.0f },//�E��
		};
		//UV��`
		VERTEX_TEXCOORD texcoord[] = {
			{ 0.0f, 1.0f },//����
			{ 0.0f, 0.0f },//����
			{ 1.0f, 1.0f },//�E��
			{ 1.0f, 0.0f },//�E��
		};

		//���_�錾(���_���ǂ̂悤�ɍ\������Ă��邩)
		D3DVERTEXELEMENT9 elements[] = {
			{ 0, 0              , D3DDECLTYPE_FLOAT4  , D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, // ���_���W
			{ 1, 0              , D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD   , 0 }, // UV
			D3DDECL_END()
		};

		_Vertex->Initialize(fbEngine::PRIMITIVETYPE::TRIANGLESTRIP, 4);
		_Vertex->CreateVertexBuffer(position, 4, sizeof(VERTEX_POSITION), elements[0]);
		_Vertex->CreateVertexBuffer(texcoord, 4, sizeof(VERTEX_TEXCOORD), elements[1]);
		_Vertex->CreateDeclaration();
	}
}
