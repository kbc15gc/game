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
		for (auto eventData : _animationEvents[nowAnim]) {
			//���Ԃ���v�������C�x���g�Ăяo���B
			if (work->GetLocalAnimationTime() >= eventData->playTime)
			{
				if (!eventData->isPlay) {
					// �C�x���g���܂����s����ĂȂ��B

					// �֐��|�C���^�ɐݒ肳�ꂽ�֐������s�B
					(gameObject->*(eventData->Event))();
					eventData->isPlay = true;
				}
			}

			if (work->GetLocalAnimationTime() >= work->GetAnimationEndTime(nowAnim)) {
				// �A�j���[�V������1���[�v�I�����Ă��邩�B

				eventData->isPlay = false;
			}
		}
	}
	else {
		// Animation�R���|�[�l���g���A�^�b�`����Ă��Ȃ��B
		abort();
	}
}