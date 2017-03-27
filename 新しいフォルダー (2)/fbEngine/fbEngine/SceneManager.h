#pragma once

#include "Bloom.h"

class Scene;
class Sprite;
class ImageObject;
struct TEXTURE;
struct RenderTarget;

//�V�[�����Ǘ�����N���X
class SceneManager : Noncopyable
{
public:
	SceneManager();
	~SceneManager();
	//�V�[���̒ǉ�
	void Add(Scene* pAdd);

	template<class T>
	void Add()
	{
		_Scenes.push_back(new T);
	}
	//�V�[���̏��������s��
	void StartScene();
	//�V�[���̍X�V���s��
	void UpdateScene();
	//�V�[���̕`����s��
	void DrawScene();
	//�V�[���̐؂�ւ�
	Scene* ChangeScene(int key);
	Scene* ChangeScene(char* Scenename);
	template<class T>
	Scene* ChangeScene()
	{
		const char* name = typeid(T).name();
		int idx = 0;
		for each (Scene* s in _Scenes)
		{
			//���O�̈�v
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
private:
	int _NowScene;	//���݂̃V�[���̓Y����
	vector<Scene*> _Scenes;
	ImageObject* _OffScreen;
	Sprite* _Sprite;
	RenderTarget* _RT;
	static SceneManager* _Instance;

	Bloom _Bloom;
};