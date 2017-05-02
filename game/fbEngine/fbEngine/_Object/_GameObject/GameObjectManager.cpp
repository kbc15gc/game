#include "GameObjectManager.h"

GameObjectManager*  GameObjectManager::_Instance = nullptr;

GameObject* GameObjectManager::Add(GameObject* pAdd, int priority)
{
	GameObject* obj = pAdd;
	_GameObjects.at(priority).push_back(obj);
	obj->Awake();
	return pAdd;
}

void GameObjectManager::StartObject()
{
	for (short priority = 0; priority <= System::MAX_PRIORITY; priority++)
	{
		for each (GameObject* obj in _GameObjects[priority])
		{
			if (obj->GetActive())
			{
				obj->Start();
				obj->GetComponentManager().Start();
			}
		}
	}
}

void GameObjectManager::UpdateObject()
{
	for (short priority = 0; priority <= System::MAX_PRIORITY; priority++)
	{
		for each (GameObject* obj in _GameObjects[priority])
		{
			if (obj->GetActive())
			{
				obj->Update();
				obj->GetComponentManager().Update();
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

	//�폜���X�g���폜
	_RemoveObject();
}

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

void GameObjectManager::AddRemoveList(GameObject * obj)
{
	//�d���`�F�b�N�B
	if (_CheckUniqueRemoveList(obj) == FALSE)
		return;

	for (short priority = 0; priority <= System::MAX_PRIORITY; priority++)
	{
		list<GameObject*>::iterator it = _GameObjects[priority].begin();

		while (it != _GameObjects[priority].end())
		{
			//�A�h���X�̔�r
			if (obj == *it)
			{
				RemoveObj remove(it, priority);
				_RemoveList.push_back(remove);
				return;
			}
			else
				it++;
		}
	}
}

void GameObjectManager::AddRemoveList(char * name)
{
	for (short priority = 0; priority <= System::MAX_PRIORITY; priority++)
	{
		list<GameObject*>::iterator it = _GameObjects[priority].begin();

		while (it != _GameObjects[priority].end())
		{
			//���O�̔�r
			if (strcmp(name, (*it)->GetName()) == 0)
			{
				//�d���`�F�b�N�B
				if (_CheckUniqueRemoveList((*it)) == FALSE)
					return;

				RemoveObj remove(it,priority);
				_RemoveList.push_back(remove);
				return;
			}
			else
				it++;
		}
	}
}

GameObject* GameObjectManager::FindObject(char* name)
{
	for (short priority = 0; priority <= System::MAX_PRIORITY; priority++)
	{
		list<GameObject*>::iterator it = _GameObjects[priority].begin();

		while (it != _GameObjects[priority].end())
		{
			//���O�̔�r
			if (strcmp(name, (*it)->GetName()) == 0)
			{
				return (*it);
			}
			else
				it++;
		}
	}
	//������Ȃ���������nullptr��ԋp
	return nullptr;
}

bool GameObjectManager::FindObjects(char* name, GameObject ** objArray)
{
	for (short priority = 0; priority <= System::MAX_PRIORITY; priority++)
	{
		list<GameObject*>::iterator it = _GameObjects[priority].begin();

		while (it != _GameObjects[priority].end())
		{
			//���O�̔�r
			if (strcmp(name, (*it)->GetName()) == 0)
			{
				objArray[0] = (*it);
			}
			else
				it++;
		}
	}
	return false;
}

bool GameObjectManager::_CheckUniqueRemoveList(GameObject * obj)
{
	for each (RemoveObj remove in _RemoveList)
	{
		//�d������
		if((*remove.iterator) == obj)
		{
			return FALSE;
		}
	}
	//�d���Ȃ��B
	return TRUE;
}

void GameObjectManager::_RemoveObject()
{
	list<RemoveObj>::iterator removeIt = _RemoveList.begin();
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
	for (short priority = 0; priority <= System::MAX_PRIORITY; priority++)
	{
		if (_GameObjects.size() > 0)
		{
			list<GameObject*>::iterator it = _GameObjects[priority].begin();
			//���ׂĒǉ�
			while (it != _GameObjects[priority].end())
			{
				if ((*it)->GetDiscard())
				{
					RemoveObj remove(it, priority);
					_RemoveList.push_back(remove);
				}
				else
				{
					//�j�����Ȃ�
					//���͔j������悤�ɂ���
					(*it)->SetDiscard(true);
				}

				it++;
			}
		}
	}
	_RemoveObject();
	mainCamera = nullptr;
	mainLight = nullptr;
}