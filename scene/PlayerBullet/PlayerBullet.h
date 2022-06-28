#pragma once
#include <3d/Model.h>

class PlayerBullet
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

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
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	// ���x
	Vector3 velocity_;
	// ����<frm>
	static const int32_t kLifeTime = 60 + 5;
	// �f�X�^�C�}�[
	int32_t deathTime_ = kLifeTime;
	// �f�X�t���O
	bool isDead_ = false;

public:
	// �Q�b�^�[
	bool IsDead() const { return isDead_; }
	// ���[���h���W�̎擾
	Vector3 GetWorldPosition();
	
};

