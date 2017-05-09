#include "TextBox.h"

TextBox::TextBox(const char * name):
	GameObject(name),
	_CharNum(0),
	_TextSpeed(1.0f),
	_Open(false),
	_PlayingAnim(false),
	_Scale(0.0f)
{
}

TextBox::~TextBox()
{
}

void TextBox::Awake()
{
	_BoxImage[0] = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BoxImage",_Priority);
	_BoxImage[1] = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BoxImage", _Priority);
	_Text = INSTANCE(GameObjectManager)->AddNew<TextObject>("Text", _Priority);

	//�J�[�\��
	_BoxImage[0]->SetTexture(LOADTEXTURE("cursor.png"));
	_BoxImage[0]->SetPivot(0.5, 1.0);
	//�{��
	_BoxImage[1]->SetTexture(LOADTEXTURE("window.png"));
	
	_Text->Initialize(L"", 25.0f);
	_Text->SetFormat((int)fbText::TextFormatE::CENTER | (int)fbText::TextFormatE::UP);
	
	//�e�q�֌W��t����B
	_BoxImage[0]->transform->SetParent(transform);
	_BoxImage[1]->transform->SetParent(_BoxImage[0]->transform);
	_Text->transform->SetParent(_BoxImage[1]->transform);

	//�I�u�W�F�N�g���\���ɂ���
	for each (Transform* child in transform->GetChildren())
	{
		child->gameObject->SetActive(false, true);
	}
	transform->SetScale(Vector3::zero);
}

void TextBox::Update()
{
	//�A�j���[�V�����B
	_Animation();
	//�\�����镶�����̑����B
	_IncreaseChar();

	//�A�j���[�V�������Ă��Ȃ��B
	if (!_PlayingAnim)
	{
		//���b�Z�[�W������(�f�o�b�O�p)
		if (KeyBoardInput->isPush(DIK_SPACE))
		{
			//�ő啶�����擾
			int max = _Text->GetMaxCharNum();
			//�e�L�X�g�̓r���Ȃ�Ō�܂ŏo���B
			if (_CharNum < max)
			{
				_CharNum = max;
				_Text->SetCharNum(_CharNum);
			}
			else
			{
				//���̃��b�Z�[�W
				NextMessage();
			}
		}
	}
}

void TextBox::OpenMessage(const int & id)
{
	//���S�ɕ��Ă���Ƃ��ɊJ�����Ƃ��ł���B
	if (!_Open && !_PlayingAnim)
	{
		_SetMessage(id);
		_PlayingAnim = true;
		_Scale = 0.0f;
	}
}

void TextBox::CloseMessage()
{
	_Message = nullptr;
	_Open = false;
	_PlayingAnim = true;
	_Scale = 0.0f;

	//�e�L�X�g�����B
	_Text->SetActive(_Open);
}

void TextBox::NextMessage()
{
	//���̃��b�Z�[�W���Đ�
	if (_Message)
	{
		_SetMessage(_Message->NextID);
	}
}

void TextBox::_SetMessage(const int & id)
{
	//�\��������������
	_CharNum = 0;
	_Text->SetCharNum((unsigned int)_CharNum);
	if (id >= 0)
	{
		//���b�Z�[�W���擾
		_Message = INSTANCE(MessageManager)->GetMess(id);
		if (_Message)
		{
			_Text->SetString(_Message->Text);
			Vector2 textSize = _Text->GetLength() + Vector2(_Text->GetText()->GetSize() * FontSize);
			_BoxImage[1]->SetSize(textSize);
			float halfHeight = textSize.y / 2;
			_BoxImage[1]->transform->SetLocalPosition(Vector3(0, -(halfHeight + _BoxImage[0]->GetTexture()->Size.y), 0));

			_Open = true;
			//�S�Ă��A�N�e�B�u�ɂ���B
			for each (Transform* child in transform->GetChildren())
			{
				child->gameObject->SetActive(_Open,true);
			}
		}
		else
		{
			//���b�Z�[�W��������Ȃ������̂ŕ���B
			CloseMessage();
		}
	}
	//-1�ȉ��Ȃ烁�b�Z�[�W�����
	else
	{
		CloseMessage();
	}
}

void TextBox::_Animation()
{
	//�A�j���[�V�����Đ����Ȃ�
	if (_PlayingAnim)
	{
		//�|���鎞��
		float time = 1.0f;
		_Scale += Time::DeltaTime() / time;
		if (_Open)
		{
			//���񂾂�傫������B
			Vector3 sca = Vector3::Lerp(Vector3::zero, Vector3::one, _Scale);
			transform->SetScale(sca);

			//�J���������Ȃ�
			if (sca.Length() >= Vector3::one.Length())
			{
				_PlayingAnim = false;
			}
		}
		else
		{
			//���񂾂񏬂�������B
			Vector3 sca = Vector3::Lerp(Vector3::one, Vector3::zero, _Scale);
			transform->SetScale(sca);

			//�傫�����Ȃ��Ȃ����Ȃ�
			if (sca.Length() <= Vector3::zero.Length())
			{
				//�{�b�N�X�����B
				for each (ImageObject* image in _BoxImage)
				{
					image->SetActive(_Open);
				}
				_PlayingAnim = false;
			}
		}
	}
}

void TextBox::_IncreaseChar()
{
	//�A�j���[�V�������Ă��Ȃ��Ƃ��ɑ���������B
	if (!_PlayingAnim)
	{
		//�ő吔�ȉ��Ȃ瑝�₷�B
		if (_CharNum < _Text->GetMaxCharNum())
		{
			//���񂾂�\�����镶�����𑝂₷�B
			_CharNum += Time::DeltaTime()*_TextSpeed;
			_Text->SetCharNum((unsigned int)_CharNum);
		}
	}
}
