#include"fbstdafx.h"
#include "Animation.h"

void Animation::Initialize(ID3DXAnimationController* anim)
{
	_AnimController = anim;
	_NumAnimSet = _AnimController->GetMaxNumAnimationSets();
	_NumMaxTracks = _AnimController->GetMaxNumTracks();
	_BlendRateTable.reset(new float[_NumMaxTracks]);
	_AnimationSets.reset(new ID3DXAnimationSet*[_NumAnimSet]);
	_EndTime.reset(new double[_NumAnimSet]);
	FOR(i, _NumMaxTracks)
	{
		_BlendRateTable[i] = 1.0f;
		//�e�g���b�N�̃��[�J���^�C����0�ɏ������B
		SetLocalAnimationTime(i, 0.0f);
	}
	//�A�j���[�V�����Z�b�g���������B
	FOR(i, _NumAnimSet)
	{
		//�A�j���[�V�����Z�b�g���擾���Ĕz��Ɋi�[
		_AnimController->GetAnimationSet(i, &_AnimationSets[i]);
		//�I�����Ԏ擾
		_EndTime[i] = _AnimationSets[i]->GetPeriod();
	}
}

Animation::Animation(GameObject * g, Transform * t) :
	Component(g, t, typeid(this).name()),
	_AnimController(nullptr),
	_NumAnimSet(0),
	_IsBlending(false),
	_IsInterpolate(false),
	_NumMaxTracks(0),
	_InterpolateTimer(0.0f),
	_InterpolateEndTime(0.0f),
	_CurrentTrackNo(0),
	_PlaySpeed(1.0f)
{
#ifdef _DEBUG
	mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
}

Animation::~Animation()
{
	//���j�[�N�|�C���^�j��
	_AnimationSets.release();
	_BlendRateTable.release();
	_EndTime.release();

	//�L���[�����
	unsigned int size = _AnimationQueue.size();
	FOR(idx,size)
	{
		PlayAnimInfo* info = _AnimationQueue.front();
		SAFE_DELETE(info);
		_AnimationQueue.pop();
	}
}

void Animation::Awake()
{
	
}

void Animation::PlayAnimation(const UINT animationSetIndex)
{
	if (animationSetIndex < _NumAnimSet) {
		if (_AnimController) {

			for (auto& call : _callback) {
				// �֐��|�C���^�ɐݒ肳�ꂽ�֐������s�B

				(call->object->*(call->callback))(animationSetIndex);
			}

			//�Đ��J�n
			_IsPlaying = true;
			_CurrentFrame = 0;
			_CurrentAnimationSetNo = animationSetIndex;
			_CurrentTrackNo = 0;

			//0�ԖڈȊO�̃g���b�N�͖����ɂ���B
			FOR(i, _NumMaxTracks)
			{
				if (i == _CurrentTrackNo)
				{
					_AnimController->SetTrackEnable(i, TRUE);
				}
				else
				{
					_AnimController->SetTrackEnable(i, FALSE);
				}
			}
			//�g���b�N�ɃA�j���[�V�����Z�b�g
			LPD3DXANIMATIONSET aniset = _AnimationSets[(_NumAnimSet - 1) - _CurrentAnimationSetNo];
			_AnimController->SetTrackAnimationSet(_CurrentTrackNo, aniset);
			_EndTime[animationSetIndex] = _EndTime[animationSetIndex] > 0.0f ? _EndTime[animationSetIndex] : aniset->GetPeriod();
			SetLocalAnimationTime(0, 0.0f);
		}
	}
	else {
		//TK_LOG("warning!!! animationSetIndex is over range!!!!!");
	}
}

void Animation::PlayAnimation(const UINT animationSetIndex, const float interpolateTime, const int lnum)
{
	if (animationSetIndex < _NumAnimSet) {
		if (_AnimController) {
			for (auto& call : _callback) {
				// �֐��|�C���^�ɐݒ肳�ꂽ�֐������s�B

				(call->object->*(call->callback))(animationSetIndex);
			}

			//���[�v���ݒ�
			_LoopNum = lnum;
			_LoopCount = 0;
			//�Đ��J�n
			_IsPlaying = true;
			_CurrentFrame = 0;
			//���ݍĐ����̃A�j���[�V�����Z�b�gNo
			_CurrentAnimationSetNo = animationSetIndex;
			//��ԊJ�n�̈�B
			_IsInterpolate = true;
			//�⊮���ԏ�����
			_InterpolateTimer = 0.0f;
			//�⊮�I�����ԏI��
			_InterpolateEndTime = interpolateTime;

			//�g���b�NNo�ύX
			_CurrentTrackNo = (_CurrentTrackNo + 1) % _NumMaxTracks;
			//�g���b�N��L���ɂ���B
			_AnimController->SetTrackEnable(_CurrentTrackNo, TRUE);
			//�g���b�N�ɃA�j���[�V�����Z�b�g
			LPD3DXANIMATIONSET aniset = _AnimationSets[(_NumAnimSet - 1) - _CurrentAnimationSetNo];
			_AnimController->SetTrackAnimationSet(_CurrentTrackNo, aniset);
			_EndTime[animationSetIndex] = _EndTime[animationSetIndex] > 0.0f ? _EndTime[animationSetIndex] : aniset->GetPeriod();
			SetLocalAnimationTime(_CurrentTrackNo, 0.0f);
		}
	}
	else {
		//TK_LOG("warning!!! animationSetIndex is over range!!!!!");
	}
}

bool Animation::PlayAnimation(const UINT animationSetIndex, const float interpolateTime, const float transitionTime, const int loopnum)
{
	bool play;
	if(play = (transitionTime <= _TimeRatio))
	{
		PlayAnimation(animationSetIndex, interpolateTime, loopnum);
	}
	return play;
}

void Animation::AddAnimationQueue(PlayAnimInfo* info)
{
	if (_IsPlaying)
	{
		//�L���[�ɒǉ��B
		_AnimationQueue.push(info);
	}
	else
	{
		//���ʂɍĐ��B
		PlayAnimation(info->Index, info->InterpolateTime, info->TransitionTime, info->LoopNum);
		//���
		SAFE_DELETE(info);
	}
}

void Animation::_NextQueue()
{
	//���̃A�j���[�V�����Y�������L���[������o���B
	PlayAnimInfo* info = _AnimationQueue.front();
	//�A�j���[�V�����Đ��B
	if (PlayAnimation(info->Index, info->InterpolateTime, info->TransitionTime, info->LoopNum))
	{
		_AnimationQueue.pop();
		//���
		SAFE_DELETE(info);
	}
}

void Animation::_EndAnimation(const float endtime)
{
	if (_LoopNum != -1 &&		//�������[�v�ł͂Ȃ�
		_LoopCount >= _LoopNum)	//�J�E���g���w�肵�����ȏ�ɂȂ���
	{
		//�A�j���[�V�����I��
		_IsPlaying = false;
		//�Ō�̕��Ŏ~�߂�B
		SetLocalAnimationTime(_CurrentTrackNo, endtime - 0.001f);
	}
	else
	{
		//�A�j���[�V�������Ԃ����Z�b�g
		SetLocalAnimationTime(_CurrentTrackNo, _LocalAnimationTime - endtime);
	}
}

void Animation::_InterpolateAnimation(const float delta)
{
	//�⊮�����B
	if (_IsInterpolate) {
		//��Ԏ��ԉ��Z�B
		_InterpolateTimer += delta;
		//�d�݁B
		float weight = 0.0f;
		if (_InterpolateTimer > _InterpolateEndTime) {
			//��ԏI���B
			_IsInterpolate = false;
			weight = 1.0f;
			_AnimController->SetTrackWeight(_CurrentTrackNo, weight);
			//���݂̃g���b�N�ȊO�𖳌��ɂ���B
			FOR(i, _NumMaxTracks) {
				if (i != _CurrentTrackNo) {
					_AnimController->SetTrackEnable(i, FALSE);
				}
			}
		}
		//�⊮��
		else {
			//������ݒ�
			weight = _InterpolateTimer / _InterpolateEndTime;
			float invWeight = 1.0f - weight;
			//�E�F�C�g��ݒ肵�Ă����B
			for (int i = 0; i < _NumMaxTracks; i++) {
				if (i != _CurrentTrackNo) {
					_AnimController->SetTrackWeight(i, _BlendRateTable[i] * invWeight);
				}
				else {
					_AnimController->SetTrackWeight(i, weight);
				}
			}
		}
	}
}

void Animation::Update()
{
	//�A�j���[�V�����Đ����Ȃ�B
	if (_IsPlaying)
	{
		//null�`�F�b�N
		if (_AnimController == nullptr)
			return;

		//�f���^�^�C��
		double delta = Time::DeltaTime() * (double)_PlaySpeed;		
		//�A�j���[�V�����̕⊮
		_InterpolateAnimation(delta);
				
		//�t���[���𑝉�������
		_CurrentFrame += 1.0f;
		//���[�J���^�C���ɉ��Z
		_LocalAnimationTime += delta;		
		//�O���[�o���^�C���ɉ��Z
		_AnimController->AdvanceTime(delta, NULL);

		//�G���h�^�C�����擾�B
		double endtime = _EndTime[_CurrentAnimationSetNo];
		//�A�j���[�V�����̍Đ��������v�Z�B( �o�ߎ��� / �I������ )
		_TimeRatio = min(1.0f, _LocalAnimationTime / endtime);

		//�L���[�����邩�ǂ����H
		if (_AnimationQueue.size() > 0)
		{
			//���̗v�f��
			_NextQueue();
		}

		//�A�j���[�V�����I�����Ԃ𒴂����B(1Loop�I�������B)
		if (endtime <= _LocalAnimationTime) 
		{
			//�o�߂����t���[��������
			_CurrentFrame = 0.0f;
			//���[�v������
			_LoopCount++;

			//�A�j���[�V�������I���������̏����B				
			_EndAnimation(endtime);
		}
		else
		{
			//���ʂɉ��Z
			SetLocalAnimationTime(_CurrentTrackNo, _LocalAnimationTime);
		}
	}
}