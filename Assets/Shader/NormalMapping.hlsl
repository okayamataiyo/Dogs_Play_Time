//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer gmodel : register(b0) //オブジェクトに関係あるやつ
{
    float4x4 matWVP;
    float4x4 matW;
    float4x4 matNormal;
    float4 diffuseColor; //マテリアルの色＝拡散反射係数
    float4 ambientColor; //環境光
    float4 specularColor; //鏡面反射＝ハイライト
    float shineness; //ハイライトの広がりの大きさ
    bool isTexture; //テクスチャーが貼られているかどうか
    bool isNormalMap; //ノーマルマップがあるかどうか
}

cbuffer gmodel : register(b1) //オブジェクトに関係ないやつ
{
    float4 lightPosition;
    float4 eyePosition;
}

//struct PS_IN
//{
//	float4 pos	  : SV_POSITION;	//位置
//	float2 uv	  : TEXCOORD;		//UV座標
//	float4 eyev   : POSITION1;
//	float4 normal : POSITION2;
//	float4 light  : POSITION3;
//};

//───────────────────────────────────────
 // テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー
Texture2D normalTex : register(t1);

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD; //UV座標
    float4 eyev : POSITION; //ワールド座標に変換された視線ベクトル
    float4 Neyev : POSITION1; //ノーマルマップ用の接空間
    float4 normal : POSITION2; //法線ベクトル
    float4 light : POSITION3; //ライトを接空間に変換したベクトル
    float4 color : POSITION4; //色（明るさ）
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT)
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData = (VS_OUT) 0;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    outData.pos = mul(pos, matWVP);
    outData.uv = uv;

    float3 binormal = cross(normal, tangent);
    binormal = mul(binormal, matNormal);
    binormal = normalize(binormal); //従法線ベクトルをローカル座標に変換したやつ

    outData.normal = normalize(mul(normal, matNormal)); //法線ベクトルをローカル座標に変換したやつ
    outData.normal.w = 0;

    tangent = mul(tangent, matNormal);
    tangent = normalize(tangent); //接線ベクトルをローカル座標に変換したやつ
    tangent.w = 0;

    float4 posw = mul(pos, matW);
    outData.eyev = normalize(posw - eyePosition);

    float4 eye = normalize(mul(pos, matW) - eyePosition); //ワールド座標の視線ベクトル
    outData.eyev = eye;
    outData.Neyev.x = dot(eye, tangent); //接空間の視線ベクトル
    outData.Neyev.y = dot(eye, binormal);
    outData.Neyev.z = dot(eye, outData.normal);
    outData.Neyev.w = 0;

    float4 light = normalize(lightPosition);
    light.w = 0;
    light = normalize(light);

    outData.color = mul(light, outData.normal);
    outData.color.w = 0.0;

	//outData.light.w = 0;
    outData.light.x = dot(light, tangent); //接空間の光源ベクトル
    outData.light.y = dot(light, binormal);
    outData.light.z = dot(light, outData.normal);
    outData.light.w = 0;

	//まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
	//return inData.Neyev;
    float4 lightSource = float4(1.0, 1.0, 1.0, 1.0);
    float4 diffuse; //光を合わせた面の色
    float4 ambient; //環境光

    if (isNormalMap)
    {
        float4 tmpNormal = normalTex.Sample(g_sampler, inData.uv) * 2 - 1;
        tmpNormal = normalize(tmpNormal);
        tmpNormal.w = 0;
        float4 NL = clamp(dot(tmpNormal, inData.light), 0, 1); //ノーマルランバーティアン
        float4 SF = dot(tmpNormal, normalize(inData.light)); //シャドウファクター
        SF = clamp(SF, 0, 1);
		
        float4 reflection = reflect(-inData.light, tmpNormal);
        float4 specular = pow(saturate(dot(reflection, inData.Neyev)), shineness) * specularColor;
        if (isTexture != 0)
        {
            diffuse = g_texture.Sample(g_sampler, inData.uv) * NL;
            ambient = g_texture.Sample(g_sampler, inData.uv) * ambientColor;
        }
        else
        {
            diffuse = diffuseColor * NL;
            ambient = diffuseColor * ambientColor;
        }
		//diffuse = diffuseColor * NL;

        return diffuse + ambient + specular;
		//return  diffuse;
    }
    else
    {
        float4 NL = saturate(dot(inData.normal, normalize(lightPosition)));
        float4 reflection = reflect(normalize(-lightPosition), inData.normal);
        float4 specular = pow(saturate(dot(reflection, normalize(inData.eyev))), shineness) * specularColor;

        if (isTexture == 0)
        {
            diffuse = lightSource * diffuseColor * inData.color;
            ambient = lightSource * diffuseColor * ambientColor;
        }
        else
        {
            diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * inData.color;
            ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
        }
        float4 result = diffuse + ambient + specular;
		//if (isTexture)
		//	result.a = inData.uv.x;
        return result;
    }
}