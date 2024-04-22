//������������������������������������������������������������������������������
 // �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[

//������������������������������������������������������������������������������
 // �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
{
    float4x4 g_matWVP; // ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
    float4x4 g_matNormalTrans; // �@���̕ϊ��s��i��]�s��Ɗg��̋t�s��j
    float4x4 g_matWorld; // ���[���h�ϊ��s��
    float4 g_vecLightDir; // ���C�g�̕����x�N�g��
    float4 g_vecDiffuse; // �f�B�t���[�Y�J���[�i�}�e���A���̐F�j
    float4 g_vecAmbient; // �A���r�G���g�J���[�i�e�̐F�j
    float4 g_vecSpeculer; // �X�y�L�����[�J���[�i�n�C���C�g�̐F�j
    float4 g_vecCameraPosition; // ���_�i�J�����̈ʒu�j
    float g_shuniness; // �n�C���C�g�̋����i�e�J����j
    bool g_isTexture; // �e�N�X�`���\���Ă��邩�ǂ���

};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos : SV_POSITION; //�ʒu
    float2 uv  : TEXCOORD0; //UV���W
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 Normal : NORMAL, float2 Uv : TEXCOORD)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
    VS_OUT outData = (VS_OUT)0;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
    outData.pos = mul(pos, g_matWVP);
    outData.uv = Uv; //���̂܂܃s�N�Z���V�F�[�_�[��

	//�܂Ƃ߂ďo��
    return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
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