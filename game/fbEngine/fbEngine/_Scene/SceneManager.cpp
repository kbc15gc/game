#include"fbstdafx.h"
#include "SceneManager.h"
#include "Scene.h"

#include "_Object\_GameObject\ImageObject.h"
#include "_Object\_GameObject\FPS.h"
#include "_Object\_Component\_Physics\Physics.h"

SceneManager* SceneManager::_Instance;


SceneManager::SceneManager():
	_NextScene(-1)
{
	_OffScreen = new ImageObject("OffScreen");
	_OffScreen->Awake();
	_Sprite = _OffScreen->GetComponent<Sprite>();
	_Sprite->Start();

	FOR(count, 2)
	{
		//レンダーターゲット作成
		_MainRT[count] = new RenderTarget();
		//大きさ二倍で作って高画質に
		//浮動小数点バッファで作成
		_MainRT[count]->Create(g_FrameSize, D3DFMT_A16B16G16R16F);
		//掛ける倍率は0.5ｆ
		_MainRT[count]->texture->Size = _MainRT[count]->texture->Size;
	}
	//レンダーターゲットのテクスチャを取得
	_Sprite->SetTexture(_MainRT[CurrentMainRT_]->texture);
	_Sprite->SetPivot(Vector2(0.0f, 0.0f));

#ifdef RELEASE_LOW
	_AntiAliasing.SetEnable(false);
	_DepthofField.SetEnable(false);
	_Bloom.SetEnable(true);
#else
	_AntiAliasing.SetEnable(true);
	_DepthofField.SetEnable(true);
	_Bloom.SetEnable(true);
#endif

	//アンチエイリアスの作成.
	_AntiAliasing.Create();
	//被写界深度の作成
	_DepthofField.Create();
	//ブルームの準備
	_Bloom.Create();
	//シャドウマップの初期化.
	_ShadowMap.Create();
	//環境マップの初期化.
	_EnvironmentMap.Create();

}

SceneManager::~SceneManager()
{
}

void SceneManager::Add(Scene* pAdd)
{
	_Scenes.push_back(pAdd);
}

void SceneManager::StartScene()
{
//#ifdef _DEBUG
	FPS* fps = INSTANCE(GameObjectManager)->AddNew<FPS>("fps", System::MAX_PRIORITY);
	//fps->transform->SetLocalPosition(Vector3(0, 30, 0));
//#endif // DEBUG

	//空描画クラスの初期化.
	_Sky = INSTANCE(GameObjectManager)->AddNew<Sky>("sky", 0);
	_Sky->SetActive(false);

	_Scenes[_NowScene]->Start();
}

void SceneManager::UpdateScene()
{
	//次のシーンが設定されている場合はシーン切り替え
	if (_NextScene >= 0 &&
		Scene::GetState() == fbScene::FadeStateE::WAIT)
		_ChangeScene();

	_Scenes[_NowScene]->Update();
	INSTANCE(GameObjectManager)->UpdateObject();
	PhysicsWorld::Instance()->Update();
	INSTANCE(GameObjectManager)->LateUpdateObject();
#ifdef _DEBUG

	INSTANCE(GameObjectManager)->DebugObject();
#endif
	//シャドウマップの更新.
	if (_Scenes[_NowScene]->GetIsShadowMap())
	{
		_ShadowMap.Update();
	}

	//環境マップの更新.
	if (_Scenes[_NowScene]->GetIsEnvironmentMap())
	{
		_EnvironmentMap.Update();
	}

}

void SceneManager::DrawScene()
{
	//事前描画(影とか深度とか輝度とか)
	INSTANCE(GameObjectManager)->PreRenderObject();

	//シャドウマップの描画.
	if (_Scenes[_NowScene]->GetIsShadowMap())
	{
		_ShadowMap.Render();
	}
	//環境マップの描画.
	if (_Scenes[_NowScene]->GetIsEnvironmentMap())
	{
		_EnvironmentMap.Render();
	}

	//0番目に設定(オフスクリーンレンダリング用)
	INSTANCE(RenderTargetManager)->ReSetRT(0, _MainRT[CurrentMainRT_]);
	(*graphicsDevice()).Clear(
		0,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_RGBA(0, 0, 0, 1),
		1.0f,
		0);
	(*graphicsDevice()).SetRenderTarget(1, _DepthofField.GetDepthRenderTarget()->buffer);
	(*graphicsDevice()).Clear(
		1,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_RGBA(0, 0,0,1),
		1.0f,
		0);
	(*graphicsDevice()).SetRenderTarget(2, _Bloom.GetLuminanceRT()->buffer);
	(*graphicsDevice()).Clear(2,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_RGBA(0, 0, 0, 1),1.0f,0);

	INSTANCE(GameObjectManager)->RenderObject();

	(*graphicsDevice()).SetRenderTarget(1, nullptr);
	(*graphicsDevice()).SetRenderTarget(2, nullptr);

	//ブルームの描画.
	_Bloom.Render();

	//被写界深度の描画.
	_DepthofField.Render();
	
	//アンチエイリアスの描画.
	_AntiAliasing.Render();

	//レンダーターゲットを元に戻す
	INSTANCE(RenderTargetManager)->BeforeRT();

	//オフスクリーンのやつ描画
	_Sprite->SetTexture(_MainRT[CurrentMainRT_]->texture);
	_Sprite->SetSize(g_WindowSize);
	_Sprite->ImageRender();
	
	INSTANCE(GameObjectManager)->PostRenderObject();
	
	//2Dとか？
	INSTANCE(GameObjectManager)->ImageRenderObject();

	//シーンのフェードのやつ
	//最前面に来るように最後に描画
	_Scenes[_NowScene]->Fade();
}

Scene* SceneManager::ChangeScene(int key, bool fade)
{
	if (fade)
		//フェードが入る。
		Scene::StartFade(true);
	//次のシーンを
	_NextScene = key;
	return _Scenes[_NextScene];
}

Scene* SceneManager::ChangeScene(char * Scenename, bool fade)
{
	//クラス名
	char* classname = new char[128];
	strcpy_s(classname, strlen("class ")+1, "class ");
	strcat_s(classname, strlen(Scenename) + strlen(classname)+1, Scenename);
	int idx = 0;
	for each (Scene* s in _Scenes)
	{
		//名前の一致
		if (strcmp(classname, typeid(*s).name()) == 0)
		{
			//シーン切り替え
			return ChangeScene(idx,fade);
		}
		idx++;
	}
	return nullptr;
}

TEXTURE* SceneManager::GetOffScreenTexture()
{
	return _MainRT[CurrentMainRT_]->texture;
}

void SceneManager::_ChangeScene()
{
	//シーンの添え字切り替え
	_NowScene = _NextScene;
	_NextScene = -1;
	//オブジェクトリリース
	INSTANCE(GameObjectManager)->Release();
	//初期化する
	SceneManager::StartScene();
	//初期化が終わったなら、フェードが明ける
	Scene::StartFade(false);
}
