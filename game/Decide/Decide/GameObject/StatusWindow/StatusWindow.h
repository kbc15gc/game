/**
* �X�e�[�^�X��ʃN���X�̒�`.
*/
#pragma once

#include"GameObject\Player\Player.h"
#include"ParameterRender.h"
#include"ItemWindow.h"
#include "GameObject\Village\EventManager.h"
#include "GameObject\TextImage\AttentionTextOnly.h"

/**
* �X�e�[�^�X��ʃN���X.
*/
class StatusWindow : public GameObject
{

public:

	/**
	* �R���X�g���N�^.
	*/
	StatusWindow(const char* name) :
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~StatusWindow()
	{
	}

	/**
	* ������.
	*/
	void Awake()override;

	/**
	* �X�V.
	*/
	void Update()override;

	/**
	* ���j�����O��������s����(�J��or���鏈��)����.
	*/
	void Execute();

	void OnEnable()override
	{
		for (int i = 0; i < _WindowCount; i++)
		{
			_ItemWindowList[i]->SetActive((i == _NowSelectWindow), true);
		}
	}

	void OnDisable()override
	{
		INSTANCE(EventManager)->NotifyEndEvent();
		static_cast<AttentionTextOnly*>(INSTANCE(GameObjectManager)->FindObject("AttentionTextOnly"))->DeleteList();
	}

private:

	/** �v���C���[�N���X�̃|�C���^. */
	Player* _Player = nullptr;

	/** �A�C�e���\�����. */
	vector<ItemWindow*> _ItemWindowList;

	int _WindowCount = 3;
	int _NowSelectWindow = 0;

	ParameterRender* _MoneyRender = nullptr;	// �����̕\���B
	ImageObject* _MoneyFrame = nullptr;			// �����̘g�\���B

public:
	static const int WindowBackPriorty;
};
