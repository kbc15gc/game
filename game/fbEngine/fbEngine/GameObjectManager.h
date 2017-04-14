#pragma once

class GameObject;
class Camera;
class Light;
class ShadowCamera;

//�I�u�W�F�N�g���Ǘ�����N���X
class GameObjectManager : Noncopyable
{
public:
	GameObjectManager()
	{
		
	}

	//�}�l�[�W���[�ɃI�u�W�F�N�g��ǉ�����
	//�߂�l�FGameObject* �ǉ��ɐ��������I�u�W�F�N�g�̃|�C���^
	//�������FGameObject* �A�h���X
	//�������Fint �D��x
	static GameObject* Add(GameObject* pAdd,int priority);

	template<class T>
	static T* AddNew(char* name, int priority)
	{
		//�D��x�������Ă�
		if(priority >= System::MAX_PRIORITY)
		{
			priority = System::MAX_PRIORITY - 1;
		}
		if (_GameObjects.size() <= 0)
		{
			_GameObjects.resize(System::MAX_PRIORITY);
		}
		T* obj = new T(name);
		_GameObjects.at(priority).push_back(obj);
		obj->Awake();

		return obj;
	}

	//Update�̑O�Ɉ�x�����Ăяo�����
	static void StartObject();

	//���t���[���Ăяo�����B
	static void UpdateObject();

	//���t���[��Update������ɌĂяo�����B
	static void LateUpdateObject();
	//�`��̑O�ɌĂяo�����
	static void PreRenderObject();

	//�`��̍ۂɌĂяo�����
	static void RenderObject();
	//�`��̍ۂɌĂяo�����
	static void PostRenderObject();
	//�`��̍ۂɌĂяo�����
	static void ImageRenderObject();
	
	//�j�����X�g�ɒǉ�
	static void AddRemoveList(GameObject* obj);
	static void AddRemoveList(char* name);

	//�I�u�W�F�N�g�����֐�
	//�߂�l�FGameObject* �q�b�g�����I�u�W�F�N�g�̃A�h���X�A���邢��null�|�C���^
	//�������Fchar* �����������I�u�W�F�N�g�̖��O
	//��ԍŏ��Ƀq�b�g�����҂����Ԃ��Ȃ��̂Œ��ӂ��ꂽ���B
	static GameObject* FindObject(char* name);

	//������
	static bool FindObjects(char* name,GameObject** objArray);

	//�m�ۂ��Ă���I�u�W�F�N�g�����
	static void Release();

	//�g�p����J����
	static Camera* mainCamera;
	//�g�p���郉�C�g
	static Light* mainLight;
	//�e����邽�߂̃J����
	static ShadowCamera* mainShadowCamera;

	struct RemoveObj
	{
		int prio;
		list<GameObject*>::iterator it;
		RemoveObj(list<GameObject*>::iterator i, int p)
		{
			it = i;
			prio = p;
		}
	};
private:
	//�Q�[���I�u�W�F�N�g���Ǘ�
	static vector<list<GameObject*>> _GameObjects;
	//�폜���X�g
	static list<RemoveObj> _RemoveList;
	//�폜���X�g�̃I�u�W�F�N�g���폜����B
	static void _RemoveObject();
};