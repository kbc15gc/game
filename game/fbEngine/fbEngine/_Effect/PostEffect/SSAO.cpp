/**
* SSAOクラスの実装.
*/
#include"fbstdafx.h"
#include"SSAO.h"

#include"_Object\Vertex.h"
#include"..\EffectManager.h"

/**
* 作成.
*/
void SSAO::Create()
{
	if (_isEnable)
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
		_NormalRT.Create(g_FrameSize, D3DFMT_A16B16G16R16F);

		_Effect = EffectManager::LoadEffect("SSAO.fx");
	}
}

/**
* 描画.
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

		//レンダリングターゲットの変更
		INSTANCE(SceneManager)->ToggleMainRenderTarget();

		//レンダリングターゲットを設定
		INSTANCE(RenderTargetManager)->ReSetRT(0, INSTANCE(SceneManager)->GetMainRenderTarget());

		_Vertex->DrawPrimitive();

		_Effect->EndPass();
		_Effect->End();
	}
}
