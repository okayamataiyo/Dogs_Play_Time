#pragma once
//インクルード
#include <vector>
#include "ItemObjectBase.h"

namespace
{
    std::string itemObjectManagerName = "ItemObjectManager";
}

enum class ITEMOBJECTSTATE
{
    FLOOR = 0,
    WOODBOX,
    FRAMEBOX,
    BONE
};

class ItemObjectManager
{
private:
    std::vector<int> woodBoxs_;
    std::vector<int> floors_;
    int number_;
    ITEMOBJECTSTATE objectState_;
    GameObject* pParent_;
    ItemObjectBase* pObjectBase_;
public:
    ItemObjectManager(GameObject* _pParent);
    ~ItemObjectManager();
    void CreateObject(ITEMOBJECTSTATE _objectState,XMFLOAT3 _pos = XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT3 _rotate = XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT3 _scale = XMFLOAT3(1.0f,1.0f,1.0f));
    void CreateObject(ITEMOBJECTSTATE _objectState, float _minX, float _maxX, float _minZ, float _maxZ);
    void SetRotate(XMFLOAT3 _rotate);
    void SetPosition(XMFLOAT3 _position);
    ItemObjectBase* GetObjectBase() { return pObjectBase_; }
    std::vector<int> GetWoodBoxs() { return woodBoxs_; }
    std::vector<int> GetFloors() { return floors_; }
};

