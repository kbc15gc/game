/**
* DOF(��ʊE�[�x)�N���X�̒�`.
*/
#pragma once

/**
* DOF(��ʊE�[�x)�N���X.
*/
class DepthofField : Noncopyable
{
public:

	/**
	* �R���X�g���N�^.
	*/
	DepthofField()
	{
	}
	
	/**
	* �f�X�g���N�^.
	*/
	~DepthofField()
	{
	}

private:

	/** �L���t���O. */
	bool _isEnable = true;

	/** �[�x�������ݗp�����_�����O�^�[�Q�b�g. */
	RenderTarget _DepthRT;



};