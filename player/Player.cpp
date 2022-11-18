#include "Player.h"
#include "Math2.h"
#define PI 3.141592653589793238462643

Player* Player::GetInstance()
{
	static Player instance;
	return &instance;
}

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(ID3D12Device* device,Model* model, DXInput* dxInput)
{
	//引数から受け取った値を代入
	this->device_ = device;
	this->model_ = model;
	this->dxInput_ = dxInput;

	//オブジェクト初期化
	Object3D* newObject = new Object3D();
	newObject->Initialize(device_, model_);
	object3d_.reset(newObject);
}

void Player::Update(XMMATRIX& matView, XMMATRIX& matProjection)
{

	Move();

	object3d_->setScale(scale_);
	object3d_->setRotation(rotation2_);
	object3d_->setPosition(position_);
	//オブジェクト更新
	object3d_->Update(matView, matProjection);
}

bool Player::Attack()
{
	//Rボタンを押し続けることで弾を連射
	if (dxInput_->GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		bulletTimer_++;
		if (bulletTimer_ % 10 == 0)
		{
			return 1;
		}
	}
	return 0;
}

void Player::Move()
{
	velocity = { 0, 0, speed };

	//左ステックの変数
	float x = dxInput_->GamePad.state.Gamepad.sThumbLY / (32767.0f) * (PI / 90.0f);
	float y = dxInput_->GamePad.state.Gamepad.sThumbLX / (32767.0f) * (PI / 90.0f) * 1.2f;
	//上下
	if (dxInput_->GamePad.state.Gamepad.sThumbLY > 15000 || dxInput_->GamePad.state.Gamepad.sThumbLY < -15000)
	{
		if (rotation1_.x < PI / 4 && rotation1_.x > -PI / 4)
		{
			rotation1_.x -= x;
			rotation2_.x -= x;
		}
		if (rotation1_.x > PI / 4)
		{
			rotation1_.x = PI / 4 - x;
			rotation2_.x = PI / 4 - x;
		}
		if (rotation1_.x < -PI / 4)
		{
			rotation1_.x = -PI / 4 - x;
			rotation2_.x = -PI / 4 - x;
		}
	}
	//左右
	if (dxInput_->GamePad.state.Gamepad.sThumbLX > 15000 || dxInput_->GamePad.state.Gamepad.sThumbLX < -15000)
	{
		if (sin(rotation1_.x + (PI / 2) >= 0))
		{
			velocity = { speed * 3 / 10,  0, speed * 7 / 10 };
			rotation1_.y += y;
			rotation2_.y += y;
		}
		else
		{
			velocity = { -speed * 3 / 10,  0, speed * 7 / 10 };
			rotation1_.y -= y;
			rotation2_.y -= y;
		}
	}
	//左スティックで機体を傾ける処理
	if (dxInput_->GamePad.state.Gamepad.sThumbLX > 15000 || dxInput_->GamePad.state.Gamepad.sThumbLX < -15000)
	{
		if (rotation2_.z < PI / 6 && rotation2_.z > -PI / 6)
		{
			rotation2_.z -= y;
		}
		if (rotation2_.z > PI / 6)
		{
			rotation2_.z = PI / 6 - y;
		}
		if (rotation2_.z < -PI / 6)
		{
			rotation2_.z = -PI / 6 + y;
		}
	}
	//ステックに触っていないときの処理
	else if (rotation2_.z != 0)
	{
		if (rotation2_.z < -(PI / 90.0f))
		{
			rotation2_.z += (PI / 90.0f);
		}
		else if (rotation2_.z > (PI / 90.0f))
		{
			rotation2_.z -= (PI / 90.0f);
		}
	}


	//直進
	/*XMFLOAT3 velocity(0, 0, speed);*/
	rollRotation(&velocity, rotation1_ );

	position_.x += velocity.x * speed;
	position_.y += velocity.y * speed;
	position_.z += velocity.z * speed;
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object3d_->Draw(cmdList,model_->vbView, model_->ibView);
}

void Player::setPosition(XMFLOAT3 pos)
{ 
	position_ = pos;
}

void Player::setRotation(XMFLOAT3 rot)
{
	rotation1_ = rot;
}

void Player::setScale(XMFLOAT3 sca)
{
	scale_ = sca;
}
