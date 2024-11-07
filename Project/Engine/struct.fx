#ifndef _STRUCT
#define _STRUCT

struct tLight2DInfo
{
    float3  vColor;     // ���� ����	
    float3  vWorldPos;  // ���� ��ġ
    float3  vDir;       // ���� ���� ����
    int     Type;       // ���� Ÿ��(LIGHT_TYPE)
    float   Radius;     // ���� �����ݰ�
    float   Angle;      // ���� ��������	  
};


// engine cpp ����ü�� ������� ��
struct tParticle
{
    float4  vColor;
    float3  vLocalPos;
    float3  vWorldPos;
    float3  vWorldInitScale;
    float3  vWorldCurrentScale;
    float3  vWorldRotation;
    
    float3  vForce;
    float3  vVelocity;
        
    float   NoiseForceAccTime;  // Noise Force �ҿ� �����ϴ� ���� �ð�
    float3  NoiseForceDir;      // ����� Noise Forec �� ����
            
    float   Mass;
    float   Life;
    float   Age;
    float   NormalizedAge;
    int     Active;
    
    float2  padding;
};

struct tParticleModule
{
    // [Create Module]
    // Spawn
    uint    SpawnRate;          // �ʴ� ��ƼŬ ���� ����
    float4  vSpawnColor;        // ���� ���� ����
    float4  vSpawnMinScale;     // ���� �� �ּ� ũ��
    float4  vSpawnMaxScale;     // ���� �� �ִ� ũ��

    float   MinLife;            // �ּ� ����
    float   MaxLife;            // �ִ� ����
    
    //  �� ������ ��ƼŬ ���� ����
    uint    SpawnShape;             // 0 : Box,  1: Sphere
    float3  SpawnShapeScale;        // SpawnShapeScale.x == Radius
    
    uint    BlockSpawnShape;        // 0 : Box,  1: Sphere
    float3  BlockSpawnShapeScale;   // SpawnShapeScale.x == Radius    
    
    //  ���ڰ� ������Ʈ ��ġ�� ������ �޴���, ���忡 ���������� �����ϳ�
    uint    SpaceType; // 0 : LocalSpcae, 1 : WorldSpace
    
        
        
    //  [Create Module]
    //  Spawn Burst
    //  ���������� ������ ����Ʈ
    uint    SpawnBurstCount; // �ѹ��� �߻���Ű�� Particle ��
    uint    SpawnBurstRepeat;
    float   SpawnBurstRepeatTime;
    
    //  Add Velocity
    //  ������� ����Ʈ
    uint    AddVelocityType;    // 0 : Random, 1 : FromCenter, 2 : ToCenter, 4 : Fixed ����
    float3  AddVelocityFixedDir;
    float   AddMinSpeed;
    float   AddMaxSpeed;
    
    //  Scale Module
    //  ���� ���� ���� ũ��, ���� ���� ũ��
    float   StartScale;
    float   EndScale;
    
    //  Drag Module
    //  ���� ���, age���� speed�� �ǵ��� ����
    float   DestNormalizedAge;
    float   LimitSpeed;
    
    //  Noise Force Module
    //  ������ ����
    float   NoiseForceTerm;     // Noise Force �����Ű�� ��
    float   NoiseForceScale;    // Noise Force �� ũ��
       
    //  Render Module
    float3  EndColor;   // ���� ����
    uint    FadeOut;    // 0 : Off, 1 : Normalized Age
    float   StartRatio; // FadeOut ȿ���� ���۵Ǵ� Normalized Age ����
    //  ������ �����ϰ� ���� �������� ȸ��
    //  �� ��� �޽ô� ���ڷ� �����ؼ� ��ü������ ���� �� �ֵ��� ��
    uint    VelocityAlignment; // �ӵ� ���� 0 : Off, 1 : On
    
	//  Module On / Off
    //  ����� �Ѿ���
    int     Module[7];
};

#endif