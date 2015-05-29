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
//�����̔����̒���
#define HAI_WIDTH 192
#define HAI_HEIGHT 64
using namespace std;
int _scenechange;
CGame* game;
ID3DXEffect* cpEffect;
LPDIRECT3DTEXTURE9* cpTexture;
bool lighting = true;
FLOAT Ang = 0.0f;   // ��]�p�x
int _time = 0;
   float offset = 0.f;
   float f=0.0f;
   bool shadowflag = false;
   bool shadercontrolflag = false;

   D3DXMATRIX LightView, LightProj;   // ���C�g�r���[�ϊ��E�ˉe�ϊ�
   float LightScale = 1.5f;


void SetSceneChange( int num );


//-----------------------------------
//     IKD���炢�낢��ǉ��n�߂܂�
UINT TexSize = 1024;	// �e�N�X�`���T�C�Y
CZTexCreator ZTexCreator;
Com_ptr<IDirect3DTexture9> cpShadowMapTex;	// Z�l�e�N�X�`��

CDepthBufShadowEffect DepthBS;		// �[�x�o�b�t�@�V���h�E�I�u�W�F�N�g
Com_ptr<ID3DXBuffer> cpMatBuf;	// ����̓}�e���A���͖������܂����
Com_ptr<ID3DXMesh> cpMeshCube, cpMeshPlate;
DWORD dwMatNum, dwMatNum_Plate;
float fll=0.0f;
int x, z, CubeNum = 4;
D3DXMATRIX CubeWorldMat;
D3DXMATRIX PlateWorldMatrix;
Com_ptr<IDirect3DDevice9> cpDev;
D3DXMATRIX CameraView, CameraProj;	// �J�����r���[�ϊ��E�ˉe�ϊ�
// �̃��[���h�ϊ��s�񐶐�
void GetPlateWorldMatrix( D3DXMATRIX *PlateWorld )
{
	float PlateScale = 1.0f;
	D3DXMATRIX Scale;
	D3DXMatrixIdentity( PlateWorld );
	D3DXMatrixScaling( &Scale, PlateScale, 1.0f, PlateScale);
	*PlateWorld *= Scale;
	PlateWorld->_42 = -60.0f;
}

// �����̂̃��[���h�ϊ��s�񐶐�
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
	SCENE_TITLE,	//�^�C�g�����
	SCENE_TALK,		//ADV�p�[�g
	SCENE_GAME,		//�Q�[����
	SCENE_GAME_OVER,	//�Q�[���I�[�o�[
	SCENE_GAME_CLEAR,	//�Q�[���N���A
	SCENE_GAME_STAFF,   //�X�^�b�t
	SCENE_GAME_TIPS,//TIPS

	SCENE_STATE_NUM
};
int SceneState = SCENE_TALK;

//==============================================================
// �A�v���P�[�V�����̃N���X

// �A�v���P�[�V�����̃N���X�錾
class CWalk3D : public CScene {
private:
	UINT uiDevZBufWidth;    // �f�o�C�X�o�b�t�@��
    UINT uiDevZBufHeight;   // �f�o�C�X�o�b�t�@��
	IDirect3DTexture9 *pZBufTex;
	UINT uiZTexWidth;
	UINT uiZTexHeight;
	enum
	{

		EF_MAX,
	};

	// ���b�V��
	CMesh *MeshFloor, *MeshBox;
	CAnimMesh *MeshWalker2;

	// �A�j���[�V�����̃^�C�}�[
	double AnimTime;

	// �v���C���[�̈ʒu�A�����A����
	CPlayer* player;
	CEnemy* enemy;
	D3DXVECTOR2 Pos, Dir;
	float Speed;
	// �J�����̈ʒu�A��]�̑���
	D3DXVECTOR2 CPos;
	float CVTurn;
	bool fade_control;	//�t�F�[�h����I���̂����点
	bool fade_flag;		//�t�F�[�h�C���H�A�E�g�H�@�t�@���X�ŃC��

	bool bgm_loopflag;
	
	// ����̕ϊ��s��
	LPD3DXMATRIX WeaponMatrix;

	// �O��̓���
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
		// �������ɃL���X�g���āA�{���̏��������s����B
		reinterpret_cast<CWalk3D*>(args)->LoadProcess();
	}

public:
	XAudio* audio;
	LPDIRECT3DSURFACE9 cpZTexZBuf;
	ID3DXSprite* cpSprite;
	float W_Scale;
	float H_Scale;
	IDirect3DSurface9* pZTexSurf;
	// �`��f�o�C�X�T�[�t�F�C�X�Q���擾�ێ�
	IDirect3DSurface9* pDeviceSurf;
	IDirect3DSurface9* pDeviceZBufSurf;
	// �R���X�g���N�^�A�f�X�g���N�^
	CWalk3D();
	~CWalk3D();

	// �f�o�C�X��Ԃ̐ݒ�
	void RestoreDevice();

	// ���샂�[�h�̑I���Ə�����
	void Init();

	// �ړ��ƕ`��i���ʁj
	virtual void Move();
	virtual void Draw();

	// �ړ��ƕ`��
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
// �R���X�g���N�^�A�f�X�g���N�^

// �R���X�g���N�^
CWalk3D::CWalk3D()
//,m_pDirectShow(NULL)
{
	//// IKD�̏�����
	if(!ZTexCreator.Init( cpDev, TexSize, TexSize, D3DFMT_A8R8G8B8 ))	// �ʏ�͂��̃e�N�X�`�����쐬�ł��܂�
	if(!ZTexCreator.Init( cpDev, TexSize, TexSize, D3DFMT_R5G6B5 ))		// �������16bit�ɗ��Ƃ��܂�
	ZTexCreator.GetZTex( cpShadowMapTex );
	
	if(FAILED(D3DXLoadMeshFromX( _T("Cube2.x"), D3DXMESH_MANAGED, cpDev.GetPtr(), NULL, cpMatBuf.ToCreator(), NULL, &dwMatNum, cpMeshCube.ToCreator() )))	return;
	if(FAILED(D3DXLoadMeshFromX( _T("Plate.x"), D3DXMESH_MANAGED, cpDev.GetPtr(), NULL, cpMatBuf.ToCreator(), NULL, &dwMatNum_Plate, cpMeshPlate.ToCreator() )))	return;
	
	D3DXMatrixPerspectiveFovLH( &CameraProj, D3DXToRadian(45), 640.0f/480.0f, 10.0f, 1000.0f);
	D3DXMatrixPerspectiveFovLH( &LightProj, D3DXToRadian(40), 1.0f, 40.0f, 300.0f);
	D3DXMatrixLookAtLH( &LightView, &D3DXVECTOR3(LightScale*100,LightScale*55,LightScale*100), &D3DXVECTOR3(0,-20,0), &D3DXVECTOR3(0,1,0) );

	// Z�l�e�N�X�`��OBJ�֓o�^
	ZTexCreator.SetViewMatrix( &LightView );
	ZTexCreator.SetProjMatrix( &LightProj );
	// �[�x�o�b�t�@�V���h�EOBJ�֓o�^
	// �J�����r���[�͖���ς��̂ŕ`�掞�ɓo�^���܂�
	DepthBS.SetLightViewMatrix( &LightView );
	DepthBS.SetLightProjMatrix( &LightProj );
	DepthBS.SetCameraProjMatrix( &CameraProj );

	// �����܂�
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
	// �[�x�o�b�t�@�T�[�t�F�C�X�̍쐬
   
   device->CreateDepthStencilSurface( uiZTexWidth, uiZTexHeight, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, FALSE, &cpZTexZBuf, NULL);
   D3DXCreateSprite( device, &cpSprite );
   W_Scale = (float)uiDevZBufWidth / uiZTexWidth;   // �\��t����Z�l�e�N�X�`������ʃT�C�Y�ɂ��镝���X�P�[���l
   H_Scale = (float)uiDevZBufHeight / uiZTexHeight;

   device->GetRenderTarget(0, &pDeviceSurf);
   device->GetDepthStencilSurface(&pDeviceZBufSurf);

	cpEffect->SetFloat("Amplitude",0.9f);
	cpEffect->SetFloat("Period",2.f);
	  // cpEffect->SetFloat("Amplitude",0.9f);
   //cpEffect->SetFloat("Period",2.f);
	//string path;
	m_scene_change = FALSE;
	// �I���A�J�[�\���Ɋւ���ݒ�
	ShowCursor(TRUE);
	loaded_mode=false;
	load_finish=false;

	//path=ExePath+"..\\Model\\";
	
	audio = new XAudio();
	audio->InitXAudio2();
	m_image_group = new CImageGroup(device,256,256);
	m_image_group->SetDeviceState();//device�ݒ�@����̂��ȁH
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
	// ���ǂݍ���
audio->LoadSoundFile(_T("SOUND\\BGM\\BGM_BTL_00i.wav"));	//0	�퓬�J�n��		
audio->LoadSoundFile(_T("SOUND\\BGM\\BGM_BTL_01L.wav"));	//1	���[�v		
audio->LoadSoundFile(_T("SOUND\\BGM\\BGM_BTL_01.wav"));		//2	�����W���O��


	//�v���C���[�錾
	player = new CPlayer;
	enemy = new CEnemy;
	

	// �A�j���[�V�����j�̎q�ǂݍ���
	MeshWalker2=new CAnimMesh(device);
	MeshWalker2->LoadFromFile("Model\\walker2.x");


	// ��
	MeshFloor=new CMesh(device);
	MeshFloor->LoadFromFile("Model\\BG_MDL.x");
	
	MeshBox=new CMesh(device);
	MeshBox->LoadFromFile("Model\\Cube2.x");

	m_billboard_group = new CBillboardGroup(device);
	// 3D���f���̏�����
	m_mutex = CreateMutex(NULL,false,NULL);
	m_thread = (HANDLE)_beginthread(&CWalk3D::executeLauncher,0,this);
}

// �f�X�g���N�^
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
// ������
void CWalk3D::Init() {

	SetCamPos(  D3DXVECTOR3(0.f,7.f,-10.f) );
	//�����ݏ�����
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
	//audio->SoundPlay(0,false);	//���Đ���@�i�ԍ�,true=�A���Đ��j
	

	enemy->size = 0.12;

	
	// �A�j���[�V����
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
// �ړ��ƕ`��

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

// �ړ��i���ʁj
//��Move
void CWalk3D::LoadedMove() {

	m_scene_change = FALSE;
	// ���[�h�̐؂�ւ�
	game->Input->UpdateState();
	const CInputState* is=game->Input->GetState(0);


	MeshWalker2->AdvanceAnimTime(1.f/60.f);
	//�V�[���؂�ւ�
	/*if( is->Button[4] )
	{
		m_scene_change = TRUE;
	}*/

	MoveWalk(is);

	// �A�j���[�V����
	AnimTime++;

	double _time = MeshWalker2->GetAnimTime();

	// ���͂̕ۑ�
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

//�x�N�g���O��
D3DXVECTOR3 ExteriorProduct(D3DXVECTOR3 _vl, D3DXVECTOR3 _vr) {
	//���~�����i��a��b�|��b��a�A��a��b�|��b��a�A��a��b�|��b��a�j

	float _x = _vl.y * _vr.z - _vr.y * _vl.z;
	float _y = _vl.z * _vr.x - _vr.z * _vl.x;
	float _z = _vl.x * _vr.y - _vr.x * _vl.y;

	return D3DXVECTOR3(_x,_y,_z);
}

// �`��i���ʁj
//��Draw
void CWalk3D::LoadedDraw() {
	
	f+=0.20f;

	// �`��J�n
	LPDIRECT3DDEVICE9 device=game->Graphics->GetDevice();
	if (game->Graphics->CheckRestored()) RestoreDevice();
	game->Graphics->BeginScene();

	//// �`���𐶐������e�p�e�N�X�`���ɂ���
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
	//// �L�����Ɗw�Z�`��
	//if( fade_flag == false )
	//{
	//	DrawWalk2(device);
	//}
	//// Z�l�e�N�X�`���K�p�̃X�v���C�g�`��
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

		// �����̕`��
		//for(z=0; z<1; z++)
		//{
		//	for(x=0; x<1; x++)
		//	{
		//		GetCubeWorldMatrix( f, x, z, &CubeWorldMat );
		//		ZTexCreator.SetWorldMatrix( &CubeWorldMat );	// �����̂̃��[���h�ϊ��s���o�^
		//		for(int i=0; i<dwMatNum; i++)
		//		{
		//			ZTexCreator.SetParamToEffect();		// �`��̒��O�ɌĂяo���K�v������܂�
		//			ZTexCreator.BeginPass();
		//			cpMeshCube->DrawSubset(i);				// ���b�V���`��
		//			ZTexCreator.EndPass();
		//		}
		//	}
		//}

		//// �`��
		//GetPlateWorldMatrix( &PlateWorldMatrix );
		//ZTexCreator.SetWorldMatrix( &PlateWorldMatrix );
		//ZTexCreator.SetParamToEffect();		// �`��̒��O�ɌĂяo���K�v������܂�
		//for(int i=0; i<dwMatNum_Plate; i++){
		//	ZTexCreator.BeginPass();
		//	//cpMeshPlate->DrawSubset(i);
		//	ZTexCreator.EndPass();
		//}

		//ZTexCreator.End();	// �V�F�[�_�I��

		/////////////////////////////////////////
		////���p�X2 : �[�x�o�b�t�@�V���h�E�`��

		//// �J�����̎��_��ω�
		//D3DXMatrixLookAtLH( &CameraView, &D3DXVECTOR3(40*sin(f/20),70,40*cos(f/20)), &D3DXVECTOR3(20,-20,20), &D3DXVECTOR3(0,1,0) );
		//DepthBS.SetCameraViewMatrix( &CameraView );

		//DepthBS.Begin();

		//// �����̕`��
		//for(z=0; z<1; z++)
		//{
		//	for(x=0; x<1; x++)
		//	{
		//		GetCubeWorldMatrix( f, x, z, &CubeWorldMat);
		//		DepthBS.SetWorldMatrix( &CubeWorldMat );
		//		for(int i=0; i<dwMatNum; i++)
		//		{
		//			DepthBS.SetParamToEffect();		// �`��̒��O�ɌĂяo���K�v������܂�
		//			DepthBS.BeginPass();
		//			cpMeshCube->DrawSubset(i);			// ���b�V���`��
		//			DepthBS.EndPass();
		//		}
		//	}
		//}

		//// �`��
		//GetPlateWorldMatrix( &PlateWorldMatrix );
		//DepthBS.SetWorldMatrix( &PlateWorldMatrix );
		//DepthBS.SetParamToEffect();		// �`��̒��O�ɌĂяo���K�v������܂�
		//for(int i=0; i<dwMatNum_Plate; i++){
		//	DepthBS.BeginPass();
		//	cpMeshPlate->DrawSubset(i);
		//	DepthBS.EndPass();
		//}

	// �`��I��
	game->Graphics->EndScene();
	game->Graphics->Present();
}

// �ړ�
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
	// �v���C���[�̈ړ�
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
	// �v���C���[�̈ړ����������߂�
	// ���͂̕����ƃJ�����̕�������ړ����������߂�
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

	// �v���C���[�̈ړ��X�s�[�h�����߂�
	// ���͂�����Ƃ��͉����A�Ȃ��Ƃ��͌�������
	if( player->status == player->ST_RUN )
	{
		float max_speed=0.1f;
		if (joy_input) Speed+=0.005f; else Speed-=0.005f;
		if (Speed>max_speed) Speed=max_speed; 
			else if (Speed<0) Speed=0;

		// �v���C���[�̈ʒu���X�V����
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
	// �J�����̈ړ�


	// �J�����̈ʒu�����߂�
	// ���݂̋����Ɗ�{�̋�������`��Ԃ���

	// �J�����̐ݒ�
	// �J�����̈ʒu����v���C���[������悤��
	// �r���[�ϊ��s���ݒ肷��
	
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

// �`��
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
		// ���[���h�r���[�ˉe�ϊ��s��ݒ�
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
// �f�o�C�X�̕��A
void CWalk3D::RestoreDevice() {
	LPDIRECT3DDEVICE9 device=game->Graphics->GetDevice();

	// �A���t�@�u�����f�B���O�̐ݒ�
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �����_�����O�̐ݒ�
	device->SetRenderState(D3DRS_DITHERENABLE, FALSE);
	device->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device->SetRenderState(D3DRS_LIGHTING, TRUE);
	device->SetRenderState(D3DRS_ZENABLE, TRUE);
	device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_ARGB(255, 255, 255, 255));

	// �e�N�X�`���������@�̐ݒ�F
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	
	// ���[���h�}�g���N�X
	D3DXMATRIX mat_world;
	D3DXMatrixIdentity(&mat_world);
	device->SetTransform(D3DTS_WORLD, &mat_world);

	// ����}�g���N�X
	D3DXMATRIX mat_view;
	D3DXVECTOR3 vec_from=D3DXVECTOR3(0, 2, -10);
	D3DXVECTOR3 vec_lookat=D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 vec_up=D3DXVECTOR3(0, 1, 0);
	D3DXMatrixLookAtLH(&mat_view, &vec_from, &vec_lookat, &vec_up);
	device->SetTransform(D3DTS_VIEW, &mat_view);
	
	// ���e�}�g���N�X
	D3DXMATRIX mat_proj;
	D3DXMatrixPerspectiveFovLH( &mat_proj, D3DXToRadian(45), 16.0f/9.0f, 1.0f, 100.f);
	device->SetTransform(D3DTS_PROJECTION, &mat_proj);
	cpEffect->SetMatrix( "matProj", &mat_proj );

	// ���C�e�B���O
	game->Graphics->SetLight(0, D3DLIGHT_DIRECTIONAL,D3DXVECTOR3(100.f,100.f,100.f), -1.0f, -1.0f, -1.0f, 10000);
	D3DXMatrixPerspectiveFovLH( &LightProj, D3DXToRadian(45), 1.0f, 40.0f, 300.0f);
	D3DXMatrixLookAtLH( &LightView, &D3DXVECTOR3(LightScale*100,LightScale*55,LightScale*100), &D3DXVECTOR3(0,-20,0), &D3DXVECTOR3(0,1,0) );

	// �T���v���ł�2����Ă邯�ǁA�Z���t�V���h�E�p�Ɖe�p�̃V�F�[�_��������Ă邩�炻�ꂼ�����Ă�
	cpEffect->SetMatrix( "matLightProj", &LightProj );
	cpEffect->SetMatrix( "matLightView", &LightView );
	// ���C�g�̍쐬
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

	// ���C�g�̐ݒ�
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
// ���C�����[�`��
INT WINAPI WinMain(HINSTANCE hinst, HINSTANCE, LPSTR, INT) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	//���������[�N
	//_CrtSetBreakAlloc(87);

	HICON icon;
	icon=LoadIcon(hinst, MAKEINTRESOURCE(1));

	// �E�B���h�E�N���X�̓o�^�F
	// �����ł̓_�~�[�̃��b�Z�[�W�n���h����o�^����
	WNDCLASSEX wc={
		sizeof(WNDCLASSEX), CS_CLASSDC, DefWindowProc, 0L, 0L,
		hinst, icon, LoadCursor(NULL, IDC_ARROW), NULL, NULL,
		"ShaderTest", NULL
	};
	RegisterClassEx(&wc);

	srand(( unsigned int)time(NULL));
	HWND hWnd;
	// �E�B���h�E�̍쐬
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
