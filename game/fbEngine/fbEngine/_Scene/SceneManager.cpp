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
	//�����_�[�^�[�Q�b�g�쐬
	_RT = new RenderTarget();
	//�傫����{�ō���č��掿��
	//���������_�o�b�t�@�ō쐬
	_RT->Create(g_WindowSize, D3DFMT_A16B16G16R16F);
	//�|����{����0.5��
	_RT->texture->Size = _RT->texture->Size;

	//�����_�[�^�[�Q�b�g�̃e�N�X�`�����擾
	_Sprite->SetTexture(_RT->texture);
	_Sprite->SetPivot(Vector2(0.0f, 0.0f));
	//�u���[���̏���
	_Bloom.Create();
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
	fps->transform->SetLocalPosition(Vector3(0, 30, 0));
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
}

void SceneManager::DrawScene()
{
	//0�Ԗڂɐݒ�(�e�̐[�x�������ݗp�o�b�t�@)
	INSTANCE(RenderTargetManager)->ReSetRT(0, INSTANCE(RenderTargetManager)->GetRTFromList(RTIdxE::SHADOWDEPTH),Color::zero);
	//���O�`��(�e�Ƃ��[�x�Ƃ��P�x�Ƃ�)
	INSTANCE(GameObjectManager)->PreRenderObject();

	//0�Ԗڂɐݒ�(�I�t�X�N���[�������_�����O�p)
	INSTANCE(RenderTargetManager)->ReSetRT(0, _RT);
	INSTANCE(GameObjectManager)->RenderObject();
	//�����_�[�^�[�Q�b�g�����ɖ߂�
	INSTANCE(RenderTargetManager)->BeforeRT();
	
	//�I�t�X�N���[���̂�`��
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
	Scene::SetFade(false);
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
	return _RT->texture;
}