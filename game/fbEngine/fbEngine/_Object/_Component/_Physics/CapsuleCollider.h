#pragma once
#include "_Object\_Component\_Physics\Collider.h"

//�J�v�Z����̃R���C�_�[
class CCapsuleCollider : public Collider
{
public:
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	CCapsuleCollider(GameObject* g, Transform* t);
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	~CCapsuleCollider();
	/*!
	* @brief	�쐬�B
	*/
	void Create(float radius, float height)
	{
		if (radius < 0.0f || height < 0.0f) {
			// �R���C�_�[�T�C�Y��0��菬�����l���ݒ肳��Ă��B
			abort();
		}
		shape = new btCapsuleShape(radius, height);
		btVector3 work = shape->getImplicitShapeDimensions();
		_halfSize = Vector3(work.x(), work.y() + work.x(), work.z());
	}

private:
	// �`��ɉ��������f���f�[�^�����[�h�B
	void ColliderModelLoad()override {
		// �J�v�Z���͂܂��Ȃ��̂Ő������Ȃ��B
	}


public:
	btCollisionShape* GetBody() override
	{
		return shape;
	}
private:
	btCapsuleShape*		shape;
};