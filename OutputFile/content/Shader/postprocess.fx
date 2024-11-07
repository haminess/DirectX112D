#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "value.fx"

// Vertex Shader 
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

// Mesh : RectMesh

VS_OUT VS_Post(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    // ����Ÿ���� �ι踦 ���� ȭ�� ��ü�� ����� �� �ֵ��� ��
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
        
    return output;
}

float4 PS_Post(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    // ȸ�� ȿ��
    float Aver = (vColor.r + vColor.g + vColor.b) / 3.f;
    vColor.rgb = Aver;
        
    return vColor;
}



VS_OUT VS_Screen(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    // ��ó�� �ؽ��İ� ���� ��ǥ�� �ݿ���
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
        
    return output;
}

float4 PS_Screen(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        
    return vColor;
}




// Mesh : RectMesh
// ��ó���� ����� ��ġ�� ���� ��µǴ� �׷��Ȱ� �Ȱ��� �������� �ʹ�
// - �ʿ��� ����
// 1. �ȼ� ���̴��� �ȼ� ��ǥ (ȭ�� �������� ȣ��� �ȼ� ��ġ)
// 2. ���� Ÿ�� �ػ� (��ü ȭ�� ũ��)
// - ���� ���
// ��ü ȭ�� ���(����Ÿ��) �ȼ��� ��ġ(0~1)�� ȭ���� 1�� ����ȭ
// �ȼ� ��ǥ�� ������ UV ��ǥ�� ����Ÿ�ٿ��� ���ø�
VS_OUT VS_Distortion(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    // ��ó�� �ؽ��İ� ���� ��ǥ�� �ݿ���
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    //output.vPosition = float4(_in.vPos, 1.f);
    output.vUV = _in.vUV;
        
    return output;
}


float4 PS_Distortion(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    // ������ζ�� NDC ��ǥ���� ���� ��ǥ�� �����Ǿ� ���´�.
    // SV_Position -> Pixelshader (�ȼ���ǥ)
    // ������ SV_Position �ø�ƽ�� ����ϸ� �ȼ� ��ǥ�� ���´�.
    _in.vPosition;
    
    // RenderTarget���� ������ �ȼ� �ڽ��� ��ġ�� ���Ѵ�.
    // uv ��ǥ�� ������
    float2 vScreenUV = _in.vPosition.xy / g_RenderResolution;
    float y = vScreenUV.y;
    
    // NoiseTexture �� ������
    if (g_btex_1)
    {
        // ������ �ؽ�ó���� ���� �� �޾ƿ���
        float4 vNoise = g_tex_1.Sample(g_sam_0, vScreenUV /*0~1*/ + 0.05f * g_Time);
        // ��ġ ����
        vNoise -= 0.5f;
        // ���� ����
        vNoise *= 0.05f;
        
        vScreenUV += vNoise.xy;
    }
    
    vColor = g_tex_0.Sample(g_sam_0, vScreenUV);
    
    return vColor;
}




struct VS_VortexOut
{
    float4 vPosition : SV_Position;
    float2 vObjectUV : TEXCOORD;
};


VS_VortexOut VS_Vortex(VS_IN _in)
{
    VS_VortexOut output = (VS_VortexOut) 0.f;
            
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
        
    float4 vProjPos = mul(float4(0.f, 0.f, 0.f, 1.f), g_matWVP);
    vProjPos.xyz = vProjPos.xyz / vProjPos.w;
    
    output.vObjectUV.x = (vProjPos.x + 1.f) / 2.f;
    output.vObjectUV.y = 1.f - ((vProjPos.y + 1.f) / 2.f);
        
    return output;
}


float4 PS_Vortex(VS_VortexOut _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    float effectRadius = 0.1f;
    float effectAngle = 1.f * PI;
    
    float2 center = _in.vObjectUV;
    
    float2 uv = (_in.vPosition.xy / g_RenderResolution.xy) - center;
    
    float len = length(uv * float2(g_RenderResolution.x / g_RenderResolution.y, 1.));
    float angle = atan2(uv.y, uv.x) + effectAngle * smoothstep(effectRadius, 0., len);
    float radius = length(uv);

    vColor = g_tex_0.Sample(g_sam_0, float2(radius * cos(angle), radius * sin(angle)) + center);
    
    return vColor;
}



VS_OUT VS_Enlarge(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    // ����Ÿ���� �ι踦 ���� ȭ�� ��ü�� ����� �� �ֵ��� ��
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
        
    return output;
}

float4 PS_Enlarge(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    // ���콺 ��ǥ
    float2 vMouse = g_vec2_0;
    float2 vMouseWorld = g_vec2_1;

    // �ȼ� ��ǥ
    float2 vScreen = _in.vPosition.xy;
    
    // Ÿ�� ��ǥ
    float vTargetDis = distance(g_vec2_2, vMouseWorld);
    float vTargetDis2 = distance(g_vec2_3, vMouseWorld);
    
    float dis = distance(vMouse, vScreen);
    
    vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    if (dis <= 100.f)
    {
        float2 vecOut = vScreen - vMouse;
	
        vColor = g_tex_0.Sample(g_sam_0, (vMouse + vecOut / 2) / g_RenderResolution);
        
        if (vTargetDis < 15.f)
            vColor.r += (0.5f + 0.5f * sin(g_Time * 10.f));
        if (vTargetDis2 < 15.f)
            vColor.b += (0.5f + 0.5f * sin(g_Time * 10.f));
    }
    else if (dis < 150.f)
    {
        float2 vecOut = vScreen - vMouse;
        float largeScale = 1 + cos((PI / 2) * (dis - 100.f) / 50.f); // 2~1
	
        vColor = g_tex_0.Sample(g_sam_0, (vMouse + vecOut / largeScale) / g_RenderResolution);
        
        if (vTargetDis < 15.f)
            vColor.r += (0.5f + 0.5f * sin(g_Time * 10.f));
        if (vTargetDis2 < 15.f)
            vColor.b += (0.5f + 0.5f * sin(g_Time * 10.f));
    }
    
    
    return vColor;
}


VS_OUT VS_Wave(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    // ����Ÿ���� �ι踦 ���� ȭ�� ��ü�� ����� �� �ֵ��� ��
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
        
    return output;
}

float4 PS_Wave(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    // ���콺 ��ǥ
    float2 vMouse = g_vec2_0;
    float2 vMouseWorld = g_vec2_1;

    // �ȼ� ��ǥ
    float2 vScreen = _in.vPosition.xy;
    
    // Ÿ�� ��ǥ
    float vTargetDis = distance(g_vec2_2, vMouseWorld);
    float vTargetDis2 = distance(g_vec2_3, vMouseWorld);
    
    float dis = distance(vMouse, vScreen);
    
    vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    float Radius = 150.f;
    if (dis < g_float_0 && g_float_0)
    {
        float2 vecOut = vScreen - vMouse;
        
        float r = sqrt(dis);
        float z = sin(dis * 0.1 - g_Time * 5);
        
        
        vColor = g_tex_0.Sample(g_sam_0, (vMouse + vecOut * z) / g_RenderResolution);
    }
    
    
    
    return vColor;
}



VS_OUT VS_Water(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    // ����Ÿ���� �ι踦 ���� ȭ�� ��ü�� ����� �� �ֵ��� ��
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
        
    return output;
}

float4 PS_Water(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    // ���콺 ��ǥ
    float2 vMouse = g_vec2_0;
    float2 vMouseWorld = g_vec2_1;

    // �ȼ� ��ǥ
    float2 vScreen = _in.vPosition.xy;
    
    // Ÿ�� ��ǥ
    float vTargetDis = distance(g_vec2_2, vMouseWorld);
    float vTargetDis2 = distance(g_vec2_3, vMouseWorld);
    
    float dis = distance(vMouse, vScreen);
    
    vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    if (dis < 150.f)
    {
        float2 vecOut = vScreen - vMouse;
        
        float r = sqrt(dis);
        float z = /*1.0 +*/ 0.05 * sin((dis + g_Time * 0.035) / 0.013);
        
        
        vColor = g_tex_0.Sample(g_sam_0, vecOut * z);
    }
    
    
    
    return vColor;
}




#endif