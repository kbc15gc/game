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
	
	//16bit�B
	_DepthRT.Create(g_WindowSize,D3DFMT_R16F);

	_BlurForward.Create(g_WindowSize.x, g_WindowSize.y, D3DFMT_A16B16G16R16F);
	_BlurBack.Create(g_WindowSize.x, g_WindowSize.y, D3DFMT_A16B16G16R16F);

	_Effect = EffectManager::LoadEffect("DepthofField.fx");

}

/**
* �`��.
*/
void DepthofField::Render()
{
	//��ʑ̂Ƃ̋�������A������Ōv�Z����B
	static float CoC = 0.033f;			//���e�����~(�P�ʂ�mm)
	float forwardDof = (CoC * _F * _Pint * _Pint) / (_FocalLength * _FocalLength + CoC * _F * _Pint);
	float backDof = (CoC * _F * _Pint * _Pint) / (_FocalLength * _FocalLength - CoC * _F * _Pint);
	//��O�{�P�A���{�P�A�s���g��m�P�ʂɕύX����GPU�ɑ���
	//float dofParam[] = {
	//	forwardDof / 1000.0f,
	//	backDof / 1000.0f,
	//	_Pint / 1000.0f
	//};
	float dofParam[] = {
		forwardDof / 1000.0f,
		backDof / 1000.0f,
		_Pint / 1000.0f
	};

	TEXTURE* SceneTextrue = INSTANCE(SceneManager)->GetOffScreenTexture();

	//�{�P�摜���쐬����B
	//��O�{�P
	{
		_BlurForward.SetTexture(SceneTextrue);
		_BlurForward.Render();
	}
	//���{�P
	{
		_BlurBack.SetTexture(SceneTextrue);
		_BlurBack.Render();
	}
	
	//�����B
	{
		_Effect->SetTechnique("DOF");
		_Effect->Begin(0, D3DXFX_DONOTSAVESTATE);
		_Effect->BeginPass(0);

		_Effect->SetValue("g_DofParam", dofParam, sizeof(dofParam));
		_Effect->SetTexture("g_Scene", SceneTextrue->pTexture);
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

		INSTANCE(SceneManager)->ToggleMainRenderTarget();
		//�����_�����O�^�[�Q�b�g��ݒ�
		INSTANCE(RenderTargetManager)->ReSetRT(0, INSTANCE(SceneManager)->GetMainRenderTarget());

		_Vertex->DrawPrimitive();

		_Effect->EndPass();
		_Effect->End();
	}

}

/**
* ���.
*/
void DepthofField::Release()
{
}
