#pragma once

#include"_Effect\PostEffect\AntiAliasing.h"
#include "_Effect\PostEffect\Bloom.h"
#include"_Effect\PostEffect\DepthofField.h"
/** �V���h�E�}�b�v�N���X. */
#include"_Effect\PreRender\ShadowMap.h"
#include"_Effect\PreRender\EnvironmentMap.h"

#include"_Nature\Sky.h"

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
	//�V�[���̐؂�ւ��O������Ăяo���p
	//�V�[���̓Y�����A�t�F�[�h���邩�ǂ����H
	Scene* ChangeScene(int key, bool fade = false);
	Scene* ChangeScene(char* Scenename, bool fade = false);
	template<class T>
	Scene* ChangeScene(bool fade = false)
	{
		const char* name = typeid(T).name();
		int idx = 0;
		for each (Scene* s in _Scenes)
		{
			//���O�̈�v
			if (name == typeid(*s).name())
			{
				//�V�[���؂�ւ�
				return ChangeScene(idx, fade);
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

	/**
	* ��ʊE�[�x�N���X�̎擾.
	*/
	DepthofField& GetDepthofField()
	{
		return _DepthofField;
	}

	/**
	* ���}�b�v�N���X���擾.
	*/
	EnvironmentMap* GetEnvironmentMap()
	{
		return &_EnvironmentMap;
	}

	/**
	* ��`��N���X���擾.
	*/
	Sky* GetSky()
	{
		return _Sky;
	}

private:
	//�V�[���؂�ւ��B
	void _ChangeScene();
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
	DepthofField _DepthofField;
	Bloom _Bloom;
	ShadowMap  _ShadowMap;

	/** ���}�b�v. */
	EnvironmentMap _EnvironmentMap;

	/** ��N���X. */
	Sky* _Sky = nullptr;
	//���̃V�[��
	int _NextScene;

};