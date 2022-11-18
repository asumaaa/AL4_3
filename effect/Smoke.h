#pragma once
#include "Model.h"
#include "Object3D.h"
#include "DirectXCommon.h"

class Smoke
{
public:
	//シングルトンインスタンス
	Smoke* GetInstance();
	Smoke();
	~Smoke();
	void Initialize(ID3D12Device* device, Model* model,XMFLOAT3 position);
	void Update(XMMATRIX& matView, XMMATRIX& matProjection, XMFLOAT3 position, XMFLOAT3 rotation);
	void Draw(ID3D12GraphicsCommandList* cmdList);
private:
	/*DirectXCommon* dx_;*/
	ID3D12Device* device_;
	Model* model_;

	//煙オブジェの数
	int smokeNum_ = 20;

	//タイマー
	int timer = 0;
	int timerNum = smokeNum_;

	//オブジェ
	std::list<std::unique_ptr<Object3D>> object3Ds_;
	
	std::vector<XMFLOAT3> position_;
	std::vector<XMFLOAT3> scale_;
	std::vector<XMFLOAT3> rotation_;

	//スケールをセットする際の数値
	float scaleSetNum_ = 0.8f;

	//煙を発生させるモデルと煙の中心からの距離
	float length_ = 7;
};

