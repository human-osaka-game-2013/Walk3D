#pragma warning(disable:4786)
#include "..\LibGame\LibGame.h"
#include "..\LibUtil\LibUtil.h"
#include "Billboard.h"

CBillboard* CBillboardGroup::NewBillboard(int anim_max)
{
	CBillboard* image=new CBillboard(this,Device,anim_max);
	m_billboard.push_back(image);
	return image;
}

CBillboard::CBillboard(CBillboardGroup *group,LPDIRECT3DDEVICE9 Device,int anim_max)
:	Group(group),m_anim_max(anim_max)
{
	// テクスチャの作成
	Texture=new LPDIRECT3DTEXTURE9;
	//Texture=NULL;
	Group->Device->CreateTexture(
		1024,1024, 1, 0, 
			D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
			Texture, NULL);
	m_anim_count=0;
	m_anim_max=anim_max;
	m_anim_frame=1;
	m_run_flag=false;
	m_frame_count = 0;
	//D3DXMatrixIdentity(&m_rot);
}

CBillboard::~CBillboard() {
	if (Texture) 
	{
		(*Texture)->Release();
	}
	delete Texture;
}
//テクスチャのロード
bool CBillboard::loadTexture(string file_name)
{
	LPDIRECT3DDEVICE9 device=Group->Device;

	/*if(SUCCEEDED(D3DXCreateTextureFromFile(device,file_name.c_str(),Texture)))
	{
		return false;
	}*/
	if(SUCCEEDED(D3DXCreateTextureFromFile(device,file_name.c_str(),Texture)))
	{
		return false;
	}
	return true;
}

CBillboardGroup::CBillboardGroup(
	LPDIRECT3DDEVICE9 device
)
:	Device(device), VertexBuffer(NULL)
{
	// 頂点バッファの作成
	Device->CreateVertexBuffer(
		4*sizeof(/*IMAGE2D_VERTEX*/BILLBOARD_VERTEX), 0, D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1/*D3DFVF_IMAGE2D_VERTEX*/, 
		D3DPOOL_MANAGED, &VertexBuffer, NULL);
}

CBillboardGroup::~CBillboardGroup() {
	for (int i=0, n=m_billboard.size(); i<n; i++) delete m_billboard[i];
	if (VertexBuffer) VertexBuffer->Release();
}

//ベクトルの長さ
float GetVectorLength( D3DXVECTOR2 _v ) {
	return pow( ( _v.x * _v.x ) + ( _v.y * _v.y ), 0.5f );
}

//ベクトル内積
float DotProduct(D3DXVECTOR2 _vl, D3DXVECTOR2 _vr) {
	return _vl.x * _vr.x + _vl.y * _vr.y;
}
D3DXMATRIX YRotation( D3DXMATRIX _m , float _rad)
{
	//回転行列を元に
	// cos 0 sin	0 x
	//  0  1  0	0 y
	//-sin 0 cos	0 z
	//  0  0  0	1 w

	//行列作る
	D3DXMATRIX _c;
	D3DXMatrixIdentity( &_c );

	_c._11 = cos( _rad );
	_c._13 = sin( _rad );
	_c._22 = 1;
	_c._31 = -sin( _rad );
	_c._33 = cos( _rad );
	_c._44 = 1;
		
	//回転行列
	return _c * _m;
}

//ビルボードの描画
void CBillboard::Draw(float xpos,float ypos,float zpos,float size,D3DXVECTOR3 _Cz,bool additivesynthesis)
{
	if(m_run_flag)
	{
		LPDIRECT3DDEVICE9 device=Group->Device;
		LPDIRECT3DVERTEXBUFFER9 vbuffer=Group->VertexBuffer;

		if(additivesynthesis)
		{
			device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
			device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
		}

		// 頂点バッファをデバイスに設定
		device->SetStreamSource(0, vbuffer, 0, sizeof(/*IMAGE2D_VERTEX*/BILLBOARD_VERTEX));
		device->SetFVF(/*D3DFVF_IMAGE2D_VERTEX*/D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

		if(Texture==NULL)
		{
			int a = 0;
		}

		// 頂点バッファのロック
		BILLBOARD_VERTEX* v;
		//IMAGE2D_VERTEX* v;
		if (SUCCEEDED(vbuffer->Lock(0, 4*sizeof(/*IMAGE2D_VERTEX*/BILLBOARD_VERTEX), (void**)&v, 0)))
		{
			// 頂点座標の設定
			v[0].X=-size;v[0].Y=-size;
			v[1].X=+size;v[1].Y=-size;
			v[2].X=+size;v[2].Y=+size;
			v[3].X=-size;v[3].Y=+size;

			//UV
			v[0].TU=0.25f*(m_anim_count%4)	;v[0].TV=0.25f*(m_anim_count/4+1);
			v[1].TU=0.25f*(m_anim_count%4+1);v[1].TV=0.25f*(m_anim_count/4+1);
			v[2].TU=0.25f*(m_anim_count%4+1);v[2].TV=0.25f*(m_anim_count/4);
			v[3].TU=0.25f*(m_anim_count%4)	;v[3].TV=0.25f*(m_anim_count/4);
			// Z, RHW, 頂点色の設定
			for (int i=0; i<4; i++) {
				v[i].Z=0;
				//v[i].RHW=0; 
				v[i].Color=D3DCOLOR_ARGB(255,255,255,255);
			}

			//D3DXMatrixIdentity(&m_rot);
			//D3DXMatrixLookAtLH(&m_rot, camPos,&D3DXVECTOR3(xpos,ypos,zpos), &D3DXVECTOR3(0,1,0));
			//D3DXMatrixInverse(&m_rot, NULL,&m_rot);
			//m_rot._41 = 0.0f;   // オフセットを切る（回転行列だけにしてしまう）
			//m_rot._42 = 0.0f;
			//m_rot._43 = 0.0f;

			//D3DXMATRIX mat;
			//D3DXMATRIX World;
			//D3DXMatrixTranslation(&mat,xpos,ypos,zpos);
			//D3DXMatrixIdentity( &World );
	  //      D3DXMatrixMultiply( &World, &World, &m_rot );		// まずターゲット方向に軸回転させて
	  //      D3DXMatrixMultiply( &World, &World, &mat );			// 次にオフセットさせます。

			//回転角度（ラジアン）
			D3DXVECTOR2 A( 0.f , 1.f );
			D3DXVECTOR2 B( _Cz.x , _Cz.z );
			float A_Length = GetVectorLength( A );
			float B_Length = GetVectorLength( B );
			float cos_sita = DotProduct( A , B ) / ( A_Length * B_Length ) ;
			float m_RotAngle = acos( cos_sita );
			if( _Cz.x < 0 ){
				m_RotAngle = -acos( cos_sita );
			}

			D3DXMATRIX m_rot;
			D3DXMatrixIdentity( &m_rot );
			m_rot = YRotation( m_rot , -m_RotAngle );
	//*/
			D3DXMATRIX mat;
			D3DXMATRIX World;
			D3DXMatrixIdentity(&mat);
			D3DXMatrixTranslation(&mat,xpos,ypos,zpos);

			D3DXMatrixIdentity( &World );
			D3DXMatrixMultiply( &World, &World, &m_rot );		// まずターゲット方向に軸回転させて
			D3DXMatrixMultiply( &World, &World, &mat );			// 次にオフセットさせます。

			device->SetTransform(D3DTS_WORLD,&World);

			// 頂点バッファの解放，テクスチャの選択，描画
			vbuffer->Unlock();
			device->SetTexture(0,*Texture);
			device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
		}
		if(additivesynthesis)
		{
			device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}
//ビルボードの制御
void CBillboard::Control()
{
	if(m_run_flag)
	{
		m_frame_count++;
		if(m_anim_frame<m_frame_count)
		{
			if(m_anim_count<m_anim_max)
			{
				m_anim_count++;
			}
			else if(m_anim_count==m_anim_max)
			{
				m_run_flag=false;
			}
			m_frame_count=0;
		}
	}
}
//アニメーションカウントのリセット
void CBillboard::ResetAnimCount()
{
	m_run_flag=false;
	m_anim_count=0;
	m_frame_count=0;
}
//アニメーションカウントの終了判定
bool CBillboard::IsAnimCountEnd()
{
	if(m_anim_count>=m_anim_max)
	{
		return true;
	}
	return false;
}
//アニメーション間隔の設定
void CBillboard::setAnimFrame(int frame)
{
	m_anim_frame=frame;
}

void CBillboard::GroundDraw(float xpos,float zpos,float size,bool additivesynthesis)
{
	if(m_run_flag)
	{
		LPDIRECT3DDEVICE9 device=Group->Device;
		LPDIRECT3DVERTEXBUFFER9 vbuffer=Group->VertexBuffer;

		if(additivesynthesis)
		{
			device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
			device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
		}

		// 頂点バッファをデバイスに設定
		device->SetStreamSource(0, vbuffer, 0, sizeof(/*IMAGE2D_VERTEX*/BILLBOARD_VERTEX));
		device->SetFVF(/*D3DFVF_IMAGE2D_VERTEX*/D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

		if(Texture==NULL)
		{
			int a = 0;
		}

		// 頂点バッファのロック
		BILLBOARD_VERTEX* v;
		//IMAGE2D_VERTEX* v;
		if (SUCCEEDED(vbuffer->Lock(0, 4*sizeof(/*IMAGE2D_VERTEX*/BILLBOARD_VERTEX), (void**)&v, 0)))
		{
			// 頂点座標の設定
			v[0].X=+size;v[0].Z=-size;
			v[1].X=-size;v[1].Z=-size;
			v[2].X=-size;v[2].Z=+size;
			v[3].X=+size;v[3].Z=+size;

			//UV
			v[0].TU=0.25f*(m_anim_count%4)	;v[0].TV=0.25f*(m_anim_count/4+1);
			v[1].TU=0.25f*(m_anim_count%4+1);v[1].TV=0.25f*(m_anim_count/4+1);
			v[2].TU=0.25f*(m_anim_count%4+1);v[2].TV=0.25f*(m_anim_count/4);
			v[3].TU=0.25f*(m_anim_count%4)	;v[3].TV=0.25f*(m_anim_count/4);
			// Z, RHW, 頂点色の設定
			for (int i=0; i<4; i++) {
				v[i].Y=0;
				//v[i].RHW=0; 
				v[i].Color=D3DCOLOR_ARGB(255,255,255,255);
			}

			//D3DXMatrixIdentity(&m_rot);
			//D3DXMatrixLookAtLH(&m_rot, camPos,&D3DXVECTOR3(xpos,ypos,zpos), &D3DXVECTOR3(0,1,0));
			//D3DXMatrixInverse(&m_rot, NULL,&m_rot);
			//m_rot._41 = 0.0f;   // オフセットを切る（回転行列だけにしてしまう）
			//m_rot._42 = 0.0f;
			//m_rot._43 = 0.0f;

			//D3DXMATRIX mat;
			//D3DXMATRIX World;
			//D3DXMatrixTranslation(&mat,xpos,ypos,zpos);
			//D3DXMatrixIdentity( &World );
	  //      D3DXMatrixMultiply( &World, &World, &m_rot );		// まずターゲット方向に軸回転させて
	  //      D3DXMatrixMultiply( &World, &World, &mat );			// 次にオフセットさせます。

			//回転角度（ラジアン）
			/*D3DXVECTOR2 A( 0.f , 1.f );
			D3DXVECTOR2 B( _Cz.x , _Cz.z );
			float A_Length = GetVectorLength( A );
			float B_Length = GetVectorLength( B );
			float cos_sita = DotProduct( A , B ) / ( A_Length * B_Length ) ;
			float m_RotAngle = acos( cos_sita );
			if( _Cz.x < 0 ){
				m_RotAngle = -acos( cos_sita );
			}*/

			/*D3DXMATRIX m_rot;
			D3DXMatrixIdentity( &m_rot );
			m_rot = YRotation( m_rot , -m_RotAngle );*/
	//*/
			D3DXMATRIX mat;
			D3DXMATRIX World;
			D3DXMatrixIdentity(&mat);
			D3DXMatrixTranslation(&mat,xpos,0,zpos);

			D3DXMatrixIdentity( &World );
			//D3DXMatrixMultiply( &World, &World, &m_rot );		// まずターゲット方向に軸回転させて
			D3DXMatrixMultiply( &World, &World, &mat );			// 次にオフセットさせます。

			device->SetTransform(D3DTS_WORLD,&World);

			// 頂点バッファの解放，テクスチャの選択，描画
			vbuffer->Unlock();
			device->SetTexture(0,*Texture);
			device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
		}
		if(additivesynthesis)
		{
			device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}