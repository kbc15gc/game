/**
* SSAO�N���X�̎���.
*/
#include"fbstdafx.h"
#include"SSAO.h"

#include"_Object\Vertex.h"
#include"..\EffectManager.h"

/**
* �쐬.
*/
void SSAO::Create()
{
	if (_isEnable)
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
		_NormalRT.Create(g_FrameSize, D3DFMT_A16B16G16R16F);

		_Effect = EffectManager::LoadEffect("SSAO.fx");
	}
}

/**
* �`��.
*/
void SSAO::Render()
{
	if (_isEnable)
	{
		_Effect->SetTechnique("RenderSSAO");

		_Effect->Begin(0, D3DXFX_DONOTSAVESTATE);
		_Effect->BeginPass(0);

		_Effect->SetTexture("g_ColorTex", INSTANCE(SceneManager)->GetMainRenderTarget()->texture->pTexture);
		_Effect->SetTexture("g_DepthTex", INSTANCE(SceneManager)->GetDepthRT()->texture->pTexture);
		_Effect->SetTexture("g_NormalTex", _NormalRT.texture->pTexture);

		float texSize[] =
		{
			(float)_NormalRT.texture->Size.x,
			(float)_NormalRT.texture->Size.y,
		};

		_Effect->SetValue("g_TexSize", texSize, sizeof(texSize));

		_Effect->CommitChanges();

		//�����_�����O�^�[�Q�b�g�̕ύX
		INSTANCE(SceneManager)->ToggleMainRenderTarget();

		//�����_�����O�^�[�Q�b�g��ݒ�
		INSTANCE(RenderTargetManager)->ReSetRT(0, INSTANCE(SceneManager)->GetMainRenderTarget());

		_Vertex->DrawPrimitive();

		_Effect->EndPass();
		_Effect->End();
	}
}
