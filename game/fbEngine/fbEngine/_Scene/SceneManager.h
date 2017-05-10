#pragma once

#include"_Effect\PostEffect\AntiAliasing.h"
#include "_Effect\PostEffect\Bloom.h"
/** �V���h�E�}�b�v�N���X. */
#include"_Effect\PreRender\ShadowMap.h"

class Scene;
class Sprite;
class ImageObject;

//�V�[�����Ǘ�����N���X
class SceneManager
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

	/**
	* �V���h�E�}�b�v�N���X�̎擾.
	*/
	ShadowMap* GetShadowMap()
	{
		return &_ShadowMap;
	}

	/**
	* ���C�������_�����O�^�[�Q�b�g��؂�ւ�.
	*/
	void ToggleMainRenderTarget()
	{
		CurrentMainRT_ ^= 1;
	}

	/**
	* ���C�������_�����O�^�[�Q�b�g�̎擾.
	*/
	RenderTarget* GetMainRenderTarget()
	{
		return _MainRT[CurrentMainRT_];
	}

private:
	int _NowScene;	//���݂̃V�[���̓Y����
	vector<Scene*> _Scenes;
	ImageObject* _OffScreen;
	Sprite* _Sprite;

	/**	�����_�����O�^�[�Q�b�g. */
	RenderTarget* _MainRT[2];
	/** ���ݎg�p����Ă��郁�C�������_�[�^�[�Q�b�g. */
	unsigned char CurrentMainRT_ = 0;

	static SceneManager* _Instance;

	AntiAliasing _AntiAliasing;
	Bloom _Bloom;
	ShadowMap  _ShadowMap;
};