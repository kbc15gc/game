#include "Animation.h"

void Animation::Initialize(ID3DXAnimationController* anim)
{
	_AnimController = anim;
	_NumAnimSet = _AnimController->GetMaxNumAnimationSets();
	_NumMaxTracks = _AnimController->GetMaxNumTracks();
	_BlendRateTable.reset(new float[_NumMaxTracks]);
	_AnimationSets.reset(new ID3DXAnimationSet*[_NumAnimSet]);
	_EndTime.reset(new double[_NumAnimSet]);
	for(int i=0; i<_NumAnimSet;i++)
	{
		_EndTime[i] = -1.0f;
	}
	for (int i = 0; i < _NumMaxTracks; i++){
		_BlendRateTable[i] = 1.0f;
	}
	//�A�j���[�V�����Z�b�g���������B
	for (int i = 0; i < _NumAnimSet; i++) {
		_AnimController->GetAnimationSet(i, &_AnimationSets[i]);
		_AnimController->SetTrackPosition(i, 0.0f);
		_AnimController->AdvanceTime(0, NULL);
	}
}

void Animation::Awake()
{
	_AnimController = nullptr;
	_NumAnimSet = 0;
	_IsBlending = false;
	_IsInterpolate = false;
	_NumMaxTracks = 0;
	_InterpolateTime = 0.0f;
	_InterpolateEndTime = 0.0f;
	_CurrentTrackNo = 0;
}

void Animation::PlayAnimation(const int& animationSetIndex)
{
	if (animationSetIndex < _NumAnimSet) {
		if (_AnimController) {
			//�Đ��J�n
			_IsPlaying = true;
			_CurrentFrame = 0;
			//�A�j���[�V�����̃O���[�o���^�C�������Z�b�g
			_AnimController->ResetTime();
			_CurrentAnimationSetNo = animationSetIndex;
			_CurrentTrackNo = 0;
			//0�ԖڈȊO�̃g���b�N�͖����ɂ���B
			for (int i = 1; i < _NumMaxTracks; i++) {
				_AnimController->SetTrackEnable(i, FALSE);
			}
			_AnimController->SetTrackWeight(0, 1.0f);
			_AnimController->SetTrackAnimationSet(_CurrentTrackNo, _AnimationSets[(_NumAnimSet - 1) - _CurrentAnimationSetNo]);
			_AnimController->SetTrackEnable(0, TRUE);
			_AnimController->SetTrackPosition(0, 0.0f);
			_AnimController->AdvanceTime(0, NULL);
			SetAnimeSpeed(1.0f);
		}
	}
	else {
		//TK_LOG("warning!!! animationSetIndex is over range!!!!!");
	}
}

void Animation::PlayAnimation(int animationSetIndex, float _InterpolateTime, int lnum)
{
	if (animationSetIndex < _NumAnimSet) {
		if (_AnimController) {
			//���[�v���ݒ�
			_LoopNum = lnum;
			_LoopCount = 0;
			//�Đ��J�n
			_IsPlaying = true;
			_CurrentFrame = 0;
			//�A�j���[�V�����̃O���[�o���^�C�������Z�b�g
			_AnimController->ResetTime();
			_CurrentAnimationSetNo = animationSetIndex;
			//��ԊJ�n�̈�B
			_IsInterpolate = true;
			this->_InterpolateTime = 0.0f;
			_InterpolateEndTime = _InterpolateTime;
			_CurrentTrackNo = (_CurrentTrackNo + 1) % _NumMaxTracks;
			//�g���b�N�ɃA�j���[�V�����Z�b�g
			_AnimController->SetTrackAnimationSet(_CurrentTrackNo, _AnimationSets[(_NumAnimSet - 1) - _CurrentAnimationSetNo]);
			_AnimController->SetTrackEnable(_CurrentTrackNo, TRUE);
			_AnimController->SetTrackPosition(_CurrentTrackNo, 0.0f);
			SetAnimeSpeed(1.0f);
			_AnimController->AdvanceTime(0, NULL);
		}
	}
	else {
		//TK_LOG("warning!!! animationSetIndex is over range!!!!!");
	}
}

void Animation::Update()
{
	//�w�肳�ꂽ���[�v���Ȃ����ǂ���
	if (_IsPlaying)
	{
		//null�`�F�b�N
		if (_AnimController == nullptr)
			return;

		//�t���[���𑝉�������
		_CurrentFrame++;
		//���݂̃g���b�N�̃A�j���[�V�����Z�b�g�擾
		LPD3DXANIMATIONSET aniset;
		//�ݒ肳��Ă���g���b�N����A�j���[�V�����Z�b�g�擾
		_AnimController->GetTrackAnimationSet(_CurrentTrackNo, &aniset);

		//�A�j���[�V�����̏I�����Ԑݒ�
		//�G���h�^�C�����w�肳��Ă���̂Ȃ炻�����D��
		double endtime = _EndTime.get()[_CurrentAnimationSetNo] > 0.0f ? _EndTime[_CurrentAnimationSetNo] : aniset->GetPeriod();
		//���Ԃ�i�߂�O�Ɍ��݂̃A�j���[�V�����̎��Ԃ��擾
		double nowtime = aniset->GetPeriodicPosition(_AnimController->GetTime());

		//�O�t���[������̍������v�Z
		double delta = Time::DeltaTime() * (double)_PlaySpeed;
		//���ڎ擾����ƍő�l�𒴂��Ă����ꍇ0�ɖ߂����̂Ŏ��O�Ōv�Z�B
		_NowTime = nowtime + delta;
		//�A�j���[�V�����̎��ԉ��Z
		_AnimController->AdvanceTime(delta, NULL);
		//�������v�Z
		_TimeRatio = min(1.0f, _NowTime / endtime);

		//�⊮�����B
		if (_IsInterpolate) {
			//��Ԓ��B
			_InterpolateTime += delta;
			float weight = 0.0f;
			if (_InterpolateTime > _InterpolateEndTime) {
				//��ԏI���B
				_IsInterpolate = false;
				weight = 1.0f;
				_AnimController->SetTrackWeight(_CurrentTrackNo, weight);
				//���݂̃g���b�N�ȊO�𖳌��ɂ���B
				for (int i = 0; i < _NumMaxTracks; i++) {
					if (i != _CurrentTrackNo) {

						_AnimController->SetTrackEnable(i, FALSE);
					}
				}
			}
			else {
				weight = _InterpolateTime / _InterpolateEndTime;
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

		//�A�j���[�V�����I�����Ԃ𒴂����B
		if (endtime <= _NowTime) {
			//�o�߂����t���[��������
			_CurrentFrame = 0;
			//���[�v������
			_LoopCount++;

			//�O���[�o���^�C�����Z�b�g
			_AnimController->ResetTime();
			//�A�j���[�V�������Ԃ����Z�b�g
			_AnimController->SetTrackPosition(_CurrentTrackNo, 0);
			//���ߕ��𑫂�
			_AnimController->AdvanceTime(_NowTime - endtime, NULL);

			if (_LoopNum != -1 &&		//�������[�v�ł͂Ȃ�
				_LoopCount >= _LoopNum)	//�J�E���g���w�肵�����ȏ�ɂȂ���
			{
				//�A�j���[�V�����I��
				_IsPlaying = false;
			}
		}
	}
}