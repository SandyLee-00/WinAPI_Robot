#pragma once

/// <summary>
/// ������� �����ϱ� ���� �޴���
/// ����� ���� �ֱ�, �̵� ��� �� ����
/// </summary>
class BossManager
{
	BossManager();
	~BossManager();

	// GameProcess���� BossManager�� ����� ������ �� ����
	// private �����ڵ� �θ� �� ����
	friend class GameProcess;

private:

	// ����� ���� Ÿ�̸�
	static double m_bossTimer;

	PathPoint m_start; // ����� ���� ����
	PathPoint m_end; // ����� ���� ���� ����
	vector<PathPoint> m_patrolPath[(int)PATH_TYPE::END]; // ��ȸ ���

	/// ��δ� �ϳ��� path �������� path�� points
	UINT m_pathIndex; // ��� index
	UINT m_destIndex; // ������ index

	Vector2 m_destination; // ����� ��ǥ ����

	// �⺻ Destination Point
	PathPoint m_point01;
	PathPoint m_point02;
	PathPoint m_point03;
	PathPoint m_point04;

	UINT m_windowPeriod; // ó�� â���� ������� ����������� �ð�
	UINT m_appearPeriod; // â������ ���̰� ���� ���忡 ������ ������ �ð�
	UINT m_disAppearPeriod; // â������ ������� Up �ϰ� ������� ������ �ð�

	double m_halfPercent;

	bool m_isAppear; // ����� ���� ����

	/// �ִϸ��̼� �� ���� ���
	bool m_isUpWindow; // ������� â ���� �ö���� �ִϸ��̼� ��� ����
	bool m_isWalkingWindow; // ������� â ���� �ö���� �ִϸ��̼� ��� ����

public:

	void Initialize();
	void Update(double deltaTime, GameProcess* _gameProcess);

	Vector2 GetDestPos() { return m_destination; }

	// ���� ������ Ž�� �� ����
	bool FindNextDest();

	// ���� ����
	void CreateBoss(GameProcess* _gameProcess);

	// ���� ����
	void DisappearBoss();

	// ���� ��� index ����
	UINT GetRandPathIndex();

	// ���� â�� ����� ���� �ֱ� ����
	UINT GetRandWindowPeriod();

	// 0.0 ~ 1.0 ������ ���� ��ȯ �Լ�
	double GetHalfPercent();
};