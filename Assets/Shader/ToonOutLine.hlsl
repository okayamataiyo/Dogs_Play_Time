//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer gmodel : register(b0) //�I�u�W�F�N�g�Ɋ֌W������
{
    float4x4 matWVP;
    float4x4 matW;
    float4x4 matNormal;
    float4 diffuseColor; //�}�e���A���̐F���g�U���ˌW��
    float4 ambientColor; //����
    float4 specularColor; //���ʔ��ˁ��n�C���C�g
    float shineness;
    bool isTextured; //�e�N�X�`���[���\���Ă��邩�ǂ���
}

cbuffer gmodel : register(b1) //�I�u�W�F�N�g�Ɋ֌W�Ȃ����
{
    float4 lightPosition;
    float4 eyePosition;
}

//struct PS_IN
//{
//	float4 pos	  : SV_POSITION;	//�ʒu
//	float2 uv	  : TEXCOORD;		//UV���W
//	float4 eyev   : POSITION1;
//	float4 normal : POSITION2;
//	float4 light  : POSITION3;
//};

//������������������������������������������������������������������������������
 // �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos      : SV_POSITION; //�ʒu
    float2 uv       : TEXCOORD0; //UV���W
    float4 color    : COLOR1; //�F�i���邳�j
    float4 eyev     : TEXCOORD1; //���_�x�N�g��
    float4 normal   : TEXCOORD2;
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
float4 VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL) : SV_POSITION
{
	//�s�N�Z���V�F�[�_�[�֓n�����
	//VS_OUT outData = (VS_OUT)0;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
    normal.w = 0;
    pos = pos + normal * 0.03f;
    pos = mul(pos, matWVP);

    return pos;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
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

	//���̕ӂŊg�U���˂̒l�����ɂ傲�ɂ傷��
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