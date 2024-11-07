#pragma once

// ���� ����ü
struct Vertex
{
	// 3���� ������ �ִ� ��ǥ
	Vector3 vPos;

	// �ؽ����� �������� ��ǥ
	Vector2 vUV;

	// ���� �� (0 ~ 1)
	Vector4 vColor; 
};

// [������� ���� ����ü]
struct TransInfo
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;	// World View
	Matrix matWVP;
};

extern TransInfo g_Trans;

struct MtrlConst
{
	int		iArr[4];
	float	fArr[4];
	Vector2 v2Arr[4];
	Vector4	v4Arr[4];
	Matrix	mat[2];

	// �ؽ��� ���� �ߴ��� ����
	int bTex[TEX_END];
};


struct tDebugShapeInfo
{
	DEBUG_SHAPE Shape;
	Vector3		WorldPos;
	Vector3		Scale;
	Vector4		Rotation;
	Matrix		matWorld;

	Vector4		Color;
	float		Duration;
	float		Time;

	bool		DepthTest;
};

struct tTask
{
	TASK_TYPE Type;

	// DWORD_PTR
	// �ü���� ���� ũ�Ⱑ �޶����� ���� �ڷ���
	// 64����Ʈ �ü�������� 8byte
	// 32����Ʈ �ü�������� 4byte
	DWORD_PTR Param0;
	DWORD_PTR Param1;
	DWORD_PTR Param2;
};


struct tLight2DInfo
{
	Vector3		vColor;		// ���� ����	
	Vector3		vWorldPos;		// ���� ���� ����
	Vector3		vDir;		// ���� ��ġ
	int			Type;		// ���� Ÿ��(LIGHT_TYPE)
	float		Radius;		// ���� �����ݰ�
	float		Angle;		// ���� ��������	
};



// Particle
struct tParticle
{
	Vector4	vColor;
	Vector3	vLocalPos;
	Vector3	vWorldPos;
	Vector3	vWorldInitScale;
	Vector3	vWorldCurrentScale;
	Vector3	vWorldRotation;

	Vector3	vForce;				// Particle �� ������ ��
	Vector3	vVelocity;

	float	NoiseForceAccTime;	// Noise Force �ҿ� �����ϴ� ���� �ð�
	Vector3	NoiseForceDir;		// Noise Force �� ����

	float	Mass;
	float	Life;				// �ִ� ����
	float	Age;				// ���� ����
	float	NormalizedAge;	// ����ȭ�� ���� -> �ұ�Ģ���� ��ƼŬ ǥ�� ��������

	int		Active;				// ��ƼŬ Ȱ��ȭ
	Vector2	Padding;
};



// Particle Module
struct tParticleModule
{
	// Spawn
	UINT	SpawnRate;				// �ʴ� ��ƼŬ �������� (Spawn Per Second)
	Vector4	vSpawnColor;			// ���� ���� ����
	Vector4	vSpawnMinScale;			// ���� �� �ּ� ũ��
	Vector4	vSpawnMaxScale;			// ���� �� �ִ� ũ��

	float	MinLife;				// �ּ� ����
	float	MaxLife;				// �ִ� ����

	UINT	SpawnShape;				// 0 : Box,  1: Sphere
	Vector3	SpawnShapeScale;		// SpawnShapeScale.x == Radius

	UINT	BlockSpawnShape;		// 0 : Box,  1: Sphere
	Vector3	BlockSpawnShapeScale;	// SpawnShapeScale.x == Radius

	UINT	SpaceType;				// 0 : LocalSpace, 1 : WorldSpace

	// Spawn Burst
	UINT	SpawnBurstCount;		// �ѹ��� �߻���Ű�� Particle ��
	UINT	SpawnBurstRepeat;
	float	SpawnBurstRepeatTime;

	// Add Velocity
	UINT	AddVelocityType;		// 0 : Random, 1 : FromCenter, 2 : ToCenter, 4 : Fixed 
	Vector3	AddVelocityFixedDir;
	float	AddMinSpeed;
	float	AddMaxSpeed;

	// Scale Module
	float	StartScale;
	float	EndScale;

	// Drag Module
	float	DestNormalizedAge;
	float	LimitSpeed;

	// Noise Force Module
	float	NoiseForceTerm;		// Noise Force �����Ű�� ��
	float	NoiseForceScale;	// Noise Force ũ��

	// Render Module
	Vector3	EndColor;			// ���� ����
	UINT	FadeOut;			// 0 : Off, 1 : Normalized Age
	float	StartRatio;			// FadeOut ȿ���� ���۵Ǵ� Normalized Age ����
	UINT	VelocityAlignment;	// �ӵ� ���� 0 : Off, 1 : On


	// Module On / Off
	int		Module[(UINT)PARTICLE_MODULE::END];
};






struct SpriteInfo
{
	Vector2 LeftTopUV;
	Vector2 SliceUV;
	Vector2 BackgroundUV;
	Vector2 OffsetUV;
	int  SpriteUse;
	int  padding[3];
};

struct GlobalData
{
	Vector2 RenderResolution;	// ������ �ػ�
	Vector2 MousePos;	
	Vector2 MouseWorldPos;

	float DeltaTime;			// DT
	float Time;					// �����ð�
	
	float DT_Engine;			// DT
	float Time_Engine;					// �����ð�

	int Light2DCount;			// 2D ���� ����
	int Light3DCount;			// 3D ���� ����
};

extern GlobalData g_Data;




// EDITOR
struct tProperty
{
	string  Type;
	string  Name;
	void* pValue;
};