#pragma once

#include"_Effect\PostEffect\AntiAliasing.h"
#include "_Effect\PostEffect\Bloom.h"
#include"_Effect\PostEffect\DepthofField.h"
/** シャドウマップクラス. */
#include"_Effect\PreRender\ShadowMap.h"
#include"_Effect\PreRender\EnvironmentMap.h"

class Scene;
class Sprite;
class ImageObject;

//シーンを管理するクラス
class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	//シーンの追加
	void Add(Scene* pAdd);

	template<class T>
	void Add()
	{
		_Scenes.push_back(new T);
	}
	//シーンの初期化を行う
	void StartScene();
	//シーンの更新を行う
	void UpdateScene();
	//シーンの描画を行う
	void DrawScene();
	//シーンの切り替え
	Scene* ChangeScene(int key);
	Scene* ChangeScene(char* Scenename);
	template<class T>
	Scene* ChangeScene()
	{
		const char* name = typeid(T).name();
		int idx = 0;
		for each (Scene* s in _Scenes)
		{
			//名前の一致
			if (name == typeid(*s).name())
			{
				_NowScene = idx;
				return _Scenes[_NowScene];
			}
			idx++;
		}
		return nullptr;
	}
	static SceneManager* Instance()
	{
		if (_Instance == nullptr)
		{
			_Instance = new SceneManager();
		}
		return _Instance;
	}
	TEXTURE* GetOffScreenTexture();

	/**
	* シャドウマップクラスの取得.
	*/
	ShadowMap* GetShadowMap()
	{
		return &_ShadowMap;
	}

	/**
	* メインレンダリングターゲットを切り替え.
	*/
	void ToggleMainRenderTarget()
	{
		CurrentMainRT_ ^= 1;
	}

	/**
	* メインレンダリングターゲットの取得.
	*/
	RenderTarget* GetMainRenderTarget()
	{
		return _MainRT[CurrentMainRT_];
	}

	/**
	* 被写界深度クラスの取得.
	*/
	DepthofField& GetDepthofField()
	{
		return _DepthofField;
	}

	/**
	* 環境マップクラスを取得.
	*/
	EnvironmentMap* GetEnvironmentMap()
	{
		return &_EnvironmentMap;
	}

private:
	int _NowScene;	//現在のシーンの添え字
	vector<Scene*> _Scenes;
	ImageObject* _OffScreen;
	Sprite* _Sprite;

	/**	レンダリングターゲット. */
	RenderTarget* _MainRT[2];
	/** 現在使用されているメインレンダーターゲット. */
	unsigned char CurrentMainRT_ = 0;

	static SceneManager* _Instance;

	AntiAliasing _AntiAliasing;
	DepthofField _DepthofField;
	Bloom _Bloom;
	ShadowMap  _ShadowMap;

	/** 環境マップ. */
	EnvironmentMap _EnvironmentMap;

};