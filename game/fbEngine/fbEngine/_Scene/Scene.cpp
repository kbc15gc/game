#include"fbstdafx.h"
#include "Scene.h"
#include "_Object\Vertex.h"
#include "_Effect\EffectManager.h"

Vertex* Scene::_Vertex = nullptr;
Effect* Scene::_Effect = nullptr;
float Scene::_FadeAlpha = 0.0f;
float Scene::_AddPerSecA = 0.0f;
fbScene::FadeStateE Scene::_FadeState = fbScene::FadeStateE::WAIT;

Scene::Scene()
{
	if (_Vertex == nullptr)
	{
		_Vertex = new Vertex();

		//ポジション定義(フルスクリーンサイズ)
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

	if(_Effect == nullptr)
	{
		_Effect = EffectManager::LoadEffect("Scene.fx");
	}
}

Scene::~Scene()
{
}

void Scene::Fade()
{
	if (_FadeState == fbScene::FadeStateE::START)
		_FadeState = fbScene::FadeStateE::RUNNING;

	if (_FadeState == fbScene::FadeStateE::RUNNING)
	{
		//1フレーム当たりの加算量を計算
		_FadeAlpha += _AddPerSecA * Time::DeltaTime();
		//0.0f~1.0fの間に収める
		_FadeAlpha = min(1.0f, max(0.0f, _FadeAlpha));

		//フェードの板ポリゴン描画
		_DrawFade();

		//状態のチェック
		if(0.0f < _FadeAlpha && _FadeAlpha < 1.0f)
		{
			_FadeState = fbScene::FadeStateE::RUNNING;
		}
		else
		{
			//フェードインかアウトか？
			if(0 < _AddPerSecA)
			{
				//IN
				_FadeState = fbScene::FadeStateE::INEND;
			}
			else
			{
				//OUT
				_FadeState = fbScene::FadeStateE::OUTEND;
			}
		}
	}
	else if(_FadeState > fbScene::FadeStateE::END)
	{
		//次のframeには待機状態に戻る。
		_FadeState = fbScene::FadeStateE::WAIT;
	}

	if(_FadeState == fbScene::FadeStateE::WAIT)
		_DrawFade();
}

void Scene::StartFade(const bool & fade, const float & fadetime)
{
	//待機状態の時のみ開始できる
	if (_FadeState == fbScene::FadeStateE::WAIT)
	{
		//正か負か？inは+、outは-
		int NegaPosi = (fade) ? 1 : -1;
		//1秒当たりの加算量
		_AddPerSecA = (1.0f / fadetime) * NegaPosi;
		_FadeState = fbScene::FadeStateE::START;
	}
}

void Scene::_DrawFade()
{
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//テクニック設定
	_Effect->SetTechnique("SceneTech");

	_Effect->Begin(NULL, 0);
	_Effect->BeginPass(0);

	//色設定
	_Effect->SetValue("g_FadeColor", &Color::black, sizeof(Color));
	//α値
	_Effect->SetFloat("g_Alpha", _FadeAlpha);

	//この関数を呼び出すことで、データの転送が確定する。描画を行う前に一回だけ呼び出す。
	_Effect->CommitChanges();
	//板ポリ描画
	_Vertex->DrawPrimitive();

	_Effect->EndPass();
	_Effect->End();

	//変更したステートを元に戻す
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
}
