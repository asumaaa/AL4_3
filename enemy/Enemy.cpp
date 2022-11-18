#include "Enemy.h"

Enemy* Enemy::GetInstance()
{
    static Enemy instance;
    return &instance;
}

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(ID3D12Device* device, Model* model)
{
	//引数から受け取った値を代入
	this->device_ = device;
	this->model_ = model;

	//オブジェクト初期化
	Object3D* newObject = new Object3D();
	newObject->Initialize(device_, model_);
	object3d_.reset(newObject);
}

void Enemy::Update(XMMATRIX& matView, XMMATRIX& matProjection)
{
	Move();

	object3d_->setScale(scale_);
	object3d_->setRotation(rotation_);
	object3d_->setPosition(position_);
	//オブジェクト更新
	object3d_->Update(matView, matProjection);
}

bool Enemy::Attack()
{
	bulletTimer_++;
	if (bulletTimer_ % 10 == 0)
	{
		return true;
	}
    return false;
}

void Enemy::Move()
{
}

void Enemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object3d_->Draw(cmdList, model_->vbView, model_->ibView);
}

void Enemy::setPosition(XMFLOAT3 pos)
{
	position_ = pos;
}

void Enemy::setRotation(XMFLOAT3 rot)
{
	rotation_ = rot;
}

void Enemy::setScale(XMFLOAT3 sca)
{
	scale_ = sca;
}
