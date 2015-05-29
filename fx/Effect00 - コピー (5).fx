float4x4 matWorldViewProj;   // ���[���h�r���[�ˉe�ϊ��s��
float4x4 matWorld[4] : WORLD;   // ���[���h�ϊ��s��z��
float4x4 matView : VIEW;        // �r���[�ϊ��s��
float4x4 matProj : PROJECTION;  // �ˉe�ϊ��s��
float4x4 matPos;
int iBlendNum;                  // �u�����h����z��̐�
float4x4 matLightView;				// ���C�g�r���[�ϊ��s��
float4x4 matLightProj;				// �ˉe�ϊ��s��
// ����
float Period;

// �U��
float Amplitude;
// �g�̈ړ�
float Offset;

texture diffuseTexture;
sampler diffuseSampler = sampler_state {
	texture = <diffuseTexture>;
};
texture texShadowMap;				// �V���h�E�}�b�v�e�N�X�`��
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


struct VS_OUTPUT_S
{
  float4 Pos : POSITION;   // �ˉe�ϊ����W
  //float4 ShadowMapTex : TEXCOORD1;   // Z�o�b�t�@�e�N�X�`��
  float2 ZCalcTex : TEXCOORD0;      // �\��t����e�N�X�`��
float temp : TEXCOORD2;
  float4 Col : COLOR0;				// �f�B�t���[�Y�F
};

// ���_�V�F�[�_
VS_OUTPUT_S ZBufferCalc_VS(float4 Pos : POSITION, float4 W : BLENDWEIGHT, float3 Norm : NORMAL, float2 inTexCoord : TEXCOORD0 )
{
   VS_OUTPUT_S Out = (VS_OUTPUT_S)0;

//----------------------------------------------------------------
//  	���ɂ߁[�����
//
//-----------------------------------------
   float   Weight[4] = (float[4])W;      // �d�݂�float�ɕ������܂�
   float    LastBlendWeight = 0.0f;        // �Ō�̍s��Ɋ|������d��
   float4x4 matCombWorld = 0.0f;           // �������[���h�ϊ��s��
   //int num = iBlendNum - 1;
   // ���[���h�ϊ��s����u�����h
//if( num > 4 ) num = 4;
   for(int i=0; i < iBlendNum-1;i++)
   {
      LastBlendWeight += Weight[i];   // �Ō�̏d�݂������Ōv�Z���Ă���
      matCombWorld += matWorld[i] * Weight[i];
   }

   // �Ō�̏d�݂𑫂��Z
   matCombWorld += matWorld[iBlendNum-1] * (1.0f-LastBlendWeight);

 //-----------------------------------------

   // ���ʂɃJ�����̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
   float4x4 mat;
   mat  = mul( matPos, matView );
   mat  = mul( mat, matCombWorld );
   mat  = mul( mat, matProj );
   Out.Pos = mul( Pos, mat );
   // ���C�g�̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
   mat  = mul( matPos, matLightView );
   mat  = mul( mat, matCombWorld );
   mat  = mul( mat, matLightProj );
   Out.ZCalcTex = mul( Pos, mat );
   
   // �@���ƃ��C�g�̕������璸�_�̐F������
   //  �Z���Ȃ肷���Ȃ��悤�ɒ��߂��Ă��܂�
   float3 N = normalize( mul(Norm, matPos) );
   float3 LightDirect = normalize( float3(matLightView._13,matLightView._23,matLightView._33) );
//   ���C�g�̂������Ŗ��邳�ύX
   Out.Col = float4(0.0,0.6,1.0,1.0) * (0.3 + dot(N, -LightDirect)*(1-0.3f));
   Out.Col.r = inTexCoord.x;
   Out.Col.g = inTexCoord.y;
   Out.Col.b = N.x;
   Out.Col.a = N.y;
   Out.temp = N.z;
   Out.Col = float4(LastBlendWeight,LastBlendWeight,LastBlendWeight,LastBlendWeight);
   return Out;
}


// �s�N�Z���V�F�[�_
float4 ZBufferPlot_PS( float4 Col : COLOR, float4 ZCalcTex : TEXCOORD0, float2 texCoord : TEXCOORD1, float temp : TEXCOORD2 ) : COLOR
{
  float4 Color;
  float3 N = float3(Col.b,Col.a,temp);
  float3 LightDirect = normalize( float3(matLightView._13,matLightView._23,matLightView._33) );
  Color=tex2D(diffuseSampler,float2(Col.r,Col.g));
//Color = Col;
  // ���C�g�ڐ��ɂ��Z�l�̍ĎZ�o
   float ZValue = Col.z / Col.w;
 // �e�N�X�`�����W�ɕϊ�
   float2 TransTexCoord;
   TransTexCoord.x = (1.0f + Col.x/Col.w)*0.5f;
   TransTexCoord.y = (1.0f - Col.y/Col.w)*0.5f;
   
   // �������W��Z�l�𒊏o
   float SM_Z = tex2D( DefSampler, TransTexCoord ).x;
   
// ���C�g�̌������烂�f���̖��邳�𒲐�
Color = Color;// * (0.3 + dot(N, -LightDirect)*(1-0.3f));
Color.a = 1.f;
   // �Z�o�_���V���h�E�}�b�v��Z�l�����傫����Ήe�Ɣ��f
   if( ZValue > SM_Z+0.005f ){
     Color.rgb = Color.rgb * 0.5f; 
    }

   return Color;
}

struct OUTPUT_VS
{
	float4 P_after : POSITION;
	float2 texCoord : TEXCOORD0;
};

OUTPUT_VS BasicTrans_VS( float4 Pos : POSITION, float4 W : BLENDWEIGHT, float2 inTexCoord : TEXCOORD0 )
{
   OUTPUT_VS outVS = (OUTPUT_VS)0;
   outVS.texCoord = inTexCoord;
   
  // outVS.P_after = Pos;
//return outVS;

   float   Weight[4] = (float[4])W;      // �d�݂�float�ɕ������܂�
   float    LastBlendWeight = 0.0f;        // �Ō�̍s��Ɋ|������d��
   float4x4 matCombWorld = 0.0f;           // �������[���h�ϊ��s��

   // ���[���h�ϊ��s����u�����h
//   for(int i=0; i<iBlendNum-1; i++)
 //  {
 //     LastBlendWeight += Weight[i];   // �Ō�̏d�݂������Ōv�Z���Ă���
 //     matCombWorld += matWorld[i] * Weight[i];
 //  }

   // �Ō�̏d�݂𑫂��Z
 //  matCombWorld += matWorld[iBlendNum-1] * (1.0f-LastBlendWeight);


 // outVS.P_after = mul( Pos, matCombWorld );   // ���[���h�ϊ�
   outVS.P_after = mul( Pos, matWorldViewProj);   // ���[���h�ϊ�
   outVS.P_after = mul( outVS.P_after, matPos );
 //
   outVS.P_after = mul( outVS.P_after, matView );    // �r���[�ϊ�
   outVS.P_after = mul( outVS.P_after, matProj );    // �ˉe�ϊ�

   return outVS;
}

// �s�N�Z���V�F�[�_
float4 NoWorkingPixelShader( float2 texCoord : TEXCOORD0 ) : COLOR
{
float4 Color;
float2 xy;
float ax;
float ay;
    //float4 output;
  //  float2 uv;
   // uv = texCoord;

	//output = float4(1.f,1.f,0.f,1.f);
   // output = tex2D( diffuseSampler, uv );
   // output *= float4( 1.0f,1.0f,1.0f,1.0f);
   // output /= 1.0;
   // return output;


  ax=texCoord.x+sin((texCoord.y+ Offset )  *  Period  ) * Amplitude;
  ay=texCoord.y;
  Color=tex2D(diffuseSampler,texCoord/*float2(ax,ay)*/);
  return Color;
 //return float4( 1.0f,1.0f,0.0f,1.0f);
}

struct VS_OutTexCreate
{
  float4 Pos : POSITION;   // �ˉe�ϊ����W
   float4 ShadowMapTex : TEXCOORD0;   // Z�o�b�t�@�e�N�X�`��
};

// ���_�V�F�[�_
VS_OutTexCreate ZTexCreate_VS( float4 Pos : POSITION, float4 W : BLENDWEIGHT )
{
   VS_OutTexCreate Out = (VS_OutTexCreate)0;

  // float   Weight[4] = (float[4])W;      // �d�݂�float�ɕ������܂�
  // float    LastBlendWeight = 0.0f;        // �Ō�̍s��Ɋ|������d��
  // float4x4 matCombWorld = 0.0f;           // �������[���h�ϊ��s��

   // ���[���h�ϊ��s����u�����h
 //  for(int i=0; i<iBlendNum-1; i++)
//   {
   //   LastBlendWeight += Weight[i];   // �Ō�̏d�݂������Ōv�Z���Ă���
     // matCombWorld += matWorld[i] * Weight[i];
 //  }

   // �Ō�̏d�݂𑫂��Z
  // matCombWorld += matWorld[iBlendNum-1] * (1.0f-LastBlendWeight);
   Out.Pos = mul( Pos, matPos);   // ���[���h�ϊ�
   //Out.Pos = mul( Out.Pos, matCombWorld );   // ���[���h�ϊ�
   Out.Pos = mul( Out.Pos, matView );    // �r���[�ϊ�
   Out.Pos = mul( Out.Pos, matProj );    // �ˉe�ϊ�

   Out.ShadowMapTex = Out.Pos;

   return Out;
}


// �s�N�Z���V�F�[�_
float4 ZTexCreate_PS( float4 ShadowMapTex : TEXCOORD0 ) : COLOR
{
   // Z�l�Z�o
//return float4(1.f,0.f,1.f,1.f);
   return ShadowMapTex.z / ShadowMapTex.w;
//return float4(1.f,0.f,1.f,1.f);
}



technique BasicTec
{
   pass P0
   {
      VertexShader = compile vs_2_0 BasicTrans_VS();
      PixelShader = compile ps_2_0 NoWorkingPixelShader();
   }
   pass P1
   {
      VertexShader = compile vs_2_0 ZBufferCalc_VS();
      PixelShader = compile ps_2_0 ZBufferPlot_PS();
   }
   pass P2
   {
      VertexShader = compile vs_2_0 ZTexCreate_VS();
      PixelShader = compile ps_2_0 ZTexCreate_PS();
   }
}
