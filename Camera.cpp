#include "Camera.h"
#include "Math.h"
#define PI 3.14159265359

Camera* Camera::GetInstance()
{
	static Camera instance;
	return &instance;
}

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Initialize(Input* input, DXInput* dxInput,Player* player)
{
	//引数から受け取ったデータを代入
	this->input_ = input;
	this->dxInput_ = dxInput;
	this->player_ = player;

	//射影変換
	 matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),			//上下画角45度
		(float)window_width / window_height,//アスペクト比(画面横幅/画面立幅)
		0.1f, 1000.0f						//前端、奥端
	);

	 //行列計算
	 matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
}


void Camera::Update()
{
	homind();
	//行列計算
	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
}

void Camera::homind()
{
	//左右
	float addX = dxInput_->GamePad.state.Gamepad.sThumbRX / (32767.0f) * (PI / 90);
	float moveRange = PI / 3;
	if (dxInput_->GamePad.state.Gamepad.sThumbRX > 15000 || dxInput_->GamePad.state.Gamepad.sThumbRX < -15000)
	{
		if (lengthX_ < moveRange && lengthX_ > -moveRange)
		{
			lengthX_ -= addX;
			lengthZ_ -= addX;
		}
		if (lengthX_ > moveRange)
		{
			lengthX_ = moveRange - addX;
			lengthZ_ = moveRange - addX;
		}
		if (lengthX_ < -moveRange)
		{
			lengthX_ = -moveRange - addX;
			lengthZ_ = -moveRange - addX;
		}
	}
	//ステックに触っていないときの処理
	else if (lengthX_ != 0)
	{
		if (lengthX_ < -(PI / 90))
		{
			lengthX_ += (PI / 90);
			lengthZ_ += (PI / 90);
		}
		else if (lengthX_ > (PI / 90))
		{
			lengthX_ -= (PI / 90);
			lengthZ_ -= (PI / 90);
		}
	}

	float x = 0;
	float y = 0;
	float z = 0;

	//カメラをプレイヤーの真後ろに移動
	x = player_->GetPosition().x + (/*sin(-player_->GetRotation().x + (PI / 2) + lengthX_) **/ cos(-player_->GetRotation().y - (PI / 2) + lengthX_) * length_);
	y = player_->GetPosition().y + (cos(-player_->GetRotation().x + (PI * 18 / 40)) * length_);
	z = player_->GetPosition().z + (/*sin(-player_->GetRotation().x + (PI / 2) + lengthZ_) **/ sin(-player_->GetRotation().y - (PI / 2) + lengthZ_) * length_);


	//天井を調整
	if (sin(-player_->GetRotation().x + (PI / 2)) <= 0)
	{
		up_ = { 0,-1,0 };
	}
	else
	{
		up_ = { 0,1,0 };
	}
	up_ = { 0,1,0 };

	//カメラの座標に代入
	eye_ = { x,y,z };
	//注視点をプレイヤーの座標にする
	target_ = { player_->GetPosition().x ,player_->GetPosition().y,player_->GetPosition().z };
}

void Camera::SetTarget(XMFLOAT3 pos)
{
	target_ = pos;
}

void Camera::SetEye(XMFLOAT3 pos)
{
	eye_ = pos;
}