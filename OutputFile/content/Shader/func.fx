#ifndef _FUNC
#define _FUNC

#include "value.fx"
#include "struct.fx"

// missing �� ���� ó��
// �ؽ��� �����Ͱ� ���ε� �ȵ����� 
// ����Ÿ, ȸ�� ��� Debug�� ������ ��� ��
// 1. shader �󿡼� �ذ�
// 2. ������ ���ؼ� �ذ�
    
// g_tex_0 texture �� ���ε� �Ǿ�����, �ؽ��ĸ� ���ø�
// ���ε� �Ǿ����� ������ ����Ÿ, ȸ�� ��� Debug �� ������ ��� ��
float4 GetDebugColor(float2 _UV, int _Level)
{
    float2 vTexPos = _UV * _Level;
    
    int Col = floor(vTexPos.x);
    int Row = floor(vTexPos.y);
        
    // Ȧ��
    if (Col % 2)
    {
        if (Row % 2)
            return float4(1.f, 0.f, 1.f, 1.f);
        else
            return float4(0.f, 0.f, 0.f, 1.f);
    }
    // ¦��
    else
    {
        if (Row % 2)
            return float4(0.f, 0.f, 0.f, 1.f);
        else
            return float4(1.f, 0.f, 1.f, 1.f);
    }
}

void CalcLight2D(int _LightIdx, float3 _WorldPos, inout float3 _LightColor)
{
    float DistRatio = 1.f;
    
    // ����ó��
    if (g_Light2DInfo[_LightIdx].Type == 0) // DirLight
    {
        _LightColor += g_Light2DInfo[_LightIdx].vColor;
    }
    else if (g_Light2DInfo[_LightIdx].Type == 1) // PointLight
    {
        float Dist = distance(_WorldPos.xy, g_Light2DInfo[_LightIdx].vWorldPos.xy);
        if (Dist <= g_Light2DInfo[_LightIdx].Radius)
        {
            //DistRatio = saturate(1.f - (Dist / g_Light2DInfo[_LightIdx].Radius));
            DistRatio = saturate(cos(saturate((Dist / g_Light2DInfo[_LightIdx].Radius)) * (PI / 2.f)));
            
            _LightColor += g_Light2DInfo[_LightIdx].vColor * DistRatio;
        }
    }
    else // SpotLight
    {
        float Dist = distance(_WorldPos, g_Light2DInfo[_LightIdx].vWorldPos);
        
        float2 vecPixel = normalize(_WorldPos.xy - g_Light2DInfo[_LightIdx].vWorldPos.xy);
        float2 vecDir = g_Light2DInfo[_LightIdx].vDir.xy;
        
        // �ȼ� ������
        float angle = dot(vecDir, vecPixel);
        
        // ���� ������
        float rangeAngle = cos(radians(g_Light2DInfo[_LightIdx].Angle / 2));
   
        if (angle >= rangeAngle)
        {
            if (Dist <= g_Light2DInfo[_LightIdx].Radius)
            {
                DistRatio = saturate(cos(saturate((Dist / g_Light2DInfo[_LightIdx].Radius)) * (PI / 2.f)));
                _LightColor += g_Light2DInfo[_LightIdx].vColor * DistRatio;
            }
        }
    }
}


float3 GetRandom(in Texture2D _NoiseTexture, uint _ID, uint _maxId)
{
    float2 vUV = (float2) 0.f;
    // ������ ���̵� ����ȭ(0~1)�ؼ� uv�� �����ϸ�
    // �������� ���������� ���ø���    
    vUV.x = ((float) _ID / (float) (_maxId - 1)) + g_Time_Engine * 0.01f;
    vUV.y = sin(vUV.x * 20 * PI) * 0.5f + g_Time_Engine * 0.1f;
    float3 vRandom = _NoiseTexture.SampleLevel(g_sam_0, vUV, 0).xyz;
    
    return vRandom;
}




#endif