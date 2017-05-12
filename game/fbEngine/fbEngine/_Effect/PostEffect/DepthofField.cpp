/**
* DOF(被写界深度)クラスの実装.
*/
#include"fbstdafx.h"
#include"DepthofField.h"
#include"_Effect\EffectManager.h"
#include "_Object\Vertex.h"

/**
* 作成.
*/
void DepthofField::Create()
{
	
	if (_Vertex == nullptr)
	{
		_Vertex = new Vertex();

		//ポジション定義
		VERTEX_POSITION position[] = {
			{ -1.0f, -1.0f, 0.0f, 1.0f },//左下
			{ -1.0f, 1.0f, 0.0f, 1.0f },//左上
			{ 1.0f, -1.0f, 0.0f, 1.0f },//右下
			{ 1.0f, 1.0f, 0.0f, 1.0f },//右上
		};
		//UV定義
		VERTEX_TEXCOORD texcoord[] = {
			{ 0.0f, 1.0f },//左下
			{ 0.0f, 0.0f },//左上
			{ 1.0f, 1.0f },//右下
			{ 1.0f, 0.0f },//右上
		};

		//頂点宣言(頂点がどのように構成されているか)
		D3DVERTEXELEMENT9 elements[] = {
			{ 0, 0              , D3DDECLTYPE_FLOAT4  , D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, // 頂点座標
			{ 1, 0              , D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD   , 0 }, // UV
			D3DDECL_END()
		};

		_Vertex->Initialize(fbEngine::PRIMITIVETYPE::TRIANGLESTRIP, 4);
		_Vertex->CreateVertexBuffer(position, 4, sizeof(VERTEX_POSITION), elements[0]);
		_Vertex->CreateVertexBuffer(texcoord, 4, sizeof(VERTEX_TEXCOORD), elements[1]);
		_Vertex->CreateDeclaration();
	}
	
	//16bit。
	_DepthRT.Create(g_WindowSize,D3DFMT_R16F);

	_BlurForward.Create(g_WindowSize.x, g_WindowSize.y, D3DFMT_A16B16G16R16F);
	_BlurBack.Create(g_WindowSize.x, g_WindowSize.y, D3DFMT_A16B16G16R16F);

	_Effect = EffectManager::LoadEffect("DepthofField.fx");

}

/**
* 描画.
*/
void DepthofField::Render()
{
	//被写体との距離から、こちらで計算する。
	static float CoC = 0.033f;			//許容錯乱円(単位はmm)
	float forwardDof = (CoC * _F * _Pint * _Pint) / (_FocalLength * _FocalLength + CoC * _F * _Pint);
	float backDof = (CoC * _F * _Pint * _Pint) / (_FocalLength * _FocalLength - CoC * _F * _Pint);
	//手前ボケ、奥ボケ、ピントをm単位に変更してGPUに送る
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

	//ボケ画像を作成する。
	//手前ボケ
	{
		_BlurForward.SetTexture(SceneTextrue);
		_BlurForward.Render();
	}
	//奥ボケ
	{
		_BlurBack.SetTexture(SceneTextrue);
		_BlurBack.Render();
	}
	
	//合成。
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
		//レンダリングターゲットを設定
		INSTANCE(RenderTargetManager)->ReSetRT(0, INSTANCE(SceneManager)->GetMainRenderTarget());

		_Vertex->DrawPrimitive();

		_Effect->EndPass();
		_Effect->End();
	}

}

/**
* 解放.
*/
void DepthofField::Release()
{
}
