#pragma once
//�I�u�W�F�N�g�̃J�����O�����̃C���^�[�t�F�[�X�N���X�B

class AABB;

/*!
*@brief	CPU�ł̑����J�����O�����̃C���^�[�t�F�[�X�N���X�B
*@details
* Strategy�p�^�[�����g�p���Đ݌v����Ă��܂��B
*/
class IObjectCulling {
public:
	IObjectCulling() {}
	virtual ~IObjectCulling() {}

	//�J�����O�A���S���Y�������s�B
	virtual void Execute(const AABB& aabb,const D3DXMATRIX& rotation, const D3DXMATRIX& view, const D3DXMATRIX& proj) = 0;
	//�J�����O����Ă��邩����B
	bool IsCulling() const
	{
		return _isCulling;
	}
protected:
	//�J�����O�t���O��ݒ�B
	void SetCullingFlag(bool flag)
	{
		_isCulling = flag;
	}
private:
	//�J�����O����Ă��邩�̃t���O�B
	bool	_isCulling = false;
};