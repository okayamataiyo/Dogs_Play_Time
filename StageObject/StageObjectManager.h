#pragma once
//インクルード
#include "StageObjectBase.h"
#include <vector>

enum class STAGEOBJECTSTATE
{
    SKY = 0,
    STAGE,
    STAGEBLOCK,
};

class StageObjectManager
{
private:
    GameObject* pParent_;
    STAGEOBJECTSTATE stageObjectState_;
    StageObjectBase* pStageObjectBase_;
public:
    StageObjectManager(GameObject* _parent);
    ~StageObjectManager();
    void CreateStageObjectOrigin(STAGEOBJECTSTATE _stageObjectState);
    void CreateStageObject(STAGEOBJECTSTATE _stageObjectState, XMFLOAT3 _pos = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 _rotate = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 _scale = XMFLOAT3(1.0f, 1.0f, 1.0f));
    void CreateStageObject(STAGEOBJECTSTATE _stageObjectState, float _minX, float _maxX, float _minZ, float _maxZ);
    void SetRotate(XMFLOAT3 _rotate);
    void SetPosition(XMFLOAT3 _position);
    StageObjectBase* GetStageObjectBase() { return pStageObjectBase_; }
};

