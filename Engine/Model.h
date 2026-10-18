#pragma once
#include <DirectXMath.h>
#include <assert.h>
#include <vector>
#include <memory>
#include <string>
#include "RayCastData.h"
#include "Transform.h"

//FBXファイルの実体。FBX SDKのヘッダーをここで読み込まないよう、前方宣言だけにしている
class Fbx;

//-----------------------------------------------------------
//3Dモデル（FBXファイル）を管理する
//-----------------------------------------------------------
namespace Model
{

	//モデル情報
	struct ModelData
	{
		//ファイル名
		std::string fileName;

		//ロードしたモデルデータ（同じファイルを読んだモデルどうしで共有し、最後の1つが消えたときにFbxも消える）
		std::shared_ptr<Fbx>	pFbx;

		//行列
		Transform 	transform;

		//アニメーションのフレーム
		float nowFrame, animSpeed;
		int startFrame, endFrame;

		//初期化
		ModelData() : pFbx(nullptr), nowFrame(0), startFrame(0), endFrame(0), animSpeed(0)
		{
		}

		//アニメーションのフレーム数をセット
		//引数：startFrame	開始フレーム
		//引数：endFrame	終了フレーム
		//引数：animSpeed	アニメーション速度
		void SetAnimFrame(int start, int end, float speed)
		{
			nowFrame = (float)start;
			startFrame = start;
			endFrame = end;
			animSpeed = speed;
		}
	};

	//初期化
	void Initialize();

	//モデルをロード
	//引数：fileName　ファイル名
	//引数：isRequired	必須のモデルならtrue（読み込めなかったら致命的なエラーとして記録し、ゲームを終了する）
	//					falseなら警告として記録するだけで続行する
	//戻値：そのモデルデータに割り当てられた番号（読み込めなかったときは-1）
	int Load(std::string fileName, bool isRequired = true);

	//描画
	//引数：handle	描画したいモデルの番号
	//引数：matrix	ワールド行列
	void Draw(int handle);

	//任意のモデルを開放
	//引数：handle	開放したいモデルの番号
	void Release(int handle);

	//全てのモデルを解放
	//（シーンが切り替わるときは必ず実行）
	void AllRelease();

	//アニメーションのフレーム数をセット
	//引数：handle		設定したいモデルの番号
	//引数：startFrame	開始フレーム
	//引数：endFrame	終了フレーム
	//引数：animSpeed	アニメーション速度
	void SetAnimFrame(int handle, int startFrame, int endFrame, float animSpeed);

	void SetOnceAnimFrame(int _handle, int _startFrame, int _endFrame, float _animSpeed,int _num);

	//現在のアニメーションのフレームを取得
	int GetAnimFrame(int handle);

	//任意のボーンの位置を取得
	//引数：handle		調べたいモデルの番号
	//引数：boneName	調べたいボーンの名前
	//戻値：ボーンの位置（ワールド座標）
	XMFLOAT3 GetBonePosition(int handle, std::string boneName);

	//ワールド行列を設定
	//引数：handle	設定したいモデルの番号
	//引数：matrix	ワールド行列
	void SetTransform(int handle, Transform& transform);

	//ワールド行列の取得
	//引数：handle	知りたいモデルの番号
	//戻値：ワールド行列
	XMMATRIX GetMatrix(int handle);


	//レイキャスト（レイを飛ばして当たり判定）　※未実装
	//引数：handle	判定したいモデルの番号
	//引数：data	必要なものをまとめたデータ
	void RayCast(int handle, RayCastData *data);

};