#pragma once
class TextObject;
class ImageObject;

class Money2D :
	public GameObject
{
public:
	Money2D(const char * name);
	~Money2D();
	void Awake()override;
	void Start()override;
	void Update()override;

	//�e�L�X�g�I�u�W�F�N�g�ƒǉ����邨���̗ʂ�ݒ�B
	void Initialize(int money = 0);

private:
	//���肵�������̗ʂ�\������e�L�X�g�B
	TextObject* _MoneyText;

	//���肵�܂����摜�B
	ImageObject* _GetImage;

	//���O�p�w�i�B
	ImageObject* _LogBackImage;

	//�e�L�X�g��\������ꏊ�B
	Vector3 _TextPos = Vector3::zero;

	//�摜��\������ꏊ�B
	Vector3 _ImagePos = Vector3::zero;

	//���O�p�w�i�̏ꏊ�B
	Vector3 _LogBackPos = Vector3::zero;

	//�ߋ��ɒǉ����ꂽ�����B
	string _AddMoenyLog[5];

	//�����O�����g���Ă��邩�B
	int _UseLogCounter = 0;

	//�ő�\�����ԁB
	float _LifeTime = 0.0f;

	//���݂̎��ԁB
	float _NowTime = 0.0f;

	//���l�B
	float _alpha = 0.0f;
};