#include "stdafx.h"
#include "GameObject\Component\AnimationEvent.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "fbEngine\_Object\_GameObject\GameObject.h"


AnimationEventPlayer::~AnimationEventPlayer() {
	for (auto& work : _animationEvents) {
		for (auto Event : work) {
			SAFE_DELETE(Event);
		}
		work.clear();
	}
	_animationEvents.clear();
};

void AnimationEventPlayer::Init(int animationNum)
{
	// �A�j���[�V�����̐������z��ǉ��B
	_animationEvents = vector<vector<EventData*>>(animationNum);
}

void AnimationEventPlayer::Update() {
	if (_animationEvents.size() <= 0) {
		// �A�j���[�V�����C�x���g������ݒ肳��Ă��Ȃ��B
		return;
	}
	Animation* work = gameObject->GetComponent<Animation>();
	if (work) {
		//���ݍĐ����̃A�j���[�V�����ԍ��擾�B
		int nowAnim = work->GetPlayAnimNo();
		if (nowAnim != _playAnimationNo) {
			// �ʂ̃A�j���[�V�������Đ����ꂽ�B

			for (auto eventData : _animationEvents[_playAnimationNo]) {
				eventData->isPlay = false;
			}
			_playAnimationNo = nowAnim;
		}
		else if(work->GetLocalAnimationTime() < _nowLocalTime){
			// 1���[�v�I�������B
			// �������͓����A�j���[�V������������x�Đ����ꂽ�B

			for (auto eventData : _animationEvents[_playAnimationNo]) {
				eventData->isPlay = false;
			}
			_nowLocalTime = work->GetLocalAnimationTime();
			return;
		}

		_nowLocalTime = work->GetLocalAnimationTime();

		for (auto eventData : _animationEvents[_playAnimationNo]) {
			//���Ԃ���v�������C�x���g�Ăяo���B
			if (_nowLocalTime >= eventData->playTime)
			{
				if (!eventData->isPlay) {
					// �C�x���g���܂����s����ĂȂ��B

					// �֐��|�C���^�ɐݒ肳�ꂽ�֐������s�B
					(gameObject->*(eventData->Event))();
					eventData->isPlay = true;
				}
			}
		}
	}
	else {
		// Animation�R���|�[�l���g���A�^�b�`����Ă��Ȃ��B
		abort();
	}
}