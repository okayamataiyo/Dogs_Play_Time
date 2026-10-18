//ゲーム本体のうち、描画（Direct3D）とFBXモデル（Model）に依存する関数のダミー
//当たり判定とGameObjectのテストを、グラフィックスの初期化もFBX SDKもなしでリンクできるようにする
//テストに必要なものだけを、本物と同じ宣言で用意している
#include "Direct3D.h"
#include "Model.h"

//---------------- Direct3D ----------------
//コリジョンの枠を表示するか
bool Direct3D::isDrawCollision_ = false;

//今描いているビューポート（テストが直接書き換えて、GameObject::DrawSubの振り分けを確かめる）
int Direct3D::viewPortType_ = 0;

int Direct3D::GetViewPort()
{
	return viewPortType_;
}

void Direct3D::SetShader(SHADER_TYPE type)
{
}

//---------------- Model ----------------
//モデルは読み込まない（-1は、読み込めなかったときの番号）
int Model::Load(std::string fileName, bool isRequired)
{
	return -1;
}

void Model::Draw(int handle)
{
}

void Model::SetTransform(int handle, Transform& transform)
{
}
