#pragma once
#include <math/Matrix4.h>
#include <math/Vector3.h>
#include <3d/WorldTransform.h>

// namespace 任意の名称　として
// {} で囲うことで
// stdのように　任意の名称::関数として　呼び出すことが出来る
// ただしこの場合　usingnamespace で任意の名称を省略するか
// 毎回　任意の名称:: とする必要がある


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
// 行列とベクトルの掛け算
Vector3 MatMulti(const Vector3& vec, const Matrix4& mat);

// ラジアンに変換
float ConvartToRadian(const float degree);

float ConvartToDegree(const float radian);

// 限界値設定
float UpperLimit(const float& num, const float& limit);
float LowerLimit(const float& num, const float& limit);
float Clamp(const float& num, const float& min, const float& max);


// 親から順に更新処理
void UpdateWorldTransform(WorldTransform& worldTransform);

//　ライン描画のラッピング関数
void DrawXYZLine3DRGB(const Vector3& startPos, const Vector3& endPosLen);

// 線形補完
float Lerp(const float& start, const float& end, float time);