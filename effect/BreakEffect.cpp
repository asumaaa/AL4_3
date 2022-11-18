#include "BreakEffect.h"
#include "stdlib.h"
#include "Math2.h"
#define PI 3.14159265359

BreakEffect* BreakEffect::GetInstance()
{
    static BreakEffect instance;
    return &instance;
}

BreakEffect::BreakEffect()
{
}

BreakEffect::~BreakEffect()
{
}

void BreakEffect::Initialize(ID3D12Device* device, Model* model, XMFLOAT3 position)
{
    this->device_ = device;
    this->model_ = model;

    position_.resize(breakEffectNum);
    scale_.resize(breakEffectNum);
	rotation_.resize(breakEffectNum);
	velocity_.resize(breakEffectNum);

	//初期値設定
	for (int i = 0; i < breakEffectNum; i++)
	{
		position_[i] = position;
		scale_[i].x = static_cast<float>(rand() % 30) / 10;
		scale_[i].y = static_cast<float>(rand() % 30) / 10;
		scale_[i].z = static_cast<float>(rand() % 30) / 10;
		rotation_[i].x= static_cast<float>(rand() % 36) / 18 * PI;
		rotation_[i].y= static_cast<float>(rand() % 36) / 18 * PI;
		rotation_[i].z= static_cast<float>(rand() % 36) / 18 * PI;
		velocity_[i] = { 0, 0, speed };
		rollRotation(&velocity_[i], rotation_[i]);
	}
	//オブジェクト初期化
	for (int i = 0; i < breakEffectNum; i++)
	{
		std::unique_ptr<Object3D> newObject = std::make_unique<Object3D>();
		newObject->Initialize(device_, model_);
		object3Ds_.push_back(std::move(newObject));
	}
	int i = 0;
	for (std::unique_ptr<Object3D>& object : object3Ds_)
	{
		object->setPosition(position_[i]);
		object->setRotation(rotation_[i]);
		object->setScale(scale_[i]);
		i++;
	}
}

void BreakEffect::Update(XMMATRIX& matView, XMMATRIX& matProjection)
{
	for (int i = 0; i < breakEffectNum; i++)
	{
		position_[i].x += velocity_[i].x * speed;
		position_[i].y += velocity_[i].y * speed;
		position_[i].z += velocity_[i].z * speed;
		scale_[i].x *= (float)29/30;
		scale_[i].y *= (float)29/30;
		scale_[i].z *= (float)29/30;
	}
	int i = 0;
	for (std::unique_ptr<Object3D>& object : object3Ds_)
	{
		object->setPosition(position_[i]);
		object->setRotation(rotation_[i]);
		object->setScale(scale_[i]);
		i++;
	}

	for (std::unique_ptr<Object3D>& object : object3Ds_)
	{
		object->Update(matView, matProjection);
	}
	//一定時間経過で弾削除
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void BreakEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	for (std::unique_ptr<Object3D>& object : object3Ds_)
	{
		object->Draw(cmdList, model_->vbView, model_->ibView);
	}
}
