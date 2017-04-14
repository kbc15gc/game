/*!
 * @brief	�A�j���[�V�����N���X�B
 */
#include <memory>
#include "Component.h"
#include "TextObject.h"

/*!
 * @brief	�A�j���[�V�����N���X�B
 */
class Animation:public Component{
public:
	/*!
	 * @brief	�R���X�g���N�^
	 */
	Animation(GameObject* g, Transform* t) :Component(g,t,typeid(this).name())
	{
		
	}
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	~Animation()
	{
		//���j�[�N�|�C���^�j��
		_AnimationSets.release();
		_BlendRateTable.release();
	}
	
	void Initialize(ID3DXAnimationController* anim);
	void Awake();
	void Update();

	void PlayAnimation(const int& animationSetIndex);
	/*!
	*@brief	�A�j���[�V�����̍Đ��B�A�j���[�V�����̕⊮���s���܂��B
	*@param[in]		animationSetIndex	�Đ��������A�j���[�V�����̃C���f�b�N�X�B
	*@param[in]		_InterpolateTimer		��Ԏ��ԁB
	//��O�����@���[�v��
	*/
	void PlayAnimation(int animationSetIndex, float _InterpolateTimer,int loopnum = -1);
	/*!
	*@brief	�A�j���[�V�����Z�b�g�̎擾�B
	*/
	int GetNumAnimationSet() const
	{
		return _NumAnimSet;
	}
	//���ݍĐ����̃A�j���[�V�����ԍ��擾
	int GetPlayAnimNo() const
	{
		return _CurrentAnimationSetNo;
	}
	//�A�j���[�V�������I�����Ă��邩�ǂ���
	bool GetPlaying()
	{
		return _IsPlaying;
	}

	double GetLocalAnimationTime()
	{
		return _LocalAnimationTime;
	}

	int NowFrame()
	{
		return _CurrentFrame;
	}
	//�����擾
	double GetTimeRatio()
	{
		return _TimeRatio;
	}

	void SetAnimeSpeed(float sp)
	{
		_PlaySpeed = sp;
		//_AnimController->SetTrackSpeed(_CurrentTrackNo, sp);
	}
	/*!
	*@brief	�A�j���[�V�����I�����Ԑݒ�
	*@param[in]		idx	�A�j���[�V�����Z�b�g�̔ԍ��B
	*@param[in]		endtime		�I�����ԁB
	*/
	void SetAnimationEndTime(int idx,double endtime)
	{
		if (idx == 4) {
			OutputDebugString(_T("�v���C���[����\n"));
		}
		_EndTime[idx] = endtime;
	}
	//�A�j���[�V�����̃��[�J���^�C���ݒ�
	void SetLocalAnimationTime(UINT track,double t)
	{
		_LocalAnimationTime = t;
		//���Ԑݒ�B
		_AnimController->SetTrackPosition(track, _LocalAnimationTime);
		//�|�W�V�������X�V���邽�߂ɌĂ΂Ȃ���΂����Ȃ�
		_AnimController->AdvanceTime(0.0, NULL);
	}
private:
	ID3DXAnimationController*				_AnimController;		//!<�A�j���[�V�����R���g���[���B
	int										_NumAnimSet;				//!<�A�j���[�V�����Z�b�g�̐��B
	std::unique_ptr<ID3DXAnimationSet*[]>	_AnimationSets;			//!<�A�j���[�V�����Z�b�g�̔z��B
	std::unique_ptr<float[]>				_BlendRateTable;			//!<�u�����f�B���O���[�g�̃e�[�u���B
	int										_CurrentAnimationSetNo;	//!<���ݍĐ����̃A�j���[�V�����g���b�N�̔ԍ��B
	int										_CurrentTrackNo;			//!<���݂̃g���b�N�̔ԍ��B
	int										_NumMaxTracks;			//!<�A�j���[�V�����g���b�N�̍ő吔�B
	bool									_IsBlending;				//!<�A�j���[�V�����u�����f�B���O���H
	bool									_IsInterpolate;			//!<��Ԓ��H
	float									_InterpolateEndTime;		//!<��ԏI�����ԁB
	float									_InterpolateTimer;		//!<��Ԏ��ԁB

	std::unique_ptr<double[]> _EndTime;	//�e�A�j���[�V�����̏I�����Ԃ��i�[�����z��
	double _TimeRatio;					//���K�����ꂽ���Ԃ̊����B
	double _LocalAnimationTime;			//���[�J���ȃA�j���[�V�����̌o�ߎ���
	int _CurrentFrame;					//�A�j���[�V�������Đ�����Č��݉��t���[���ڂ��B
	float _PlaySpeed;					//�Đ����x
	int _LoopNum;						//�A�j���[�V���������[�v�����鐔�B
	int _LoopCount;						//���[�v�����J�E���g����B
	bool _IsPlaying;					//�A�j���[�V�����Đ����ł��邱�Ƃ������B
};