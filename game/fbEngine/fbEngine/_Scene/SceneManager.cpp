#include"fbstdafx.h"
#include "SceneManager.h"
#include "Scene.h"

#include "_Object\_GameObject\ImageObject.h"
#include "_Object\_GameObject\FPS.h"
#include "_Object\_Component\_Physics\Physics.h"

SceneManager*  SceneManager::_Instance;


SceneManager::SceneManager()
{
	_OffScreen = new ImageObject("OffScreen");
	_OffScreen->Awake();
	_Sprite = _OffScreen->GetComponent<Sprite>();
	_Sprite->Start();

	FOR(count, 2)
	{
		//�����_�[�^�[�Q�b�g�쐬
		_MainRT[count] = new RenderTarget();
		//�傫����{�ō���č��掿��
		//���������_�o�b�t�@�ō쐬
		_MainRT[count]->Create(g_WindowSize, D3DFMT_A16B16G16R16F);
		//�|����{����0.5��
		_MainRT[count]->texture->Size = _MainRT[count]->texture->Size;
	}
	//�����_�[�^�[�Q�b�g�̃e�N�X�`�����擾
	_Sprite->SetTexture(_MainRT[CurrentMainRT_]->texture);
	_Sprite->SetPivot(Vector2(0.0f, 0.0f));

	//�A���`�G�C���A�X�̍쐬.
	_AntiAliasing.Create();

	//��ʊE�[�x�̍쐬
	_DepthofField.Create();

	//�u���[���̏���
	_Bloom.Create();

	//�V���h�E�}�b�v�̏�����.
	_ShadowMap.Create();
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
	_Scenes[_NowScene]->Start();
	INSTANCE(GameObjectManager)->StartObject();
}

void SceneManager::UpdateScene()
{
	_Scenes[_NowScene]->Update();
	INSTANCE(GameObjectManager)->UpdateObject();
	PhysicsWorld::Instance()->Update();
	INSTANCE(GameObjectManager)->LateUpdateObject();

	//�V���h�E�}�b�v�̍X�V.
	_ShadowMap.Update();
}

void SceneManager::DrawScene()
{
	//���O�`��(�e�Ƃ��[�x�Ƃ��P�x�Ƃ�)
	INSTANCE(GameObjectManager)->PreRenderObject();

	//�V���h�E�}�b�v�̕`��.
	_ShadowMap.Render();

	//0�Ԗڂɐݒ�(�I�t�X�N���[�������_�����O�p)
	INSTANCE(RenderTargetManager)->ReSetRT(0, _MainRT[CurrentMainRT_]);
	(*graphicsDevice()).SetRenderTarget(1, _DepthofField.GetDepthRenderTarget()->buffer);
	(*graphicsDevice()).Clear(
		1,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_RGBA(1, 1,1,1),
		1.0f,
		0);

	INSTANCE(GameObjectManager)->RenderObject();

	(*graphicsDevice()).SetRenderTarget(1, nullptr);

	//_DepthofField.Render();
	
	_AntiAliasing.Render();

	//�����_�[�^�[�Q�b�g�����ɖ߂�
	INSTANCE(RenderTargetManager)->BeforeRT();

	//�I�t�X�N���[���̂�`��
	_Sprite->SetTexture(_MainRT[CurrentMainRT_]->texture);
	_Sprite->ImageRender();
	
	_Bloom.Render();
	
	INSTANCE(GameObjectManager)->PostRenderObject();
	
	//2D�Ƃ��H
	INSTANCE(GameObjectManager)->ImageRenderObject();

	//�V�[���̃t�F�[�h�̂��
	//�őO�ʂɗ���悤�ɍŌ�ɕ`��
	_Scenes[_NowScene]->Fade();
}

Scene* SceneManager::ChangeScene(int key)
{
	//�t�F�[�h��������
	Scene::StartFade(false);
	//�V�[���̓Y�����؂�ւ�
	_NowScene = key;
	//�I�u�W�F�N�g�����[�X
	INSTANCE(GameObjectManager)->Release();
	//����������
	SceneManager::StartScene();
	return _Scenes[_NowScene];
}

Scene* SceneManager::ChangeScene(char * Scenename)
{
	//�N���X��
	char* classname = new char[128];
	strcpy_s(classname, strlen("class ")+1, "class ");
	strcat_s(classname, strlen(Scenename) + strlen(classname)+1, Scenename);
	int idx = 0;
	for each (Scene* s in _Scenes)
	{
		//���O�̈�v
		if (strcmp(classname, typeid(*s).name()) == 0)
		{
			//�V�[���؂�ւ�
			return ChangeScene(idx);
		}
		idx++;
	}
	return nullptr;
}

TEXTURE* SceneManager::GetOffScreenTexture()
{
	return _MainRT[CurrentMainRT_]->texture;
}