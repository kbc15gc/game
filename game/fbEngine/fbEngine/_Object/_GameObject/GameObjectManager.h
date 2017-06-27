#pragma once
#include "GameObject.h"

class Light;
class Camera;
class ShadowCamera;

namespace
{
	//�폜����Ƃ��Ɏg��
	struct RemoveObj
	{
	public:
		RemoveObj(list<GameObject*>::iterator i, int p)
		{
			iterator = i;
			priority = p;
		}

		int priority;								//�D��x
		list<GameObject*>::iterator iterator;		//�C�e���[�^
	};
}

//�I�u�W�F�N�g���Ǘ�����N���X
class GameObjectManager
{
public:
	GameObjectManager()
	{
		//�x�N�^�[���T�C�Y
		if (_GameObjects.size() <= 0)
		{
			_GameObjects.resize(System::MAX_PRIORITY + 1);
		}
	}

	//�}�l�[�W���[�ɃI�u�W�F�N�g��ǉ�����
	//�߂�l�FGameObject* �ǉ��ɐ��������I�u�W�F�N�g�̃|�C���^
	//�������FGameObject* �A�h���X
	//�������Fint �D��x
	//*���ӓ_:���������������ꍇ��Awake()�������ŌĂ�ŁB
	GameObject* Add(GameObject* pAdd,int priority);


	template<class T>
	T* AddNew(char* name, unsigned int priority)
	{
		//�D��x�������Ă�
		if(priority > System::MAX_PRIORITY)
		{
			priority = System::MAX_PRIORITY - 1;
		}
		T* obj = new T(name);
		//�D��x�Z�b�g
		obj->SetPriority(priority);
		_GameObjects.at(priority).push_back(obj);
		obj->Awake();

		return obj;
	}

	//Update�̑O�Ɉ�x�����Ăяo�����
	void StartObject();

	//���t���[���Ăяo�����B
	void UpdateObject();

	//���t���[��Update������ɌĂяo�����B
	void LateUpdateObject();
	//�`��̑O�ɌĂяo�����
	void PreRenderObject();

	//�`��̍ۂɌĂяo�����
	void RenderObject();
	//�`��̍ۂɌĂяo�����
	void PostRenderObject();
	//�`��̍ۂɌĂяo�����
	void ImageRenderObject();
	
	//�j�����X�g�ɒǉ�
	// ���̊֐����ĂԂƃt���[���I�����ɍ폜�����B
	void AddRemoveList(GameObject* obj);
	void AddRemoveList(char* name);

	//�I�u�W�F�N�g�����֐�
	//�߂�l�FGameObject* �q�b�g�����I�u�W�F�N�g�̃A�h���X�A���邢��null�|�C���^
	//�������Fchar* �����������I�u�W�F�N�g�̖��O
	//��ԍŏ��Ƀq�b�g�����҂����Ԃ��Ȃ��̂Œ��ӂ��ꂽ���B
	GameObject* FindObject(char* name);

	//������
	const vector<GameObject*>& FindObjects(char* name,vector<GameObject*>& objArray);

	//�m�ۂ��Ă���I�u�W�F�N�g�����
	void Release();

	static GameObjectManager* Instance()
	{
		if (_Instance == nullptr)
		{
			_Instance = new GameObjectManager();
		}
		return _Instance;
	}
public:
	//�g�p����J����
	Camera* mainCamera;
	//�g�p���郉�C�g
	Light* mainLight;
private:
	//���X�g�ɏd�����Ȃ����`�F�b�N����B
	bool _CheckUniqueRemoveList(GameObject* obj);
	// �e��`�F�b�N���I�������ɍ폜���X�g�ɐςށB
	void _AddRemoveList(list<GameObject*>::iterator itr, int priority);
	//�폜���X�g�̃I�u�W�F�N�g���폜����B
	void _RemoveObject();
private:
	//�Q�[���I�u�W�F�N�g���Ǘ����郊�X�g
	vector<list<GameObject*>> _GameObjects;
	//�폜���X�g
	list<RemoveObj> _RemoveList;
	static GameObjectManager* _Instance;
};