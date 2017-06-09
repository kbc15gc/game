#pragma once

class Vertex;
class Effect;

namespace fbScene
{
	//�t�F�[�h�̃X�e�[�g
	enum class FadeStateE
	{
		WAIT,		//�ҋ@
		START,		//�J�n
		RUNNING,	//���s��
		END	=100,	//�I��
		INEND,		//�t�F�[�h�C���̏I��
		OUTEND,		//�t�F�[�h�A�E�g�̏I��
	};
}

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
	//�t�F�[�h�̎��s
	//�������̓t�F�[�h�C�����t�F�[�h�A�E�g���w��
	//�������̓t�F�[�h�ɂ����鎞��(�b)
	static void StartFade(const bool& fade,const float& fadetime = 2.0f);
	//�ҋ@��Ԃֈڍs����
	static void WaitFade()
	{
		_FadeState = fbScene::FadeStateE::WAIT;
	}

	/**
	* �V���h�E�}�b�v�̗L���t���O.
	*/
	bool GetIsShadowMap()
	{
		return _isShadowMap;
	}

private:
	//�t�F�[�h�`��
	void _DrawFade();
private:
	//�S�ẴV�[���ŋ��L�������̂�static
	//���_
	static Vertex *_Vertex;
	//�V�F�[�_
	static Effect *_Effect;
	//�t�F�[�h�̃�
	static float _FadeAlpha;
	//1�b�Ԃ�����ɉ��Z����郿�l
	static float _AddPerSecA;
protected:
	//�t�F�[�h��
	static fbScene::FadeStateE _FadeState;

protected:

	/** �V���h�E�}�b�v�t���O. */
	bool _isShadowMap = false;


};
