#include"fbstdafx.h"
#include "Transform.h"

Transform::Transform(GameObject * g, Transform * t) :
	Component(g, t, typeid(this).name(),0),
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
#ifdef _DEBUG
	mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
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
		//�e�̃��[���h�s����擾�B
		D3DXMATRIX PWorld = _Parent->GetWorldMatrix();
		//�q�̃��[���h���W�B
		D3DXVECTOR4 pos;
		//�q�̃��[�J�����W�B
		D3DXVECTOR3 lpos;
		//���[�J�����R�s�[�B
		_LocalPosition.CopyFrom(lpos);
		//�e�̃��[���h�s�����Z���āA���[�J�����W�����[���h���W�ɕϊ�����B
		//�q�̃��[�J�����W�~�e�̃��[���h�s��B
		D3DXVec3Transform(&pos, &lpos, &PWorld);
		//���[���h���W�B
		_Position = Vector3(pos.x, pos.y, pos.z);
		//�X�P�[���l
		SetLocalScale(_LocalScale);
		//�I�C���[�p
		_Angle = _LocalAngle - _Parent->_Angle;
		//�N�H�[�^�j�I���v�Z�B
		_Rotation = _LocalRotation * _Parent->GetRotation();
	}
	else {
		//���[�J�������̂܂�
		_Position = _LocalPosition;
		_Scale = _LocalScale;
		_Angle = _LocalAngle;
		_Rotation = _LocalRotation;
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

	_Right = Vector3(_WorldMatrix.m[0][0], _WorldMatrix.m[0][1], _WorldMatrix.m[0][2]);
	_Right.Normalize();
	_Up = Vector3(_WorldMatrix.m[1][0], _WorldMatrix.m[1][1], _WorldMatrix.m[1][2]);
	_Up.Normalize();
	_Forward = Vector3(_WorldMatrix.m[2][0], _WorldMatrix.m[2][1], _WorldMatrix.m[2][2]);
	_Forward.Normalize();

	//�q���X�V����
	for each (Transform* t in _Children)
	{
		t->UpdateTransform();
	}
}

const Vector3& Transform::GetRight()
{
	return _Right;	// ���[���h�s���X������Ԃ��B
}

const Vector3& Transform::GetUp()
{
	return _Up;	// ���[���h�s���Y������Ԃ��B
}

const Vector3& Transform::GetForward()
{
	return _Forward;;	// ���[���h�s���Z������Ԃ��B
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

void Transform::LockAt(const Vector3 & wpos)
{
	//�����_�Ǝ��_
	D3DXVECTOR3 target, eye;
	D3DXMATRIX view;
	wpos.CopyFrom(target);
	this->_Position.CopyFrom(eye);
	//�r���[�s�񏉊���
	D3DXMatrixIdentity(&view);
	//�r���[�s����쐬
	D3DXMatrixLookAtLH(&view, &eye, &target, (D3DXVECTOR3*)&Vector3::up);
	//�r���[�s����t�s��ɂ��ă��[���h�s���
	D3DXMatrixInverse(&_RotateMatrix, NULL, &view);
	// �I�t�Z�b�g��؂��ĉ�]�s�񂾂��ɂ��Ă��܂�
	_RotateMatrix._41 = 0.0f;
	_RotateMatrix._42 = 0.0f;
	_RotateMatrix._43 = 0.0f;

	//��]�s�񂩂�N�H�[�^�j�I������
	D3DXQUATERNION q;
	D3DXQuaternionRotationMatrix(&q, &_RotateMatrix);
	//�����o�ϐ��̃N�H�[�^�j�I���ɃR�s�[
	_Rotation = q;
	SetRotation(_Rotation);
	//�N�H�[�^�j�I������I�C���[�p�����߂�
	//SetAngle(_Rotation.GetAngle());
	//���[���h�s������߂�B
	UpdateWolrdMatrix();
}

void Transform::LockAt(GameObject * obj)
{
	LockAt(obj->transform->_Position);
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

void Transform::AddChild(Transform* t) {
	bool isRegistered = false;	// ���łɎq�ǂ��o�^����Ă��邩�B
	for (auto child : _Children) {
		if (child == t) {
			// �o�^�ς݁B
			return;
		}
	}
	
	// �܂��o�^����Ă��Ȃ��B
	_Children.push_back(t);	//�q�ɓo�^�B
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

void Transform::SetParent(Transform * parent)
{
	if(_Parent)
	{
		// ���X�e�������B

		if (parent == nullptr)
		{
			// �e���O���ꂽ�B

			_LocalPosition = _Position;
			_LocalScale = _Scale;
			_LocalAngle = _Angle + _Parent->_Angle;
			_LocalRotation = _Rotation;
		}
		// �e���O���̂ŁA���݂̐e�̎q�����X�g���玩�����O���B
		this->_Parent->RemoveChild(this);
	}
	if (parent) {
		// �e���ݒ肳�ꂽ�B

		// �e�̎q���Ɏ�����ǉ��B
		parent->AddChild(this);

		//�e�̐ݒ���擾
		if (gameObject) {
			if (parent->gameObject) {
				gameObject->SetDiscard(parent->gameObject->GetDiscard());
			}
		}
	}

	//�e�ɓo�^
	this->_Parent = parent;

	UpdateTransform();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�Z�b�^�[�E�Q�b�^�[

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

void Transform::SetPosition(const Vector3 & v)
{
	_Position = v;
	//�ŏI�I�ȃ|�W�V�������烍�[�J�����t�Z
	if (_Parent)
	{
		D3DXMATRIX pworldInv = _Parent->GetWorldMatrix();
		//�t�s���
		D3DXMatrixInverse(&pworldInv, NULL, &pworldInv);
		D3DXVECTOR4 lpos;
		D3DXVECTOR3 pos;
		//�R�s�[
		_Position.CopyFrom(pos);
		//�t�s�����Z���āA���[�J�������߂�B
		D3DXVec3Transform(&lpos, &pos, &pworldInv);
		
		_LocalPosition.x = lpos.x;
		_LocalPosition.y = lpos.y;
		_LocalPosition.z = lpos.z;
	}
	else
	{
		//���̂܂܃��[�J����
		_LocalPosition = _Position;
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

void Transform::SetLocalAngle(const Vector3& v, bool update)
{
	//���[�J���A���O������B
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

	//�N�H�[�^�j�I���v�Z�B
	if (update)
	{
		//�N�H�[�^�j�I���X�V
		_LocalRotation.SetEuler(_LocalAngle);
		//�N�H�[�^�j�I���ݒ�
		SetLocalRotation(_LocalRotation,false);
	}
}

void Transform::SetAngle(const Vector3& v, bool update)
{
	_Angle = v;
	//���[�J���̊p�x�t�Z�B
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

	//�N�H�[�^�j�I���v�Z�B
	if (update)
	{
		//�N�H�[�^�j�I���X�V
		_Rotation.SetEuler(_Angle);
		//�N�H�[�^�j�I���ݒ�
		SetRotation(_Rotation,false);
	}
}

void Transform::SetLocalRotation(const Quaternion & q, bool update)
{
	_LocalRotation = q;
	//�e�����邩�ǂ����H
	if (_Parent) {
		//�e�̃N�H�[�^�j�I���擾
		Quaternion prot = _Parent->GetRotation();
		//���[�J���Ɗ|����
		//_Rotation = _LocalRotation * prot;
		_Rotation = prot * _LocalRotation;
	}
	else
	{
		//���[�J�����̂܂�
		_Rotation = _LocalRotation;
	}
	//��]�s��X�V
	_RotateMatrix = _Rotation.GetRotationMatrix();

	//�I�C���[�p�̍X�V�B
	if (update)
	{
		SetLocalAngle(_LocalRotation.GetAngle(), false);
	}

	//���[���h�s��X�V
	UpdateWolrdMatrix();
}

void Transform::SetRotation(const Quaternion& q, bool update)
{
	_Rotation = q;
	//�e�����邩�ǂ����H
	if (_Parent) {
		//�e�̃N�H�[�^�j�I���擾
		Quaternion protInv = _Parent->GetRotation();
		//�t�N�H�[�^�j�I���ɂ���B
		protInv.Inverse();
		//���[�J�����t�Z
		_LocalRotation = _Rotation * protInv;
		_LocalRotation.GetAngle();
	}
	else
	{
		//���[�J�����̂܂�
		_LocalRotation = _Rotation;
	}
	//��]�s��X�V
	_RotateMatrix = _Rotation.GetRotationMatrix();
	//�I�C���[�p�̍X�V�B
	if (update)
	{
		SetAngle(_Rotation.GetAngle(), false);
	}
	//���[���h�s��X�V
	UpdateWolrdMatrix();
}

void Transform::SetRotateMatrix(const D3DXMATRIX r)
{
	_RotateMatrix = r;
	//��]�s�񂩂�N�H�[�^�j�I������
	D3DXQUATERNION q;
	D3DXQuaternionRotationMatrix(&q, &_RotateMatrix);
	//�����o�ϐ��̃N�H�[�^�j�I����
	SetRotation(q);
	UpdateWolrdMatrix();
}

void Transform::SetWorldMatrix(D3DXMATRIX w)
{
	_WorldMatrix = w;
}
//�Q�b�^�[
const Vector3& Transform::GetLocalPosition()const
{
	return _LocalPosition;
}

const Vector3& Transform::GetPosition()const
{
	return _Position;
}

const Vector3& Transform::GetLocalScale()const
{
	return _LocalScale;
}

const Vector3& Transform::GetScale()const
{
	return _Scale;
}

const Vector3& Transform::GetLocalAngle()const
{
	return _LocalAngle;
}

const Vector3& Transform::GetAngle()const
{
	return _Angle;
}

const Quaternion& Transform::GetLocalRotation()const
{
	return _LocalRotation;
}

const Quaternion& Transform::GetRotation()const
{
	return _Rotation;
}

const D3DXMATRIX & Transform::GetWorldMatrix()const
{
	return _WorldMatrix;
}

const D3DXMATRIX & Transform::GetRotateMatrix()const
{
	return _RotateMatrix;
}

D3DXMATRIX * Transform::GetRotateMatrixAddress()
{
	return &_RotateMatrix;
}