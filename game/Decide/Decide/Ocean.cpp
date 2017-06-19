/**
* 海クラスの実装.
*/
#include"stdafx.h"
#include"Ocean.h"
#include"fbEngine\_Object\Vertex.h"
#include"fbEngine\_Effect\EffectManager.h"
#include"fbEngine\_Object\_Component\_3D\Light.h"

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
			{ -80.0f, -80.0f },//左上
			{ 81.0f, -80.0f },//右上
			{ -80.0f, 81.0f },//左下
			{ 81.0f, 81.0f },//右下
		};

		VERTEX_NORMAL normal[] =
		{
			{ 0, 1, 0, 1 },
			{ 0, 1, 0, 1 },
			{ 0, 1, 0, 1 },
			{ 0, 1, 0, 1 },
		};

		VERTEX_TANGENT tangent[] =
		{
			{ 1, 0, 0, 1 },
			{ 1, 0, 0, 1 },
			{ 1, 0, 0, 1 },
			{ 1, 0, 0, 1 },
		};
	

		//頂点レイアウト.
		D3DVERTEXELEMENT9 elements[] = {
			{ 0, 0	, D3DDECLTYPE_FLOAT4  , D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },	// 頂点座標.
			{ 1, 0	, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD   , 0 }, // UV座標.
			{ 2, 0	, D3DDECLTYPE_FLOAT4  , D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },	// 法線ベクトル.
			{ 2, 0	, D3DDECLTYPE_FLOAT4  , D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },	// 接ベクトル.
			D3DDECL_END()
		};

		_Vertex->Initialize(fbEngine::PRIMITIVETYPE::TRIANGLESTRIP, 4);
		_Vertex->CreateVertexBuffer(position, 4, sizeof(VERTEX_POSITION), elements[0]);
		_Vertex->CreateVertexBuffer(texcoord, 4, sizeof(VERTEX_TEXCOORD), elements[1]);
		_Vertex->CreateVertexBuffer(normal, 4, sizeof(VERTEX_NORMAL), elements[2]);
		_Vertex->CreateVertexBuffer(tangent, 4, sizeof(VERTEX_TANGENT), elements[3]);
		_Vertex->CreateDeclaration();
	}

	_Effect = EffectManager::LoadEffect("Ocean.fx");

	_NormalTexture[0] = LOADTEXTURE("Ocean/Ocean2_Normal.png");
	_NormalTexture[1] = LOADTEXTURE("Ocean/Ocean3_Normal.png");

}

/**
* 初期化.
*/
void Ocean::Start()
{
	transform->SetLocalPosition(Vector3(0, 50, 0));
}

/**
* 更新.
*/
void Ocean::Update()
{
	_Wave += 0.0003f;

	static float miti = 0;

	transform->SetLocalPosition(Vector3(0, 50 + sin(miti), 0));
	miti += 0.01;

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
	
	//アルファテスト.
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//元のテクスチャはそのままのRGBA
	(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//書き込むテクスチャは(1.0f - R(GBA))計算.
	(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	_Effect->SetTechnique("Ocean");

	_Effect->Begin(0, D3DXFX_DONOTSAVESTATE);
	_Effect->BeginPass(0);

	_Effect->SetMatrix("g_WorldMatrix", &(D3DXMATRIX)transform->GetWorldMatrix());
	_Effect->SetMatrix("g_ViewMatrix",&(D3DXMATRIX)INSTANCE(GameObjectManager)->mainCamera->GetViewMat());
	_Effect->SetMatrix("g_ProjMatrix", &(D3DXMATRIX)INSTANCE(GameObjectManager)->mainCamera->GetProjectionMat());

	_Effect->SetTexture("g_Normal_1", _NormalTexture[0]->pTexture);
	_Effect->SetTexture("g_Normal_2", _NormalTexture[1]->pTexture);

	_Effect->SetFloat("g_Wave", _Wave);

	//ライト.
	{
		const int num = INSTANCE(GameObjectManager)->mainLight->GetNum();
		Vector4 dir[System::MAX_LIGHTNUM];
		Color color[System::MAX_LIGHTNUM];
		ZeroMemory(dir, sizeof(Vector4)*System::MAX_LIGHTNUM);
		const vector<DirectionalLight*>& vec = INSTANCE(GameObjectManager)->mainLight->GetLight();
		FOR(i, num)
		{
			dir[i] = vec[i]->Direction();
			color[i] = vec[i]->GetColor();
			color[i].a = 5;
		}
		//ライトの向きを転送。
		_Effect->SetValue("g_diffuseLightDirection", &dir, sizeof(Vector4)*System::MAX_LIGHTNUM);
		//ライトのカラーを転送。
		_Effect->SetValue("g_diffuseLightColor", &color, sizeof(Color)*System::MAX_LIGHTNUM);
		//ライト数セット
		_Effect->SetInt("g_LightNum", num);
		//環境光
		Vector3 ambient = INSTANCE(GameObjectManager)->mainLight->GetAmbientLight();
		_Effect->SetVector("g_ambientLight", &D3DXVECTOR4(ambient.x, ambient.y, ambient.z, 1.0f));
	}

	//カメラのポジションセット(スペキュラライト用)
	Vector3 campos = INSTANCE(GameObjectManager)->mainCamera->transform->GetPosition();
	_Effect->SetValue("g_cameraPos", &D3DXVECTOR4(campos.x, campos.y, campos.z, 1.0f), sizeof(D3DXVECTOR4));

	_Effect->SetTexture("g_EnvironmentMap", INSTANCE(SceneManager)->GetEnvironmentMap()->GetCubeTexture());

	_Effect->CommitChanges();

	_Vertex->DrawPrimitive();

	_Effect->EndPass();
	_Effect->End();

	//変更したステートを元に戻す
	(*graphicsDevice()).SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	(*graphicsDevice()).SetRenderState(D3DRS_ZENABLE, FALSE);

}
