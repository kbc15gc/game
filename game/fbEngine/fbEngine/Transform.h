#pragma once
#include "Component.h"

class Camera;

//�g�����X�t�H�[���N���X
class Transform : public Component
{
public:
	Transform(GameObject* g, Transform* t);
	~Transform();

	void Awake()override;
	void Start()override;
	void Update()override;

	//�q������������֐�
	//�߂�l�FTransform* ��ԍŏ��Ƀq�b�g�����q�̃A�h���X
	//�������Fchar* �q���̖��O
	Transform* FindChild(char* name);
	//�Y�����Ŏ擾
	Transform* FindChild(unsigned int idx);
	//�q������������֐�(������)
	//�߂�l�FTransform** �q�b�g�����q�B�̃A�h���X�̔z��
	//�������Fchar* �q���̖��O
	Transform** FindChilds(char* name);

	//���[�J������g�����X�t�H�[�����X�V
	void UpdateTransform();

	//�g�����X�t�H�[�����烏�[���h�s��X�V
	void UpdateWolrdMatrix();

	//�O�����̃x�N�g���擾
	Vector3 GetForward();
	//�󂯎�����x�N�g�����������猩�������ɕϊ�
	Vector3 Direction(const Vector3& v);
	//���[�J���Ȉʒu�ɕϊ�(��]����)
	Vector3 Local(const Vector3& v);
	//���[�J���Ȉʒu�ɕϊ�(��]����)
	Vector3 LocalPos(const Vector3& v);
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
		UpdateTransform();
	}

	//�q�������擾
	const vector<Transform*>& GetChildren()
	{
		return _Children;
	};

	//�e�Z�b�^�[�E�Q�b�^�[

	void SetLocalPosition(const Vector3& v);
	void SetPosition(const Vector3& v);
	void SetLocalScale(const Vector3& v);
	void SetScale(const Vector3& v);
	void SetLocalAngle(const Vector3& v);
	void SetAngle(const Vector3& v);
	void SetLocalRotation(const Quaternion& q);
	void SetRotation(const Quaternion& q);
	void SetWorldMatrix(D3DXMATRIX w);

	Vector3&	GetLocalPosition();
	const Vector3& GetPosition();
	const Vector3& GetLocalScale();
	const Vector3& GetScale();
	const Vector3&	GetLocalAngle();
	const Vector3& GetAngle();
	const Quaternion& GetLocalRotation();
	const Quaternion& GetRotation();
	D3DXMATRIX* GetRotateMatrixAddress();
	const D3DXMATRIX& GetRotateMatrix();
	const D3DXMATRIX& GetWorldMatrix();
private:
	Transform* _Parent;		//�e�̃A�h���X
	vector<Transform*> _Children;		//�q���B�̃A�h���X���i�[����
	
	Vector3	_Position;		//�ŏI�|�W�V����
	Vector3	_LocalPosition;	//���[�J���|�W�V����
	Vector3	_Scale;			//�ŏI�X�P�[��
	Vector3	_LocalScale;	//���[�J���X�P�[��
	Vector3	_Angle;			//�p�x
	Vector3	_LocalAngle;	//���[�J���p�x
	Quaternion _Rotation;		//��]�i�N�E�H�[�^�j�I���j
	Quaternion _LocalRotation;	//���[�J����]�i�N�E�H�[�^�j�I���j
	D3DXMATRIX _RotateMatrix;	//��]�s��
	D3DXMATRIX _WorldMatrix;	//���[���h�s��
};