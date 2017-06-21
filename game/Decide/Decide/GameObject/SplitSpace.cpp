#include "stdafx.h"
#include "SplitSpace.h"
#include "fbEngine\_Object\_Component\_Physics\BoxCollider.h"
#include "fbEngine\_Object\_Component\_Physics\GostCollision.h"
#include "GameSystem.h"
#include "SpaceCollisionObject.h"

void SplitSpace::Awake() {
	// �אڗv�f��񋓁B
	_AdjacentIndex[Space::Up] = Vector3::down;
	_AdjacentIndex[Space::Down] = Vector3::up;
	_AdjacentIndex[Space::Right] = Vector3::right;
	_AdjacentIndex[Space::Left] = Vector3::left;
	_AdjacentIndex[Space::Front] = Vector3::front;
	_AdjacentIndex[Space::Back] = Vector3::back;

	_AdjacentIndex[Space::RightUp] = Vector3::down + Vector3::right;
	_AdjacentIndex[Space::RightDown] = Vector3::up + Vector3::right;
	_AdjacentIndex[Space::LeftUp] = Vector3::down + Vector3::left;
	_AdjacentIndex[Space::LeftDown] = Vector3::up + Vector3::left;

	_AdjacentIndex[Space::RightUpFront] = _AdjacentIndex[Space::RightUp] + Vector3::front;
	_AdjacentIndex[Space::RightDownFront] = _AdjacentIndex[Space::RightDown] + Vector3::front;
	_AdjacentIndex[Space::LeftUpFront] = _AdjacentIndex[Space::LeftUp] + Vector3::front;
	_AdjacentIndex[Space::LeftDownFront] = _AdjacentIndex[Space::LeftDown] + Vector3::front;
	_AdjacentIndex[Space::RightUpBack] = _AdjacentIndex[Space::RightUp] + Vector3::back;
	_AdjacentIndex[Space::RightDownBack] = _AdjacentIndex[Space::RightDown] + Vector3::back;
	_AdjacentIndex[Space::LeftUpBack] = _AdjacentIndex[Space::LeftUp] + Vector3::back;
	_AdjacentIndex[Space::LeftDownBack] = _AdjacentIndex[Space::LeftDown] + Vector3::back;
}

void SplitSpace::Split(const SkinModelData* data,const Transform& transform, int x, int y, int z) {
	if (data == nullptr) {
		abort();
		// ���f���f�[�^��null�B
	}

	_splitX = x;
	_splitY = y;
	_splitZ = z;

	CreateSplitBox(CreateSpaceBox(*data, _unSplitSpaceSize),transform,x,y,z);
}

const Vector3& SplitSpace::CreateSpaceBox(const SkinModelData& data, Vector3& size) {
	float minX,minY,minZ,maxX,maxY,maxZ;	// ���_�̍ŏ��ʒu�ƍő�ʒu(���f�������ł���{�b�N�X���`����̂ŁA�v�f���Ƃɍŏ��l�ƍő�l�����߂�)�B
	minX = minY = minZ = FLT_MAX;
	maxX = maxY = maxZ = 0.0f;
	
	vector<LPD3DXMESH> meshList = data.GetMeshList();	// ���ׂẴ��b�V�����擾�B
	for (auto mesh : meshList) {
		// ���ׂẴ��b�V�����Q�Ƃ���B

		int offset = 0;	// ���_�f�[�^�̐擪�������f�[�^�܂ł̍����B
		int stride = 0;	// ���_����̃o�C�g�T�C�Y�B
		D3DXVECTOR3* localPosition;	// ���f���̍��W�n�ł̒��_�ʒu�B

		LPDIRECT3DVERTEXBUFFER9 vb = nullptr;
		mesh->GetVertexBuffer(&vb);	// ���_�o�b�t�@�擾�B 

		D3DVERTEXBUFFER_DESC desc;
		vb->GetDesc(&desc);	// ���_�o�b�t�@�Ɋւ�������擾�B
		stride = desc.Size / mesh->GetNumVertices();	// ���_����̃o�C�g�T�C�Y���擾(���_�o�b�t�@�̃o�C�g�T�C�Y / ���_��)�B

		D3DVERTEXELEMENT9 element[MAX_FVF_DECL_SIZE];	// ���_�f�[�^�̃��C�A�E�g����D3DVERTEXELEMENT9�\���̂̔z��ŕ\�����B
		mesh->GetDeclaration(element);	// ���_�̍\���v�f�����擾�B
		for (int idx = 0; true;idx++) {
			// ���_�f�[�^���炨�]�݂̃f�[�^�܂ł�Offset��T���B

			if (element[idx].Usage == D3DDECLUSAGE_POSITION) {
				// ���]�݂̃f�[�^�����B
				offset = element[idx].Offset;
				break;
			}
		}

		char* pData = nullptr;	// 1�o�C�g�P�ʂŎg�p�ł���char*�Ńf�[�^���擾�B
		vb->Lock(0,0,(void**)&pData, D3DLOCK_READONLY);	// ���_�o�b�t�@�����b�N�B�������͎b��A�Ƃ肠�����ǂݎ���p�B
		for (int idx = 0; idx < mesh->GetNumVertices(); idx++) {
			// ���_���ׂĂ��Q�Ƃ���B

			localPosition = reinterpret_cast<D3DXVECTOR3*>(pData + offset);	// �e���_�f�[�^�̐擪����ʒu���̓������f�[�^�܂ō��������Z�B
			if (localPosition->x < minX) {
				minX = localPosition->x;
			}
			else if (localPosition->x > maxX) {
				maxX = localPosition->x;
			}

			if (localPosition->y < minY) {
				minY = localPosition->y;
			}
			else if (localPosition->y > maxY) {
				maxY = localPosition->y;
			}

			if (localPosition->z < minZ) {
				minZ = localPosition->z;
			}
			else if (localPosition->z > maxZ) {
				maxZ = localPosition->z;
			}

			pData += stride;	// 1���_���|�C���^��i�߂�B
		}
		vb->Unlock();	// ���_�o�b�t�@�̃��b�N�������B
	}

	// ���߂��ŏ��ʒu�ƍő�ʒu����{�b�N�X�̃T�C�Y���`�B
	size = Vector3(maxX - minX,maxY - minY,maxZ - minZ);
	return size;
}

void SplitSpace::CreateSplitBox(const Vector3& size,const Transform& transform, int x, int y, int z) {
	if (x <= 0 || y <= 0 || z <= 0) {
		abort();
		// ��������0��菬�����l���ݒ肳�ꂽ�B
	}

	// �O�����z�񕪃������̈�m�ہB
	_SpaceCollisions = vector<vector<vector<SpaceCollisionObject*>>>(y, vector<vector<SpaceCollisionObject*>>(x, vector<SpaceCollisionObject*>(z,nullptr)));

	for (int idxX = 0; idxX < x; idxX++) {
		for (int idxY = 0; idxY < y; idxY++) {
			for (int idxZ = 0; idxZ < z; idxZ++) {
				// ���������̃R���W���������B

				SpaceCollisionObject* box = INSTANCE(GameObjectManager)->AddNew<SpaceCollisionObject>("SpaceBox", 1);
				_splitSpaceSize = Vector3(size.x / x, size.y / y, size.z / z);
				box->Create(transform.GetPosition() - (size * 0.5f) + Vector3((idxX * (_splitSpaceSize.x)) + (_splitSpaceSize.x * 0.5f), (idxY * (_splitSpaceSize.y)) + (_splitSpaceSize.y * 0.5f), (idxZ * (_splitSpaceSize.z)) + (_splitSpaceSize.z * 0.5f)),transform.GetRotation(), _splitSpaceSize,Collision_ID::SPACE);
				// ���̈ʒu���𒆐S�Ƃ��ĕ����ł���悤�|�W�V���������B

				_SpaceCollisions[idxY][idxX][idxZ] = box;
			}
		}
	}

	_AdjacentSpace();
}

void SplitSpace::_AdjacentSpace() {
	for (int y = 0; y < _splitY; y++) {
		for (int x = 0; x < _splitX; x++) {
			for (int z = 0; z < _splitZ; z++) {
				for (int idx = 0; idx < Space::Max; idx++) {
					int workX = x + static_cast<int>(_AdjacentIndex[idx].x);
					int workY = y + static_cast<int>(_AdjacentIndex[idx].y);
					int workZ = z + static_cast<int>(_AdjacentIndex[idx].z);

					if (workX >= 0 && workX < _splitX && workY >= 0 && workY < _splitY && workZ >= 0 && workZ < _splitZ) {
						// �z��O�ɃA�N�Z�X���Ă��Ȃ��B
						_SpaceCollisions[y][x][z]->AddAdjacentSpaceObject(_SpaceCollisions[workY][workX][workZ]);
					}
				}
			}
		}
	}
}

void SplitSpace::AddObjectHitSpace(const GameObject& object) {
	for (auto& y : _SpaceCollisions) {
		for (auto& x : y) {
			for (auto z : x) {
				z->AddObjectHitSpace(const_cast<GameObject&>(object));
			}
		}
	}
}

void SplitSpace::RegistrationObject() {
	for (auto& y : _SpaceCollisions) {
		for (auto& x : y) {
			for (auto z : x) {
				z->RegistrationObject();
			}
		}
	}
}

