#pragma once
//インクルード
#include "Engine/GameObject.h"
#include "Engine/VFX.h"

namespace
{
	std::string particleManagerName = "ParticleManager";
}


class ParticleManager : public GameObject
{
private:
	EmitterData collectBone_;
public:
	ParticleManager(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void CreateVFX(XMFLOAT3 _pos);
	void SetMode(int _mode);
};

