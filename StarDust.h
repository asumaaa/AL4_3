#pragma once
#include "DirectXCommon.h"
#include "DirectXMath.h"
#include "Model.h"
#include "Object3D.h"

using namespace DirectX;

class StarDust
{
public:
	//シングルトンインスタンス
	StarDust* GetInstance();
	StarDust();
	~StarDust();
	void Initialize(ID3D12Device* device, Model* model);
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void Delete();
private:
	/*DirectXCommon* dx_;*/
	ID3D12Device* device_;
	Model* model_;

	//星屑の数
	int starDustNum = 1000;
	//星屑の大きさ
	float starDustScale = 0.2;

	std::list<std::unique_ptr<Object3D>> object3Ds_;

	std::vector<XMFLOAT3> position_;
	std::vector<XMFLOAT3> scale_;
	std::vector<XMFLOAT3> rotation_;
};

