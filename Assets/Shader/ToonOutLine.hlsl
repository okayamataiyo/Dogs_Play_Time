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
    float shineness;
    bool isTextured; //テクスチャーが貼られているかどうか
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

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos      : SV_POSITION; //位置
    float2 uv       : TEXCOORD0; //UV座標
    float4 color    : COLOR1; //色（明るさ）
    float4 eyev     : TEXCOORD1; //視点ベクトル
    float4 normal   : TEXCOORD2;
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
float4 VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL) : SV_POSITION
{
	//ピクセルシェーダーへ渡す情報
	//VS_OUT outData = (VS_OUT)0;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    normal.w = 0;
    pos = pos + normal * 0.03f;
    pos = mul(pos, matWVP);

    return pos;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    float4 lightSource = float4(1.0, 1.0, 1.0, 1.0);
    float4 ambientSource = ambientColor;
    float4 diffuse; // = lightsource * g_texture.sample(g_sampler, indata.uv) * indata.color;
    float4 ambient; // = lightsource * g_texture.sample(g_sampler, indata.uv) * ambientsource;
    float4 NL = saturate(dot(inData.normal, normalize(lightPosition)));
	//float4 reflect = normalize(2 * NL * inData.normal - normalize(lightPosition));
    float4 reflection = reflect(normalize(-lightPosition), inData.normal);
    float4 specular = pow(saturate(dot(reflection, normalize(inData.eyev))), shineness) * specularColor;

	//この辺で拡散反射の値をごにょごにょする
    float4 n1 = float4(1 / 4.0, 1 / 4.0, 1 / 4.0, 1);
    float4 n2 = float4(2 / 4.0, 2 / 4.0, 2 / 4.0, 1);
    float4 n3 = float4(3 / 4.0, 3 / 4.0, 3 / 4.0, 1);
    float4 n4 = float4(4 / 4.0, 4 / 4.0, 4 / 4.0, 1);

    float4 tI = 0.1 * step(n1, inData.color) + 0.2 * step(n2, inData.color)
		+ 0.2 * step(n3, inData.color) + 0.3 * step(n4, inData.color);

    if (isTextured == false)
    {
        diffuse = lightSource * diffuseColor * inData.color;
        ambient = lightSource * diffuseColor * ambientSource;
    }
    else
    {
        diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * inData.color;
        ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientSource;
    }
	//return diffuse + ambient + specular;
	//return specular;
    return float4(0, 0, 0, 1);
}