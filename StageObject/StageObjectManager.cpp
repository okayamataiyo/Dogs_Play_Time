//インクルード
#include <random>
#include "StageObjectManager.h"
#include "Sky.h"
#include "Stage.h"
#include "StageBlock.h"

void StageObjectManager::CreateStageObjectOrigin(STAGEOBJECTSTATE _stageObjectState)
{
	switch (_stageObjectState)
	{
	case STAGEOBJECTSTATE::SKY:
		pStageObjectBase_ = InstantiateFront<Sky>(pParent_);
		break;
	case STAGEOBJECTSTATE::STAGE:
		pStageObjectBase_ = Instantiate<Stage>(pParent_);
		break;
	case STAGEOBJECTSTATE::STAGEBLOCK:
		pStageObjectBase_ = Instantiate<StageBlock>(pParent_);
		break;
	}
}

void StageObjectManager::CreateStageObject(STAGEOBJECTSTATE _stageObjectState, XMFLOAT3 _pos, XMFLOAT3 _rotate, XMFLOAT3 _scale)
{
	switch (_stageObjectState)
	{
	case STAGEOBJECTSTATE::SKY:
		pStageObjectBase_ = InstantiateFront<Sky>(pParent_);
		break;
	case STAGEOBJECTSTATE::STAGE:
		pStageObjectBase_ = Instantiate<Stage>(pParent_);
		break;
	case STAGEOBJECTSTATE::STAGEBLOCK:
		pStageObjectBase_ = Instantiate<StageBlock>(pParent_);
		break;
	}
	pStageObjectBase_->SetPosition(_pos);
	pStageObjectBase_->SetRotate(_rotate);
	pStageObjectBase_->SetScale(_scale);
}

void StageObjectManager::CreateStageObject(STAGEOBJECTSTATE _stageObjectState, float _minX, float _maxX, float _minZ, float _maxZ)
{
	switch (_stageObjectState)
	{
	case STAGEOBJECTSTATE::STAGEBLOCK:
		float minX = _minX;
		float maxX = _maxX;
		float minY = 0;
		float maxY = 0;
		float minZ = _minZ;
		float maxZ = _maxZ;
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> distX(minX, maxX);
		std::uniform_real_distribution<float> distY(minY, maxY);
		std::uniform_real_distribution<float> distZ(minZ, maxZ);
		XMFLOAT3 randomPos = { distX(gen), distY(gen), distZ(gen) };
		pStageObjectBase_ = Instantiate<StageBlock>(pParent_);
		pStageObjectBase_->SetPosition(randomPos);
		break;
	}
}

void StageObjectManager::SetRotate(XMFLOAT3 _rotate)
{
	pStageObjectBase_->SetRotate(_rotate);
}

void StageObjectManager::SetPosition(XMFLOAT3 _position)
{
	pStageObjectBase_->SetPosition(_position);
}

StageObjectManager::StageObjectManager(GameObject* _parent)
	:pParent_(_parent), pStageObjectBase_(nullptr), stageObjectState_(STAGEOBJECTSTATE::SKY)
{
}

StageObjectManager::~StageObjectManager()
{
}
