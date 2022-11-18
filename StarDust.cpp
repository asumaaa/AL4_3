#include "StarDust.h"
#include "stdlib.h"

StarDust* StarDust::GetInstance()
{
	static StarDust instance;
	return &instance;
}

StarDust::StarDust()
{
}

StarDust::~StarDust()
{
}

void StarDust::Initialize(ID3D12Device* device, Model* model)
{
	this->device_ = device;
	this->model_ = model;


	position_.resize(starDustNum);
	scale_.resize(starDustNum);
	rotation_.resize(starDustNum);

	//初期値設定
	for (int i = 0; i < starDustNum; i++)
	{
		position_[i].x = static_cast<float>(rand() % 800 - 400);
		position_[i].y = static_cast<float>(rand() % 800 - 400);
		position_[i].z = static_cast<float>(rand() % 800 - 400);
		scale_[i] = { starDustScale,starDustScale,starDustScale };
		rotation_[i] = { 0.0f,0.0f,0.0f };
	}
	//オブジェクト初期化
	for (int i = 0; i < starDustNum; i++)
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

void StarDust::Update(XMMATRIX& matView, XMMATRIX& matProjection)
{
	for (std::unique_ptr<Object3D>& object : object3Ds_)
	{
		object->Update(matView, matProjection);
	}
}

void StarDust::Draw(ID3D12GraphicsCommandList* cmdList)
{
	for (std::unique_ptr<Object3D>& object : object3Ds_)
	{
		object->Draw(cmdList,model_->vbView, model_->ibView);
	}
}

void StarDust::Delete()
{
	/*for (std::unique_ptr<Object3D>& object : object3Ds_)
	{
		object->Delete();
	}*/
	/*auto itr = object3Ds_.begin();*/
	/*object3Ds_.clear();
	while (itr != object3Ds_.end())
	{
		object3Ds_.erase(itr);
		itr++;
	}*/
	/*for (std::unique_ptr<Object3D>& object : object3Ds_)
	{
		object.reset();
	}*/

	/*auto i = object3Ds_.begin();
	auto e = object3Ds_.end();
	for (int a = 0; a < 10; a++)
	{
		object3Ds_.erase(i);
		i++;
	}*/
}
