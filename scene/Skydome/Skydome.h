#pragma once
#include <3d/WorldTransform.h>
#include <3d/Model.h>
class Skydome
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">3Dモデル</param>
	void Initialize(Model* model);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(ViewProjection viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
};

