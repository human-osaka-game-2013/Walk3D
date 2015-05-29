float4x4 matWorld[4] : WORLD;   // ���[���h�ϊ��s��z��
float4x4 matView : VIEW;        // �r���[�ϊ��s��
float4x4 matProj : PROJECTION;  // �ˉe�ϊ��s��
float4x4 matPos : POSITION;
int iBlendNum;                  // �u�����h����z��̐�
float4x4 matLightView;				// ���C�g�r���[�ϊ��s��
float4x4 matLightProj;				// �ˉe�ϊ��s��


texture diffuseTexture;
sampler diffuseSampler = sampler_state
{
	texture = (diffuseTexture);
};
texture texShadowMap;
sampler DefSampler = sampler_state	// �T���v���[�X�e�[�g
{
    texture = (texShadowMap);
    AddressU  = CLAMP;
    AddressV  = CLAMP;
    AddressW = CLAMP;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

struct VS_OUTPUT
{
   float4 Pos : POSITION;  			 // �ˉe�ϊ����W
   float4 ZCalcTex : TEXCOORD0;   	// Z�l�Z�o�p�e�N�X�`��
   float4 Col : COLOR0;				// �f�B�t���[�Y�F
   float2 temp : TEXCOORD1;
};
// �Z���t�V���h�E�A�j���[�V�����V�F�[�_
VS_OUTPUT BasicTrans_VS( float4 Pos : POSITION , float3 Norm : NORMAL)
{
  
VS_OUTPUT Out = (VS_OUTPUT)0;

   // ���ʂɃJ�����̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
   float4x4 mat;
   mat  = mul( matPos, matView );
   mat  = mul( mat, matProj );
   Out.Pos = mul( Pos, mat );
   
   // ���C�g�̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
   mat  = mul( matPos, matLightView );
   mat  = mul( mat, matLightProj );
   Out.ZCalcTex = mul( Pos, mat );
   
   // �@���ƃ��C�g�̕������璸�_�̐F������
   //  �Z���Ȃ肷���Ȃ��悤�ɒ��߂��Ă��܂�
   float3 N = normalize( mul(Norm, matPos) );
   float3 LightDirect = normalize( float3(matLightView._13,matLightView._23,matLightView._33) );
   Out.Col = float4(1.0,0.0,1.0,1.0);// * (0.3 + dot(N, -LightDirect)*(1-0.3f));

   return Out;
}


float4 PS( float4 Col : COLOR, float4 ZCalcTex : TEXCOORD0 ) : COLOR
{
// ���C�g�ڐ��ɂ��Z�l�̍ĎZ�o
   float ZValue = ZCalcTex.z / ZCalcTex.w;
   
   // �e�N�X�`�����W�ɕϊ�
   float2 TransTexCoord;
   TransTexCoord.x = (1.0f + ZCalcTex.x/ZCalcTex.w)*0.5f;
   TransTexCoord.y = (1.0f - ZCalcTex.y/ZCalcTex.w)*0.5f;
   
   // �������W��Z�l�𒊏o
   float SM_Z = tex2D( DefSampler, TransTexCoord ).z;
   
   // �Z�o�_���V���h�E�}�b�v��Z�l�����傫����Ήe�Ɣ��f
   if( ZValue > SM_Z+0.005f ){
     Col.rgb = Col.rgb * 0.5f; 
    }
   
   return Col;
}

// �Z���t�V���h�E�V�F�[�_ not anim
VS_OUTPUT Shadow_VS( float4 Pos : POSITION , float3 Norm : NORMAL)
{
     VS_OUTPUT Out = (VS_OUTPUT)0;

   // ���ʂɃJ�����̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
   float4x4 mat;
   mat  = mul( matPos, matView );
   mat  = mul( mat, matProj );
   Out.Pos = mul( Pos, mat );
   
   // ���C�g�̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
   mat  = mul( matPos, matLightView );
   mat  = mul( mat, matLightProj );
   Out.ZCalcTex = mul( Pos, mat );
   
   // �@���ƃ��C�g�̕������璸�_�̐F������
   //  �Z���Ȃ肷���Ȃ��悤�ɒ��߂��Ă��܂�
  // float3 N = normalize( mul(Norm, matPos) );
   Out.Col = float4(0.0,0.6,1.0,1.0);// * (0.3 + dot(N, -LightDirect)*(1-0.3f));
   Out.temp.x = Norm.x;
   Out.temp.y = Norm.y;
   return Out;
}

float4 ShadowPS( float4 Col : COLOR, float4 ZCalcTex : TEXCOORD0, float2 temp : TEXCOORD1 ) : COLOR
{
   // ���C�g�ڐ��ɂ��Z�l�̍ĎZ�o
   float ZValue = ZCalcTex.z / ZCalcTex.w;
   //ZValue = temp.x/temp.y;
   // �e�N�X�`�����W�ɕϊ�
   float2 TransTexCoord;
   TransTexCoord.x = (1.0f + ZCalcTex.x/ZCalcTex.w)*0.5f;
   TransTexCoord.y = (1.0f - ZCalcTex.y/ZCalcTex.w)*0.5f;
   
   // �������W��Z�l�𒊏o
   float SM_Z = tex2D( DefSampler, TransTexCoord ).z;
   
   // �Z�o�_���V���h�E�}�b�v��Z�l�����傫����Ήe�Ɣ��f
   if( ZValue > SM_Z+0.005f ){
     Col.rgb = Col.rgb * 0.1f; 
    }
   
   return Col;
}

// �e�p�e�N�X�`�������V�F�[�_�p�\����
struct VS_OutTexCreate
{
  float4 Pos : POSITION;   // �ˉe�ϊ����W
   float4 ShadowMapTex : TEXCOORD0;   // Z�o�b�t�@�e�N�X�`��
};

// �e�����V�F�[�_
VS_OutTexCreate ZTexCreate_VS( float4 Pos : POSITION )
{
   VS_OutTexCreate Out = (VS_OutTexCreate)0;

   // ���ʂɃ��[���h�r���[�ˉe�s�������
   float4x4 mat;
   mat = mul( matPos, matView );
   mat = mul( mat, matProj );
   Out.Pos = mul( Pos, mat );
   
   // �e�N�X�`�����W�𒸓_�ɍ��킹��
   Out.ShadowMapTex = Out.Pos;

   return Out;
}
// �e�����p�s�N�Z���V�F�[�_
float4 ZTexCreate_PS( float4 ShadowMapTex : TEXCOORD0 ) : COLOR
{
   // Z�l�Z�o
   return ShadowMapTex.z / ShadowMapTex.w;
}

technique BasicTec {
	pass p0 {
		VertexShader = compile vs_2_0 BasicTrans_VS();
		PixelShader = compile ps_2_0 PS();
	}
pass p01 {
		VertexShader = compile vs_2_0 Shadow_VS();
		PixelShader = compile ps_2_0 ShadowPS();
	}
pass p2 {
		VertexShader = compile vs_2_0 ZTexCreate_VS();
		PixelShader = compile ps_2_0 ZTexCreate_PS();
	}
}