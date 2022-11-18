#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete dxInput;
	/*delete starDust_.get();
	delete starDustModel_.get();
	delete playerModel_.get();
	delete player_.get();
	delete camera_.get();*/
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	this->dxCommon_ = dxCommon;
	this->input_ = input;

	//プレイヤーのモデル初期化
	Model* newModel = new Model();
	newModel->Initialize(dxCommon_->GetDevice(), "fighter", "Resources/fighter.png");
	playerModel_.reset(newModel);

	//敵のモデル初期化
	Model* newEnemyModel = new Model();
	newEnemyModel->Initialize(dxCommon_->GetDevice(), "enemy", "Resources/enemy.png");
	enemyModel_.reset(newEnemyModel);

	//弾のモデルの初期化
	Model* newBulletModel = new Model();
	newBulletModel->Initialize(dxCommon_->GetDevice(), "bullet", "Resources/bullet.png");
	bulletModel_.reset(newBulletModel);


	//敵の弾のモデルの初期化
	Model* newEnemyBulletModel = new Model();
	newEnemyBulletModel->Initialize(dxCommon_->GetDevice(), "bullet", "Resources/enemyBullet.png");
	enemyBulletModel_.reset(newEnemyBulletModel);

	//星屑のモデルの初期化
	Model* newStarModel = new Model();
	newStarModel->Initialize(dxCommon_->GetDevice(), "star", "Resources/star.png");
	starDustModel_.reset(newStarModel);

	//プレイヤー初期化
	Player* newPlayer = new Player();
	newPlayer->Initialize(dxCommon_->GetDevice(), playerModel_.get(),dxInput);
	player_.reset(newPlayer);

	//プレイヤーの煙初期化
	Smoke* newSmoke = new Smoke();
	newSmoke->Initialize(dxCommon_->GetDevice(), starDustModel_.get(), player_->GetPosition());
	smoke_.reset(newSmoke);

	//敵初期化
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(dxCommon_->GetDevice(), enemyModel_.get());
	enemy_.reset(newEnemy);

	//星屑初期化
	StarDust* newStarDust = new StarDust();
	newStarDust->Initialize(dxCommon_->GetDevice(), starDustModel_.get());
	starDust_.reset(newStarDust);

	//カメラ初期化
	Camera* newCamera = new Camera();
	newCamera->Initialize(input_,dxInput, player_.get());
	camera_.reset(newCamera);
}

void GameScene::Update()
{
	//コントローラー更新
	dxInput->InputProcess();

	//カメラ更新
	camera_->Update();
	XMMATRIX matView = camera_->GetMatView();
	XMMATRIX matProjection = camera_->GetMatProjection();

	//デバッグ用視点
	/*XMFLOAT3 eye_(0,5,-30);
	XMFLOAT3 target_(0,0,0);
	XMFLOAT3 up_(0,1,0);
	XMMATRIX matView = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));*/

	//当たり判定更新
	EnmeyCollition();
	PlayerCollition();

	//プレイヤー更新
	player_->Update(matView, matProjection);

	//プレイヤーの煙更新
	smoke_->Update(matView, matProjection,player_->GetPosition(), player_->GetRotation());

	//星屑更新
	starDust_->Update(matView, matProjection);

	//敵更新
	enemy_->Update(matView, matProjection);
	

	//プレイヤーから受け取ったフラグで弾生成
	if (player_->Attack() == 1)
	{
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(dxCommon_->GetDevice(), bulletModel_.get(), player_->GetPosition(), player_->GetVelocity());
		playerBullet_.push_back(std::move(newBullet));
	}
	//プレイヤーの弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullet_)
	{
		bullet->Update(matView, matProjection);
	}
	//デスフラグの立った弾を削除
	playerBullet_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{return bullet->IsDead(); }
	);

	//敵から受け取ったフラグで弾生成
	if (enemy_->Attack() == 1)
	{
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(dxCommon_->GetDevice(), enemyBulletModel_.get(), enemy_->GetPosition(), player_->GetVelocity());
		enemyBullet_.push_back(std::move(newBullet));
	}
	//敵の弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullet_)
	{
		bullet->Update(matView, matProjection);
	}
	//デスフラグの立った弾を削除
	enemyBullet_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{return bullet->IsDead(); }
	);

	//エフェクト更新
	for (std::unique_ptr<BreakEffect>& effect : breakEffect_)
	{
		effect->Update(matView, matProjection);
	}
	//デスフラグの立ったエフェクトを削除
	breakEffect_.remove_if([](std::unique_ptr<BreakEffect>& effect)
		{return effect->IsDead(); }
	);
}

void GameScene::Draw()
{
	starDust_->Draw(dxCommon_->GetCommandList());
	player_->Draw(dxCommon_->GetCommandList());
	smoke_->Draw(dxCommon_->GetCommandList());
	enemy_->Draw(dxCommon_->GetCommandList());
	//プレイヤーの弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullet_)
	{
		bullet->Draw(dxCommon_->GetCommandList());
	}
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullet_)
	{
		bullet->Draw(dxCommon_->GetCommandList());
	}
	//爆発エフェクト描画
	for (std::unique_ptr<BreakEffect>& effect : breakEffect_)
	{
		effect->Draw(dxCommon_->GetCommandList());
	}
}

void GameScene::EnmeyCollition()
{
	//敵に弾が当たったら弾消滅 爆発エフェクト作成
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullet_)
	{
		float x = bullet->GetPosition().x - enemy_->GetPosition().x;
		float y = bullet->GetPosition().y - enemy_->GetPosition().y;
		float z = bullet->GetPosition().z - enemy_->GetPosition().z;
		if ((x * x) + (y * y) + (z * z) <= 100)
		{
			std::unique_ptr<BreakEffect> newEffect = std::make_unique<BreakEffect>();
			newEffect->Initialize(dxCommon_->GetDevice(), starDustModel_.get(), bullet->GetPosition());
			breakEffect_.push_back(std::move(newEffect));

			bullet->SetIsDeadTrue();
		}
	}
}

void GameScene::PlayerCollition()
{
	//敵に弾が当たったら弾消滅 爆発エフェクト作成
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullet_)
	{
		float x = bullet->GetPosition().x - player_->GetPosition().x;
		float y = bullet->GetPosition().y - player_->GetPosition().y;
		float z = bullet->GetPosition().z - player_->GetPosition().z;
		if ((x * x) + (y * y) + (z * z) <= 25)
		{
			std::unique_ptr<BreakEffect> newEffect = std::make_unique<BreakEffect>();
			newEffect->Initialize(dxCommon_->GetDevice(), starDustModel_.get(), bullet->GetPosition());
			breakEffect_.push_back(std::move(newEffect));

			bullet->SetIsDeadTrue();
		}
	}
}