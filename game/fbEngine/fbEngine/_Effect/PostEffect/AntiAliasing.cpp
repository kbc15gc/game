/**
* アンチエイリアスクラスの実装.
*/
#include"fbstdafx.h"
#include"AntiAliasing.h"
#include"_Object\Vertex.h"
#include"..\EffectManager.h"

/**
* 作成.
*/
void AntiAliasing::Create()
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
}

/**
* 描画.
*/
void AntiAliasing::Render()
{
	if (_isEnable)
	{
		//有効.
		_Effect = EffectManager::LoadEffect("FXAA.fx");

		float texSize[2] =
		{
			INSTANCE(SceneManager)->GetMainRenderTarget()->texture->Size.x,
			INSTANCE(SceneManager)->GetMainRenderTarget()->texture->Size.y,
		};

		_Effect->SetTechnique("FXAA");

		_Effect->Begin(0, D3DXFX_DONOTSAVESTATE);
		_Effect->BeginPass(0);

		_Effect->SetTexture("g_Texture", INSTANCE(SceneManager)->GetMainRenderTarget()->texture->pTexture);
		_Effect->SetValue("g_TexSize", texSize, sizeof(texSize));

		_Effect->CommitChanges();

		//レンダリングターゲットの変更
		INSTANCE(SceneManager)->ToggleMainRenderTarget();

		//レンダリングターゲットを設定
		INSTANCE(RenderTargetManager)->ReSetRT(0,INSTANCE(SceneManager)->GetMainRenderTarget());

		_Vertex->DrawPrimitive();

		_Effect->EndPass();
		_Effect->End();

	}
}
