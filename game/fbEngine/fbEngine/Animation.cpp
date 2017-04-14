#include "Animation.h"

void Animation::Initialize(ID3DXAnimationController* anim)
{
	_AnimController = anim;
	_NumAnimSet = _AnimController->GetMaxNumAnimationSets();
	_NumMaxTracks = _AnimController->GetMaxNumTracks();
	_BlendRateTable.reset(new float[_NumMaxTracks]);
	_AnimationSets.reset(new ID3DXAnimationSet*[_NumAnimSet]);
	_EndTime.reset(new double[_NumAnimSet]);
	FOR(i,_NumAnimSet)
	{
		_EndTime[i] = -1.0f;
	}
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
	}
}

void Animation::Awake()
{
	_AnimController = nullptr;
	_NumAnimSet = 0;
	_IsBlending = false;
	_IsInterpolate = false;
	_NumMaxTracks = 0;
	_InterpolateTimer = 0.0f;
	_InterpolateEndTime = 0.0f;
	_CurrentTrackNo = 0;
	_PlaySpeed = 1.0f;
}

void Animation::PlayAnimation(const int& animationSetIndex)
{
	if (animationSetIndex < _NumAnimSet) {
		if (_AnimController) {
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
			_AnimController->SetTrackAnimationSet(_CurrentTrackNo, _AnimationSets[(_NumAnimSet - 1) - _CurrentAnimationSetNo]);
			SetLocalAnimationTime(0, 0.0f);
		}
	}
	else {
		//TK_LOG("warning!!! animationSetIndex is over range!!!!!");
	}
}

void Animation::PlayAnimation(int animationSetIndex, float Timer, int lnum)
{
	if (animationSetIndex < _NumAnimSet) {
		if (_AnimController) {
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
			_InterpolateEndTime = Timer;
			//�g���b�NNo�ύX
			_CurrentTrackNo = (_CurrentTrackNo + 1) % _NumMaxTracks;
			//�g���b�N��L���ɂ���B
			_AnimController->SetTrackEnable(_CurrentTrackNo, TRUE);
			//�g���b�N�ɃA�j���[�V�����Z�b�g
			_AnimController->SetTrackAnimationSet(_CurrentTrackNo, _AnimationSets[(_NumAnimSet - 1) - _CurrentAnimationSetNo]);
			SetLocalAnimationTime(_CurrentTrackNo,0.0f);
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

		//�f���^�^�C��
		double delta = Time::DeltaTime() * (double)_PlaySpeed;

		//�⊮�����B
		if (_IsInterpolate) {
			//��Ԓ��B
			_InterpolateTimer += delta;
			float weight = 0.0f;
			if (_InterpolateTimer > _InterpolateEndTime) {
				//��ԏI���B
				_IsInterpolate = false;
				weight = 1.0f;
				_AnimController->SetTrackWeight(_CurrentTrackNo, weight);
				//���݂̃g���b�N�ȊO�𖳌��ɂ���B
				FOR(i, _NumMaxTracks){
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

		//�t���[���𑝉�������
		_CurrentFrame++;
		//���݂̃g���b�N�̃A�j���[�V�����Z�b�g�擾
		LPD3DXANIMATIONSET aniset;
		//�ݒ肳��Ă���g���b�N����A�j���[�V�����Z�b�g�擾
		_AnimController->GetTrackAnimationSet(_CurrentTrackNo, &aniset);

		//�A�j���[�V�����̏I�����Ԑݒ�
		//�G���h�^�C�����w�肳��Ă���̂Ȃ炻�����D��
		double endtime = _EndTime[_CurrentAnimationSetNo] > 0.0f ? _EndTime[_CurrentAnimationSetNo] : aniset->GetPeriod();

		//���[�J���^�C���ɉ��Z
		_LocalAnimationTime += delta;
		//�O���[�o���^�C���ɉ��Z
		_AnimController->AdvanceTime(delta, NULL);
		//�������v�Z
		_TimeRatio = min(1.0f, _LocalAnimationTime / endtime);

		SetLocalAnimationTime(_CurrentTrackNo, _LocalAnimationTime);
		FOR(i, _NumMaxTracks)
		{
			D3DXTRACK_DESC desc;
			_AnimController->GetTrackDesc(i, &desc);
			int a = 0;
		}

		//�A�j���[�V�����I�����Ԃ𒴂����B
		if (endtime <= _LocalAnimationTime) {
			//�o�߂����t���[��������
			_CurrentFrame = 0;
			//���[�v������
			_LoopCount++;

			//�A�j���[�V�������Ԃ����Z�b�g
			SetLocalAnimationTime(_CurrentTrackNo, _LocalAnimationTime - endtime);

			if (_LoopNum != -1 &&		//�������[�v�ł͂Ȃ�
				_LoopCount >= _LoopNum)	//�J�E���g���w�肵�����ȏ�ɂȂ���
			{
				//�A�j���[�V�����I��
				_IsPlaying = false;
				//�Ō�̕��Ŏ~�߂�B
				//SetLocalAnimationTime(_CurrentTrackNo, endtime - 0.001f);
			}
		}
	}
}