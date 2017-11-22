/**
* ���j�ύX���j���[��ʃN���X�̒�`.
*/
#pragma once

#include "fbEngine\_Object\_GameObject\GameObject.h"

#include"..\HistoryInfo.h"

#include"Chip2D.h"

class HistoryBook;
class TextObject;

/**
* ���j�ύX���j���[��ʃN���X.
* ���j�����J�����Ƃ��ɕ\�������.
*/
class HistoryMenu : public GameObject
{
public:

	/**
	* �I���R�[�h.
	*/
	enum class SelectCodeE
	{
		Min = 0,
		Location = Min,	//!< �ꏊ.
		Page,		//!< �y�[�W.
		Max = Page,
	};

public:

	/**
	* �R���X�g���N�^.
	*/
	HistoryMenu(const char* name) :
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~HistoryMenu()
	{
	}

	/**
	* ������. 
	*/
	void Awake()override;

	/**
	* ������.
	*/
	void Start()override;

	/**
	* �X�V.
	*/
	void Update()override;

	/**
	* �`�b�v��ǉ�.
	*/
	void AddChip(ChipID chipID, bool isSave = true);

	/**
	* �����̃`�b�v���������܂�Ă��邩����.
	*/
	bool IsSetChip(ChipID id)
	{
		for (auto it : _Chip2DList)
		{
			if (it->GetChipID() == id)
			{
				return true;
			}
		}
		return false;
	}



private:

	/**
	* �\�����̍X�V.
	*/
	void EnableUpdate();

	/**
	* �ꏊ�I�𒆂̍X�V.
	*/
	void SelectLocationUpdate();

	/**
	* �y�[�W�I�𒆂̍X�V.
	*/
	void SelectPageUpdate();

	/**
	* Chip�I�𒆂̍X�V
	*/
	void SelectChipUpdate();

	/**
	* �`�b�v�̍��W�ړ�.
	*/
	void ChipMove();

	/**
	* �y�[�W�̍��W�ړ�.
	*/
	void PageMove();

	void SaveChip()
	{
		picojson::array chipList;
		for (auto it : _Chip2DList)
		{
			chipList.push_back((picojson::value)(double)it->GetChipID());
		}
		JsonData ChipData;
		ChipData.SetDataObject("ChipID", chipList);
		ChipData.Save("HaveChip");
	}

	void LoadChip()
	{
		JsonData ChipData;
		if (ChipData.Load("HaveChip"))
		{
			picojson::array chipList = ChipData.GetDataArray("ChipID");

			for (auto it : chipList)
			{
				AddChip((ChipID)(int)it.get<double>(), false);
			}
		}
	}

private:

	/** �J�[�\���\���N���X. */
	ImageObject* _CursorSpriteL = nullptr;
	ImageObject* _CursorSpriteR = nullptr;

	/** �I������Ă������. */
	int _SelectCode = (int)SelectCodeE::Location;

	/** �ꏊ���\���I�u�W�F�N�g. */
	TextObject* _LocationNameRender = nullptr;
	/** ���ݑI�𒆂̏ꏊ. */
	int _NowSelectLocation = 0;

	/** �������Ă���ꏊ�̌�. */
	int _ReleaseLocation = 0;

	/** ���j���N���X�̃|�C���^. */
	HistoryBook* _HistoryBook = nullptr;

	/** �����`�b�v��\������N���X���X�g. */
	vector<Chip2D*> _Chip2DList;

	/** ���ݑI�����Ă��鏊���`�b�v. */
	int _NowSelectChip = 0;
	/** ���݂݂Ă���y�[�W. */	
	int _NowLookPage = 0;

	/** ����\�t���O. */
	bool _IsOperation = true;

};