#include "FbxParts.h"
#include "Fbx.h"
#include "Global.h"
#include "Direct3D.h"
#include "Camera.h"
#include "Debug.h"

//コンストラクタ
FbxParts::FbxParts():
	vertexCount_(0), polygonCount_(0), indexCount_(0), materialCount_(0), polygonVertexCount_(0),
	pSkinInfo_(nullptr), numBone_(0)
{
}

//FBXファイルから情報をロードして諸々準備する
HRESULT FbxParts::Init(FbxNode *pNode)
{
	//ノードからメッシュの情報を取得
	FbxMesh* mesh = pNode->GetMesh();
	mesh->SplitPoints(FbxLayerElement::eTextureDiffuse);

	//各情報の個数を取得
	vertexCount_ = mesh->GetControlPointsCount();			//頂点の数
	polygonCount_ = mesh->GetPolygonCount();				//ポリゴンの数
	polygonVertexCount_ = mesh->GetPolygonVertexCount();	//ポリゴン頂点インデックス数 


	HRESULT hr = InitVertex(mesh);		//頂点バッファ準備
	if (FAILED(hr))	return hr;

	InitMaterial(pNode);	//マテリアル準備

	hr = InitIndex(mesh);		//インデックスバッファ準備
	if (FAILED(hr))	return hr;

	InitSkelton(mesh);		//骨の情報を準備

	hr = IntConstantBuffer();	//コンスタントバッファ（シェーダーに情報を送るやつ）準備
	if (FAILED(hr))	return hr;

	return S_OK;
}


//頂点バッファ準備
HRESULT FbxParts::InitVertex(fbxsdk::FbxMesh * mesh)
{
	vertexData_.resize(vertexCount_);

	for (DWORD poly = 0; poly < polygonCount_; poly++)
	{
		//3頂点分
		for (int vertex = 0; vertex < 3; vertex++)
		{
			int index = mesh->GetPolygonVertex(poly, vertex);

			/////////////////////////頂点の位置/////////////////////////////////////
			FbxVector4 pos = mesh->GetControlPointAt(index);
			vertexData_[index].position = XMFLOAT3((float)pos[0], (float)pos[1], (float)pos[2]);

			/////////////////////////頂点の法線/////////////////////////////////////
			FbxVector4 Normal;
			mesh->GetPolygonVertexNormal(poly, vertex, Normal);	//ｉ番目のポリゴンの、ｊ番目の頂点の法線をゲット
			vertexData_[index].normal = XMFLOAT3((float)Normal[0], (float)Normal[1], (float)Normal[2]);

			///////////////////////////頂点のＵＶ/////////////////////////////////////
			FbxLayerElementUV * pUV = mesh->GetLayer(0)->GetUVs();
			int uvIndex = mesh->GetTextureUVIndex(poly, vertex, FbxLayerElement::eTextureDiffuse);
			FbxVector2  uv = pUV->GetDirectArray().GetAt(uvIndex);
			vertexData_[index].uv = XMFLOAT3((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f);
		}
	}


	///////////////////////////頂点のＵＶ/////////////////////////////////////
	int m_dwNumUV = mesh->GetTextureUVCount();
	FbxLayerElementUV * pUV = mesh->GetLayer(0)->GetUVs();
	if (m_dwNumUV > 0 && pUV->GetMappingMode() == FbxLayerElement::eByControlPoint)
	{
		for (int k = 0; k < m_dwNumUV; k++)
		{
			FbxVector2 uv = pUV->GetDirectArray().GetAt(k);
			vertexData_[k].uv = XMFLOAT3((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f);
		}
	}


	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * mesh->GetControlPointsCount();
	bd_vertex.Usage = D3D11_USAGE_DYNAMIC;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertexData_.data();
	return Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, pVertexBuffer_.ReleaseAndGetAddressOf());


}

//マテリアル準備
void FbxParts::InitMaterial(fbxsdk::FbxNode * pNode)
{

	// マテリアルバッファの生成
	materialCount_ = pNode->GetMaterialCount();
	materials_.resize(materialCount_);



	for (DWORD i = 0; i < materialCount_; i++)
	{
		// フォンシェーディングを想定したマテリアルバッファの抽出
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);
		FbxSurfacePhong* pPhong = (FbxSurfacePhong*)pMaterial;

		// 環境光＆拡散反射光＆鏡面反射光の反射成分値を取得
		FbxDouble3  ambient = FbxDouble3(0, 0, 0);
		FbxDouble3  diffuse = FbxDouble3(0, 0, 0);
		FbxDouble3  specular = FbxDouble3(0, 0, 0);
		ambient = pPhong->Ambient;
		diffuse = pPhong->Diffuse;



		// 環境光＆拡散反射光＆鏡面反射光の反射成分値をマテリアルバッファにコピー
		materials_[i].ambient = XMFLOAT4((float)ambient[0], (float)ambient[1], (float)ambient[2], 1.0f);
		materials_[i].diffuse = XMFLOAT4((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f);
		materials_[i].specular = XMFLOAT4(0, 0, 0, 0);
		materials_[i].shininess = 0;


		if (pMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
		{
			specular = pPhong->Specular;
			materials_[i].specular = XMFLOAT4((float)specular[0], (float)specular[1], (float)specular[2], 1.0f);
			materials_[i].shininess = (float)pPhong->Shininess;
		}


		InitTexture(pMaterial, i);

	}

}

//テクスチャ準備
void FbxParts::InitTexture(fbxsdk::FbxSurfaceMaterial * pMaterial, const DWORD &i)
{
	materials_[i].pTexture = nullptr;


	// テクスチャー情報の取得
	FbxProperty  lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);

	//テクスチャの数
	int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();

	if (fileTextureCount > 0)
	{
		FbxFileTexture* texture = lProperty.GetSrcObject<FbxFileTexture>(0);

		//ファイル名+拡張だけにする
		char name[_MAX_FNAME];	//ファイル名
		char ext[_MAX_EXT];		//拡張子
		_splitpath_s(texture->GetRelativeFileName(), nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
		wsprintf(name, "%s%s", name, ext);



		materials_[i].pTexture = std::make_unique<Texture>();
		if (FAILED(materials_[i].pTexture->Load(name)))
		{
			//読み込めないときはテクスチャ無しとして扱う（マテリアルの色だけで無地に表示される）
			Debug::Warning(std::string("テクスチャを読み込めません。無地で表示します: ") + name + " (" + materials_[i].pTexture->GetErrorMessage() + ")");
			materials_[i].pTexture.reset();
		}


	}
}

//インデックスバッファ準備
HRESULT FbxParts::InitIndex(fbxsdk::FbxMesh * mesh)
{
	// マテリアルの数だけインデックスバッファーを作成
	indexBuffers_.resize(materialCount_);
	indexData_.resize(materialCount_);

	

	int count = 0;

	// マテリアルから「ポリゴン平面」の情報を抽出する
	for (DWORD i = 0; i < materialCount_; i++)
	{
		count = 0;
		std::vector<DWORD> index(polygonCount_ * 3);

		// ポリゴンを構成する三角形平面が、
		// 「頂点バッファ」内のどの頂点を利用しているかを調べる
		for (DWORD j = 0; j < polygonCount_; j++)
		{
			FbxLayerElementMaterial *   mtl = mesh->GetLayer(0)->GetMaterials();
			int mtlId = mtl->GetIndexArray().GetAt(j);
			if (mtlId == i)
			{
				for (DWORD k = 0; k < 3; k++)
				{
					index[count + k] = mesh->GetPolygonVertex(j, k);
				}
				count += 3;
			}
		}

		// インデックスバッファを生成する
		D3D11_BUFFER_DESC   bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * count;
		bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = index.data();
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		//ポリゴンが1つもないマテリアル（使われていないマテリアル）は、インデックスバッファを作れないので作らない
		//（従来からバッファはnullptrのままで、描画しても何も出ない）
		if (count > 0)
		{
			HRESULT hr = Direct3D::pDevice_->CreateBuffer(&bd, &InitData, indexBuffers_[i].ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				return hr;
			}
		}
		materials_[i].polygonCount = count / 3;
		indexData_[i].assign(index.begin(), index.begin() + count);
	}

	return S_OK;
}

//骨の情報を準備
void FbxParts::InitSkelton(FbxMesh * pMesh)
{
	// デフォーマ情報（ボーンとモデルの関連付け）の取得
	FbxDeformer *   pDeformer = pMesh->GetDeformer(0);
	if (pDeformer == nullptr)
	{
		//ボーン情報なし
		return;
	}


	// デフォーマ情報からスキンメッシュ情報を取得
	pSkinInfo_ = (FbxSkin *)pDeformer;

	// 頂点からポリゴンを逆引きするための情報を作成する
	struct  POLY_INDEX
	{
		std::vector<int>	polyIndex;      // ポリゴンの番号
		std::vector<int>	vertexIndex;    // 頂点の番号
		int     numRef;         // 頂点を共有するポリゴンの数
	};

	std::vector<POLY_INDEX> polyTable(vertexCount_);
	for (DWORD i = 0; i < vertexCount_; i++)
	{
		// 三角形ポリゴンに合わせて、頂点とポリゴンの関連情報を構築する
		// 総頂点数＝ポリゴン数×３頂点
		polyTable[i].polyIndex.assign(polygonCount_ * 3, 0);
		polyTable[i].vertexIndex.assign(polygonCount_ * 3, 0);
		polyTable[i].numRef = 0;

		// ポリゴン間で共有する頂点を列挙する
		for (DWORD k = 0; k < polygonCount_; k++)
		{
			for (int m = 0; m < 3; m++)
			{
				if (pMesh->GetPolygonVertex(k, m) == i)
				{
					polyTable[i].polyIndex[polyTable[i].numRef] = k;
					polyTable[i].vertexIndex[polyTable[i].numRef] = m;
					polyTable[i].numRef++;
				}
			}
		}
	}

	// ボーン情報を取得する
	numBone_ = pSkinInfo_->GetClusterCount();
	clusters_.resize(numBone_);
	for (int i = 0; i < numBone_; i++)
	{
		clusters_[i] = pSkinInfo_->GetCluster(i);
	}

	// ボーンの数に合わせてウェイト情報を準備する
	weightArray_.resize(vertexCount_);
	for (DWORD i = 0; i < vertexCount_; i++)
	{
		weightArray_[i].posOrigin = vertexData_[i].position;
		weightArray_[i].normalOrigin = vertexData_[i].normal;
		weightArray_[i].boneIndex.assign(numBone_, -1);
		weightArray_[i].boneWeight.assign(numBone_, 0.0f);
	}




	// それぞれのボーンに影響を受ける頂点を調べる
	// そこから逆に、頂点ベースでボーンインデックス・重みを整頓する
	for (int i = 0; i < numBone_; i++)
	{
		int numIndex = clusters_[i]->GetControlPointIndicesCount();   //このボーンに影響を受ける頂点数
		int * piIndex = clusters_[i]->GetControlPointIndices();       //ボーン/ウェイト情報の番号
		double * pdWeight = clusters_[i]->GetControlPointWeights();     //頂点ごとのウェイト情報

																				 //頂点側からインデックスをたどって、頂点サイドで整理する
		for (int k = 0; k < numIndex; k++)
		{
			// 頂点に関連付けられたウェイト情報がボーン５本以上の場合は、重みの大きい順に４本に絞る
			for (int m = 0; m < 4; m++)
			{
				if (m >= numBone_)
					break;

				if (pdWeight[k] > weightArray_[piIndex[k]].boneWeight[m])
				{
					for (int n = numBone_ - 1; n > m; n--)
					{
						weightArray_[piIndex[k]].boneIndex[n] = weightArray_[piIndex[k]].boneIndex[n - 1];
						weightArray_[piIndex[k]].boneWeight[n] = weightArray_[piIndex[k]].boneWeight[n - 1];
					}
					weightArray_[piIndex[k]].boneIndex[m] = i;
					weightArray_[piIndex[k]].boneWeight[m] = (float)pdWeight[k];
					break;
				}
			}

		}
	}

	//ボーンを生成
	boneArray_.resize(numBone_);
	for (int i = 0; i < numBone_; i++)
	{
		// ボーンのデフォルト位置を取得する
		FbxAMatrix  matrix;
		clusters_[i]->GetTransformLinkMatrix(matrix);

		// 行列コピー（Fbx形式からDirectXへの変換）
		XMFLOAT4X4 pose;
		for (DWORD x = 0; x < 4; x++)
		{
			for (DWORD y = 0; y < 4; y++)
			{
				pose(x,y) = (float)matrix.Get(x, y);
			}
		}
		boneArray_[i].bindPose = XMLoadFloat4x4(&pose);
	}

}

//コンスタントバッファ（シェーダーに情報を送るやつ）準備
HRESULT FbxParts::IntConstantBuffer()
{
	// 定数バッファの作成(パラメータ受け渡し用)
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	return Direct3D::pDevice_->CreateBuffer(&cb, NULL, pConstantBuffer_.ReleaseAndGetAddressOf());
}

//描画
void FbxParts::Draw(Transform& transform)
{
	//今から描画する頂点情報をシェーダに伝える
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext_->IASetVertexBuffers(0, 1, pVertexBuffer_.GetAddressOf(), &stride, &offset);

	//使用するコンスタントバッファをシェーダに伝える
	Direct3D::pContext_->VSSetConstantBuffers(0, 1, pConstantBuffer_.GetAddressOf());
	Direct3D::pContext_->PSSetConstantBuffers(0, 1, pConstantBuffer_.GetAddressOf());


	//シェーダーのコンスタントバッファーに各種データを渡す
	for (DWORD i = 0; i < materialCount_; i++)
	{
		// インデックスバッファーをセット
		UINT    stride = sizeof(int);
		UINT    offset = 0;
		Direct3D::pContext_->IASetIndexBuffer(indexBuffers_[i].Get(), DXGI_FORMAT_R32_UINT, 0);


		// パラメータの受け渡し
		D3D11_MAPPED_SUBRESOURCE pdata;
		CONSTANT_BUFFER cb;
		cb.worldVewProj =	XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());						// リソースへ送る値をセット
		cb.world =		XMMatrixTranspose(transform.GetWorldMatrix());
		cb.normalTrans =	XMMatrixTranspose(transform.matRotate_ * XMMatrixInverse(nullptr, transform.matScale_));
		cb.ambient = materials_[i].ambient;
		cb.diffuse = materials_[i].diffuse;
		cb.speculer = materials_[i].specular;
		cb.shininess = materials_[i].shininess;
		for (int i = 0u; i <= 1; i++)
		{
			cb.cameraPosition = XMFLOAT4(Camera::GetPosition(i).x, Camera::GetPosition(i).y, Camera::GetPosition(i).z, 0);
		}
		cb.lightDirection = XMFLOAT4(1, -1, 1, 0);
		cb.isTexture = materials_[i].pTexture != nullptr;


		Direct3D::pContext_->Map(pConstantBuffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのリソースアクセスを一時止める
		memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));		// リソースへ値を送る



		// テクスチャをシェーダーに設定

		if (cb.isTexture)
		{
			ID3D11SamplerState*			pSampler = materials_[i].pTexture->GetSampler();
			Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);

			ID3D11ShaderResourceView*	pSRV = materials_[i].pTexture->GetSRV();
			Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);
		}
		Direct3D::pContext_->Unmap(pConstantBuffer_.Get(), 0);									// GPUからのリソースアクセスを再開

		 //ポリゴンメッシュを描画する
		Direct3D::pContext_->DrawIndexed(materials_[i].polygonCount * 3, 0, 0);
	}

}

//ボーン有りのモデルを描画
void FbxParts::DrawSkinAnime(Transform& transform, FbxTime time)
{
	// ボーンごとの現在の行列を取得する
	for (int i = 0; i < numBone_; i++)
	{
		FbxAnimEvaluator * evaluator = clusters_[i]->GetLink()->GetScene()->GetAnimationEvaluator();
		FbxMatrix mCurrentOrentation = evaluator->GetNodeGlobalTransform(clusters_[i]->GetLink(), time);

		// 行列コピー（Fbx形式からDirectXへの変換）
		XMFLOAT4X4 pose;
		for (DWORD x = 0; x < 4; x++)
		{
			for (DWORD y = 0; y < 4; y++)
			{
				pose(x, y) = (float)mCurrentOrentation.Get(x, y);
			}
		}

		// オフセット時のポーズの差分を計算する
		boneArray_[i].newPose = XMLoadFloat4x4(&pose);
		boneArray_[i].diffPose = XMMatrixInverse(nullptr, boneArray_[i].bindPose);
		boneArray_[i].diffPose *= boneArray_[i].newPose;
	}

	// 各ボーンに対応した頂点の変形制御
	for (DWORD i = 0; i < vertexCount_; i++)
	{
		// 各頂点ごとに、「影響するボーン×ウェイト値」を反映させた関節行列を作成する
		XMMATRIX  matrix;
		ZeroMemory(&matrix, sizeof(matrix));
		for (int m = 0; m < numBone_; m++)
		{
			if (weightArray_[i].boneIndex[m] < 0)
			{
				break;
			}
			matrix += boneArray_[weightArray_[i].boneIndex[m]].diffPose * weightArray_[i].boneWeight[m];

		}

		// 作成された関節行列を使って、頂点を変形する
		XMVECTOR Pos = XMLoadFloat3(&weightArray_[i].posOrigin);
		XMVECTOR Normal = XMLoadFloat3(&weightArray_[i].normalOrigin);
		XMStoreFloat3(&vertexData_[i].position,XMVector3TransformCoord(Pos, matrix));
		XMFLOAT3X3 mat33;
		XMStoreFloat3x3(&mat33, matrix);
		XMMATRIX matrix33 = XMLoadFloat3x3(&mat33);
		XMStoreFloat3(&vertexData_[i].normal, XMVector3TransformCoord(Normal, matrix));

	}

	// 頂点バッファをロックして、変形させた後の頂点情報で上書きする
	D3D11_MAPPED_SUBRESOURCE msr = {};
	Direct3D::pContext_->Map(pVertexBuffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	if (msr.pData)
	{
		memcpy_s(msr.pData, msr.RowPitch, vertexData_.data(), sizeof(VERTEX) * vertexCount_);
		Direct3D::pContext_->Unmap(pVertexBuffer_.Get(), 0);
	}


	Draw(transform);

}

void FbxParts::DrawMeshAnime(Transform& transform, FbxTime time, FbxScene * scene)
{
	//// その瞬間の自分の姿勢行列を得る
	//FbxAnimEvaluator *evaluator = scene->GetAnimationEvaluator();
	//FbxMatrix mCurrentOrentation = evaluator->GetNodeGlobalTransform(_pNode, time);

	//// Fbx形式の行列からDirectX形式の行列へのコピー（4×4の行列）
	//for (DWORD x = 0; x < 4; x++)
	//{
	//	for (DWORD y = 0; y < 4; y++)
	//	{
	//		_localMatrix(x, y) = (float)mCurrentOrentation.Get(x, y);
	//	}
	//}

	Draw(transform);
}

bool FbxParts::GetBonePosition(std::string boneName, XMFLOAT3 * position)
{
	for (int i = 0; i < numBone_; i++)
	{
		if (boneName == clusters_[i]->GetLink()->GetName())
		{
			FbxAMatrix  matrix;
			clusters_[i]->GetTransformLinkMatrix(matrix);

			position->x = (float)matrix[3][0];
			position->y = (float)matrix[3][1];
			position->z = (float)matrix[3][2];

			return true;
		}

	}

	return false;
}

void FbxParts::RayCast(RayCastData * data)
{
	data->hit = FALSE;

	//マテリアル毎
	for (DWORD i = 0; i < materialCount_; i++)
	{
		//そのマテリアルのポリゴン毎
		for (DWORD j = 0; j < materials_[i].polygonCount; j++)
		{
			//3頂点
			XMFLOAT3 ver[3];
			ver[0] = vertexData_[indexData_[i][j * 3 + 0]].position;
			ver[1] = vertexData_[indexData_[i][j * 3 + 1]].position;
			ver[2] = vertexData_[indexData_[i][j * 3 + 2]].position;

			BOOL  hit = FALSE;
			float dist = 0.0f;

			hit = Direct3D::Intersect(data->start, data->dir, ver[0], ver[1], ver[2], &dist);


			if (hit && dist < data->dist)
			{
				data->hit = TRUE;
				data->dist = dist;
			}
		}
	}
}
