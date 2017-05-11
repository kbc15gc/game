#include "HistoryButton.h"

#include "fbEngine\_Object\_Component\_2D\Sprite.h"
#include "fbEngine\_Object\_Component\_2D\CircleCollision.h"
#include "GameObject\Village\HistoryManager.h"

namespace
{
	//�e�N�X�`���t�@�C����
	char* ChipTexture[(int)ChipID::NUM]
	{
		"nullChip.png",
		"FireChip.png",
		"IronChip.png",
		"OilChip.png",
	};
}

HistoryMenuButton::HistoryMenuButton(const char * name):
	GameObject(name),
	_ChipID(ChipID::NONE)
{
}

void HistoryMenuButton::Awake()
{
	_Sprite = AddComponent<Sprite>();
	_Circle = AddComponent<CircleCollision>();
}

void HistoryMenuButton::Update()
{
	if(MouseInput->GetUp(MouseInE::L_CLICK) &&		//���N���b�N
		_Circle->Judgment(MouseInput->GetCursorPosOnWindow(g_MainWindow)))	//�d�Ȃ��Ă���B
	{
		_NextChip();
	}
}

void HistoryMenuButton::SetChipID(const ChipID & chipid)
{
	//ID�ݒ�
	_ChipID = chipid;
	//�e�N�X�`���ǂݍ���
	TEXTURE* tex = LOADTEXTURE(ChipTexture[(int)_ChipID]);
	_Sprite->SetTexture(tex);
	//���a
	float len = max(tex->Size.x, tex->Size.y);
	_Circle->SetRadius(len / 2);
}

void HistoryMenuButton::_NextChip()
{
	int id = (int)_ChipID;
	while (true)
	{
		//���֐i�߂鏈��
		id = (id + 1) % (int)ChipID::NUM;
		//�`�b�v���Z�b�g����
		if (INSTANCE(HistoryManager)->SetHistoryChip(_Id, _Idx, (const int)id))
		{
			//�Z�b�g���ł����̂Ȃ�
			SetChipID((ChipID)id);
			break;
		}
	}
}
