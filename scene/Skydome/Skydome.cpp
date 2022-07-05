#include "Skydome.h"

#include<assert.h>
#include <scene/UtilityFunction.h>

void Skydome::Initialize(Model* model)
{
	assert(model);
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0,0 };
	worldTransform_.scale_ = { 30,30,30 };
	this->model_ = model;
}

void Skydome::Update()
{
	MatSyntheticZXY(worldTransform_);
	worldTransform_.TransferMatrix();
}

void Skydome::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
