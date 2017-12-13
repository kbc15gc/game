#include"fbstdafx.h"
#include "GameObjectManager.h"

GameObjectManager*  GameObjectManager::_Instance = nullptr;

GameObject* GameObjectManager::Add(GameObject* pAdd, int priority)
{
	GameObject* obj = pAdd;
	_GameObjects.at(priority).push_back(obj);
	/*obj->Awake();*/
	return pAdd;
}

void GameObjectManager::UpdateObject()
{
	//�폜���X�g���폜
	_RemoveObject();


	for (short priority = 0; priority <= System::MAX_PRIORITY; priority++)
	{
		for each (GameObject* obj in _GameObjects[priority])
		{
			if (obj->GetActive())
			{
				if (!(obj->GetIsStopUpdate())) {
					// �X�V�X�g�b�v�t���O���I�t�B
					obj->ConnoteUpdate();
					obj->GetComponentManager().ConnoteUpdate();
				}
			}
		}
	}
}

void GameObjectManager::LateUpdateObject()
{
	for (short priority = 0; priority <= System::MAX_PRIORITY; priority++)
	{
		for each (GameObject* obj in _GameObjects[priority])
		{
			if (obj->GetActive())
			{
				obj->LateUpdate();
				obj->GetComponentManager().LateUpdate();
			}
		}
	}
}

#ifdef _DEBUG
void GameObjectManager::DebugObject() {
	for (short priority = 0; priority <= System::MAX_PRIORITY; priority++)
	{
		for each (GameObject* obj in _GameObjects[priority])
		{
			obj->Debug();
			obj->GetComponentManager().Debug();
		}
	}
}
#endif


void GameObjectManager::PreRenderObject()
{
	//�����_�[�^�[�Q�b�g�Z�b�g
	for (short priority = 0; priority <= System::MAX_PRIORITY; priority++)
	{
		for each (GameObject* obj in _GameObjects[priority])
		{
			if (obj->GetActive())
			{
				obj->PreRender();
				obj->GetComponentManager().PreRender();
			}
		}
	}
}

void GameObjectManager::RenderObject()
{
	for (short priority = 0; priority <= System::MAX_PRIORITY; priority++)
	{
		for each (GameObject* obj in _GameObjects[priority])
		{
			if (obj->GetActive())
			{
				obj->Render();
				obj->GetComponentManager().Render();
			}
		}
	}
}

void GameObjectManager::PostRenderObject()
{
	for (short priority = 0; priority <= System::MAX_PRIORITY; priority++)
	{
		for each (GameObject* obj in _GameObjects[priority])
		{
			if (obj->GetActive())
			{
				obj->PostRender();
				obj->GetComponentManager().PostRender();
			}
		}
	}
}

void GameObjectManager::ImageRenderObject()
{
	for (short priority = 0; priority <= System::MAX_PRIORITY; priority++)
	{
		for each (GameObject* obj in _GameObjects[priority])
		{
			if (obj->GetActive())
			{
				obj->ImageRender();
				obj->GetComponentManager().ImageRender();
			}
		}
	}
}

void GameObjectManager::AddRemoveList(GameObject * addres)
{
	//�܂��o�^����Ă��Ȃ���(�B�ꐫ)�`�F�b�N�B
	if (_CheckUniqueRemoveList(addres) == FALSE)
		return;

	//�S�x�N�^�[�T���B
	for (short priority = 0; priority <= System::MAX_PRIORITY; priority++)
	{
		auto& vec = _GameObjects[priority];
		//�����Ɉ�v������̂������B
		auto& itr = find(vec.begin(), vec.end(), addres);
		//��v����Βǉ��B
		if(itr != vec.end())
			_AddRemoveList(itr, priority);
	}
}

void GameObjectManager::AddRemoveList(char * name)
{
	for (short priority = 0; priority <= System::MAX_PRIORITY; priority++)
	{
		auto& vec = _GameObjects[priority];
		//���O����v�����ꍇ�C�e���[�^��Ԃ��B
		auto& itr = std::find_if(vec.begin(), vec.end(), [name](GameObject* obj) {return (strcmp(name, obj->GetName()) == 0); });
		if (itr != vec.end())
		{
			//�d���`�F�b�N�B
			if (_CheckUniqueRemoveList((*itr)) == FALSE)
				return;

			_AddRemoveList(itr, priority);
			return;
		}
	}
}

GameObject* GameObjectManager::FindObject(char* name)
{
	for (auto& vec : _GameObjects)
	{
		//���O����v�����ꍇ�C�e���[�^��Ԃ��B
		auto& itr = std::find_if(vec.begin(), vec.end(), [name](GameObject* obj) {return (strcmp(name, obj->GetName()) == 0); });
		if (itr != vec.end())
			return (*itr);
	}

	//������Ȃ���������nullptr��ԋp
	return nullptr;
}

const vector<GameObject*>& GameObjectManager::FindObjects(char* name,vector<GameObject*>& objArray)
{
	objArray.clear();
	for (short priority = 0; priority <= System::MAX_PRIORITY; priority++)
	{
		list<GameObject*>::iterator it = _GameObjects[priority].begin();

		while (it != _GameObjects[priority].end())
		{
			//���O�̔�r
			if (strcmp(name, (*it)->GetName()) == 0)
			{
				objArray.push_back((*it));
			}
			it++;
		}
	}
	return objArray;
}

bool GameObjectManager::_CheckUniqueRemoveList(GameObject * addres)
{
	//������Ȃ�������true��Ԃ��B
	return (_RemoveList.end() == find_if(_RemoveList.begin(), _RemoveList.end(), [addres](RemoveObj remove) {return ((*remove.iterator) == addres); }));
}

void GameObjectManager::_AddRemoveList(list<GameObject*>::iterator iterator, int priority)
{
	(*iterator)->OnDestroy();
	(*iterator)->GetComponentManager().OnDestroy();
	_RemoveList.push_back(RemoveObj(iterator, priority));
}

void GameObjectManager::_RemoveObject()
{
	auto& removeIt = _RemoveList.begin();
	while (removeIt != _RemoveList.end())
	{
		//�I�u�W�F�N�g�̃��������
		SAFE_DELETE(*removeIt->iterator);
		//_GameObjects����C�e���[�^�폜
		_GameObjects[removeIt->priority].erase(removeIt->iterator);
		//_RemoveList����C�e���[�^�폜
		removeIt = _RemoveList.erase(removeIt);
	}

	_RemoveList.clear();
}

void GameObjectManager::Release()
{
	//�S�x�N�^�[�T���B
	for (short priority = 0; priority <= System::MAX_PRIORITY; priority++)
	{
		auto& vec = _GameObjects[priority];
		for (auto& itr = vec.begin(), end = vec.end(); itr != end; itr++)
		{
			auto& obj = (*itr);
			//�d���`�F�b�N�B
			if (_CheckUniqueRemoveList(obj) == FALSE)
				continue;

			if (obj->GetDiscard())
			{
				_AddRemoveList(itr, priority);
			}
			else
			{
				//�j�����Ȃ�
				//���͔j������悤�ɂ���
				obj->SetDiscard(true);
			}
		}	
	}
	_RemoveObject();

	//char text[256];
	//sprintf(text, "ObjectManager::ReleaseEnd\n");
	//OutputDebugString(text);

	mainCamera = nullptr;
	mainLight = nullptr;
}
