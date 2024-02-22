#pragma once

class Texture;
class ResourceManager;

enum class CAMERA_EFFECT
{
	FOCUSED_ON,
	FADE_IN,
	ALLBLACK,
	SHINE_LIGHT,
};
struct CameraEffect
{
	CAMERA_EFFECT effect; // 카메라 효과
	float duration; // 효과 진행 시간
	float curTime; // 카메라 효과  현재 진행된 시간
	Vector2 targetPos; // 포커스 되는 목적 위치
};

class CameraManager
{
private:
	CameraManager();
	~CameraManager();

	friend class GameProcess;

private:
	/// 멤버 변수

	Vector2 m_targetLookAt;     // 카메라가 향하는 위치
	Vector2 m_curLookAt;        // 이전 위치와 카메라 보정 위치
	Vector2 m_preLookAt;        // 카메라가 보는 이전 프레임 위치

	Vector2 m_renderGap; // 해상도 중심위치와 카메라 LookAt간의 차이 값

	float m_time;    // 타겟을 따라가는데 걸리는 시간
	float m_speed;   // 타겟을 따라가는 속도
	float m_accTime; // 누적시간

	const float m_radian; // 카메라 포커스 이펙트 원 최소 크기

	Texture* m_veilTex; // 카메라 가림막 텍스처
	Texture* m_lightTex; // 라이트 텍스처
	list<CameraEffect> m_cameraEffectlist; // 카메라 효과 리스트

public:
	/// 멤버 함수
	void SetTargetLookAt(Vector2 _look);
	Vector2 GetCurLookAt() { return m_curLookAt; }
	Vector2 GetRenderPos(Vector2 _objPos) { return _objPos - m_renderGap; }
	Vector2 GetRealPos(Vector2 _renderPos) { return _renderPos + m_renderGap; }

	void Initialize(POINT _resolution, ResourceManager* _resourceManager, PathManager* _pathManager, HDC _mainDC);
	void Update(double _deltaTime, KeyManager* keyManager);
	void Render(HDC _dc);

public:

	// 카메라 포커스 온 함수
	void FocusedOn(float _duration, Vector2 _targetPos);
	// duration 시간으로 서서히 밝아짐
	void FadeIn(float _duration);

	// 처음 아무것도 안보이는 상태
	void AllBlack(float _duration);

	// 메인화면 빛비추는 카메라효과
	void ShineLight(float _duration);

	void ClearCameraEffectList();

private:
	void CalculateRenderGap();

	void ShineLightUpdate(KeyManager* keyManager);
	void ShineLightRender(HDC _dc);

	void AllBlackUpdate();
	void AllBlackRender(HDC _dc);

	void FadeInUpdate();
	void FadeInRender(HDC _dc);

	void FocusedOnUpdate();
	void FocusedOnRender(HDC _dc);

};

