/*!
 * @brief	�A�j���[�V�����N���X�B
 */
#pragma once
#include "_Object\_Component\Component.h"

/*!
 * @brief	�A�j���[�V�����N���X�B
 */
class Animation : public Component{
public:
	/*!
	 * @brief	�R���X�g���N�^
	 */
	Animation(GameObject* g, Transform* t);
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	~Animation()
	{
		//���j�[�N�|�C���^�j��
		_AnimationSets.release();
		_BlendRateTable.release();
		_EndTime.release();
	}
	
	void Initialize(ID3DXAnimationController* anim);
	void Awake();
	void Update();
	//�A�j���[�V�����Đ�(�⊮�Ȃ�)
	void PlayAnimation(const UINT& animationSetIndex);
	//�A�j���[�V�����Đ�(�⊮����)
	//��1�����@�Đ��������A�j���[�V�����̃C���f�b�N�X
	//��2�����@��Ԏ���
	//��3�����@���[�v��
	void PlayAnimation(const UINT& animationSetIndex, const float& interpolateTime, const int& loopnum = -1);
	//�A�j���[�V�����Đ�(�⊮����)
	//��1�����@�Đ��������A�j���[�V�����̃C���f�b�N�X
	//��2�����@��Ԏ���
	//��3�����@���[�v��
	void PlayAnimation(const UINT& animationSetIndex, const float& interpolateTime, const float& transitionTime, const int& loopnum = -1);
	/*!
	*@brief	�A�j���[�V�����Z�b�g�̎擾�B
	*/
	int GetNumAnimationSet() const
	{
		return _NumAnimSet;
	}
	//���ݍĐ����̃A�j���[�V�����ԍ��擾
	const UINT& GetPlayAnimNo() const
	{
		return _CurrentAnimationSetNo;
	}
	//�A�j���[�V�������I�����Ă��邩�ǂ���
	const bool& GetPlaying()
	{
		return _IsPlaying;
	}

	const double& GetLocalAnimationTime()
	{
		return _LocalAnimationTime;
	}

	const double& NowFrame()
	{
		return _CurrentFrame;
	}
	//�����擾
	const double& GetTimeRatio()
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
	void SetAnimationEndTime(const UINT& idx, const double& endtime)
	{
		//�͈͓����H
		if (idx < _NumAnimSet)
			_EndTime[idx] = endtime;
	}
	const double& GetAnimationEndTime(const UINT& idx)
	{
		//�͈͓����H
		if (idx < _NumAnimSet)
			return _EndTime[idx];
	}
	//�A�j���[�V�����̃��[�J���^�C���ݒ�
	void SetLocalAnimationTime(const UINT& track, const double& t)
	{
		_LocalAnimationTime = t;
		//���Ԑݒ�B
		_AnimController->SetTrackPosition(track, _LocalAnimationTime);
		//�|�W�V�������X�V���邽�߂ɌĂ΂Ȃ���΂����Ȃ�
		_AnimController->AdvanceTime(0.0, NULL);
	}
private:
	ID3DXAnimationController*				_AnimController;		//!<�A�j���[�V�����R���g���[���B
	UINT									_NumAnimSet;				//!<�A�j���[�V�����Z�b�g�̐��B
	std::unique_ptr<ID3DXAnimationSet*[]>	_AnimationSets;			//!<�A�j���[�V�����Z�b�g�̔z��B
	std::unique_ptr<float[]>				_BlendRateTable;			//!<�u�����f�B���O���[�g�̃e�[�u���B
	UINT									_CurrentAnimationSetNo;	//!<���ݍĐ����̃A�j���[�V�����g���b�N�̔ԍ��B
	UINT									_CurrentTrackNo;			//!<���݂̃g���b�N�̔ԍ��B
	UINT									_NumMaxTracks;			//!<�A�j���[�V�����g���b�N�̍ő吔�B
	bool									_IsBlending;				//!<�A�j���[�V�����u�����f�B���O���H
	bool									_IsInterpolate;			//!<��Ԓ��H
	float									_InterpolateEndTime;		//!<��ԏI�����ԁB
	float									_InterpolateTimer;		//!<��Ԏ��ԁB

	std::unique_ptr<double[]> _EndTime;	//�e�A�j���[�V�����̏I�����Ԃ��i�[�����z��
	double _TimeRatio;					//���K�����ꂽ���Ԃ̊����B
	double _LocalAnimationTime;			//���[�J���ȃA�j���[�V�����̌o�ߎ���
	double _CurrentFrame;					//�A�j���[�V�������Đ�����Č��݉��t���[���ڂ��B
	float _PlaySpeed;					//�Đ����x
	int _LoopNum;						//�A�j���[�V���������[�v�����鐔�B
	int _LoopCount;						//���[�v�����J�E���g����B
	bool _IsPlaying;					//�A�j���[�V�����Đ����ł��邱�Ƃ������B
};