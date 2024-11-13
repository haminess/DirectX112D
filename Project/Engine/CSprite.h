#pragma once
#include "CAsset.h"

#include "CTexture.h"

class CSprite :
	public CAsset
{
private:
	Ptr<CTexture>          m_Atlas;
	Vector2                m_LeftTopUV;
	Vector2                m_SliceUV;

	Vector2                m_BackgroundUV;
	Vector2                m_OffsetUV;

public:
	void SetAtlasTexture(Ptr<CTexture> _Tex);
	void SetLeftTop(Vector2 _LeftTopPixel);
	void SetSlice(Vector2 _SlicePixel);
	void SetBackground(Vector2 _BackgroundPixel);
	void SetOffset(Vector2 _OffsetPixel);

	Ptr<CTexture> GetAtlasTexture() { return m_Atlas; }
	Vector2 GetLeftTop() { return m_LeftTopUV; }
	Vector2 GetSlice() { return m_SliceUV; }
	Vector2 GetBackground() { return m_BackgroundUV; }
	Vector2 GetOffset() { return m_OffsetUV; }

	void Binding();
	static void Clear();


private:
	virtual int Load(const wstring& _FilePath) override;

public:
	virtual int Save(const wstring& _strFilePath) override;

public:
	CLONE_DISABLE(CSprite);
	CSprite(bool _bEngineRes = false);
	~CSprite();

public:
	static Vector2 GetUVFromPixel(Vector2 _Pixel, Vector2 _Resolution);
	static Vector2 GetPixelFromUV(Vector2 _Pixel, Vector2 _Resolution);
};
