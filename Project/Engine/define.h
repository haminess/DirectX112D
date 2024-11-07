#pragma once


typedef DirectX::SimpleMath::Vector2	Vector2;
typedef DirectX::SimpleMath::Vector3	Vector3;
typedef DirectX::SimpleMath::Vector4	Vector4;
typedef DirectX::SimpleMath::Matrix		Matrix;


#define RELEASE(Inst) if(nullptr != Inst) Inst->Release();

#define DEVICE CDevice::GetInst()->GetDevice().Get()
#define CONTEXT CDevice::GetInst()->GetContext().Get()

#define DELETE(p) if(nullptr != p) {delete p; p = nullptr;}
#define DELETE_ARR(p) if(nullptr != p) {delete[] p; p = nullptr;}

#define MAX_LAYER 32


#define KEY_CHECK(KEY, STATE) CKeyMgr::GetInst()->GetKeyState(KEY) == STATE

#define KEY_TAP(KEY) KEY_CHECK(KEY, KEY_STATE::TAP)
#define KEY_RELEASED(KEY) KEY_CHECK(KEY, KEY_STATE::RELEASED)
#define KEY_PRESSED(KEY) KEY_CHECK(KEY, KEY_STATE::PRESSED)
#define KEY_NONE(KEY) KEY_CHECK(KEY, KEY_STATE::NONE)

#define DT CTimeMgr::GetInst()->GetDeltaTime()
#define GameTime CTimeMgr::GetInst()->GetTime()
#define EngineDT CTimeMgr::GetInst()->GetEngineDeltaTime()

#define Radian(_Degree) _Degree / 180.f * XM_PI
#define Degree(_Radian) _Radian / XI_PI * 180.f

#define CLONE(Type) virtual Type* Clone() { return new Type(*this); }
#define CLONE_DISABLE(Type) virtual Type* Clone() { return nullptr; }\
							Type(const Type& _Other) = delete;