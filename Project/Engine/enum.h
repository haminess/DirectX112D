#pragma once

enum class CB_TYPE
{
	// ��� ���� �������� ��ȣ
	TRANSFORM, 
	MATERIAL,
	SPRITE,
	GLOBAL,

	END
};

enum ASSET_TYPE
{
	MESH, // �������� �� ���
	MESH_DATA,
	TEXTURE,
	SOUND,
	PREFAB,
	FLIPBOOK,
	SPRITE,
	MATERIAL,
	GRAPHIC_SHADER,
	COMPUTE_SHADER,
	//ANIMATION,
	END,
};
extern const char* ASSET_TYPE_STRING[];

enum class LIGHT_TYPE
{
	DIRECTIONAL,	// ���⼺ ����		�¾籤, ��
	POINT,			// ������			����, ������, ��ų ����Ʈ, ȶ��
	SPOT,			// ����Ʈ ����Ʈ		������, 
	END,
};


enum class COMPONENT_TYPE
{
	TRANSFORM,			// ��ġ, ũ��, ȸ��, �θ��ڽ� ��������
	COLLIDER2D,			// 2D �浹ü
	COLLIDER3D,			// 3D �浹ü
	RIGIDBODY2D,		// 2D �߷�
	FLIPBOOKPLAYER,		// 2D Animation(FlipBook) ���� �� ���
	ANIMATOR3D,			// 3D Animation ���� �� ���
	CAMERA,				// ī�޶� ���
	LIGHT2D,			// 2D ����
	LIGHT3D,			// 3D ����
	STATEMACINE,		// ���� ����

	MESHRENDER,			// �⺻���� ������ ���
	TILEMAP,			// 2D Ÿ�ϱ�� ������
	PARTICLE_SYSTEM,		// ����(Particle) �ùķ��̼� �� ������
	SKYBOX,				// 3D ���
	DECAL,				// ���� üũ, ���� ������
	LANDSCALE,			// 3D ���� �ùķ��̼� �� ������

	END,

	SCRIPT,				// �뺻
};


// ī�޶� ���� �ɼ�(Projection)
enum PROJ_TYPE
{
	PERSPECTIVE,	// ���� ����
	ORTHOGRAPHIC,	// ���� ����
};

// ī�޶� ���� �ɼ�(Projection)
enum class DIR_TYPE
{
	FRONT,
	UP,
	RIGHT,
	END,
};

// Blend State
enum class BS_TYPE
{
	DEFAULT,				// ���� ���
	ALPHABLEND,				// ���İ��� ���� ����
	ALPHABLEND_COVERAGE,	// Alpha Blending + BlendToCoverage
	ONE_ONE,				// 1:1 ����
	END,
};

// DepthStencilState
enum class DS_TYPE
{
	LESS,
	LESS_EQUAL,

	GREATER,			// Decal, Volumesh, �����ִ� ����

	NO_TEST,			// �������� X
	NO_WRITE,			// �������� Less O, ���� ��� X
	NO_TEST_NO_WRITE,	// �������� X, ���̱�� X

	END,
};



// ���̴� ���� �з�
enum class SHADER_DOMAIN
{
	DOMAIN_OPAQUE,			// ������
	DOMAIN_MASK,			// ������ or ����
	DOMAIN_TRANSPARENT,		// ������ + ����
	DOMAIN_PARTICLE,		// ������ + ����, ��ƼŬ ����
	DOMAIN_POSTPROCESS,		// ��ó��

	DOMAIN_NONE,			// ������
};



// RasterizerState Type
enum class RS_TYPE
{
	CULL_BACK,	// �޸� �ø� ���
	CULL_FRONT,	// �ո� �ø� ���
	CULL_NONE,	// �ø����� ����
	WIRE_FRAME,	// WireFrame ���
	END,
};

enum class DEBUG_SHAPE
{
	RECT,
	CIRCLE,
	CROSS,
	LINE,
	TEXT,
	END
};

enum SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
};

enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,

	TEX_ARR_0,
	TEX_ARR_1,
	TEX_ARR_2,
	TEX_ARR_3,

	TEX_CUBE_0,
	TEX_CUBE_1,

	TEX_END,
};


// Task
enum class TASK_TYPE
{
	// 0 : Parent Address, 1 : Child Address
	ADD_CHILD,

	// 0 : Object Address, 1: Layer Index, 2: ChildMove
	CREATE_OBJECT,

	// 0 : Object Address
	DELETE_OBJECT,

	// 0 : Level Address
	CHANGE_LEVEL,

	// 0 : Next State
	CHANGE_LEVEL_STATE,
};


enum class LEVEL_STATE
{
	PLAY,
	PAUSE,
	STOP,
	NONE,
};


enum class PARTICLE_MODULE
{
	SPAWN,
	SPAWN_BURST,
	ADD_VELOCITY,
	SCALE,
	DRAG,
	NOISE_FORCE,
	RENDER,

	END,
};