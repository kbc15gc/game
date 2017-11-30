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
		//�����_�[�^�[�Q�b�g�쐬
		_MainRT[count] = new RenderTarget();
		//�傫����{�ō���č��掿��
		//���������_�o�b�t�@�ō쐬
		_MainRT[count]->Create(g_FrameSize, D3DFMT_A16B16G16R16F);
		//�|����{����0.5��
		_MainRT[count]->texture->Size = _MainRT[count]->texture->Size;
	}
	//�����_�[�^�[�Q�b�g�̃e�N�X�`�����擾
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

	//�A���`�G�C���A�X�̍쐬.
	_AntiAliasing.Create();
	//��ʊE�[�x�̍쐬
	_DepthofField.Create();
	//�u���[���̏���
	_Bloom.Create();
	//�V���h�E�}�b�v�̏�����.
	_ShadowMap.Create();
	//���}�b�v�̏�����.
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

	//��`��N���X�̏�����.
	_Sky = INSTANCE(GameObjectManager)->AddNew<Sky>("sky", 0);
	_Sky->SetActive(false);

	_Scenes[_NowScene]->Start();
}

void SceneManager::UpdateScene()
{
	//���̃V�[�����ݒ肳��Ă���ꍇ�̓V�[���؂�ւ�
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
	//�V���h�E�}�b�v�̍X�V.
	if (_Scenes[_NowScene]->GetIsShadowMap())
	{
		_ShadowMap.Update();
	}

	//���}�b�v�̍X�V.
	if (_Scenes[_NowScene]->GetIsEnvironmentMap())
	{
		_EnvironmentMap.Update();
	}

}

void SceneManager::DrawScene()
{
	//���O�`��(�e�Ƃ��[�x�Ƃ��P�x�Ƃ�)
	INSTANCE(GameObjectManager)->PreRenderObject();

	//�V���h�E�}�b�v�̕`��.
	if (_Scenes[_NowScene]->GetIsShadowMap())
	{
		_ShadowMap.Render();
	}
	//���}�b�v�̕`��.
	if (_Scenes[_NowScene]->GetIsEnvironmentMap())
	{
		_EnvironmentMap.Render();
	}

	//0�Ԗڂɐݒ�(�I�t�X�N���[�������_�����O�p)
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

	//�u���[���̕`��.
	_Bloom.Render();

	//��ʊE�[�x�̕`��.
	_DepthofField.Render();
	
	//�A���`�G�C���A�X�̕`��.
	_AntiAliasing.Render();

	//�����_�[�^�[�Q�b�g�����ɖ߂�
	INSTANCE(RenderTargetManager)->BeforeRT();

	//�I�t�X�N���[���̂�`��
	_Sprite->SetTexture(_MainRT[CurrentMainRT_]->texture);
	_Sprite->SetSize(g_WindowSize);
	_Sprite->ImageRender();
	
	INSTANCE(GameObjectManager)->PostRenderObject();
	
	//2D�Ƃ��H
	INSTANCE(GameObjectManager)->ImageRenderObject();

	//�V�[���̃t�F�[�h�̂��
	//�őO�ʂɗ���悤�ɍŌ�ɕ`��
	_Scenes[_NowScene]->Fade();
}

Scene* SceneManager::ChangeScene(int key, bool fade)
{
	if (fade)
		//�t�F�[�h������B
		Scene::StartFade(true);
	//���̃V�[����
	_NextScene = key;
	return _Scenes[_NextScene];
}

Scene* SceneManager::ChangeScene(char * Scenename, bool fade)
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
	//�V�[���̓Y�����؂�ւ�
	_NowScene = _NextScene;
	_NextScene = -1;
	//�I�u�W�F�N�g�����[�X
	INSTANCE(GameObjectManager)->Release();
	//����������
	SceneManager::StartScene();
	//���������I������Ȃ�A�t�F�[�h��������
	Scene::StartFade(false);
}
