#pragma once
#include "Windows.h"
#include "d3d12.h"
#include "dxgi1_6.h"
#include "cassert"
#include "vector"
#include "string"
#include "DirectXMath.h"
#include "assert.h"
#include "DirectXTex.h"
#include "wrl.h"
#include "Model.h"
#include "DirectXCommon.h"

using namespace DirectX;
using namespace Microsoft::WRL;
class Object3D
{
public:
	//シングルトンインスタンス
	Object3D* GetInstance();
	Object3D();
	~Object3D();
	void Initialize(ID3D12Device* device,Model* model);
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);
	void Draw(ID3D12GraphicsCommandList* cmdList,D3D12_VERTEX_BUFFER_VIEW& vbView,D3D12_INDEX_BUFFER_VIEW& ibView);
	void Delete();
	//ゲッター　セッター　
	XMFLOAT3 GetPosition() { return position; };
	XMFLOAT3 GetRotation() { return rotation; };
	XMFLOAT3 GetScale() { return scale; };
	void setPosition(XMFLOAT3 pos);
	void setRotation(XMFLOAT3 rot);
	void setScale(XMFLOAT3 sca);
public:
	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		XMMATRIX mat;	// ３Ｄ変換行列
	};
	// 定数バッファ用データ構造体B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;	//アンビエント係数
		float pad1;			//パディング
		XMFLOAT3 diffuse;	//ディフューズ係数
		float pad2;			//パディング
		XMFLOAT3 specular;	//スペキュラー係数
		float alpha;		//アルファ
	};
	//定数バッファ用データ構造体（3D変換行列）
	/*struct ConstBufferDataTransform
	{
		XMMATRIX mat;
	};*/
private:
	//定数バッファマップ
	/*ConstBufferDataTransform* constMapTransform;*/
	//モデル
	Model* model = nullptr;
	//デバイス
	/*DirectXCommon* dx = nullptr;*/
	ID3D12Device* device;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuffB0; 
	ComPtr<ID3D12Resource> constBuffB1;
	// コマンドリスト
	ID3D12GraphicsCommandList* cmdList;
private:
	//アフィン変換情報
	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotation = { 0,0,0};
	XMFLOAT3 position = { 0,0,0 };
	//ワールド変換行列
	XMMATRIX matWorld;
};