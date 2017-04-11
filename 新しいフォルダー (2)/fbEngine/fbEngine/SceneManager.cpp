#include "SceneManager.h"
#include "Scene.h"

SceneManager*  SceneManager::_Instance;

#include "ImageObject.h"


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
	_RT->Create(g_WindowSize * 2, D3DFMT_A16B16G16R16F);
	//�|����{����0.5��
	_RT->texture->Size = _RT->texture->Size / 2;

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
#include "FPS.h"
void SceneManager::StartScene()
{
//#ifdef _DEBUG
	FPS* fps = GameObjectManager::AddNew<FPS>("fps", System::MAX_PRIORITY);
	fps->transform->SetLocalPosition(Vector3(0, 30, 0));
//#endif // DEBUG
	_Scenes[_NowScene]->Start();
	GameObjectManager::StartObject();
}

void SceneManager::UpdateScene()
{
	_Scenes[_NowScene]->Update();
	GameObjectManager::UpdateObject();
	PhysicsWorld::Instance()->Update();
	GameObjectManager::LateUpdateObject();
}

void SceneManager::DrawScene()
{
	//0�Ԗڂɐݒ�(�e�̐[�x�������ݗp�o�b�t�@)
	INSTANCE(RenderTargetManager)->ReSetRT(0, INSTANCE(RenderTargetManager)->GetRTFromList(RTIdxE::SHADOWDEPTH));
	//���O�`��(�e�Ƃ��[�x�Ƃ��P�x�Ƃ�)
	GameObjectManager::PreRenderObject();

	//0�Ԗڂɐݒ�(�I�t�X�N���[�������_�����O�p)
	INSTANCE(RenderTargetManager)->ReSetRT(0, _RT);
	GameObjectManager::RenderObject();
	//�����_�[�^�[�Q�b�g�����ɖ߂�
	INSTANCE(RenderTargetManager)->BeforeRT();
	
	//�I�t�X�N���[���̂�`��
	_Sprite->ImageRender();
	_Bloom.Render();
	GameObjectManager::PostRenderObject();
	
	//2D�Ƃ��H
	GameObjectManager::ImageRenderObject();

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
	GameObjectManager::Release();
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