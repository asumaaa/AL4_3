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

	// FbxManager�쐬
	FbxManager* fbx_manager = FbxManager::Create();

	// FbxImporter�쐬
	FbxImporter* fbx_importer = FbxImporter::Create(fbx_manager, "ImportTest");
	if (fbx_importer == nullptr)
	{
		fbx_manager->Destroy();
	}

	// FbxScene�쐬
	FbxScene* fbx_scene = FbxScene::Create(fbx_manager, "SceneTest");
	if (fbx_scene == nullptr)
	{
		fbx_importer->Destroy();
		fbx_manager->Destroy();
	}

	// �t�@�C��������������
	if (fbx_importer->Initialize("Box.fbx") == false)
	{
		fbx_importer->Destroy();
		fbx_scene->Destroy();
		fbx_manager->Destroy();
	}

	// �C���|�[�g
	/*if (fbx_importer->Import(fbx_scene) == false)
	{
		fbx_importer->Destroy();
		fbx_scene->Destroy();
		fbx_manager->Destroy();
	}*/
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