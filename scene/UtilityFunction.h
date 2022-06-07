#pragma once
#include <math/Matrix4.h>
#include <math/Vector3.h>
#include <3d/WorldTransform.h>

// 円周率
const float PI = 3.1415f;


// 関数群プロトタイプ宣言
// スケーリング行列
Matrix4 MatScaling(const Vector3& scale);
// X軸回転行列
Matrix4 MatRotX(const float& rotX);
// Y軸回転行列
Matrix4 MatRotY(const float& rotY);
// Z軸回転行列
Matrix4 MatRotZ(const float& rotZ);
// 平行移動行列
Matrix4 MatTrans(const Vector3& translation_);
// 行列のZXY合成
void MatSyntheticZXY(WorldTransform& worldTransform_);

// ラジアンに変換
float ConvartToRadian(const float degree);

float ConvartToDegree(const float radian);

// 限界値設定
float UpperLimit(const float& num, const float& limit);
float LowerLimit(const float& num, const float& limit);
float Clamp(const float& num, const float& min, const float& max);


// 親から順に更新処理
void UpdateWorldTransform(WorldTransform& worldTransform);
