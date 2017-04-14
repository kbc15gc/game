#include "Transform.h"
#include "GameObject.h"

Transform::Transform(GameObject * g, Transform * t) :
	Component(g, t, typeid(this).name()),
	_Parent(nullptr),
	_LocalPosition(Vector3::zero),
	_Position(Vector3::zero),
	_LocalScale(Vector3::one),
	_Scale(Vector3::one),
	_LocalAngle(Vector3::zero),
	_Angle(Vector3::zero),
	_LocalRotation(Quaternion::Identity),
	_Rotation(Quaternion::Identity)
{
	_Children.clear();
	D3DXMatrixIdentity(&_RotateMatrix);
	D3DXMatrixIdentity(&_WorldMatrix);
}

Transform::~Transform()
{
	//�e������Ȃ�
	if(_Parent)
	{
		//�e�̎q���玩�����O��
		_Parent->RemoveChild(this);
	}
	//�q������Ȃ�
	if(_Children.size() > 0)
	{
		//�q�̐e��null��
		vector<Transform*>::iterator it = _Children.begin();
		while (it != _Children.end())
		{
			(*it)->_Parent = nullptr;
			it++;
		}
	}
}

void Transform::Awake()
{
	
}

void Transform::Start()
{
	
}

void Transform::Update()
{
	//UpdateTransform();
}

//�q������������֐�
//�߂�l�FTransform* �q�b�g�����q�̃A�h���X
//�������Fchar* �q���̖��O
Transform* Transform::FindChild(char* name)
{
	//�C�e���[�^�擾
	vector<Transform*>::iterator it = _Children.begin();
	//�Ō�܂Ō���
	while (it != _Children.end())
	{
		//�q�̖��O�擾
		const char* childname = (*it)->gameObject->GetName();
		//���O��r
		if(strcmp(childname,name) == 0)
		{
			return (*it);
		}
		it++;
	}

	return nullptr;
}
Transform * Transform::FindChild(unsigned int idx)
{
	//�͈͊O�A�N�Z�X���͂���
	if (_Children.size() <= idx)
	{
		return nullptr;
	}
	//�Y�����Ԗڂ�Ԃ�
	return _Children[idx];
}
//������
Transform ** Transform::FindChilds(char * name)
{
	vector<Transform*> v;
	//�C�e���[�^�擾
	vector<Transform*>::iterator it = _Children.begin();
	//�Ō�܂Ō���
	while (it != _Children.end())
	{
		//�q�̖��O�擾
		const char* childname = (*it)->gameObject->GetName();
		//���O��r
		if (strcmp(childname, name) == 0)
		{
			
		}
		it++;
	}

	return nullptr;
}

//���[�J������g�����X�t�H�[�����X�V
//�e����X�V���Ă����ׂ�
//�߂�l�F�Ȃ�
//�����F�Ȃ�
void Transform::UpdateTransform()
{
	//�e������Ȃ�
	if (_Parent) {
		//�e������B
		D3DXMATRIX PWorld = _Parent->GetWorldMatrix();
		//�e�̃��[���h�s�����Z���āA���[�J�����W�����[���h���W�ɕϊ�����B
		D3DXVECTOR4 pos;
		D3DXVECTOR3 lpos;
		//���[�J�����R�s�[
		_LocalPosition.CopyFrom(lpos);
		//���[�J���~���[���h
		D3DXVec3Transform(&pos, &lpos, &PWorld);
		//�|�W�V����
		_Position.x = pos.x;
		_Position.y = pos.y;
		_Position.z = pos.z;
		//�X�P�[���l
		_Scale = _LocalScale * _Parent->_Scale;
		//�I�C���[�p
		Vector3 pang = _Parent->GetAngle();
		_Angle = _LocalAngle + pang;
		//�N�H�[�^�j�I������
		_Rotation.SetEuler(_Angle);
	}
	else {
		//���[�J�������̂܂�
		_Position = _LocalPosition;
		_Scale = _LocalScale;
		_Angle = _LocalAngle;
		_Rotation.SetEuler(_Angle);
	}
	//��]�s��擾
	_RotateMatrix = _Rotation.GetRotationMatrix();

	UpdateWolrdMatrix();
}

//�g�����X�t�H�[�����烏�[���h�s��X�V
//�߂�l�F�Ȃ�
//�����F�Ȃ�
void Transform::UpdateWolrdMatrix()
{
	//������
	D3DXMATRIX Scale, Pos;
	D3DXMatrixIdentity(&Scale);
	D3DXMatrixIdentity(&Pos);
	//�X�P�[��
	D3DXMatrixScaling(&Scale, _Scale.x, _Scale.y, _Scale.z);
	//�|�W�V����
	D3DXMatrixTranslation(&Pos, _Position.x, _Position.y, _Position.z);
	//���[���h�s��ݒ�
	_WorldMatrix = Scale * _RotateMatrix * Pos;

	//�q���X�V����
	for each (Transform* t in _Children)
	{
		t->UpdateTransform();
	}
}

Vector3 Transform::GetForward()
{
	return Direction(Vector3::front);
}

Vector3 Transform::Direction(const Vector3& v)
{
	D3DXVECTOR3 in;
	D3DXVECTOR3 out;
	v.CopyFrom(in);
	//�󂯎�����x�N�g������]�s����g���ĉ�]������B
	D3DXVec3TransformCoord(&out, &in, &_RotateMatrix);

	Vector3 r = out;
	return r;
}

Vector3 Transform::Local(const Vector3& v)
{
	D3DXVECTOR4 pos;	//�|�W�V����
	D3DXVECTOR3 lpos;	//���[�J���|�W�V����
	//v���R�s�[
	v.CopyFrom(lpos);
	//���[���h�s�񂩂猩�����[�J���|�W�V������pos�Ɋi�[
	//���[���h�s����g���Ĉړ�������B
	D3DXVec3Transform(&pos, &lpos, &_WorldMatrix);
	return Vector3(pos.x, pos.y, pos.z);
}

Vector3 Transform::LocalPos(const Vector3& v)
{
	D3DXVECTOR4 pos;
	D3DXVECTOR3 lpos;
	v.CopyFrom(lpos);
	D3DXMATRIX offset;
	D3DXMatrixIdentity(&offset);
	offset._41 = _WorldMatrix._41;
	offset._42 = _WorldMatrix._42;
	offset._43 = _WorldMatrix._43;
	//�ړ��s����g���Ĉړ�������
	D3DXVec3Transform(&pos, &lpos, &offset);
	return Vector3(pos.x, pos.y, pos.z);
}

void Transform::LockAt(GameObject * obj)
{
	//�����_�Ǝ��_
	D3DXVECTOR3 target, eye;
	obj->transform->_Position.CopyFrom(target);
	this->_Position.CopyFrom(eye);
	//��]�s�񏉊���
	D3DXMatrixIdentity(&_RotateMatrix);
	//���_���猩��������擾
	Vector3 vup = this->Direction(Vector3::up);
	//�^�[�Q�b�g���猩������
	//��l�����͎��_�̏����
	D3DXMatrixLookAtLH(&_RotateMatrix, &target, &eye, (D3DXVECTOR3*)&vup);
	//�r���[�s����t�s��ɂ��ă��[���h�s���
	D3DXMatrixInverse(&_RotateMatrix, NULL, &_RotateMatrix);
	// �I�t�Z�b�g��؂��ĉ�]�s�񂾂��ɂ��Ă��܂�
	_RotateMatrix._41 = 0.0f;   
	_RotateMatrix._42 = 0.0f;
	_RotateMatrix._43 = 0.0f;

	//��]�s�񂩂�N�H�[�^�j�I������
	D3DXQUATERNION q;
	D3DXQuaternionRotationMatrix(&q, &_RotateMatrix);
	//�����o�ϐ��̃N�H�[�^�j�I���ɃR�s�[
	_Rotation = q;
	//�N�H�[�^�j�I������I�C���[�p�����߂�
	SetAngle(_Rotation.GetAngle());
	//���[���h�s������߂�B
	UpdateWolrdMatrix();
}

//�g�����X�t�H�[�����
void Transform::Release()
{
	vector<Transform*>::iterator it = _Children.begin();
	while (it != _Children.end())
	{
		//�e���폜
		(*it)->_Parent = nullptr;
		it++;
	}
	_Children.clear();
}

void Transform::RemoveChild(Transform * t)
{
	vector<Transform*>::iterator it = _Children.begin();
	while (it != _Children.end())
	{
		//�A�h���X��r
		if((*it) == t)
		{
			(*it)->_Parent = nullptr;
			_Children.erase(it);
			return;
		}
		it++;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�Z�b�^�[�E�Q�b�^�[

void Transform::SetPosition(const Vector3 & v)
{
	_Position = v;
	//�ŏI�I�ȃ|�W�V�������烍�[�J�����t�Z
	if (_Parent)
	{
		Vector3 ppos = _Parent->GetPosition();
		_LocalPosition = _Position - ppos;
	}
	else
	{
		//���̂܂܃��[�J����
		_LocalPosition = _Position;
	}
	//���[���h�s��X�V
	UpdateWolrdMatrix();
}

void Transform::SetLocalPosition(const Vector3& v)
{
	_LocalPosition = v;

	//���[�J������ŏI�I�ȃ|�W�V�������v�Z
	if (_Parent)
	{
		D3DXMATRIX pworld = _Parent->GetWorldMatrix();

		D3DXVECTOR3 lpos;
		D3DXVECTOR4 pos;
		//���[�J�����R�s�[
		_LocalPosition.CopyFrom(lpos);
		//�e�̃��[���h�s�����Z���āA���[�J�����W�����[���h���W�ɕϊ�����B
		D3DXVec3Transform(&pos, &lpos, &pworld);
		//�|�W�V����
		_Position.x = pos.x;
		_Position.y = pos.y;
		_Position.z = pos.z;
	}
	else
	{
		//���[�J�����̂܂�
		_Position = _LocalPosition;
	}
	//���[���h�s��X�V
	UpdateWolrdMatrix();
}

void Transform::SetLocalScale(const Vector3& v)
{
	_LocalScale = v;
	//���[�J������ŏI�I�ȃX�P�[���v�Z
	if (_Parent) {
		//���[�J���X�P�[���~�e�̃X�P�[��
		_Scale = _LocalScale * _Parent->_Scale;
	}
	else
	{
		//���[�J�����̂܂�
		_Scale = _LocalScale;
	}
	//���[���h�s��X�V
	UpdateWolrdMatrix();
}

void Transform::SetScale(const Vector3& v)
{
	_Scale = v;
	//�ŏI�I�ȃX�P�[�����烍�[�J�����t�Z
	if (_Parent)
	{
		//���[�J���X�P�[�������߂�
		_LocalScale = _Scale / _Parent->_Scale;
	}
	else
	{
		//���̂܂܃��[�J����
		_LocalScale = _Scale;
	}
	//���[���h�s��X�V
	UpdateWolrdMatrix();
}

void Transform::SetLocalAngle(const Vector3& v)
{
	_LocalAngle = v;
	//���[�J������ŏI�I�Ȋp�x�v�Z
	if (_Parent) {
		//�e�̃N�H�[�^�j�I���擾
		Quaternion prot = _Parent->GetRotation();
		//�e���̉�]�ʂ��擾
		Vector3 pang = prot.GetAngle();
		//����
		_Angle = _LocalAngle + pang;
	}
	else
	{
		//���[�J�����̂܂�
		_Angle = _LocalAngle;
	}
	//�N�H�[�^�j�I���X�V
	_LocalRotation.SetEuler(_Angle);
	//�N�H�[�^�j�I���ݒ�
	SetLocalRotation(_LocalRotation);
}

void Transform::SetAngle(const Vector3& v)
{
	_Angle = v;
	//���[�J������ŏI�I�Ȋp�x�v�Z
	if (_Parent) {
		//�e�̃N�H�[�^�j�I���擾
		Quaternion prot = _Parent->GetRotation();
		//�e���̉�]�ʂ��擾
		Vector3 pang = prot.GetAngle();
		//����
		_LocalAngle = _Angle - pang;
	}
	else
	{
		//���[�J�����̂܂�
		_LocalAngle = _Angle;
	}
	//�N�H�[�^�j�I���X�V
	_LocalRotation.SetEuler(_Angle);
	//�N�H�[�^�j�I���ݒ�
	SetLocalRotation(_LocalRotation);
}

void Transform::SetLocalRotation(const Quaternion & q)
{
	_LocalRotation = q;
	//�e�����邩�ǂ����H
	if (_Parent) {
		//�e�̃N�H�[�^�j�I���擾
		Quaternion protInv = _Parent->GetRotation();
		//�t�N�H�[�^�j�I���ɂ���B
		protInv.Inverse();
		//���[�J���Ɗ|����
		_Rotation = _LocalRotation * protInv;
	}
	else
	{
		//���[�J�����̂܂�
		_Rotation = _LocalRotation;
	}
	//��]�s��X�V
	_RotateMatrix = _Rotation.GetRotationMatrix();
	//���[���h�s��X�V
	UpdateWolrdMatrix();
}

void Transform::SetRotation(const Quaternion& q)
{
	_Rotation = q;
	//�e�����邩�ǂ����H
	if (_Parent) {
		//�e�̃N�H�[�^�j�I���擾
		Quaternion protInv = _Parent->GetRotation();
		//�t�N�H�[�^�j�I���ɂ���B
		protInv.Inverse();
		//���[�J���Ɗ|����
		_LocalRotation = _Rotation / protInv;
	}
	else
	{
		//���[�J�����̂܂�
		_LocalRotation = _Rotation;
	}
	//��]�s��X�V
	_RotateMatrix = _Rotation.GetRotationMatrix();
	//���[���h�s��X�V
	UpdateWolrdMatrix();
}

void Transform::SetWorldMatrix(D3DXMATRIX w)
{
	_WorldMatrix = w;
}
//�Q�b�^�[
Vector3& Transform::GetLocalPosition()
{
	return _LocalPosition;
}

const Vector3& Transform::GetPosition()
{
	return _Position;
}

const Vector3& Transform::GetLocalScale()
{
	return _LocalScale;
}

const Vector3& Transform::GetScale()
{
	return _Scale;
}

const Vector3& Transform::GetLocalAngle()
{
	return _LocalAngle;
}

const Vector3& Transform::GetAngle()
{
	return _Angle;
}

const Quaternion& Transform::GetLocalRotation()
{
	return _LocalRotation;
}

const Quaternion& Transform::GetRotation()
{
	return _Rotation;
}

const D3DXMATRIX & Transform::GetWorldMatrix()
{
	return _WorldMatrix;
}

const D3DXMATRIX & Transform::GetRotateMatrix()
{
	return _RotateMatrix;
}

D3DXMATRIX * Transform::GetRotateMatrixAddress()
{
	return &_RotateMatrix;
}