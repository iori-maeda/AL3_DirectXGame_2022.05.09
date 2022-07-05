#pragma once
#include <3d/WorldTransform.h>
#include <3d/Model.h>
class Skydome
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">3D���f��</param>
	void Initialize(Model* model);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(ViewProjection viewProjection);

private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// ���f��
	Model* model_ = nullptr;
};

