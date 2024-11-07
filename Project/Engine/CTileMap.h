#pragma once
#include "CRenderComponent.h"

#include "CTexture.h"

struct tTileInfo
{
    Vector2 LeftTopUV;
    Vector2 SliceUV;
};


class CStructuredBuffer;
class CTileMap :
    public CRenderComponent
{
private:
    // 타일 행렬 개수
    int            m_Col;
    int            m_Row;

    // 타일 1개의 크기
    float           m_TileSizeX;
    float           m_TileSizeY;

    // 타일 아틀라스
    Ptr<CTexture>   m_TileAtlas;

    // 아틀라스 텍스쳐에서 타일 1개가 차지하는 크기(Pixel)
    UINT            m_TileAtlasSizeX;
    UINT            m_TileAtlasSizeY;

    // 아틀라스 텍스쳐에서 타일이 가로 세로 행렬 개수
    UINT            m_TileAtlasCol;
    UINT            m_TileAtlasRow;

    vector<tTileInfo>   m_vecTileInfo;  // 타일 데이터  
    CStructuredBuffer* m_Buffer;       // 타일 데이터를 GPU 로 전달할 때 사용 할 버퍼


public:
    void SetTileColRow(UINT _Col, UINT _Row);
    void SetTileSize(float _TileSizeX, float _TileSizeY);
    void SetAtlasTexture(Ptr<CTexture> _Tex) { m_TileAtlas = _Tex; }
    void SetTileAtlasSize(UINT _X, UINT _Y);
    void SetTileAtlasColRow(UINT _Col, UINT _Row);

private:
    void CalcObjectScale();

public:
    virtual void Init() override;
    virtual void FinalTick() override {};
    virtual void Render() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;


public:
    CLONE(CTileMap);
    CTileMap();
    CTileMap(const CTileMap& _Origin);
    ~CTileMap();
};

