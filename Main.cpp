#pragma warning(disable:4786)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <time.h>
#include "..\LibGame\LibGame.h"
#include "..\LibGame\Scene.h"
#include "GetGame.h"
#include "enemy.h"
#include "player.h"
#include "Billboard.h"
#include <string>
#include <tchar.h>
#include "XAudio.h"
#include "camera.h"
#include "..\LibGame\effect.h"
#include <process.h>

#include "Comptr.h"
#include "ZTexCreator.h"
#include "DepthBufShadowEffect.h"

using namespace IKD;
#define FADE_STOPTIME 30
#define hai_pos_X 640.f
#define hai_pos_Y 448.f
#define iie_pos_X 640.f
#define iie_pos_Y 576.f
//横幅の半分の長さ
#define HAI_WIDTH 192
#define HAI_HEIGHT 64
using namespace std;
int _scenechange;
CGame* game;
ID3DXEffect* cpEffect;
LPDIRECT3DTEXTURE9* cpTexture;
bool lighting = true;
FLOAT Ang = 0.0f;   // 回転角度
int _time = 0;
   float offset = 0.f;
   float f=0.0f;
   bool shadowflag = false;
   bool shadercontrolflag = false;

   D3DXMATRIX LightView, LightProj;   // ライトビュー変換・射影変換
   float LightScale = 1.5f;


void SetSceneChange( int num );


//-----------------------------------
//     IKDからいろいろ追加始めます
UINT TexSize = 1024;	// テクスチャサイズ
CZTexCreator ZTexCreator;
Com_ptr<IDirect3DTexture9> cpShadowMapTex;	// Z値テクスチャ

CDepthBufShadowEffect DepthBS;		// 深度バッファシャドウオブジェクト
Com_ptr<ID3DXBuffer> cpMatBuf;	// 今回はマテリアルは無視します･･･
Com_ptr<ID3DXMesh> cpMeshCube, cpMeshPlate;
DWORD dwMatNum, dwMatNum_Plate;
float fll=0.0f;
int x, z, CubeNum = 4;
D3DXMATRIX CubeWorldMat;
D3DXMATRIX PlateWorldMatrix;
Com_ptr<IDirect3DDevice9> cpDev;
D3DXMATRIX CameraView, CameraProj;	// カメラビュー変換・射影変換
// 板のワールド変換行列生成
void GetPlateWorldMatrix( D3DXMATRIX *PlateWorld )
{
	float PlateScale = 1.0f;
	D3DXMATRIX Scale;
	D3DXMatrixIdentity( PlateWorld );
	D3DXMatrixScaling( &Scale, PlateScale, 1.0f, PlateScale);
	*PlateWorld *= Scale;
	PlateWorld->_42 = -60.0f;
}

// 立方体のワールド変換行列生成
void GetCubeWorldMatrix( float f, int x, int z, D3DXMATRIX *mat)
{
	D3DXMATRIX RotY, RotZ;
	D3DXMatrixIdentity( mat );
	D3DXMatrixRotationY( &RotY, D3DXToRadian( f ) );
	D3DXMatrixRotationZ( &RotZ, D3DXToRadian( f*2.353f ) );
	*mat *= RotY * RotZ;
	mat->_41 = x*20.0f;  mat->_43 = z*20.0f; mat->_42=sin(f/10)*40;
}

//==========================================================
enum SCENE_STATE
{
	SCENE_TITLE,	//タイトル画面
	SCENE_TALK,		//ADVパート
	SCENE_GAME,		//ゲーム中
	SCENE_GAME_OVER,	//ゲームオーバー
	SCENE_GAME_CLEAR,	//ゲームクリア
	SCENE_GAME_STAFF,   //スタッフ
	SCENE_GAME_TIPS,//TIPS

	SCENE_STATE_NUM
};
int SceneState = SCENE_TALK;

//==============================================================
// アプリケーションのクラス

// アプリケーションのクラス宣言
class CWalk3D : public CScene {
private:
	UINT uiDevZBufWidth;    // デバイスバッファ幅
    UINT uiDevZBufHeight;   // デバイスバッファ高
	IDirect3DTexture9 *pZBufTex;
	UINT uiZTexWidth;
	UINT uiZTexHeight;
	enum
	{

		EF_MAX,
	};

	// メッシュ
	CMesh *MeshFloor, *MeshBox;
	CAnimMesh *MeshWalker2;

	// アニメーションのタイマー
	double AnimTime;

	// プレイヤーの位置、方向、速さ
	CPlayer* player;
	CEnemy* enemy;
	D3DXVECTOR2 Pos, Dir;
	float Speed;
	// カメラの位置、回転の速さ
	D3DXVECTOR2 CPos;
	float CVTurn;
	bool fade_control;	//フェード制御終了のおしらせ
	bool fade_flag;		//フェードイン？アウト？　ファルスでイン

	bool bgm_loopflag;
	
	// 武器の変換行列
	LPD3DXMATRIX WeaponMatrix;

	// 前回の入力
	CInputState PrevState;

	CBillboardGroup *m_billboard_group;
	CImageGroup *m_image_group;
	CImage *image_game_fade;
	CImage *image_continue;
	CImage *image_ui;
	int fade_alpha;
	FLOATRECT m_rect;
	//CDirectShow* m_pDirectShow;

	HANDLE m_mutex;
	HANDLE m_thread;
	CImage *loading_image[3];
	bool load_finish;
	bool loaded_mode;
	void UnloadMove();
	void LoadedMove();
	void UnloadDraw();
	void LoadedDraw();
	void LoadProcess();

	static void executeLauncher(void* args){
		// 無理やりにキャストして、本命の処理を実行する。
		reinterpret_cast<CWalk3D*>(args)->LoadProcess();
	}

public:
	XAudio* audio;
	LPDIRECT3DSURFACE9 cpZTexZBuf;
	ID3DXSprite* cpSprite;
	float W_Scale;
	float H_Scale;
	IDirect3DSurface9* pZTexSurf;
	// 描画デバイスサーフェイス群を取得保持
	IDirect3DSurface9* pDeviceSurf;
	IDirect3DSurface9* pDeviceZBufSurf;
	// コンストラクタ、デストラクタ
	CWalk3D();
	~CWalk3D();

	// デバイス状態の設定
	void RestoreDevice();

	// 動作モードの選択と初期化
	void Init();

	// 移動と描画（共通）
	virtual void Move();
	virtual void Draw();

	// 移動と描画
	void MoveWalk(const CInputState* is);
	void DrawWalk2(LPDIRECT3DDEVICE9 device);
	void Draw2D();
	void GetHP(int*,int*);
};

void CWalk3D::LoadProcess()
{
	

	Init();

	load_finish=true;
}

//==============================================================
// コンストラクタ、デストラクタ

// コンストラクタ
CWalk3D::CWalk3D()
//,m_pDirectShow(NULL)
{
	//// IKDの初期化
	if(!ZTexCreator.Init( cpDev, TexSize, TexSize, D3DFMT_A8R8G8B8 ))	// 通常はこのテクスチャが作成できます
	if(!ZTexCreator.Init( cpDev, TexSize, TexSize, D3DFMT_R5G6B5 ))		// 万が一は16bitに落とします
	ZTexCreator.GetZTex( cpShadowMapTex );
	
	if(FAILED(D3DXLoadMeshFromX( _T("Cube2.x"), D3DXMESH_MANAGED, cpDev.GetPtr(), NULL, cpMatBuf.ToCreator(), NULL, &dwMatNum, cpMeshCube.ToCreator() )))	return;
	if(FAILED(D3DXLoadMeshFromX( _T("Plate.x"), D3DXMESH_MANAGED, cpDev.GetPtr(), NULL, cpMatBuf.ToCreator(), NULL, &dwMatNum_Plate, cpMeshPlate.ToCreator() )))	return;
	
	D3DXMatrixPerspectiveFovLH( &CameraProj, D3DXToRadian(45), 640.0f/480.0f, 10.0f, 1000.0f);
	D3DXMatrixPerspectiveFovLH( &LightProj, D3DXToRadian(40), 1.0f, 40.0f, 300.0f);
	D3DXMatrixLookAtLH( &LightView, &D3DXVECTOR3(LightScale*100,LightScale*55,LightScale*100), &D3DXVECTOR3(0,-20,0), &D3DXVECTOR3(0,1,0) );

	// Z値テクスチャOBJへ登録
	ZTexCreator.SetViewMatrix( &LightView );
	ZTexCreator.SetProjMatrix( &LightProj );
	// 深度バッファシャドウOBJへ登録
	// カメラビューは毎回変わるので描画時に登録します
	DepthBS.SetLightViewMatrix( &LightView );
	DepthBS.SetLightProjMatrix( &LightProj );
	DepthBS.SetCameraProjMatrix( &CameraProj );

	// ここまで
	LPDIRECT3DDEVICE9 device=game->Graphics->GetDevice();
	cpTexture = new LPDIRECT3DTEXTURE9[1];
	D3DXCreateTextureFromFile(
				device, "Model\\Tiny_skin.dds", 
				&cpTexture[0]);
	D3DXCreateEffectFromFile(
      device,
      _T("fx\\Effect00.fx"),
      NULL,
      NULL,
      D3DXSHADER_DEBUG,
      NULL,
      &cpEffect,
      NULL
      );
	IDirect3DSurface9 *pTmpSf;
	device->GetDepthStencilSurface(&pTmpSf);
	GetSurfaceWH( pTmpSf, uiDevZBufWidth, uiDevZBufHeight );
	pTmpSf->Release();
	D3DXCreateTexture( device, uiDevZBufWidth, uiDevZBufHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pZBufTex );
	pZBufTex->GetSurfaceLevel( 0, &pZTexSurf );
	GetSurfaceWH( pZTexSurf, uiZTexWidth, uiZTexHeight );
	// 深度バッファサーフェイスの作成
   
   device->CreateDepthStencilSurface( uiZTexWidth, uiZTexHeight, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, FALSE, &cpZTexZBuf, NULL);
   D3DXCreateSprite( device, &cpSprite );
   W_Scale = (float)uiDevZBufWidth / uiZTexWidth;   // 貼り付けるZ値テクスチャを画面サイズにする幅高スケール値
   H_Scale = (float)uiDevZBufHeight / uiZTexHeight;

   device->GetRenderTarget(0, &pDeviceSurf);
   device->GetDepthStencilSurface(&pDeviceZBufSurf);

	cpEffect->SetFloat("Amplitude",0.9f);
	cpEffect->SetFloat("Period",2.f);
	  // cpEffect->SetFloat("Amplitude",0.9f);
   //cpEffect->SetFloat("Period",2.f);
	//string path;
	m_scene_change = FALSE;
	// 終了、カーソルに関する設定
	ShowCursor(TRUE);
	loaded_mode=false;
	load_finish=false;

	//path=ExePath+"..\\Model\\";
	
	audio = new XAudio();
	audio->InitXAudio2();
	m_image_group = new CImageGroup(device,256,256);
	m_image_group->SetDeviceState();//device設定　いるのかな？
	loading_image[0] = m_image_group->NewImage();
	loading_image[0]->LoadTexture("Model\\2D_UI\\TIP_1.png");
	loading_image[1] = m_image_group->NewImage();
	loading_image[1]->LoadTexture("Model\\2D_UI\\TIP_2.png");
	loading_image[2] = m_image_group->NewImage();
	loading_image[2]->LoadTexture("Model\\2D_UI\\TIP_3.png");
	image_ui=m_image_group->NewImage();
	image_ui->LoadTexture("Model\\2D_UI\\UI.png");
	string path;
	path=game->ExePath+"Model\\";

	image_game_fade=m_image_group->NewImage();
	image_game_fade->LoadTexture("Model\\2D_UI\\black.png");
	image_continue=m_image_group->NewImage();
	image_continue->LoadTexture("Model\\2D_UI\\continue.png");
	// 音読み込み
audio->LoadSoundFile(_T("SOUND\\BGM\\BGM_BTL_00i.wav"));	//0	戦闘開始時		
audio->LoadSoundFile(_T("SOUND\\BGM\\BGM_BTL_01L.wav"));	//1	ループ		
audio->LoadSoundFile(_T("SOUND\\BGM\\BGM_BTL_01.wav"));		//2	勝利ジングル


	//プレイヤー宣言
	player = new CPlayer;
	enemy = new CEnemy;
	

	// アニメーション男の子読み込み
	MeshWalker2=new CAnimMesh(device);
	MeshWalker2->LoadFromFile("Model\\walker2.x");


	// 床
	MeshFloor=new CMesh(device);
	MeshFloor->LoadFromFile("Model\\BG_MDL.x");
	
	MeshBox=new CMesh(device);
	MeshBox->LoadFromFile("Model\\Cube2.x");

	m_billboard_group = new CBillboardGroup(device);
	// 3Dモデルの初期化
	m_mutex = CreateMutex(NULL,false,NULL);
	m_thread = (HANDLE)_beginthread(&CWalk3D::executeLauncher,0,this);
}

// デストラクタ
CWalk3D::~CWalk3D() {
	NULL_DELETE(MeshWalker2);
	NULL_DELETE(MeshBox);
	NULL_DELETE(MeshFloor);

	NULL_DELETE(m_billboard_group);

	delete m_image_group;
	NULL_DELETE(player);
	NULL_DELETE(enemy);
	NULL_DELETE(audio);
	//delete pZBufTex;
	delete cpTexture;
}


//==============================================================
// 初期化
void CWalk3D::Init() {

	SetCamPos(  D3DXVECTOR3(0.f,7.f,-10.f) );
	//あさみ初期化
	m_scene_change = false;
	audio->SoundAllStop();
	player->status = player->ST_WAIT;
	player->Pos = D3DXVECTOR3(0.f,0.f,0.f);
	player->size = 0.04f;
	player->shader_num = 0;
	m_scene_change = FALSE;
	Pos=D3DXVECTOR2(0, 0);
	Dir=D3DXVECTOR2(0, -1);
	Speed=0;
	CVTurn=0;
	RestoreDevice();
	//audio->SoundPlay(0,false);	//音再生例　（番号,true=連続再生）
	

	enemy->size = 0.12;

	
	// アニメーション
	AnimTime=0;

	MeshWalker2->SetAnimSet(0, 0);
	MeshWalker2->SetAnimEnabled(0, true);
	MeshWalker2->AnimController->SetTrackPosition(0,1.25);
	


	fade_control = false;
	fade_flag = false;
	fade_alpha = 0;
	bgm_loopflag = false;
/*
	m_pDirectShow = new CDirectShow();
	BOOL isOk = m_pDirectShow->Init(L"smile.mp4",HWnd);
	if(isOk )
		m_pDirectShow->PlayMovie();
*/
}


//==============================================================
// 移動と描画

void CWalk3D::UnloadMove()
{
	game->Input->UpdateState();
	const CInputState* is=game->Input->GetState(0);

	if(load_finish)
	{
		CloseHandle(m_mutex);
		loaded_mode=true;
	}
	PrevState=*is;
}

void CWalk3D::Move()
{
	if(loaded_mode)
	{
		LoadedMove();
	}
	else
	{
		UnloadMove();
	}
}

// 移動（共通）
//元Move
void CWalk3D::LoadedMove() {

	m_scene_change = FALSE;
	// モードの切り替え
	game->Input->UpdateState();
	const CInputState* is=game->Input->GetState(0);


	MeshWalker2->AdvanceAnimTime(1.f/60.f);
	//シーン切り替え
	/*if( is->Button[4] )
	{
		m_scene_change = TRUE;
	}*/

	MoveWalk(is);

	// アニメーション
	AnimTime++;

	double _time = MeshWalker2->GetAnimTime();

	// 入力の保存
	static int count=0;
	count++;
	PrevState=*is;
}

void CWalk3D::UnloadDraw()
{
	game->Graphics->BeginScene();
	game->Graphics->Clear(D3DCOLOR_XRGB(255,255,255));

	game->Graphics->EndScene();
	game->Graphics->Present();
}

void CWalk3D::Draw()
{
	if(loaded_mode)
	{
		LoadedDraw();
	}
	else
	{
		UnloadDraw();
	}
}

//ベクトル外積
D3DXVECTOR3 ExteriorProduct(D3DXVECTOR3 _vl, D3DXVECTOR3 _vr) {
	//ａ×ｂ＝（ｙaｚb－ｙbｚa、ｚaｘb－ｚbｘa、ｘaｙb－ｘbｙa）

	float _x = _vl.y * _vr.z - _vr.y * _vl.z;
	float _y = _vl.z * _vr.x - _vr.z * _vl.x;
	float _z = _vl.x * _vr.y - _vr.x * _vl.y;

	return D3DXVECTOR3(_x,_y,_z);
}

// 描画（共通）
//元Draw
void CWalk3D::LoadedDraw() {
	
	f+=0.20f;

	// 描画開始
	LPDIRECT3DDEVICE9 device=game->Graphics->GetDevice();
	if (game->Graphics->CheckRestored()) RestoreDevice();
	game->Graphics->BeginScene();

	//// 描画先を生成した影用テクスチャにする
	//device->SetRenderTarget(0, pZTexSurf);
	//device->SetDepthStencilSurface( cpZTexZBuf );
	//
	//device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0 );

	////^^-
	////device->SetRenderTarget(0, pDeviceSurf);
 //   //device->SetDepthStencilSurface( pDeviceZBufSurf );
	////device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0 );
	////----
	//game->Graphics->Clear(D3DCOLOR_XRGB(255, 255, 255));
	//int x, z;
	//UINT numPass = 2;

	//		cpEffect->Begin( &numPass, 0 );
	//		cpEffect->BeginPass(2);
	//		//-
	//	D3DXMATRIX mat_view;
	//D3DXVECTOR3 from;
	//D3DXVECTOR3 lookat;

	//	CPos=D3DXVECTOR2( 0.f, -10.f);
	//	from=D3DXVECTOR3(GetCamPos().x, PL_CAMERA_HIGH, GetCamPos().z);
	//	lookat=D3DXVECTOR3(player->Pos.x, player->Pos.y, player->Pos.z);
	//
	//D3DXVECTOR3 up=D3DXVECTOR3(0, 1, 0);
	//D3DXMatrixLookAtLH(&mat_view, &from, &lookat, &up);
	//D3DXMATRIX  Proj;
	//D3DXMatrixPerspectiveFovLH( &Proj, D3DXToRadian(45), 16.0f/9.0f, 1.0f, 100.f);
	////-
	//	    D3DXMATRIX mat, RotY, RotZ,scale;

 //              D3DXMatrixIdentity( &mat );
	//		   D3DXMatrixIdentity( &scale );
	//		   D3DXMatrixTranslation( &mat,player->Pos.x,0.f,player->Pos.z);
	//		  // cpEffect->SetMatrix( "matPos",&mat);
	//		  // D3DXMatrixScaling(&scale, 0.04f, 0.04f, 0.04f);
	//		   D3DXMatrixScaling(&scale, 0.2f, 0.2f, 0.2f);
	//		   D3DXMatrixRotationY( &RotY,  player->angle  );
 //              //D3DXMatrixRotationZ( &RotZ, D3DXToRadian( f*2.353f ) );
 //             // mat = mat;// * RotY * scale;
 //             // mat._41 = 0*15.0f;  mat._43 = 0*15.0f;
	//		   //GetEffect()->SetMatrix( "matPos", &mat );
 //              //mat = mat * mat_view * Proj;

 //            //  GetEffect()->SetMatrix( "matWorldViewProj", &mat );
	//		   GetEffect()->SetMatrix("matView",&mat_view);
	//		   GetEffect()->SetMatrix("matProj",&Proj);
	//		   //MeshBox->Draw(0.f,0.f,0.f,player->size,player->size,player->size,0.f,0.f,0.f,TO_Y,1.f,false);
	//		MeshWalker2->ShaderDraw(player->Pos.x, 0, player->Pos.z, player->size, player->size, player->size, 0, player->angle, 0, TO_Y,true);
	//		cpEffect->EndPass();

	//cpEffect->End();
	//device->SetRenderTarget(0, pDeviceSurf);
 //   device->SetDepthStencilSurface( pDeviceZBufSurf );
	//device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0 );
	//// キャラと学校描画
	//if( fade_flag == false )
	//{
	//	DrawWalk2(device);
	//}
	//// Z値テクスチャ適用のスプライト描画
	//if( shadowflag )
	//{
 //        D3DXMATRIX SpMat;
 //        D3DXMatrixScaling( &SpMat, W_Scale, H_Scale, 1.0f ); 
 //        cpSprite->SetTransform( &SpMat );
 //        RECT SpRect;
 //        SetRect( &SpRect, 0, 0, (int)(uiZTexWidth/2.0f), uiZTexHeight );
 //        cpSprite->Begin(0);
 //        cpSprite->Draw(pZBufTex, &SpRect, NULL, NULL, 0xffffffff );
 //        cpSprite->End();
	//}
 //        

	//Draw2D();
	//FLOATRECT pos_rect = {100.0f,300.0f,100.0f,300.0f};
	//

	/*	D3DXVECTOR3 cam_vac;
		cam_vac.x=GetCamPos().x;
		cam_vac.y=PL_CAMERA_HIGH;
		cam_vac.z=GetCamPos().z;*/

		// 立方体描画
		//for(z=0; z<1; z++)
		//{
		//	for(x=0; x<1; x++)
		//	{
		//		GetCubeWorldMatrix( f, x, z, &CubeWorldMat );
		//		ZTexCreator.SetWorldMatrix( &CubeWorldMat );	// 立方体のワールド変換行列を登録
		//		for(int i=0; i<dwMatNum; i++)
		//		{
		//			ZTexCreator.SetParamToEffect();		// 描画の直前に呼び出す必要があります
		//			ZTexCreator.BeginPass();
		//			cpMeshCube->DrawSubset(i);				// メッシュ描画
		//			ZTexCreator.EndPass();
		//		}
		//	}
		//}

		//// 板描画
		//GetPlateWorldMatrix( &PlateWorldMatrix );
		//ZTexCreator.SetWorldMatrix( &PlateWorldMatrix );
		//ZTexCreator.SetParamToEffect();		// 描画の直前に呼び出す必要があります
		//for(int i=0; i<dwMatNum_Plate; i++){
		//	ZTexCreator.BeginPass();
		//	//cpMeshPlate->DrawSubset(i);
		//	ZTexCreator.EndPass();
		//}

		//ZTexCreator.End();	// シェーダ終了

		/////////////////////////////////////////
		////■パス2 : 深度バッファシャドウ描画

		//// カメラの視点を変化
		//D3DXMatrixLookAtLH( &CameraView, &D3DXVECTOR3(40*sin(f/20),70,40*cos(f/20)), &D3DXVECTOR3(20,-20,20), &D3DXVECTOR3(0,1,0) );
		//DepthBS.SetCameraViewMatrix( &CameraView );

		//DepthBS.Begin();

		//// 立方体描画
		//for(z=0; z<1; z++)
		//{
		//	for(x=0; x<1; x++)
		//	{
		//		GetCubeWorldMatrix( f, x, z, &CubeWorldMat);
		//		DepthBS.SetWorldMatrix( &CubeWorldMat );
		//		for(int i=0; i<dwMatNum; i++)
		//		{
		//			DepthBS.SetParamToEffect();		// 描画の直前に呼び出す必要があります
		//			DepthBS.BeginPass();
		//			cpMeshCube->DrawSubset(i);			// メッシュ描画
		//			DepthBS.EndPass();
		//		}
		//	}
		//}

		//// 板描画
		//GetPlateWorldMatrix( &PlateWorldMatrix );
		//DepthBS.SetWorldMatrix( &PlateWorldMatrix );
		//DepthBS.SetParamToEffect();		// 描画の直前に呼び出す必要があります
		//for(int i=0; i<dwMatNum_Plate; i++){
		//	DepthBS.BeginPass();
		//	cpMeshPlate->DrawSubset(i);
		//	DepthBS.EndPass();
		//}

	// 描画終了
	game->Graphics->EndScene();
	game->Graphics->Present();
}

// 移動
void CWalk3D::MoveWalk(const CInputState* is) {
	if( bgm_loopflag == false )
	{
		if( audio->GetSoundPlayingFlag(0) == false )
		{
			//audio->SoundPlay(1,true);
			bgm_loopflag = true;
		}
	}
	//==========================================================
	// プレイヤーの移動
	f+=0.55f;
	_time++;

	if( _time >= 2 )
	{
		_time = 0;
		if( shadercontrolflag )offset += 0.05f;
		else offset -= 0.05f;
			if (offset >= 3.14 * 2 / 0.9f)
			{
            shadercontrolflag = false;
			}
			if (offset <= -3.14 * 2 / 0.9f)
			{
            shadercontrolflag = true;
			}
			cpEffect->SetFloat("Offset",offset);
	}
	CameraControl(player->Pos,is);
	bool _Inputflag = false;
	// プレイヤーの移動方向を決める
	// 入力の方向とカメラの方向から移動方向を決める
	D3DXVECTOR2 joy(is->AnalogX, -is->AnalogY);
	bool joy_input=(joy.x!=0 || joy.y!=0 );
	if (joy_input) {
		D3DXVECTOR2 dir= D3DXVECTOR2( player->Pos.x,player->Pos.z)-D3DXVECTOR2(GetCamPos().x,GetCamPos().z);
		D3DXVec2Normalize(&dir, &dir);
		D3DXVec2Normalize(&joy, &joy);
		Dir=D3DXVECTOR2(joy.y*dir.x+joy.x*dir.y, joy.y*dir.y-joy.x*dir.x);
		player->status = player->ST_RUN;
		_Inputflag = true;
	}
	else
	{
		player->status = player->ST_WAIT;
	}

	// プレイヤーの移動スピードを決める
	// 入力があるときは加速、ないときは減速する
	if( player->status == player->ST_RUN )
	{
		float max_speed=0.1f;
		if (joy_input) Speed+=0.005f; else Speed-=0.005f;
		if (Speed>max_speed) Speed=max_speed; 
			else if (Speed<0) Speed=0;

		// プレイヤーの位置を更新する
		player->Pos.x += Dir.x*Speed;
		player->Pos.z += Dir.y*Speed;
		player->angle = atan2( Dir.x,Dir.y ) + D3DX_PI;
	}
	if( is->Button[0] && !PrevState.Button[0] )
	{
		if( player->shader_num == 0 )
		{
			player->shader_num = 1;
		}
		else
		{
			player->shader_num = 0;
		}
		
	}
	if( is->Button[1] && !PrevState.Button[1] )
	{
		if( lighting )
		{
			lighting = false;
			game->Graphics->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
		}
		else
		{
			lighting = true;
			game->Graphics->GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
		//player->shader_num = 1;
	}
	if( is->Button[2] && !PrevState.Button[2] )
	{
		if(shadowflag) shadowflag = false;
		else shadowflag = true;
		//game->Graphics->GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
	//
	//
	//
	//-----------------------------


	//==========================================================
	// カメラの移動


	// カメラの位置を決める
	// 現在の距離と基本の距離を線形補間する

	// カメラの設定
	// カメラの位置からプレイヤーを見るように
	// ビュー変換行列を設定する
	
	LPDIRECT3DDEVICE9 device=game->Graphics->GetDevice();
	D3DXMATRIX mat_view;
	D3DXVECTOR3 from;
	D3DXVECTOR3 lookat;

		CPos=D3DXVECTOR2( 0.f, -10.f);
		from=D3DXVECTOR3(GetCamPos().x, PL_CAMERA_HIGH, GetCamPos().z);
		//from=D3DXVECTOR3(0, 20, -50);
		lookat=D3DXVECTOR3(player->Pos.x, player->Pos.y, player->Pos.z);
	
	D3DXVECTOR3 up=D3DXVECTOR3(0, 1, 0);
	D3DXMatrixLookAtLH(&mat_view, &from, &lookat, &up);
	device->SetTransform(D3DTS_VIEW, &mat_view);
	GetEffect()->SetMatrix( "matView", &mat_view );
	D3DXMATRIX  Proj;
	D3DXMatrixPerspectiveFovLH( &Proj, D3DXToRadian(45), 16.0f/9.0f, 1.0f, 100.f);
	D3DXMATRIX allin;
	allin = mat_view * Proj;
	//cpEffect->SetMatrix( "matWorldViewProj", &allin );

}

// 描画
void CWalk3D::DrawWalk2(LPDIRECT3DDEVICE9 device) {
	device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_ARGB(255, 255, 255, 255));
	if(player->shader_num == 1)
	{
		//-
		D3DXMATRIX mat_view;
	D3DXVECTOR3 from;
	D3DXVECTOR3 lookat;

		CPos=D3DXVECTOR2( 0.f, -10.f);
		from=D3DXVECTOR3(GetCamPos().x, PL_CAMERA_HIGH, GetCamPos().z);
		//from=D3DXVECTOR3(0, 20, -50);
		lookat=D3DXVECTOR3(player->Pos.x, player->Pos.y, player->Pos.z);
	
	D3DXVECTOR3 up=D3DXVECTOR3(0, 1, 0);
	D3DXMatrixLookAtLH(&mat_view, &from, &lookat, &up);
	D3DXMATRIX  Proj;
	D3DXMatrixPerspectiveFovLH( &Proj, D3DXToRadian(45), 16.0f/9.0f, 1.0f, 100.f);

	GetEffect()->SetMatrix("matView",&mat_view);
	GetEffect()->SetMatrix("matProj",&Proj);
	//-
		cpEffect->SetTechnique( "BasicTec" );
			UINT numPass = 0;
		   cpEffect->Begin( &numPass, 0 );
		   cpEffect->BeginPass(0);
		    D3DXMATRIX mat, RotY, RotZ,scale;
			D3DXMATRIX tempmat;
               D3DXMatrixIdentity( &mat );
			   D3DXMatrixTranslation( &mat,player->Pos.x,0.f,player->Pos.z);
			   //D3DXMatrixScaling(&scale, 0.04f, 0.04f, 0.04f);
			   //tempmat = mat * scale;
			   //GetEffect()->SetMatrix("matPos",&tempmat);
			   ////D3DXMatrixScaling(&scale, 0.7f, 0.7f, 0.7f);
      //         D3DXMatrixRotationY( &RotY, D3DXToRadian( f ) );
      //         D3DXMatrixRotationZ( &RotZ, D3DXToRadian( f*2.353f ) );
      //         mat *= RotY * RotZ * scale;
      //         
      //         mat = mat * mat_view * Proj;
			   
               cpEffect->SetMatrix( "matWorldViewProj", &mat );
		// ワールドビュー射影変換行列設定
		cpEffect->SetTexture("diffuseTexture", cpTexture[0]);
		cpEffect->SetTexture("texShadowMap", pZBufTex);
		MeshWalker2->ShaderDraw(player->Pos.x, 0, player->Pos.z, player->size, player->size, player->size, 0, player->angle, 0, TO_Y,true);
		cpEffect->EndPass();
		cpEffect->End();
		numPass = 1;
		cpEffect->Begin( &numPass, 0 );
		cpEffect->BeginPass(1);
		D3DXMatrixIdentity( &mat );
		D3DXMatrixTranslation( &mat,0.f,0.f,0.f);
		cpEffect->SetMatrix( "matWorldViewProj", &mat );
		GetEffect()->SetMatrix("matView",&mat_view);
	    GetEffect()->SetMatrix("matProj",&Proj);
		MeshFloor->Draw(0.f,0.f,0.f,player->size,player->size,player->size,0.f,0.f,0.f,TO_Y,1.f,false);
		cpEffect->EndPass();
		cpEffect->End();
		  
		
	}
	else
	{
		MeshWalker2->Draw(player->Pos.x, 0, player->Pos.z, player->size, player->size, player->size, 0, player->angle, 0, TO_Y,false);
		MeshFloor->Draw(0.f,0.f,0.f,player->size,player->size,player->size,0.f,0.f,0.f,TO_Y,1.f,false);
	}
	

}


//==============================================================
// デバイスの復帰
void CWalk3D::RestoreDevice() {
	LPDIRECT3DDEVICE9 device=game->Graphics->GetDevice();

	// アルファブレンディングの設定
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// レンダリングの設定
	device->SetRenderState(D3DRS_DITHERENABLE, FALSE);
	device->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device->SetRenderState(D3DRS_LIGHTING, TRUE);
	device->SetRenderState(D3DRS_ZENABLE, TRUE);
	device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_ARGB(255, 255, 255, 255));

	// テクスチャ処理方法の設定：
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	
	// ワールドマトリクス
	D3DXMATRIX mat_world;
	D3DXMatrixIdentity(&mat_world);
	device->SetTransform(D3DTS_WORLD, &mat_world);

	// 視野マトリクス
	D3DXMATRIX mat_view;
	D3DXVECTOR3 vec_from=D3DXVECTOR3(0, 2, -10);
	D3DXVECTOR3 vec_lookat=D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 vec_up=D3DXVECTOR3(0, 1, 0);
	D3DXMatrixLookAtLH(&mat_view, &vec_from, &vec_lookat, &vec_up);
	device->SetTransform(D3DTS_VIEW, &mat_view);
	
	// 投影マトリクス
	D3DXMATRIX mat_proj;
	D3DXMatrixPerspectiveFovLH( &mat_proj, D3DXToRadian(45), 16.0f/9.0f, 1.0f, 100.f);
	device->SetTransform(D3DTS_PROJECTION, &mat_proj);
	cpEffect->SetMatrix( "matProj", &mat_proj );

	// ライティング
	game->Graphics->SetLight(0, D3DLIGHT_DIRECTIONAL,D3DXVECTOR3(100.f,100.f,100.f), -1.0f, -1.0f, -1.0f, 10000);
	D3DXMatrixPerspectiveFovLH( &LightProj, D3DXToRadian(45), 1.0f, 40.0f, 300.0f);
	D3DXMatrixLookAtLH( &LightView, &D3DXVECTOR3(LightScale*100,LightScale*55,LightScale*100), &D3DXVECTOR3(0,-20,0), &D3DXVECTOR3(0,1,0) );

	// サンプルでは2つやってるけど、セルフシャドウ用と影用のシェーダが分かれてるからそれぞれやってる
	cpEffect->SetMatrix( "matLightProj", &LightProj );
	cpEffect->SetMatrix( "matLightView", &LightView );
	// ライトの作成
	D3DLIGHT9 light;
	D3DXVECTOR3 vec_lightdir(-1.0f, -1.0f, -1.0f);
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type=D3DLIGHT_DIRECTIONAL;
	light.Diffuse.a=1.0f;
	light.Diffuse.r=0.7f;
	light.Diffuse.g=0.7f;
	light.Diffuse.b=0.7f;

//	light.Ambient.r=0.0f;
//	light.Ambient.g=0.0f;
//	light.Ambient.b=0.0f;

	D3DXVec3Normalize(
		(D3DXVECTOR3*)&light.Direction, &vec_lightdir);
	light.Position = D3DXVECTOR3(100.f,100.f,100.f);
	light.Range=10000;

	// ライトの設定
	device->SetLight(0, &light);
	device->LightEnable(0, TRUE);
	device->SetRenderState(D3DRS_LIGHTING, TRUE);

}


SCENE_STATE GetNextScene()
{
	SCENE_STATE NextScene;

	NextScene = SCENE_GAME;
	return NextScene;
}

CScene* SceneCreate()
{
	CScene *_scene = NULL;
	switch( SceneState )
	{
		case SCENE_GAME: _scene = new CWalk3D();
		break;
	};
	return _scene;
}
//==============================================================
// メインルーチン
INT WINAPI WinMain(HINSTANCE hinst, HINSTANCE, LPSTR, INT) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	//メモリリーク
	//_CrtSetBreakAlloc(87);

	HICON icon;
	icon=LoadIcon(hinst, MAKEINTRESOURCE(1));

	// ウィンドウクラスの登録：
	// ここではダミーのメッセージハンドラを登録する
	WNDCLASSEX wc={
		sizeof(WNDCLASSEX), CS_CLASSDC, DefWindowProc, 0L, 0L,
		hinst, icon, LoadCursor(NULL, IDC_ARROW), NULL, NULL,
		"ShaderTest", NULL
	};
	RegisterClassEx(&wc);

	srand(( unsigned int)time(NULL));
	HWND hWnd;
	// ウィンドウの作成
	hWnd=CreateWindow(
		"ShaderTest", "ShaderTest",
		WS_OVERLAPPEDWINDOW,
		0, 0, 1280, 720, NULL, NULL, wc.hInstance, NULL);
	SceneState = SCENE_TITLE;
	game = new CGame( "ShaderTest",false,false,false,hWnd );
	game->scene = new CWalk3D();
	SetWindowLong(hWnd, GWL_WNDPROC, (long)::WndProc);
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	game->ResetTime();
	while (msg.message!=WM_QUIT)
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			game->unloopRun(&msg,hWnd);
			if(game->IsSceneChange())
			{
				NULL_DELETE( game->scene );
				
				SceneState = GetNextScene();
				game->scene = SceneCreate();
				game->ResetTime();
			}
		}
	}
	DeleteHierarchy();
	delete game;
	return 0;
}
void SetSceneChange( int num )
{
	_scenechange = num;
}
void CWalk3D::Draw2D()
{

}

CGraphics* GetGraphics()
{
	return game->Graphics;
}

CInput* GetInput()
{
	return game->Input;
}

ID3DXEffect* GetEffect()
{
	return cpEffect;
}

LPDIRECT3DTEXTURE9* GetTexture()
{
	return cpTexture;
}

bool GetSurfaceWH( IDirect3DSurface9 *pSurf, UINT &uiWidth, UINT &uiHeight )
{
        if( !pSurf ) return false;
        D3DSURFACE_DESC SufDesc;
        pSurf->GetDesc( &SufDesc );
        uiWidth  = SufDesc.Width;
        uiHeight = SufDesc.Height;

        return true;
}
