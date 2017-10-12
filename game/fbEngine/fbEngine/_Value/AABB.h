#pragma once
#include "Vector.h"

//AABB
//�����s�o�E���f�B���O�{�b�N�X�N���X�B</br>
//�����o�Ƀo�E���f�B���O�{�b�N�X�̒��S�ʒu�A</br>
//�T�C�Y�A�o�E���f�B���O�{�b�N�X���\������8���_�̍��W��ێ����Ă��܂��B
class AABB {
public:
	AABB();
	//�������B
	void Init(const Vector3& centerPosition, const Vector3& halfSize);
	//�X�V�B
	//[in] ���S���W�B
	//[in] �o�E���f�B���O�{�b�N�X�̃n�[�t�T�C�Y�B
	void Update(const Vector3& centerPosition, const Vector3& halfSize);
	//�o�E���f�B���O�{�b�N�X���\������8���_�̍��W���擾�B
	//[in] ���_�ԍ��B0�`7�̔ԍ���n���Ă��������B
	const Vector3& GetVertexPosition(unsigned int vertNo) const
	{
		//TK_ASSERT(vertNo < 8, "vertNo is invalid");
		return _VertexPosition[vertNo];
	}
private:
	void CalcVertexPosition();
private:
	Vector3	_CenterPosition = Vector3::zero;		//���S���W�B
	Vector3	_HalfSize = Vector3::zero;			//�n�[�t�T�C�Y�B
	Vector3	_VertexPosition[8];	//�o�E���f�B���O�{�b�N�X���\������8���_�̍��W�B
};