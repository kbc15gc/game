#pragma once

class Vertex;
class Effect;

//�V�[���̊��N���X
class Scene
{
public:
	Scene();
	~Scene();
	//�������֐�
	//�V�[�����؂�ւ���ꂽ��ɌĂ΂��
	virtual void Start() {};
	//�X�V�֐�
	//�؂�ւ���ꂽ�疈�t���[���Ăяo����鏈��(����̂��H)
	virtual void Update() {};

	//�t�F�[�h
	//�t�F�[�h����true���A��B
	void Fade();
	//�������̓t�F�[�h�C�����t�F�[�h�A�E�g���w��
	//�������̓t�F�[�h�ɂ����鎞��(�b)
	static void SetFade(bool inout,float fadetime = 2.0f);
private:
	//�S�ẴV�[���ŋ��L�������̂�static
	//���_
	static Vertex *_Vertex;
	//�V�F�[�_
	static Effect *_Effect;
	//�t�F�[�h�̃�
	static float _FadeAlpha;
	//�������l
	static float _AddAlpha;
protected:
	//�t�F�[�h��
	static bool _IsFade;
	//�V�[���؂�ւ��t���O
	bool _ChangeScene;
	//�V�[���؂�ւ��̏���������
	//virtual void _ChangeScene() = 0;
};
