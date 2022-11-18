#include "EnemyBullet.h"

EnemyBullet* EnemyBullet::GetInstance()
{
	static EnemyBullet instance;
	return &instance;
}

EnemyBullet::EnemyBullet()
{
}

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::Initialize(ID3D12Device* device, Model* model, const XMFLOAT3& position, const XMFLOAT3& velocity)
{
	//ˆø”‚©‚çó‚¯æ‚Á‚½’l‚ğ•Ï”‚É‘ã“ü
	this->device_ = device;
	this->model_ = model;
	this->position_ = position;
	this->velocity_ = velocity;

	Object3D* newObject = new Object3D();
	newObject->Initialize(device_, model_);
	object3d_.reset(newObject);

	object3d_->setPosition(position_);
	object3d_->setRotation(rotation_);
	object3d_->setScale(scale_);
}

void EnemyBullet::Update(XMMATRIX& matView, XMMATRIX& matProjection)
{
	//’e‚ÌˆÚ“®
	Move();

	object3d_->setPosition(position_);
	object3d_->setRotation(rotation_);
	object3d_->setScale(scale_);

	object3d_->Update(matView, matProjection);

	//ˆê’èŠÔŒo‰ß‚Å’eíœ
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Move()
{
	position_.x += velocity_.x * speed;
	position_.y += velocity_.y * speed;
	position_.z += velocity_.z * speed;
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}

void EnemyBullet::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object3d_->Draw(cmdList, model_->vbView, model_->ibView);
}
