#include "Transform.h"
#include "GameObject.h"

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
		//�q�̐e��unull��
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
	_Parent = nullptr;
	_Children.clear();

	position = localPosition = angle = localAngle = Vector3::zero;
	scale = localScale = Vector3::one;

	D3DXMatrixIdentity(&_WorldMatrix);
	D3DXMatrixIdentity(&_RotateMatrix);
}

void Transform::Start()
{
	UpdateTransform();
}

void Transform::Update()
{
	UpdateTransform();
}

//�q������������֐�
//�߂�l�FTransform* �q�b�g�����q�̃A�h���X
//�������Fchar* �q���̖��O
Transform* Transform::FindChild(char* childname)
{
	//�C�e���[�^�擾
	vector<Transform*>::iterator it = _Children.begin();
	//�Ō�܂Ō���
	while (it != _Children.end())
	{
		//�q�b�g�������H
		if((*it)->gameObject->GetName() == childname)
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
Transform ** Transform::FindChilds(char * childname)
{
	vector<Transform*> v;
	//�C�e���[�^�擾
	vector<Transform*>::iterator it = _Children.begin();
	//�Ō�܂Ō���
	while (it != _Children.end())
	{
		//�q�b�g�������H
		if ((*it)->gameObject->GetName() == childname)
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
		localPosition.CopyFrom(lpos);
		//���[�J���~���[���h
		D3DXVec3Transform(&pos, &lpos, &PWorld);
		//�|�W�V����
		position.x = pos.x;
		position.y = pos.y;
		position.z = pos.z;
		//�X�P�[���l
		scale.x = localScale.x * _Parent->scale.x;
		scale.y = localScale.y * _Parent->scale.y;
		scale.z = localScale.z * _Parent->scale.z;
		//�N�H�[�^�j�I���@������
		angle.x = localAngle.x;
		angle.y = localAngle.y;
		angle.z = localAngle.z;
		//angle = localAngle * qParentRot;
		D3DXQUATERNION q;
		D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(angle.y), D3DXToRadian(angle.x), D3DXToRadian(angle.z));
		rotation = q;
	}
	else {
		//���[�J�������̂܂�
		position = localPosition;
		scale = localScale;
		angle = localAngle;
		D3DXQUATERNION q;
		D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(angle.y), D3DXToRadian(angle.x), D3DXToRadian(angle.z));
		rotation = q;
	}
	//��]
	D3DXMatrixIdentity(&_RotateMatrix);
	D3DXQUATERNION q;
	rotation.CopyFrom(q);
	D3DXMatrixRotationQuaternion(&_RotateMatrix, &q);

	UpdateWolrdMatrix();
}

//�g�����X�t�H�[�����烏�[���h�s��X�V
//�߂�l�F�Ȃ�
//�����F�Ȃ�
void Transform::UpdateWolrdMatrix()
{
	//���[���h�s������߂�B
	D3DXMATRIX Scale, Pos;
	D3DXMatrixIdentity(&Scale);
	D3DXMatrixIdentity(&Pos);
	//�T�C�Y
	D3DXMatrixScaling(&Scale, scale.x, scale.y, scale.z);
	//�|�W�V����
	D3DXMatrixTranslation(&Pos, position.x, position.y, position.z);
	_WorldMatrix = Scale * _RotateMatrix * Pos;
}

Vector3 Transform::Direction(Vector3 v)
{
	D3DXVECTOR3 in;
	D3DXVECTOR3 out;
	v.CopyFrom(in);
	D3DXVec3TransformCoord(&out, &in, &_RotateMatrix);

	Vector3 r = out;
	//���K���͂ǂ����悤���H�H�H
	//r.Normalize();
	return r;
}

Vector3 Transform::Local(Vector3 v)
{
	D3DXVECTOR4 pos;	//�|�W�V����
	D3DXVECTOR3 lpos;	//���[�J���|�W�V����
	//v���R�s�[
	v.CopyFrom(lpos);
	//���[���h�s�񂩂猩�����[�J���|�W�V������pos�Ɋi�[
	D3DXVec3Transform(&pos, &lpos, &_WorldMatrix);
	return Vector3(pos.x, pos.y, pos.z);
}

Vector3 Transform::LocalPos(Vector3 v)
{
	D3DXVECTOR4 pos;
	D3DXVECTOR3 lpos;
	v.CopyFrom(lpos);
	D3DXMATRIX offset;
	D3DXMatrixIdentity(&offset);
	offset._41 = _WorldMatrix._41;
	offset._42 = _WorldMatrix._42;
	offset._43 = _WorldMatrix._43;
	D3DXVec3Transform(&pos, &lpos, &offset);
	return Vector3(pos.x, pos.y, pos.z);
}

void Transform::LockAt(GameObject * obj)
{
	D3DXVECTOR3 target, me;
	obj->transform->position.CopyFrom(target);
	this->position.CopyFrom(me);
	D3DXMatrixIdentity(&_RotateMatrix);
	//�^�[�Q�b�g���猩������
	D3DXMatrixLookAtLH(&_RotateMatrix, &target, &me, &D3DXVECTOR3(0, 1, 0));
	//�t�s���
	D3DXMatrixInverse(&_RotateMatrix, NULL, &_RotateMatrix);
	_RotateMatrix._41 = 0.0f;   // �I�t�Z�b�g��؂�i��]�s�񂾂��ɂ��Ă��܂��j
	_RotateMatrix._42 = 0.0f;
	_RotateMatrix._43 = 0.0f;

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