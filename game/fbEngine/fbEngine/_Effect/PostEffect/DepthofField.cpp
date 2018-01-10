/**
* DOF(��ʊE�[�x)�N���X�̎���.
*/
#include"fbstdafx.h"
#include"DepthofField.h"
#include"_Effect\EffectManager.h"
#include "_Object\Vertex.h"

/**
* �쐬.
*/
void DepthofField::Create()
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

		Vector2 size = g_FrameSize;

		//16bit�B
		_DepthRT.Create(size, D3DFMT_R16F);

		_BlurForward.Create(size.x, size.y, D3DFMT_A16B16G16R16F);
		_BlurForward.SetBlurPower(20.0f);
		_BlurForward.SetUseWeights(GaussianBlur::Weight_8);

		_BlurBack.Create(size.x, size.y, D3DFMT_A16B16G16R16F);
		_BlurBack.SetBlurPower(1.0f);
		_BlurBack.SetUseWeights(GaussianBlur::Weight_8);

		_Effect = EffectManager::LoadEffect("DepthofField.fx");

		_CombineRenderTarget.Create(size, D3DFMT_A16B16G16R16F);
	}
}

/**
* �`��.
*/
void DepthofField::Render()
{
	if (_isEnable)
	{
		//��ʑ̂Ƃ̋�������A������Ōv�Z����B
		static float CoC = 0.033f;			//���e�����~(�P�ʂ�mm)
		float forwardDof = (CoC * _F * _Pint * _Pint) / (_FocalLength * _FocalLength + CoC * _F * _Pint);
		float backDof = (CoC * _F * _Pint * _Pint) / (_FocalLength * _FocalLength - CoC * _F * _Pint);
		if (backDof < 0.0f)
		{
			return;
		}
		//��O�{�P�A���{�P�A�s���g��m�P�ʂɕύX����GPU�ɑ���
		float dofParam[] = {
			forwardDof / 1000.0f,
			backDof / 1000.0f,
			_Pint / 1000.0f
		};

		TEXTURE* SceneTextrue = INSTANCE(SceneManager)->GetOffScreenTexture();

		//�{�P�摜���쐬����B
		//��O�{�P
		{
			_BlurForward.SetSrcTexture(*SceneTextrue);
			_BlurForward.Render();
		}
		//���{�P
		{
			_BlurBack.SetSrcTexture(*SceneTextrue);
			_BlurBack.Render();
		}

		//��O�{�P�Ɖ��{�P�������B
		{
			INSTANCE(RenderTargetManager)->ReSetRT(0, &_CombineRenderTarget);

			_Effect->SetTechnique("CombineBackForwardBoke");
			_Effect->Begin(0, D3DXFX_DONOTSAVESTATE);
			_Effect->BeginPass(0);

			_Effect->SetValue("g_DofParam", dofParam, sizeof(dofParam));
			_Effect->SetTexture("g_Depth", _DepthRT.texture->pTexture);
			_Effect->SetTexture("g_BlurBack", _BlurBack.GetTexture()->pTexture);
			_Effect->SetTexture("g_BlurForward", _BlurForward.GetTexture()->pTexture);

			float texSize[] =
			{
				(float)_DepthRT.texture->Size.x,
				(float)_DepthRT.texture->Size.y,
			};

			_Effect->SetValue("g_SceneTexSize", texSize, sizeof(texSize));

			_Effect->CommitChanges();

			_Vertex->DrawPrimitive();

			_Effect->EndPass();
			_Effect->End();
		}

		//�{�P�摜�ƃV�[��������
		{

			INSTANCE(RenderTargetManager)->SetRT(0, INSTANCE(SceneManager)->GetMainRenderTarget());

			_Effect->SetTechnique("TransformedPrim");
			_Effect->Begin(0, D3DXFX_DONOTSAVESTATE);
			_Effect->BeginPass(0);

			float offset[] = {
				0.5f / static_cast<float>(INSTANCE(SceneManager)->GetMainRenderTarget()->texture->Size.x),
				0.5f / static_cast<float>(INSTANCE(SceneManager)->GetMainRenderTarget()->texture->Size.y),
			};

			_Effect->SetValue("g_TexelOffset", offset, sizeof(offset));
			_Effect->SetTexture("g_Tex", _CombineRenderTarget.texture->pTexture);
			_Effect->CommitChanges();

			(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			(*graphicsDevice()).SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
			(*graphicsDevice()).SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
			(*graphicsDevice()).SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ZERO);

			_Vertex->DrawPrimitive();

			_Effect->EndPass();
			_Effect->End();

			(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

			(*graphicsDevice()).SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, FALSE);
			(*graphicsDevice()).SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO);
			(*graphicsDevice()).SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
		}
	}
}

/**
* ���.
*/
void DepthofField::Release()
{
}
