#include "Animation.h"

void Animation::Initialize(ID3DXAnimationController* anim)
{
	_AnimController = anim;
	_NumAnimSet = _AnimController->GetMaxNumAnimationSets();
	_NumMaxTracks = _AnimController->GetMaxNumTracks();
	_BlendRateTable.reset(new float[_NumMaxTracks]);
	_AnimationSets.reset(new ID3DXAnimationSet*[_NumAnimSet]);
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
		_CurrentFrame++;
		if (_AnimController == nullptr)
			return;
		//���݂̃g���b�N�̃A�j���[�V�����Z�b�g�擾
		LPD3DXANIMATIONSET aniset;
		_AnimController->GetTrackAnimationSet(_CurrentTrackNo, &aniset);
		//���̃A�j���[�V�������Đ�������܂ł̎���
		double maxtime = aniset->GetPeriod() / (double)_PlaySpeed;
		//���݂̃A�j���[�V�����̎��Ԃ��擾
		_NowTime = aniset->GetPeriodicPosition(_AnimController->GetTime());
		double delta = Time::DeltaTime();
		//����
		_TimeRatio = min(1.0f, (_NowTime + delta) / maxtime);

		//�A�j���[�V�����̎��ԉ��Z
		//_AnimController->AdvanceTime(delta, NULL);
		_AnimController->AdvanceTime(Time::DeltaTime(), NULL);

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

		//�Đ����Ԃ𒴂���
		if (maxtime <= _NowTime + delta)
		{
			_CurrentFrame = 0;
			_LoopCount++;
			//�������[�v�ł͂Ȃ�
			//�J�E���g���w�肵�����ȏ�ɂȂ���
			if (_LoopNum != -1 &&
				_LoopCount >= _LoopNum)
			{
				//�A�j���[�V�����I��
				_IsPlaying = false;
				//�Ō�̕��Ŏ~�߂Ă���
				_AnimController->SetTrackPosition(_CurrentTrackNo, maxtime - 0.001f);
				_AnimController->AdvanceTime(0,NULL);
			}
		}

	}
}