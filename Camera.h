#pragma once
#include "DirectXMath.h"
#include "Input.h"
#include "WinApp.h"
#include "Player.h"

using namespace DirectX;

class Camera
{
public:
	//シングルトンインスタンス
	static Camera* GetInstance();
	//インストラクタ デストラクタ
	Camera();
	~Camera();
	//初期化
	void Initialize(Input* input, DXInput* dxInput,Player* player);
	//更新
	void Update();
	//プレイヤーの後ろをつける挙動
	void homind();
	//ゲッターセッター
	void SetTarget(XMFLOAT3 pos);
	void SetEye(XMFLOAT3 pos);
	XMFLOAT3 GetEye() { return eye_; };
	XMFLOAT3 GetTraget() { return target_; };
	XMFLOAT3 GetUp() { return up_; };
	XMMATRIX GetMatProjection() { return matProjection_; };
	XMMATRIX GetMatView() { return matView_; };
private:
	//プレイヤー
	Player* player_;
	//入力
	Input* input_;
	DXInput* dxInput_;
	//射影変換
	XMMATRIX matProjection_;
	//ビュー変換行列
	XMMATRIX matView_;
	XMFLOAT3 eye_ = { -10, 10, 30 };
	XMFLOAT3 target_ = { 0, 0, 0 };
	XMFLOAT3 up_ = { 0, 1, 0 };
	//プレイヤーの後ろをつける挙動
	//プレイヤーとカメラの距離
	float length_ = 40.0f;
	//入力によって加算させる変数
	float lengthX_ = 0.0f;
	float lengthZ_ = 0.0f;
};

