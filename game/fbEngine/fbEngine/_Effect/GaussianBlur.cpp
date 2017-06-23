/**
* �K�E�V�A���u���[�N���X�̎���.
*/
#include"fbstdafx.h"
#include"GaussianBlur.h"
#include"EffectManager.h"
#include"_Object\Vertex.h"

namespace {
	int weights_tbl[] = {
		2,4,6,8
	};
}

/**
* ������.
*/
void GaussianBlur::Create(int w, int h, const TEXTURE & srcTexture)
{
	//�\�[�X�e�N�X�`����ݒ�.
	_SrcTexture = &srcTexture;

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
	_Effect = EffectManager::LoadEffect("GaussianBlur.fx");

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


/**
* ������.
*/
void GaussianBlur::Create(int w, int h, _D3DFORMAT fmt)
{
	_SrcTexture = nullptr;

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
	_Effect = EffectManager::LoadEffect("GaussianBlur.fx");

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

/**
* �`��.
*/
void GaussianBlur::Render()
{
	UpdateWeight(_BlurPower);

	//X�u���[�B
	{
		
		//�����_�����O�^�[�Q�b�g���u���[�ɐݒ�
		INSTANCE(RenderTargetManager)->SetRT(0, &_BlurRT[0]);

		float size[2] =
		{
			static_cast<float>(_SrcTextureSize[0]),
			static_cast<float>(_SrcTextureSize[1])
		};

		float texelOffset[] = {
			0.5f / _BlurRT[0].texture->Size.x,
			0.5f / _BlurRT[0].texture->Size.y
		};

		float offset[] = {
			16.0f / static_cast<float>(_SrcTextureSize[0]),
			0.0f
		};

		const char* tecNameTbl[] = {
			"TransformedPrimGBlurX_2",
			"TransformedPrimGBlurX_4",
			"TransformedPrimGBlurX_6",
			"TransformedPrimGBlurX_8"
		};

		_Effect->SetTechnique(tecNameTbl[_WeightType]);

		_Effect->Begin(0, D3DXFX_DONOTSAVESTATE);
		_Effect->BeginPass(0);

		_Effect->SetTexture("g_Blur", _SrcTexture->pTexture);
		_Effect->SetValue("g_texSize", size, sizeof(size));
		_Effect->SetValue("g_texelOffset", texelOffset, sizeof(texelOffset));
		_Effect->SetValue("g_offset", offset, sizeof(offset));
		_Effect->SetValue("g_weight", _Weight, sizeof(_Weight));

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
			static_cast<float>(_BlurRT[0].texture->Size.x),
			static_cast<float>(_BlurRT[0].texture->Size.y)
		};

		float texelOffset[] = {
			0.5f / _BlurRT[1].texture->Size.x,
			0.5f / _BlurRT[1].texture->Size.y
		};

		float offset[] = {
			0.0f,
			16.0f / _BlurRT[0].texture->Size.y,
		};

		const char* tecNameTbl[] = {
			"TransformedPrimGBlurY_2",
			"TransformedPrimGBlurY_4",
			"TransformedPrimGBlurY_6",
			"TransformedPrimGBlurY_8"
		};

		_Effect->SetTechnique(tecNameTbl[_WeightType]);

		_Effect->Begin(0, D3DXFX_DONOTSAVESTATE);
		_Effect->BeginPass(0);

		_Effect->SetTexture("g_Blur", _BlurRT[0].texture->pTexture);
		_Effect->SetValue("g_texSize", size, sizeof(size));
		_Effect->SetValue("g_texelOffset", texelOffset, sizeof(texelOffset));
		_Effect->SetValue("g_offset", offset, sizeof(offset));
		_Effect->SetValue("g_weight", _Weight, sizeof(_Weight));

		_Effect->CommitChanges();

		//�v���~�e�B�u�̕`��
		_Vertex->DrawPrimitive();

		_Effect->EndPass();
		_Effect->End();
	}
}

/**
* �d�݂��v�Z.
*/
void GaussianBlur::UpdateWeight(float dispersion)
{
	float total = 0;
	for (int i = 0; i < weights_tbl[_WeightType]; i++) {
		_Weight[i] = expf(-0.5f*(float)(i*i) / dispersion);
		if (i == 0) {
			total += _Weight[i];
		}
		else {
			total += 2.0f * _Weight[i];
		}
	}
	// �K�i��
	for (int i = 0; i < weights_tbl[_WeightType]; i++) {
		_Weight[i] /= total;
	}
}