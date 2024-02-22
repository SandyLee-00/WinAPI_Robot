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
	CAMERA_EFFECT effect; // ī�޶� ȿ��
	float duration; // ȿ�� ���� �ð�
	float curTime; // ī�޶� ȿ��  ���� ����� �ð�
	Vector2 targetPos; // ��Ŀ�� �Ǵ� ���� ��ġ
};

class CameraManager
{
private:
	CameraManager();
	~CameraManager();

	friend class GameProcess;

private:
	/// ��� ����

	Vector2 m_targetLookAt;     // ī�޶� ���ϴ� ��ġ
	Vector2 m_curLookAt;        // ���� ��ġ�� ī�޶� ���� ��ġ
	Vector2 m_preLookAt;        // ī�޶� ���� ���� ������ ��ġ

	Vector2 m_renderGap; // �ػ� �߽���ġ�� ī�޶� LookAt���� ���� ��

	float m_time;    // Ÿ���� ���󰡴µ� �ɸ��� �ð�
	float m_speed;   // Ÿ���� ���󰡴� �ӵ�
	float m_accTime; // �����ð�

	const float m_radian; // ī�޶� ��Ŀ�� ����Ʈ �� �ּ� ũ��

	Texture* m_veilTex; // ī�޶� ������ �ؽ�ó
	Texture* m_lightTex; // ����Ʈ �ؽ�ó
	list<CameraEffect> m_cameraEffectlist; // ī�޶� ȿ�� ����Ʈ

public:
	/// ��� �Լ�
	void SetTargetLookAt(Vector2 _look);
	Vector2 GetCurLookAt() { return m_curLookAt; }
	Vector2 GetRenderPos(Vector2 _objPos) { return _objPos - m_renderGap; }
	Vector2 GetRealPos(Vector2 _renderPos) { return _renderPos + m_renderGap; }

	void Initialize(POINT _resolution, ResourceManager* _resourceManager, PathManager* _pathManager, HDC _mainDC);
	void Update(double _deltaTime, KeyManager* keyManager);
	void Render(HDC _dc);

public:

	// ī�޶� ��Ŀ�� �� �Լ�
	void FocusedOn(float _duration, Vector2 _targetPos);
	// duration �ð����� ������ �����
	void FadeIn(float _duration);

	// ó�� �ƹ��͵� �Ⱥ��̴� ����
	void AllBlack(float _duration);

	// ����ȭ�� �����ߴ� ī�޶�ȿ��
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

