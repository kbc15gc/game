#pragma once

class Vertex;
class Effect;

/**
* �u���[���̃|�X�g�G�t�F�N�g.
*/
class Bloom
{
public:

	/**
	* �R���X�g���N�^.
	*/
	Bloom(){}

	/**
	* �f�X�g���N�^.
	*/
	~Bloom(){}

	/**
	* �쐬.
	*/
	void Create();
	
	/**
	* �`��.
	*/
	void Render();

	/**
	* ���.
	*/
	void Release();

	/**
	* �L���t���O��ݒ�.
	*/
	void SetEnable(bool value)
	{
		_IsEnable = value;
	}

	/**
	* �P�x�p�����_�����O�^�[�Q�b�g���擾.
	*/
	RenderTarget* GetLuminanceRT()
	{
		if (_IsEnable)
		{
			return _LuminanceRT;
		}
		return nullptr;
	}

private:

	/**
	* �d�݂��v�Z.
	*/
	void _UpdateWeight(const float& dis);

private:

	//
	Vertex* _Vertex;
	//�L���t���O. 
	bool _IsEnable = true;

	//�d�݂̐�
	static const int NUM_WEIGHTS = 8;
	//�d��
	float _Weights[NUM_WEIGHTS];

	//�G�t�F�N�g
	Effect* _Effect;

	/**
	* �P�x�p�̃����_�����O�^�[�Q�b�g
	*/
	RenderTarget* _LuminanceRT;

	/** �P�x�J���[�p�̃����_�����O�^�[�Q�b�g. */
	RenderTarget* _LuminanceColorRT = nullptr;

	//�ڂ��������p��RT
	RenderTarget* _CombineRT;

	//�_�E���T���v�����O�pRT�̐�
	static const int NUM_DOWN_SAMPLING_RT = 10;
	//�P�x���_�E���T���v�����O���邽�߂�RT
	RenderTarget* _DownSamplingRT[NUM_DOWN_SAMPLING_RT];
};