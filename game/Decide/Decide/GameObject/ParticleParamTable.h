#pragma once
#include "fbEngine\fbstdafx.h"
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"

namespace ParticleParamTable {
	enum Type { Min = -1, FoxFireViolet = 0, BombViolet, KatamariViolet, ShockViolet, TiroTiroViolet,TiroTiroTowerViolet, Max };

	static ParticleParameter Params[/*Type::Max*/] = {
		// ���̌ω΃G�t�F�N�g�B
		{
			"MurasakiHonoo.png",	// �e�N�X�`�����B
			Vector3(0.0f,3.0f,0.0f),			// �����x�B
			Vector2(0.5f, 0.5f),	// �e�N�X�`���T�C�Y�B
			0.1f,					// �����B
			0.008f,					// �����҂����ԁB
			Vector3(0.3f, 0.3f, 0.4f),	// �����ʒu�̃����_�����B
			Vector3(0.0f, 0.0f, 0.0f),	// �����x�̃����_�����B
			Vector3(0.4f,0.8f,0.4f),	// ���x�̐ϕ����̃����_�����B
			0.0f,						// �d�́B
			true,					// �t�F�[�h�A�E�g���邩�B
			0.2f,					// �t�F�[�h���ԁB
			1.0f,					// �����A���t�@�B
			true,					// �r���{�[�h�t���O(��ɃJ�����̂ق���������)�B
			0.7f,					// �P�x�B
			1,						// �������[�h(0�ŏ�Z�A1�ŉ��Z)�B
			Color(1.3f,1.0f,1.3f,1.0f),	// ��Z�J���[�B
			true					// �G�~�b�^�[�Ɛ��������p�[�e�B�N���̊ԂŐe�q�֌W�����Ԃ��B
		},

		// ���̔����G�t�F�N�g�B
		{
			"MurasakiHonoo.png",	// �e�N�X�`�����B
			Vector3::zero,			// �����x�B
			Vector2(2.0f, 2.0f),	// �e�N�X�`���T�C�Y�B
			0.002f,					// �����B
			0.005f,					// �����҂����ԁB
			Vector3(0.1f, 0.1f, 0.1f),	// �����ʒu�̃����_�����B
			Vector3(6.0f, 6.0f, 6.0f),	// �����x�̃����_�����B
			Vector3(2.0f, 2.0f, 2.0f),	// ���x�̐ϕ����̃����_�����B
			0.0f,						// �d�́B
			true,					// �t�F�[�h�A�E�g���邩�B
			0.5f,					// �t�F�[�h���ԁB
			1.0f,					// �����A���t�@�B
			true,					// �r���{�[�h�t���O(��ɃJ�����̂ق���������)�B
			1.1f,					// �P�x�B
			1,						// �������[�h(0�ŏ�Z�A1�ŉ��Z)�B
			Color(1.3f, 1.0f, 1.3f, 1.0f),	// ��Z�J���[�B
			false					// �G�~�b�^�[�Ɛ��������p�[�e�B�N���̊ԂŐe�q�֌W�����Ԃ��B
		},

		// ���̐^�񒆂ɉ�A���ӂɂ�����̃G�t�F�N�g�B
		//// ���g���邩������Ȃ��̂Œu���Ă����B
		{
			"MurasakiHonoo.png",	// �e�N�X�`�����B
			Vector3::zero,			// �����x�B
			Vector2(0.5f, 0.5f),	// �e�N�X�`���T�C�Y�B
			0.2f,					// �����B
			0.008f,					// �����҂����ԁB
			Vector3(0.1f, 0.1f, 0.1f),	// �����ʒu�̃����_�����B
			Vector3(3.0f, 3.0f, 3.0f),	// �����x�̃����_�����B
			Vector3(2.0f, 2.0f, 2.0f),	// ���x�̐ϕ����̃����_�����B
			0.0f,						// �d�́B
			true,					// �t�F�[�h�A�E�g���邩�B
			0.5f,					// �t�F�[�h���ԁB
			1.0f,					// �����A���t�@�B
			true,					// �r���{�[�h�t���O(��ɃJ�����̂ق���������)�B
			0.55f,					// �P�x�B
			1,						// �������[�h(0�ŏ�Z�A1�ŉ��Z)�B
			Color(1.3f, 1.0f, 1.3f, 1.0f),	// ��Z�J���[�B
			true					// �G�~�b�^�[�Ɛ��������p�[�e�B�N���̊ԂŐe�q�֌W�����Ԃ��B
		},

		// ���̏Ռ��g�B
		{
			"MurasakiHonoo.png",	// �e�N�X�`�����B
			Vector3(0.0f,4.5f,0.0f) + Vector3(0.0f,0.0f,-3.0f),	// �����x�B
			Vector2(0.5f, 0.5f),	// �e�N�X�`���T�C�Y�B
			0.1f,					// �����B
			0.004f,					// �����҂����ԁB
			Vector3(0.25f, 0.0f, 0.5f),	// �����ʒu�̃����_�����B
			Vector3(1.0f, 3.0f, 2.0f),	// �����x�̃����_�����B
			Vector3(0.0f, 0.0f, 0.0f),	// ���x�̐ϕ����̃����_�����B
			0.0f,						// �d�́B
			true,					// �t�F�[�h�A�E�g���邩�B
			0.2f,					// �t�F�[�h���ԁB
			1.0f,					// �����A���t�@�B
			true,					// �r���{�[�h�t���O(��ɃJ�����̂ق���������)�B
			1.1f,					// �P�x�B
			1,						// �������[�h(0�ŏ�Z�A1�ŉ��Z)�B
			Color(1.3f, 1.0f, 1.3f, 1.0f),	// ��Z�J���[�B
			true					// �G�~�b�^�[�Ɛ��������p�[�e�B�N���̊ԂŐe�q�֌W�����Ԃ��B
		},

		// ���̂��낿��(�����ʒu�̃����_�����̓R�s�[��ł��������ɂ��Ă�����)�B
		{
			"MurasakiHonoo.png",	// �e�N�X�`�����B
			Vector3(0.0f,0.25f,0.0f),	// �����x�B
			Vector2(0.5f, 0.5f),	// �e�N�X�`���T�C�Y�B
			0.1f,					// �����B
			0.006f,					// �����҂����ԁB
			Vector3(0.2f, 0.0f, 0.0f),	// �����ʒu�̃����_�����B
			Vector3(0.5f, 0.5f, 0.5f),	// �����x�̃����_�����B
			Vector3(0.5f, 0.0f, 10.0f),	// ���x�̐ϕ����̃����_�����B
			0.0f,					// �d�́B
			true,					// �t�F�[�h�A�E�g���邩�B
			1.0f,					// �t�F�[�h���ԁB
			1.0f,					// �����A���t�@�B
			true,					// �r���{�[�h�t���O(��ɃJ�����̂ق���������)�B
			0.5f,					// �P�x�B
			1,						// �������[�h(0�ŏ�Z�A1�ŉ��Z)�B
			Color(1.3f, 1.0f, 1.3f, 1.0f),	// ��Z�J���[�B
			false					// �G�~�b�^�[�Ɛ��������p�[�e�B�N���̊ԂŐe�q�֌W�����Ԃ��B
		},

		// ���̌��̓�(���낿��)�B
		{
			"MurasakiHonoo.png",	// �e�N�X�`�����B
			Vector3(0.0f,3.0f,0.0f),	// �����x�B
			Vector2(0.5f, 0.5f),	// �e�N�X�`���T�C�Y�B
			5.0f,					// �����B
			0.05f,					// �����҂����ԁB
			Vector3(0.4f, 0.0f, 0.4f),	// �����ʒu�̃����_�����B
			Vector3(0.0f,2.0f,0.0f),	// �����x�̃����_�����B
			Vector3::zero,	// ���x�̐ϕ����̃����_�����B
			0.0f,						// �d�́B
			true,					// �t�F�[�h�A�E�g���邩�B
			2.0f,					// �t�F�[�h���ԁB
			1.0f,					// �����A���t�@�B
			true,					// �r���{�[�h�t���O(��ɃJ�����̂ق���������)�B
			1.1f,					// �P�x�B
			1,						// �������[�h(0�ŏ�Z�A1�ŉ��Z)�B
			Color(1.3f, 1.0f, 1.3f, 1.0f),	// ��Z�J���[�B
			true					// �G�~�b�^�[�Ɛ��������p�[�e�B�N���̊ԂŐe�q�֌W�����Ԃ��B
		},
	};
};