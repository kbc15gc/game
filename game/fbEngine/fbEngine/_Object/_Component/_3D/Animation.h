/*!
 * @brief	�A�j���[�V�����N���X�B
 */
#pragma once
#include "_Object\_Component\Component.h"
#include <queue>

/*!
 * @brief	�A�j���[�V�����N���X�B
 */
class Animation : public Component{
public:
	typedef void (Object::*CallBack)(int);	// �֐��|�C���^(�����͍Đ����ꂽ�A�j���[�V�����ԍ�)�B

	struct StartAnimationCallBack {
		Object* object = nullptr;	// �R�[���o�b�N�֐��̎�����B
		CallBack callback = nullptr;	// �A�j���[�V�����I�����ɌĂяo�������֐��̃|�C���^�B

		StartAnimationCallBack(Object* obj, CallBack call) {
			object = obj;
			callback = call;
		}
	};

	//�A�j���[�V�����Đ��ɕK�v�ȏ��
	struct PlayAnimInfo
	{
	public:
		//�A�j���[�V�����̓Y����
		//�A�j���[�V�����⊮����
		//�A�j���[�V�����J�ڊJ�n����
		//���[�v���B
		PlayAnimInfo(const UINT index, const float interpolateTime, const float transitionTime, const int loopnum = -1)
		{
			Index = index;
			InterpolateTime = interpolateTime;
			TransitionTime = transitionTime;
			LoopNum = loopnum;
		}
	
		//�A�j���[�V�����Z�b�g�̓Y�����B
		unsigned int Index;
		//�A�j���[�V�����⊮����
		float InterpolateTime;
		//�A�j���[�V�����̑J�ڊJ�n���ԁB(����)
		float TransitionTime;
		//���[�v���鐔
		int LoopNum;
	};
	/*!
	 * @brief	�R���X�g���N�^
	 */
	Animation(GameObject* g, Transform* t);
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	~Animation();
	
	
	void Initialize(ID3DXAnimationController* anim);
	void Awake();
	void Update();
	//�A�j���[�V�����Đ�(�⊮�Ȃ�)
	void PlayAnimation(const UINT animationSetIndex);
	//�A�j���[�V�����Đ�(�⊮����)
	//��1�����@�Đ��������A�j���[�V�����̃C���f�b�N�X
	//��2�����@��Ԏ���
	//��3�����@���[�v��
	void PlayAnimation(const UINT animationSetIndex, const float interpolateTime, const int loopnum = -1);
	//�A�j���[�V�����Đ�(�⊮����)
	//��1�����@�Đ��������A�j���[�V�����̃C���f�b�N�X
	//��2�����@��Ԏ���
	//��3�����@�A�j���[�V�������J�n���鎞�ԁB
	//��4�����@���[�v��
	//�߂�l�@�Đ��ł������ǂ����H
	bool PlayAnimation(const UINT animationSetIndex, const float interpolateTime, const float transitionTime, const int loopnum = -1);

	//�L���[�ɃA�j���[�V�����̍Đ�����ǉ��B
	//�m�ۂ����A�h���X�̓A�j���[�V�����Ȃ��ŉ�������B
	void AddAnimationQueue(PlayAnimInfo* info);

	inline void PopAnimationQueue()
	{
		while (!_AnimationQueue.empty()) _AnimationQueue.pop();
	}

	/*!
	*@brief	�A�j���[�V�����Z�b�g�̎擾�B
	*/
	int GetNumAnimationSet() const
	{
		return _NumAnimSet;
	}
	//���ݍĐ����̃A�j���[�V�����ԍ��擾
	const UINT GetPlayAnimNo() const
	{
		return _CurrentAnimationSetNo;
	}
	//�A�j���[�V�������Đ�����
	const bool GetPlaying()
	{
		return _IsPlaying;
	}

	const double GetLocalAnimationTime()
	{
		return _LocalAnimationTime;
	}

	const double NowFrame()
	{
		return _CurrentFrame;
	}
	//�����擾
	const double GetTimeRatio()
	{
		return _TimeRatio;
	}

	void SetAnimeSpeed(float sp)
	{
		_PlaySpeed = sp;
	}
	/*!
	*@brief	�A�j���[�V�����I�����Ԑݒ�
	*@param[in]		idx	�A�j���[�V�����Z�b�g�̔ԍ��B
	*@param[in]		endtime		�I�����ԁB
	*/
	void SetAnimationEndTime(const UINT idx, const double endtime)
	{
		//�͈͓����H
		if (idx < _NumAnimSet)
			_EndTime[idx] = endtime;
	}
	const double GetAnimationEndTime(const UINT idx)
	{
		//�͈͓����H
		if (idx < _NumAnimSet)
			return _EndTime[idx];
	}
	//�A�j���[�V�����̃��[�J���^�C���ݒ�
	void SetLocalAnimationTime(const UINT track, const double t)
	{
		_LocalAnimationTime = t;
		//���Ԑݒ�B
		_AnimController->SetTrackPosition(track, _LocalAnimationTime);
		//�|�W�V�������X�V���邽�߂ɌĂ΂Ȃ���΂����Ȃ�
		_AnimController->AdvanceTime(0.0, NULL);
	}

	inline void AddCallBack(unique_ptr<StartAnimationCallBack> callback) {
		_callback.push_back(move(callback));
	}
private:
	//�A�j���[�V�����̕⊮������֐��B
	void _InterpolateAnimation(const float delta);
	//���̗v�f���L���[������o���B
	void _NextQueue();
	//�A�j���[�V�������I���������̏����B
	void _EndAnimation(const float endtime);
private:
	ID3DXAnimationController*				_AnimController = nullptr;		//!<�A�j���[�V�����R���g���[���B
	UINT									_NumAnimSet = 0;				//!<�A�j���[�V�����Z�b�g�̐��B
	std::unique_ptr<ID3DXAnimationSet*[]>	_AnimationSets = nullptr;			//!<�A�j���[�V�����Z�b�g�̔z��B
	vector<float>							_BlendRateTable;			//!<�u�����f�B���O���[�g�̃e�[�u���B
	UINT									_CurrentAnimationSetNo = 0;	//!<���ݍĐ����̃A�j���[�V�����g���b�N�̔ԍ��B
	UINT									_CurrentTrackNo = 0;			//!<���݂̃g���b�N�̔ԍ��B
	UINT									_NumMaxTracks = 0;			//!<�A�j���[�V�����g���b�N�̍ő吔�B
	bool									_IsBlending = true;				//!<�A�j���[�V�����u�����f�B���O���H
	bool									_IsInterpolate = true;			//!<��Ԓ��H
	float									_InterpolateEndTime = 0.0f;		//!<��ԏI�����ԁB
	float									_InterpolateTimer = 0.0f;		//!<��Ԏ��ԁB

	vector<double> _EndTime;			//�e�A�j���[�V�����̏I�����Ԃ��i�[�����z��
	double _TimeRatio = 0.0;					//���K�����ꂽ���Ԃ̊����B
	double _LocalAnimationTime = 0.0;			//���[�J���ȃA�j���[�V�����̌o�ߎ���(��₱���������̂Ŏ����ŊǗ����邱�Ƃɂ���)
	double _CurrentFrame = 0.0;				//�A�j���[�V�������Đ�����Č��݉��t���[���ڂ��B
	float _PlaySpeed = 0.0f;					//�Đ����x
	int _LoopNum = 0;						//�A�j���[�V���������[�v�����鐔�B
	int _LoopCount = 0;						//���[�v�����J�E���g����B
	bool _IsPlaying = true;					//�A�j���[�V�����Đ����ł��邱�Ƃ������B

	std::queue<PlayAnimInfo*> _AnimationQueue;	//�A�j���[�V������ێ�����L���[�B

	vector<unique_ptr<StartAnimationCallBack>> _callback;	// �A�j���[�V�������X�^�[�g����ۂɌĂ΂��R�[���o�b�N(���[�v�Đ��Ȃ�1���[�v���ƂɌĂ΂��)�B
};