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
    float shineness; //�n�C���C�g�̍L����̑傫��
    bool isTexture; //�e�N�X�`���[���\���Ă��邩�ǂ���
    bool isNormalMap; //�m�[�}���}�b�v�����邩�ǂ���
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
Texture2D normalTex : register(t1);

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos : SV_POSITION; //�ʒu
    float2 uv : TEXCOORD; //UV���W
    float4 eyev : POSITION; //���[���h���W�ɕϊ����ꂽ�����x�N�g��
    float4 Neyev : POSITION1; //�m�[�}���}�b�v�p�̐ڋ��
    float4 normal : POSITION2; //�@���x�N�g��
    float4 light : POSITION3; //���C�g��ڋ�Ԃɕϊ������x�N�g��
    float4 color : POSITION4; //�F�i���邳�j
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
    VS_OUT outData = (VS_OUT) 0;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
    outData.pos = mul(pos, matWVP);
    outData.uv = uv;

    float3 binormal = cross(normal, tangent);
    binormal = mul(binormal, matNormal);
    binormal = normalize(binormal); //�]�@���x�N�g�������[�J�����W�ɕϊ��������

    outData.normal = normalize(mul(normal, matNormal)); //�@���x�N�g�������[�J�����W�ɕϊ��������
    outData.normal.w = 0;

    tangent = mul(tangent, matNormal);
    tangent = normalize(tangent); //�ڐ��x�N�g�������[�J�����W�ɕϊ��������
    tangent.w = 0;

    float4 posw = mul(pos, matW);
    outData.eyev = normalize(posw - eyePosition);

    float4 eye = normalize(mul(pos, matW) - eyePosition); //���[���h���W�̎����x�N�g��
    outData.eyev = eye;
    outData.Neyev.x = dot(eye, tangent); //�ڋ�Ԃ̎����x�N�g��
    outData.Neyev.y = dot(eye, binormal);
    outData.Neyev.z = dot(eye, outData.normal);
    outData.Neyev.w = 0;

    float4 light = normalize(lightPosition);
    light.w = 0;
    light = normalize(light);

    outData.color = mul(light, outData.normal);
    outData.color.w = 0.0;

	//outData.light.w = 0;
    outData.light.x = dot(light, tangent); //�ڋ�Ԃ̌����x�N�g��
    outData.light.y = dot(light, binormal);
    outData.light.z = dot(light, outData.normal);
    outData.light.w = 0;

	//�܂Ƃ߂ďo��
    return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
	//return inData.Neyev;
    float4 lightSource = float4(1.0, 1.0, 1.0, 1.0);
    float4 diffuse; //�������킹���ʂ̐F
    float4 ambient; //����

    if (isNormalMap)
    {
        float4 tmpNormal = normalTex.Sample(g_sampler, inData.uv) * 2 - 1;
        tmpNormal = normalize(tmpNormal);
        tmpNormal.w = 0;
        float4 NL = clamp(dot(tmpNormal, inData.light), 0, 1); //�m�[�}�������o�[�e�B�A��
        float4 SF = dot(tmpNormal, normalize(inData.light)); //�V���h�E�t�@�N�^�[
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