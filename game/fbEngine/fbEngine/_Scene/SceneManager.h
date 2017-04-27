#pragma once

#include "_Effect\Bloom.h"
/** シャドウマップクラス. */
#include"_Effect\PreRender\ShadowMap.h"

class Scene;
struct TEXTURE;
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

private:
	int _NowScene;	//現在のシーンの添え字
	vector<Scene*> _Scenes;
	ImageObject* _OffScreen;
	Sprite* _Sprite;
	RenderTarget* _RT;
	static SceneManager* _Instance;

	Bloom _Bloom;
	ShadowMap  _ShadowMap;
};