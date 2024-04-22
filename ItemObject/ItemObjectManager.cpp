//インクルード
#include <random>
#include "../Engine/Global.h"
#include "ItemObjectManager.h"
#include "Floor.h"
#include "WoodBox.h"
#include "FrameBox.h"
#include "Bone.h"

ItemObjectManager::ItemObjectManager(GameObject* _pParent) :woodBoxs_{}, floors_{}, number_{ 0 }, objectState_{ ITEMOBJECTSTATE::FLOOR }
, pParent_{ _pParent }, pObjectBase_{ nullptr }
{
}

ItemObjectManager::~ItemObjectManager()
{
}


void ItemObjectManager::CreateObject(ITEMOBJECTSTATE _objectState,XMFLOAT3 _pos, XMFLOAT3 _rotate, XMFLOAT3 _scale)
{
	std::string woodBoxNumber = woodBoxName + std::to_string(number_);
	switch (_objectState)
	{
	case ITEMOBJECTSTATE::FLOOR:
		pObjectBase_ = Instantiate<Floor>(pParent_);
		floors_.push_back(pObjectBase_->GetModelHandle());
		break;
	case ITEMOBJECTSTATE::WOODBOX:
		pObjectBase_ = Instantiate<WoodBox>(pParent_);
		woodBoxs_.push_back(pObjectBase_->GetModelHandle());
		pObjectBase_->SetObjectName(woodBoxNumber);
		number_++;
		break;
	case ITEMOBJECTSTATE::FRAMEBOX:
		pObjectBase_ = Instantiate<FrameBox>(pParent_);
		break;
	case ITEMOBJECTSTATE::BONE:
		pObjectBase_ = Instantiate<Bone>(pParent_);
		break;
	}
	pObjectBase_->SetPosition(_pos);
	pObjectBase_->SetRotate(_rotate);
	pObjectBase_->SetScale(_scale);
}

void ItemObjectManager::CreateObject(ITEMOBJECTSTATE _objectState, float _minX, float _maxX, float _minZ, float _maxZ)
{
	switch (_objectState)
	{
	case ITEMOBJECTSTATE::BONE:
		float minX = _minX;
		float maxX = _maxX;
		float minY = initZeroFloat;
		float maxY = initZeroFloat;
		float minZ = _minZ;
		float maxZ = _maxZ;
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> distX(minX, maxX);
		std::uniform_real_distribution<float> distY(minY, maxY);
		std::uniform_real_distribution<float> distZ(minZ, maxZ);
		XMFLOAT3 randomPos = { distX(gen), distY(gen), distZ(gen) };
		pObjectBase_ = Instantiate<Bone>(pParent_);
		pObjectBase_->SetPosition(randomPos);
		break;
	}
}

void ItemObjectManager::SetRotate(XMFLOAT3 _rotate)
{
	pObjectBase_->SetRotate(_rotate);
}

void ItemObjectManager::SetPosition(XMFLOAT3 _position)
{
	pObjectBase_->SetPosition(_position);
}