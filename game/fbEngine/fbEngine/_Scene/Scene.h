#pragma once

class Vertex;
class Effect;

namespace fbScene
{
	//�t�F�[�h�̃X�e�[�g
	enum class FadeStateE
	{
		In = BIT(1),
		Out = BIT(2),
		//���s���B
		Fade = BIT(3),
		FadeIn = Fade | In,
		FadeOut = Fade | Out,
		//�t�F�[�h�I���B
		End	= BIT(4),
		EndFadeIn = End | In,
		EndFadeOut = End | Out,
	};
	FadeStateE operator-(FadeStateE L, FadeStateE R);
	FadeStateE operator|(FadeStateE L, FadeStateE R);
	FadeStateE operator&(FadeStateE L, FadeStateE R);
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

	//�t�F�[�h�̏����B
	void Fade();
	//�t�F�[�h�̎��s
	//[in] �t�F�[�h�C�����ǂ����H(false�Ȃ�t�F�[�h�A�E�g)
	//[in] �t�F�[�h�ɂ����鎞��(�b)
	static void StartFade(bool fadein,float fadetime = 2.0f);

	/**
	* �V���h�E�}�b�v�̗L���t���O.
	*/
	bool GetIsShadowMap()
	{
		return _isShadowMap;
	}

	/**
	* ���}�b�v�t���O���擾.
	*/
	bool GetIsEnvironmentMap()
	{
		return _isEnvironmentMap;
	}

	static fbScene::FadeStateE GetFadeState()
	{
		return _FadeState;
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
	//�t�F�[�h�̃X�e�[�g
	static fbScene::FadeStateE _FadeState;

protected:

	/** �V���h�E�}�b�v�t���O. */
	bool _isShadowMap = false;

	/** ���}�b�v�t���O. */
	bool _isEnvironmentMap = false;


};
