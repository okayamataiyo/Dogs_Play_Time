#include "../Engine/Image.h"
#include "../Engine/Global.h"
#include "../Engine/Input.h"
#include "ParticleManager.h"


ParticleManager::ParticleManager(GameObject* _pParent)
	:GameObject(_pParent, particleManagerName)
{
}

void ParticleManager::Initialize()
{
	collectBone_.delay = 0;
	collectBone_.number = 100;
	collectBone_.lifeTime = 100;
	collectBone_.positionRnd = XMFLOAT3(0.5f, 0.0f, 0.5f);
	collectBone_.direction = XMFLOAT3(0.0f, 1.0f, 0.0f);
	collectBone_.directionRnd = XMFLOAT3(90.0f, 90.0f, 90.0f);
	collectBone_.speed = 0.25f;
	collectBone_.speedRnd = 1.0f;
	collectBone_.accel = 0.93f;
	collectBone_.size = XMFLOAT2(0.3f, 0.3f);
	collectBone_.sizeRnd = XMFLOAT2(0.4f, 0.4f);
	collectBone_.scale = XMFLOAT2(0.99f, 0.99f);
	collectBone_.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.3f);
	collectBone_.deltaColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	collectBone_.gravity = 0.003f;
}

void ParticleManager::Update()
{

}

void ParticleManager::BothViewDraw()
{
}

void ParticleManager::LeftViewDraw()
{
}

void ParticleManager::RightViewDraw()
{
}

void ParticleManager::UPSubViewDraw()
{
}

void ParticleManager::Release()
{
}

void ParticleManager::CreateVFX(XMFLOAT3 _pos)
{
	collectBone_.position = _pos;
	VFX::Start(collectBone_);
}

void ParticleManager::SetMode(int _mode)
{
}
