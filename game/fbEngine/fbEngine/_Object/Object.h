#pragma once
#include "_Object/Noncopyable.h"
//�I�u�W�F�N�g�̊��N���X
class Object : Noncopyable
{
public:
	Object()
	{
		_Name = "Empty";
	}
	Object(const char* name)
	{
		_Name = new char[strlen(name) + 1];
		//������R�s�[
		strcpy(_Name, name);
	}
	virtual ~Object()
	{
		SAFE_DELETE(_Name);
	};

	//�C���X�^���X���쐬���ꂽ���Ɏ��s�����
	//�������z�֐��ł͂Ȃ��̂Ő�΃I�[�o�[���C�h���Ȃ��Ă�����
	virtual void Awake(){};

	//�V�[���̏��������Ɏ��s�����B
	//���������s���������z�֐�
	virtual void Start(){};

	//�X�V���s���������z�֐�
	virtual void Update(){};

	//�X�V�̌�ɌĂяo�����֐�
	//�������z�֐��ł͂Ȃ��̂Ő�΃I�[�o�[���C�h���Ȃ��Ă�����
	virtual void LateUpdate(){};

#ifdef _DEBUG
	// �f�o�b�O�p�̃R�[�h���Ăяo���B
	// �I�u�W�F�N�g�̃A�N�e�B�u��A�N�e�B�u�Ɋւ�炸�Ăяo�����B
	virtual void Debug() {};
#endif

	//�`��̑O�ɌĂяo����܂�
	virtual void PreRender(){};
	//�`����s��
	virtual void Render(){};
	//�|�X�g�G�t�F�N�g�`��
	virtual void PostRender(){};
	//�C���[�W(2D)�`��
	virtual void ImageRender(){};

	// �}�l�[�W���[�ɓo�^���Ă���ꍇ�A�폜���X�g�ɐς܂ꂽ�u�ԂɌĂяo����鏈���B
	// ���f�X�g���N�^�Ƃ͕ʁB
	// ���f�X�g���N�^�����O�ɌĂ΂��B
	virtual void OnDestroy() {};

	//[�L��/�A�N�e�B�u]�ɂȂ������ɌĂ΂��֐��B
	virtual void OnEnable() {};

	//[����/��A�N�e�B�u]�ɂȂ������ɌĂ΂��֐��B
	virtual void OnDisable() {};

	const char* GetName() const
	{
		return _Name;
	}
protected:
	//�I�u�W�F�N�g�̖��O
	char* _Name;
};