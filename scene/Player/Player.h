#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include <input/Input.h>
#include <2d/DebugText.h>
#include <scene/PlayerBullet/PlayerBullet.h>
#include<memory>
#include<list>

class Player
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="textureHandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model,uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// ��]
	/// </summary>
	/// <param name="moveState"></param>
	/// <param name="rotate"></param>
	void Rotate(const Vector3& moveState, const Vector3& rotate);

	/// <summary>
	/// �Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	/// </summary>
	void OnCollision();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����</param>
	void Draw(const ViewProjection& viewProjection);

private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// ���f��
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	// ���͏������邽��
	Input* input_ = nullptr;
	// �f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;
	// �e
	std::list<std::unique_ptr<PlayerBullet>>bullets_;
	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

public:
	// �Q�b�^�[
	WorldTransform GetWorldTransform() { return worldTransform_; }
	// ���[���h���W���擾
	Vector3 GetWorldPosition();

	// �Z�b�^�[
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullet() { return bullets_; }
};

