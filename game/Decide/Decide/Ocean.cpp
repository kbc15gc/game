/**
* 海クラスの実装.
*/
#include"stdafx.h"
#include"Ocean.h"
#include"fbEngine\_Object\Vertex.h"
#include"fbEngine\_Effect\EffectManager.h"

/**
* 早めの初期化.
*/
void Ocean::Awake()
{
	if (_Vertex == nullptr)
	{
		_Vertex = new Vertex();

		//ポジション定義
		VERTEX_POSITION position[] =
		{
			{ -10000.0f, 0.0f, -10000.0f, 1.0f },//左下
			{ 10000.0f, 0.0f, -10000.0f, 1.0f },//右下
			{ -10000.0f, 0.0f, 10000.0f, 1.0f },//左上
			{ 10000.0f, 0.0f, 10000.0f, 1.0f },//右上
		};
		//UV定義
		VERTEX_TEXCOORD texcoord[] = 
		{
			{ -9.0f, -9.0f },//左上
			{ 10.0f, -9.0f },//右上
			{ -9.0f, 10.0f },//左下
			{ 10.0f, 10.0f },//右下
		};

		//頂点レイアウト.
		D3DVERTEXELEMENT9 elements[] = {
			{ 0, 0	, D3DDECLTYPE_FLOAT4  , D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },	// 頂点座標.
			{ 1, 0	, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD   , 0 }, // UV座標.
			D3DDECL_END()
		};

		_Vertex->Initialize(fbEngine::PRIMITIVETYPE::TRIANGLESTRIP, 4);
		_Vertex->CreateVertexBuffer(position, 4, sizeof(VERTEX_POSITION), elements[0]);
		_Vertex->CreateVertexBuffer(texcoord, 4, sizeof(VERTEX_TEXCOORD), elements[1]);
		_Vertex->CreateDeclaration();
	}

	_Effect = EffectManager::LoadEffect("Ocean.fx");

	_Texture = LOADTEXTURE("Ocean.png");

}

/**
* 初期化.
*/
void Ocean::Start()
{
	transform->SetLocalPosition(Vector3(374, 69, -1275));
}

/**
* 更新.
*/
void Ocean::Update()
{
	//トランスフォームの更新.
	transform->UpdateTransform();
}

/**
* 描画.
*/
void Ocean::Render()
{
	
	//両面描画.
	(*graphicsDevice()).SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	(*graphicsDevice()).SetRenderState(D3DRS_ZENABLE, TRUE);
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	_Effect->SetTechnique("Ocean");

	_Effect->Begin(0, D3DXFX_DONOTSAVESTATE);
	_Effect->BeginPass(0);

	_Effect->SetMatrix("g_WorldMatrix", &(D3DXMATRIX)transform->GetWorldMatrix());
	_Effect->SetMatrix("g_ViewMatrix",&(D3DXMATRIX)INSTANCE(GameObjectManager)->mainCamera->GetViewMat());
	_Effect->SetMatrix("g_ProjMatrix", &(D3DXMATRIX)INSTANCE(GameObjectManager)->mainCamera->GetProjectionMat());

	_Effect->SetTexture("g_Texture", _Texture->pTexture);

	_Effect->CommitChanges();

	_Vertex->DrawPrimitive();

	_Effect->EndPass();
	_Effect->End();

	//変更したステートを元に戻す
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
}
