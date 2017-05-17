#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class Player;
class HistoryMenuSelect;

class AburaChip : public GameObject
{
public:
	AburaChip(const char* name);
	void Awake()override;
	void Start()override;
	void Update()override;
private:
	//�v���C���[�I�u�W�F�N�g
	Player*		_Player;
	//�q�X�g���[���j���[�Z���N�g�I�u�W�F�N�g
	HistoryMenuSelect* _HistoryMenuSelect;
};
#pragma once
