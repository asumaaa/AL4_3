#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	this->dxCommon_ = dxCommon;
	this->input_ = input;

	//�J����������
	Camera* newCamera = new Camera();
	newCamera->Initialize(input_,dxInput, XMFLOAT3(0,0,-10));
	camera_.reset(newCamera);
}

void GameScene::Update()
{
	//�R���g���[���[�X�V
	dxInput->InputProcess();

	//�J�����X�V
	camera_->Update();
	XMMATRIX matView = camera_->GetMatView();
	XMMATRIX matProjection = camera_->GetMatProjection();
}

void GameScene::Draw()
{
}