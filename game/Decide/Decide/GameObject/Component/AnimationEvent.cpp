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

void AnimationEventPlayer::Init()
{
	// �A�j���[�V�����̐������z��ǉ��B
	if (gameObject->GetComponent<Animation>()) {
		_animationEvents = vector<vector<EventData*>>(gameObject->GetComponent<Animation>()->GetNumAnimationSet());
	}
#ifdef _DEBUG
	else {
		// ���A�j���[�V�����R���|�[�l���g���ɒǉ��������B
		abort();
	}
#endif // _DEBUG
	_isFirst = false;
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
			if (fabsf(work->GetLocalAnimationTime() - eventData->playTime) <= 0.001f)
			{
				// �֐��|�C���^�ɐݒ肳�ꂽ�֐������s�B
				(gameObject->*(eventData->Event))();
			}
		}
	}
	else {
		// Animation�R���|�[�l���g���A�^�b�`����Ă��Ȃ��B
		abort();
	}
}