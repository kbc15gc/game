#pragma once
#include "Component.h"

//�g�����X�t�H�[���N���X
class Transform : public Component
{
public:
	Transform(GameObject* g, Transform* t) :
		Component(g, t, typeid(this).name())
	{
		
	}
	~Transform();

	void Awake()override;
	void Start()override;
	void Update()override;

	//�q������������֐�
	//�߂�l�FTransform* ��ԍŏ��Ƀq�b�g�����q�̃A�h���X
	//�������Fchar* �q���̖��O
	Transform* FindChild(char* childname);
	//�Y�����Ŏ擾
	Transform* FindChild(unsigned int idx);
	//�q������������֐�(������)
	//�߂�l�FTransform** �q�b�g�����q�B�̃A�h���X�̔z��
	//�������Fchar* �q���̖��O
	Transform** FindChilds(char* childname);

	//���[�J������g�����X�t�H�[�����X�V
	void UpdateTransform();

	//�g�����X�t�H�[�����烏�[���h�s��X�V
	void UpdateWolrdMatrix();

	//�󂯎�����x�N�g�����������猩�������ɕϊ�
	Vector3 Direction(Vector3 v);
	//���[�J���Ȉʒu�ɕϊ�(��]����)
	Vector3 Local(Vector3 v);
	//���[�J���Ȉʒu�ɕϊ�(��]����)
	Vector3 LocalPos(Vector3 v);
	//�󂯎�����I�u�W�F�N�g�̌���������
	void LockAt(GameObject* obj);

	//���
	void Release();
	//�q���O��
	void RemoveChild(Transform* t);

	//�e��Ԃ��֐�
	//�߂�l�FTransform* �e�̃A�h���X
	Transform* Transform::GetParent()
	{
		return _Parent;
	};

	//�����̃g�����X�t�H�[���������̐e�ɓo�^����
	//�������FTransform
	void Transform::SetParent(Transform* _Parent)
	{
		//�e�ɓo�^
		this->_Parent = _Parent;
		//�e�̎q�Ɏ�����o�^
		_Parent->_Children.push_back(this);
	}

	//�q�������擾
	const vector<Transform*>& GetChildren()
	{
		return _Children;
	};

	//�Q�b�^�[
	const D3DXMATRIX& Transform::GetWorldMatrix()
	{
		return _WorldMatrix;
	}

	//�Z�b�^�[
	void Transform::SetWorldMatrix(D3DXMATRIX w)
	{
		_WorldMatrix = w;
	}

	//�Q�b�^�[
	const D3DXMATRIX& Transform::RotateMatrix()
	{
		return _RotateMatrix;
	}

	//�Q�b�^�[
	D3DXMATRIX* Transform::RotateMatrixAddress()
	{
		return &_RotateMatrix;
	}


	//�ȒP�ɃA�N�Z�X�����������E�E�E�B
	Vector3	position;		//�ŏI�|�W�V����
	Vector3	localPosition;	//���[�J���|�W�V����
	Vector3	scale;			//�ŏI�X�P�[��
	Vector3	localScale;		//���[�J���X�P�[��
	Vector3	angle;			//��](�x)
	Vector3	localAngle;		//���[�J����]
	Quaternion rotation;	//��]�i�N�E�H�[�^�j�I���j
private:
	Transform* _Parent;		//�e�̃A�h���X
	vector<Transform*> _Children;		//�q���B�̃A�h���X���i�[����
	
	D3DXMATRIX _RotateMatrix;	//��]�s��
	D3DXMATRIX _WorldMatrix;	//���[���h�s��
};