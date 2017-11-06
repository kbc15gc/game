/**
* ���j���̃y�[�W�N���X�̒�`.
*/
#pragma once

/** ���j��. */
class HistoryBook;

#include"..\..\HistoryInfo.h"

#include"HFSM\HistoryPageState.h"

/** ���j���̃y�[�W�̏�Ԃ̊��N���X. */
using HistoryPageStatePtr = unique_ptr<IHistoryPageState>;

/**
* ���j���̃y�[�W�N���X.
*/
class HistoryPage : public GameObject
{
public:

	/** ��ԃR�[�h. */
	enum class StateCodeE
	{
		Invalid = -1,	//!< ����.
		PutIn,			//!< ����.
		PutOut,			//!< ����.
		Turn,			//!< ����.
		TakeOff,		//!< �{��؂藣��.
		Close,			//!< ����.
		StateNum,		//!< ��Ԑ�.
	};

public:

	/**
	* �R���X�g���N�^.
	*/
	HistoryPage(const char* name) : 
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~HistoryPage()
	{
	}

	/**
	* �R���X�g���N�^��̏�����.
	*/
	void Awake()override;

	/**
	* ������.
	*/
	void Start(ChipID chipID, LocationCodeE code,Vector3& pos);

	/**
	* �X�V.
	*/
	void Update()override;

	/**
	* �x�߂̍X�V.
	*/
	void LateUpdate()override;

	/**
	* �`��.
	*/
	void Render()override;

	/**
	* ���j���̃|�C���^��ݒ�.
	*/
	void SetHistoryBook(HistoryBook* book)
	{
		_HistoryBook = book;
	}

	/**
	* ��]�s���ݒ�.
	*/
	void Rotation(float angle)
	{
		_Angle = angle;
		Quaternion rot;
		rot.SetEuler(Vector3(0.0f, _Angle, 0.0f));
		_RotationMatrix = rot.GetRotationMatrix();
	}

	/**
	* �p�x���擾.
	*/
	float GetAngle() const
	{
		return _Angle;
	}

	void SetRotAngle(const float angle)
	{
		_RotAngle = angle;
	}

	float GetRotAngle() const
	{
		return _RotAngle;
	}

	/**
	* ��Ԃ̕ω�. 
	*/
	void ChangeState(StateCodeE state);

	//���ǂ̏ꏊ�ɂ��邩��Ԃ��B
	LocationCodeE GetNowLocation()
	{
		return _NowLocatuion;
	}

	ChipID GetChipID()
	{
		return _ChipID;
	}

	Vector3& GetInitPos()
	{
		return _InitPos;
	}

private:

	/**
	* ��ԃ��X�g�̏�����.
	*/
	void InitState();

private:

	/** �y�[�W�̃��f��. */
	SkinModel* _Model = nullptr;

	/** ���j���̃|�C���^. */
	HistoryBook* _HistoryBook = nullptr;
	/** �e�̍s��. */
	D3DXMATRIX* _ParentMatrix = nullptr;

	/** �p�x. */
	float _Angle = 0.0f;
	/** �����܂ŉ�]����p�x. */
	float _RotAngle = 0.0f;
	/** ��]�s��. */
	D3DXMATRIX _RotationMatrix;

	/** �������W. */
	Vector3 _InitPos;

	/** ��ԃ��X�g. */
	vector<HistoryPageStatePtr> _StateList;
	/** ���݂̏��. */
	int _NowState = (int)StateCodeE::Invalid;

	/** �}�e���A��. */
	Material* _Material = nullptr;

	/** �`�b�vID. */
	ChipID _ChipID;
	//���������ǂ̏ꏊ�ɂ���̂��B
	LocationCodeE _NowLocatuion;
};