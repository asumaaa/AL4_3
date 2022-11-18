#include "Model.h"
#include "Object3D.h"
#include "DirectXCommon.h"
#include "DXInput.h"

#pragma once
class Player
{
public:
	//シングルトンインスタンス
	Player* GetInstance();
	Player();
	~Player();
	void Initialize(ID3D12Device* device,Model* model, DXInput* dxInput);
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);
	bool Attack();
	void Move();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//ゲッター　セッター　
	XMFLOAT3 GetPosition() { return position_; };
	XMFLOAT3 GetRotation() { return rotation1_; };
	XMFLOAT3 GetScale() { return scale_; };
	XMFLOAT3 GetVelocity() { return velocity; };
	void setPosition(XMFLOAT3 pos);
	void setRotation(XMFLOAT3 rot);
	void setScale(XMFLOAT3 sca);
private:
	/*DirectXCommon* dx_ = nullptr;*/
	ID3D12Device* device_;
	Model* model_ = nullptr;
	std::unique_ptr<Object3D> object3d_;
	DXInput* dxInput_;
	//アフィン変換情報
	XMFLOAT3 scale_ = { 1,1,1 };
	XMFLOAT3 rotation1_ = { 0,0,0 };	//プレイヤー本来の角度
	XMFLOAT3 rotation2_ = { 0,0,0 };	//オブジェクトに渡す角度
	XMFLOAT3 position_ = { 0,0,-100 };

	//スピード
	XMFLOAT3 velocity;
	float speed = 1.0f;

	//弾のタイマー
	int bulletTimer_ = 4;

	//体力
	float hp_ = 5;
};

