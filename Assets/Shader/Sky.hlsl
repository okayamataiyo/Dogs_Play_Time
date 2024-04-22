//───────────────────────────────────────
 // テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

//───────────────────────────────────────
 // コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
    float4x4 g_matWVP; // ワールド・ビュー・プロジェクションの合成行列
    float4x4 g_matNormalTrans; // 法線の変換行列（回転行列と拡大の逆行列）
    float4x4 g_matWorld; // ワールド変換行列
    float4 g_vecLightDir; // ライトの方向ベクトル
    float4 g_vecDiffuse; // ディフューズカラー（マテリアルの色）
    float4 g_vecAmbient; // アンビエントカラー（影の色）
    float4 g_vecSpeculer; // スペキュラーカラー（ハイライトの色）
    float4 g_vecCameraPosition; // 視点（カメラの位置）
    float g_shuniness; // ハイライトの強さ（テカリ具合）
    bool g_isTexture; // テクスチャ貼ってあるかどうか

};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos : SV_POSITION; //位置
    float2 uv  : TEXCOORD0; //UV座標
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 Normal : NORMAL, float2 Uv : TEXCOORD)
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData = (VS_OUT)0;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    outData.pos = mul(pos, g_matWVP);
    outData.uv = Uv; //そのままピクセルシェーダーへ

	//まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    if (g_isTexture == true)
    {
        return g_texture.Sample(g_sampler, inData.uv);
    }
    else
    {
        return g_vecDiffuse;
    }
}