/*!
 * @brief	�A�j���[�V�����N���X�B
 */
#include <memory>
#include "Component.h"

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
	*@param[in]		_InterpolateTime		��Ԏ��ԁB
	//��O�����@���[�v��
	*/
	void PlayAnimation(int animationSetIndex, float _InterpolateTime,int loopnum = -1);
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

	double GetNowTime()
	{
		return _NowTime;
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
		_AnimController->SetTrackSpeed(_CurrentTrackNo, sp);
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
	float									_InterpolateTime;		//!<��Ԏ��ԁB

	double _TimeRatio;	//���K�����ꂽ���Ԃ̊����B
	double _NowTime;	//���݂̃A�j���[�V�����̎���
	int _CurrentFrame;		//���݂̃t���[����
	float _PlaySpeed;//�Đ����x
	int _LoopNum;//�w�肵�����[�v��
	int _LoopCount;//���[�v�̃J�E���g
	bool _IsPlaying;//�A�j���[�V�����Đ���
};