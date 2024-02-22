#include "pch.h"
#include "GameProcess.h"
#include "CameraMgr.h"
#include "TimeManager.h"
#include "KeyManager.h"
#include "ResourceManager.h"
#include "SelectGDI.h"
#include "Texture.h"

CameraManager::CameraManager()
	:m_renderGap{}
	,m_curLookAt{}
	, m_targetLookAt{}
	, m_preLookAt{}
	, m_time(0.f)    // 타겟을 
   , m_speed(0.f)   // 타겟을 따라가는 속도 
   , m_accTime(0.f) // 누적시간
	,m_radian(250.f) // 카메라 포커스 이펙트 원 최소 크기
	,m_veilTex(nullptr)
	,m_lightTex(nullptr)
{

}

CameraManager::~CameraManager()
{
}

// _look 위치 받아서 타겟으로 설정, 타겟을 따라가는데 걸리는 시간 설정
void CameraManager::SetTargetLookAt(Vector2 _look)
{
	m_targetLookAt = _look;
	float moveDist = (m_targetLookAt - m_preLookAt).Size();
	m_speed = moveDist / m_time;
	m_accTime = 0.f;
}

void CameraManager::Initialize(POINT _resolution, ResourceManager* _resourceManager, PathManager* _pathManager,HDC _mainDC)
{
	// 가림막 텍스처 생성
	m_veilTex = _resourceManager->CreateTexture(L"CameraVeil", (UINT)_resolution.x, (UINT)_resolution.y, _mainDC);

	// 라이트 텍스처 받아오기
	m_lightTex = _resourceManager->LoadTexture(L"LightTex", L"\\Texture\\Camera\\light.bmp", _pathManager, _mainDC);
}

void CameraManager::Update(double _deltaTime,KeyManager* keyManager)
{
	if (m_cameraEffectlist.empty())
		return;

	CameraEffect& effect = m_cameraEffectlist.front();
	effect.curTime += static_cast<float>(_deltaTime);
	


	switch (effect.effect)
	{
	case CAMERA_EFFECT::FOCUSED_ON:
	{
		FocusedOnUpdate();
	}
	break;
	case CAMERA_EFFECT::ALLBLACK:
	{
		AllBlackUpdate();
	}
	case CAMERA_EFFECT::FADE_IN:
	{
		FadeInUpdate();
	}
	break;
	case CAMERA_EFFECT::SHINE_LIGHT:
	{
		ShineLightUpdate(keyManager);
	}
	break;
	default:
		break;
	}

}

void CameraManager::Render(HDC _dc)
{
	if (m_cameraEffectlist.empty())
		return;
	 
	CameraEffect& effect = m_cameraEffectlist.front();

	switch (effect.effect)
	{
	case CAMERA_EFFECT::FOCUSED_ON:
	{
		FocusedOnRender(_dc);
	}
	break;
	case CAMERA_EFFECT::FADE_IN:
	{
		FadeInRender(_dc);
	}
	break;
	case CAMERA_EFFECT::ALLBLACK:
	{
		AllBlackRender(_dc);
	}
	case CAMERA_EFFECT::SHINE_LIGHT:
	{
		ShineLightRender(_dc);
	}
	break;
	default:
		break;
	}

	if (effect.duration < effect.curTime)
	{
		m_cameraEffectlist.pop_front();
	}
}

void CameraManager::FocusedOn(float _duration, Vector2 _targetPos)
{
	assert(_duration > 0.f);

	CameraEffect effect = {};
	effect.effect = CAMERA_EFFECT::FOCUSED_ON;
	effect.curTime = 0.f;
	effect.duration = _duration;
	effect.targetPos = _targetPos;

	m_cameraEffectlist.push_back(effect);
}

void CameraManager::FadeIn(float _duration)
{
	assert(_duration > 0.f);

	CameraEffect effect = {};
	effect.effect = CAMERA_EFFECT::FADE_IN;
	effect.curTime = 0.f;
	effect.duration = _duration;
	// FadeIn 에서는 아래 인자는 사용하지 않음
	effect.targetPos = Vector2(0.f,0.f);

	// 검정색으로 텍스처를 채워두기
	SelectGDI brush(m_veilTex->GetDC(), BRUSH_TYPE::BLACK);
	Rectangle(m_veilTex->GetDC(), 0, 0, m_veilTex->GetWidth(), m_veilTex->GetHeight());

	m_cameraEffectlist.push_back(effect);
}

void CameraManager::AllBlack(float _duration)
{
	assert(_duration > 0.f);

	CameraEffect effect = {}; 
	effect.effect = CAMERA_EFFECT::ALLBLACK;
	effect.curTime = 0.f;
	effect.duration = _duration;
	effect.targetPos = Vector2(0.f, 0.f);

	// 검정색으로 텍스처를 채워두기
	SelectGDI brush(m_veilTex->GetDC(), BRUSH_TYPE::BLACK);
	Rectangle(m_veilTex->GetDC(), 0, 0, m_veilTex->GetWidth(), m_veilTex->GetHeight());

	m_cameraEffectlist.push_back(effect);
}

void CameraManager::ShineLight(float _duration)
{
	assert(_duration > 0.f);

	CameraEffect effect = {};
	effect.effect = CAMERA_EFFECT::SHINE_LIGHT;
	effect.curTime = 0.f;
	effect.duration = _duration;
	// 오프셋
	effect.targetPos = Vector2(290.f, 290.f);

	m_cameraEffectlist.push_back(effect);
}

void CameraManager::ClearCameraEffectList()
{
	m_cameraEffectlist.clear();
}

// 
void CameraManager::CalculateRenderGap()
{
	// Normalize 한 값이 1보다 크면 m_curLookAt를 이동한다 
	//Vector2 lookDir = m_targetLookAt - m_preLookAt;
	//if (lookDir.Size() > 1.f)
	//{
	//	if (!(lookDir.x == 0 && lookDir.y == 0))
	//	{
	//		lookDir.Normalize();
	//	}
	//	m_curLookAt = m_preLookAt + m_targetLookAt * (m_speed * (float)TimeManager::GetInst()->GetDeltaTime());
	//}
	//else
	//{
	//	m_curLookAt = m_targetLookAt;
	//}

	//// 해상도 center에서 카메라가 보는 위치를 빼면 카메라가 보정해야할 위치가 나온다
	////POINT resPoint = GameProcess::GetInst()->GetResolution();
	//Vector2 resolution = Vector2((float) resPoint.x, (float)resPoint.y);
	//Vector2 center = resolution / 2.f;
	//m_renderGap = m_curLookAt - center;
	//m_preLookAt = m_curLookAt;
}

void CameraManager::ShineLightUpdate(KeyManager* keyManager)
{
	// 검정색으로 텍스처를 채워두기
	SelectGDI brush(m_veilTex->GetDC(), BRUSH_TYPE::BLACK);
	Rectangle(m_veilTex->GetDC(), 0, 0, m_veilTex->GetWidth(), m_veilTex->GetHeight());

	CameraEffect& effect = m_cameraEffectlist.front();
	Vector2 offset = effect.targetPos;

	// 마우스 위치에 따라서 회전하는 각도 값을 설정
	Vector2 mousePos = keyManager->GetMousePos();
	const Vector2 middlePos{ 960.f, -500.f };

	// 시선 벡터 = 위치- 원점
	Vector2 gaze = mousePos - middlePos;

	// 마우스와 중심좌표의 각도 윈도우는 y좌표가 반대이므로 뒤집는다
	gaze.y *= -1.f;

	float gazeRadian = gaze.Angle();
	//gazeRadian = Math::Clamp<float>(gazeRadian, -Math::HalfPI, 0.f);

	// 실제 움직이는 각도 
	float moveRadian = Math::HalfPI + gazeRadian;
	moveRadian *= -1.f;

	moveRadian = Math::Clamp<float>(moveRadian, -Math::HalfPI, Math::HalfPI);

	// 각도에따라서 그려주는 RECT를 계산한다.
	Vector2 rectVector[3]{};
	rectVector[0].x = -offset.x;
	rectVector[0].y = -offset.y;

	rectVector[1].x = -offset.x + static_cast<float>(m_lightTex->GetWidth());
	rectVector[1].y = -offset.y;

	rectVector[2].x = -offset.x;
	rectVector[2].y = -offset.y + static_cast<float>(m_lightTex->GetHeight());

	rectVector[0] = Vector2::RotateRadian(rectVector[0], middlePos, moveRadian);
	rectVector[1] = Vector2::RotateRadian(rectVector[1], middlePos, moveRadian);
	rectVector[2] = Vector2::RotateRadian(rectVector[2], middlePos, moveRadian);

	// 0: 왼쪽 위 1:오른쪽 위, 2. 왼쪽 아래
	POINT rect[3]{};

	rect[0].x = static_cast<LONG>(rectVector[0].x);
	rect[0].y = static_cast<LONG>(rectVector[0].y);
	rect[1].x = static_cast<LONG>(rectVector[1].x);
	rect[1].y = static_cast<LONG>(rectVector[1].y);
	rect[2].x = static_cast<LONG>(rectVector[2].x);
	rect[2].y = static_cast<LONG>(rectVector[2].y);

	// TODO: m_veilTex 에 PlgBlt으로 m_lightTex를 그린다
	PlgBlt(m_veilTex->GetDC(), rect, m_lightTex->GetDC()
		, 0, 0, m_lightTex->GetWidth(), m_lightTex->GetHeight(), 0, 0, 0);
}

void CameraManager::ShineLightRender(HDC _dc)
{
	UINT height = m_veilTex->GetHeight();
	UINT width = m_veilTex->GetWidth();

	BLENDFUNCTION blendFunc{};
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.BlendFlags = 0;
	blendFunc.SourceConstantAlpha = 40;  // 알파값(0~255), 0이면 완전 투명, 255이면 불투명
	blendFunc.AlphaFormat = 0; // 여기 값을 0이다!!!!!!!1

	AlphaBlend(_dc, 0, 0,
		width, height, m_veilTex->GetDC()
		, 0, 0, width, height, blendFunc);

}

void CameraManager::AllBlackUpdate()
{
	// 아무것도 안함
}

void CameraManager::AllBlackRender(HDC _dc)
{
	int width= m_veilTex->GetWidth();
	int height = m_veilTex->GetHeight();

	BitBlt(_dc, 0, 0, width, height, m_veilTex->GetDC(), 0, 0, SRCCOPY);
}

void CameraManager::FadeInUpdate() 
{
	// 아무것도 안함
}

void CameraManager::FadeInRender(HDC _dc)
{
	UINT height =m_veilTex->GetHeight();
	UINT width =m_veilTex->GetWidth();

	float curTime = m_cameraEffectlist.front().curTime;
	float duration = m_cameraEffectlist.front().duration;

	float ratio = 1.f - (curTime / duration);
	int alpha = static_cast<int>(ratio * 255);
	alpha = Math::Clamp<int>(alpha, 40, 255);

	BLENDFUNCTION blendFunc{};
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.BlendFlags = 0;
	blendFunc.SourceConstantAlpha = alpha;  // 알파값(0~255), 0이면 완전 투명, 255이면 불투명
	blendFunc.AlphaFormat = 0; // 여기 값을 0이다!!!!!!!1

	AlphaBlend(_dc, 0, 0,
		width, height, m_veilTex->GetDC()
		, 0, 0, width, height, blendFunc);

}

void CameraManager::FocusedOnUpdate()
{
	CameraEffect& effect = m_cameraEffectlist.front();
	// 먼저 검정색으로 초기화
	{
		SelectGDI brush(m_veilTex->GetDC(), BRUSH_TYPE::BLACK);
		Rectangle(m_veilTex->GetDC(), 0, 0, m_veilTex->GetWidth(), m_veilTex->GetHeight());
	}

	// 원의 반지름 계산
	float ratio = effect.curTime / effect.duration;

	const float minimum = 0.05f;
	const float maximum = 0.7f;

	ratio =Math::Clamp<float>(ratio, minimum, maximum);

	// 원의 반지름
	float radian = m_radian / ratio;
	
	// 원의 원점 좌표
	Vector2 pos = effect.targetPos;

	// 목표 크기만큼 마젠타생상으로 원을 그린다.
	{
		SelectGDI brush(m_veilTex->GetDC(), BRUSH_TYPE::MAGENTA);

		Vector2 lefttop{ pos.x-radian*0.5f,pos.y-radian * 0.5f };
		Vector2 rightbottom{ pos.x + radian * 0.5f, pos.y + radian * 0.5f };

		Ellipse(m_veilTex->GetDC(), (int)lefttop.x, (int)lefttop.y, (int)rightbottom.x, (int)rightbottom.y);
	}

}

void CameraManager::FocusedOnRender(HDC _dc)
{
	TransparentBlt(_dc, 0, 0, (int)m_veilTex->GetWidth(), (int)m_veilTex->GetHeight(), m_veilTex->GetDC()
		, 0, 0, (int)m_veilTex->GetWidth(), (int)m_veilTex->GetHeight(), RGB(255, 0, 255));
}
