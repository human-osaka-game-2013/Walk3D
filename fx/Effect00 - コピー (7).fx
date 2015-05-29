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
	texture = <diffuseTexture>;
};
texture texShadowMap;
sampler DefSampler = sampler_state	// �T���v���[�X�e�[�g
{
    texture = <texShadowMap>;
    AddressU  = CLAMP;
    AddressV  = CLAMP;
    AddressW = CLAMP;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

struct VS_OUTPUT
{
	float4 Pos : POSITION;
	float2 Texcord: TEXCOORD0;
	float4 ZCalcTex: TEXCOORD1;
	float temp : TEXCOORD2;
	float4 Col : COLOR0;
};
// �Z���t�V���h�E�A�j���[�V�����V�F�[�_
VS_OUTPUT BasicTrans_VS( float4 Pos : POSITION, float4 W : BLENDWEIGHT, float2 inTexCoord : TEXCOORD0, float3 Norm : NORMAL )
{
   VS_OUTPUT Out = (VS_OUTPUT)0;
   Out.Texcord = inTexCoord;

   float   Weight[4] = (float[4])W;      // �d�݂�float�ɕ������܂�
   float    LastBlendWeight = 0.0f;        // �Ō�̍s��Ɋ|������d��
   float4x4 matCombWorld = 0.0f;           // �������[���h�ϊ��s��

   // ���[���h�ϊ��s����u�����h
   for(int i=0; i<iBlendNum-1; i++)
   {
      LastBlendWeight += Weight[i];   // �Ō�̏d�݂������Ōv�Z���Ă���
      matCombWorld += matWorld[i] * Weight[i];
   }

   // �Ō�̏d�݂𑫂��Z
   matCombWorld += matWorld[iBlendNum-1] * (1.0f-LastBlendWeight);

   float4x4 mat_light;

   Out.Pos = mul( Pos, matCombWorld );   // ���[���h�ϊ�
   //mat_light = Out.Pos;
   Out.Pos = mul( Out.Pos, matView );    // �r���[�ϊ�
   Out.Pos = mul( Out.Pos, matProj );    // �ˉe�ϊ�
   
   //mat_light = mul( mat_light, matLightView );    // �r���[�ϊ�
   //mat_light = mul( mat_light, matLightProj );    // �ˉe�ϊ�
   mat_light  = mul( matPos, matLightView );
   mat_light  = mul( mat_light, matLightProj );
   Out.ZCalcTex = mul( Pos, mat_light );
   Out.ZCalcTex = mat_light;
   
   float3 N = normalize( mul(Norm, matPos) );
   Out.Col.r = inTexCoord.x;
   Out.Col.g = inTexCoord.y;
   Out.Col.b = N.x;
   Out.Col.a = N.y;
   Out.temp = N.z;
   return Out;
}


float4 PS( float4 Col : COLOR, float2 inTex : TEXCOORD0, float4 ZCalcTex : TEXCOORD1,float temp : TEXCOORD2) : COLOR0
{
	float4 Color;
	Color=tex2D(diffuseSampler,float2(Col.r,Col.g));
	float3 N = float3(Col.b,Col.a,temp);
	// ���C�g�ڐ��ɂ��Z�l�̍ĎZ�o
	float ZValue = Col.z / Col.w;
	float2 TransTexCoord;
	TransTexCoord.x = (1.0f + ZCalcTex.x/ZCalcTex.w)*0.5f;
	TransTexCoord.y = (1.0f - ZCalcTex.y/ZCalcTex.w)*0.5f;
	
	Color=tex2D(diffuseSampler,inTex);
	float SM_Z = tex2D( DefSampler, TransTexCoord ).x;
   
	// �Z�o�_���V���h�E�}�b�v��Z�l�����傫����Ήe�Ɣ��f
	if( ZValue > SM_Z+0.025f ){
	Color.rgb = Color.rgb * 0.5f; 
	}
	return Color;
}

// �Z���t�V���h�E�V�F�[�_ not anim
VS_OUTPUT Shadow_VS( float4 Pos : POSITION, float2 inTexCoord : TEXCOORD0, float3 Norm : NORMAL )
{
   VS_OUTPUT Out = (VS_OUTPUT)0;
   Out.Texcord = inTexCoord;
   float4 mat_light;

   Out.Pos = mul( Pos,     matPos  );
   Out.Pos = mul( Out.Pos, matView );    // �r���[�ϊ�
   Out.Pos = mul( Out.Pos, matProj );    // �ˉe�ϊ�
   
   mat_light = mul( Pos, matLightView );    // �r���[�ϊ�
   mat_light = mul( mat_light, matLightProj );    // �ˉe�ϊ�
   
   Out.ZCalcTex = mat_light;
   
   float3 N = normalize( mul(Norm, matPos) );
   Out.Col.r = inTexCoord.x;
   Out.Col.g = inTexCoord.y;
   Out.Col.b = N.x;
   Out.Col.a = N.y;
   Out.temp = N.z;
   return Out;
}

float4 ShadowPS( float4 Col : COLOR, float2 inTex : TEXCOORD0, float4 ZCalcTex : TEXCOORD1,float temp : TEXCOORD2) : COLOR0
{
	float4 Color;
	Color=tex2D(diffuseSampler,float2(Col.r,Col.g));
	float3 N = float3(Col.b,Col.a,temp);
	// ���C�g�ڐ��ɂ��Z�l�̍ĎZ�o
	float ZValue = Col.z / Col.w;
	float2 TransTexCoord;
	TransTexCoord.x = (1.0f + ZCalcTex.x/ZCalcTex.w)*0.5f;
	TransTexCoord.y = (1.0f - ZCalcTex.y/ZCalcTex.w)*0.5f;
	
	Color=tex2D(diffuseSampler,inTex);
	float SM_Z = tex2D( DefSampler, TransTexCoord ).x;
   
	// �Z�o�_���V���h�E�}�b�v��Z�l�����傫����Ήe�Ɣ��f
	if( ZValue > SM_Z ){
	Color.rgb = Color.rgb * 0.1f; 
	}
	return Color;
}

// �e�p�e�N�X�`�������V�F�[�_�p�\����
struct VS_OutTexCreate
{
  float4 Pos : POSITION;   // �ˉe�ϊ����W
   float4 ShadowMapTex : TEXCOORD0;   // Z�o�b�t�@�e�N�X�`��
};

// �e�����V�F�[�_
VS_OutTexCreate ZTexCreate_VS( float4 Pos : POSITION, float4 W : BLENDWEIGHT )
{
   VS_OutTexCreate Out = (VS_OutTexCreate)0;

   float   Weight[4] = (float[4])W;      // �d�݂�float�ɕ������܂�
   float    LastBlendWeight = 0.0f;        // �Ō�̍s��Ɋ|������d��
   float4x4 matCombWorld = 0.0f;           // �������[���h�ϊ��s��

   // ���[���h�ϊ��s����u�����h
   for(int i=0; i<iBlendNum-1; i++)
   {
      LastBlendWeight += Weight[i];   // �Ō�̏d�݂������Ōv�Z���Ă���
      matCombWorld += matWorld[i] * Weight[i];
   }

   // �Ō�̏d�݂𑫂��Z
   matCombWorld += matWorld[iBlendNum-1] * (1.0f-LastBlendWeight);

   Out.Pos = mul( Pos, matCombWorld );   // ���[���h�ϊ�
   Out.Pos = mul( Out.Pos, matView );    // �r���[�ϊ�
   Out.Pos = mul( Out.Pos, matProj );    // �ˉe�ϊ�

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