#pragma once
#include <3d/Model.h>
#include <input/Input.h>
#include <2d/DebugText.h>
#include <EnemyBullet.h>
class Enemy
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	void Initialize(Model* model);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	void Rotate(const Vector3& moveState, const Vector3& rotate);


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
	// �ړ����x
	Vector3 velocity_;
	// �e
	std::list<std::unique_ptr<EnemyBullet>>bullets_;
	/// <summary>
	/// �U��
	/// </summary>
	void Fire();

public:
	// ���ˊԊu
	static const int kFireInterVal = 10;
private:
	// ���˃^�C�}�[
	int32_t fireTimer = 0;

public:
	// �Q�b�^�[
	WorldTransform GetWorldTransform() { return worldTransform_; }


public:
	// �s���t�F�[�Y
	enum class Phaze {
		Default,		// ����
		Approach,		// �ڋ�
		Leave,			// ���E
	};

	// �ڋ߃t�F�[�Y������
	void MoveApproachInitialize();

	void MoveDefault(const Vector3& limit);
	void MoveApproach(const Vector3& limit);
	void MoveLeave(const Vector3& limit);
private:
	// �t�F�[�Y
	Phaze phaze_ = Phaze::Default;
};

