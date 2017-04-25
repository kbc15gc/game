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
		
		//	//sort�Ɏg��
		//bool operator<(const RemoveObj& left) const
		//{
		//	//�A�h���X���قȂ�Ȃ����
		//	if (left.addres != this->addres)
		//	{
		//		return true;
		//	}
		//	//���ёւ����Ȃ�
		//	return false;
		//}
		//
		////��r���Z�q unique�Ɏg��
		//bool operator==(const RemoveObj& left)
		//{
		//	//�A�h���X��r
		//	if (left.addres == this->addres)
		//	{
		//		return true;
		//	}
		//	return false;
		//}
		
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
	GameObject* Add(GameObject* pAdd,int priority);

	template<class T>
	T* AddNew(char* name, UINT priority)
	{
		//�D��x�������Ă�
		if(priority > System::MAX_PRIORITY)
		{
			priority = System::MAX_PRIORITY - 1;
		}
		T* obj = new T(name);
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
	void AddRemoveList(GameObject* obj);
	void AddRemoveList(char* name);

	//�I�u�W�F�N�g�����֐�
	//�߂�l�FGameObject* �q�b�g�����I�u�W�F�N�g�̃A�h���X�A���邢��null�|�C���^
	//�������Fchar* �����������I�u�W�F�N�g�̖��O
	//��ԍŏ��Ƀq�b�g�����҂����Ԃ��Ȃ��̂Œ��ӂ��ꂽ���B
	GameObject* FindObject(char* name);

	//������
	bool FindObjects(char* name,GameObject** objArray);

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
	//�e����邽�߂̃J����
	ShadowCamera* mainShadowCamera;
private:
	//���X�g�ɏd�����Ȃ����`�F�b�N����B
	bool _CheckUniqueRemoveList(GameObject* obj);
	//�폜���X�g�̃I�u�W�F�N�g���폜����B
	void _RemoveObject();
private:
	//�Q�[���I�u�W�F�N�g���Ǘ����郊�X�g
	vector<list<GameObject*>> _GameObjects;
	//�폜���X�g
	list<RemoveObj> _RemoveList;
	static GameObjectManager* _Instance;
};