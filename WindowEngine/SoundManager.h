#pragma once

enum class SOUND_TYPE;

/// <summary>
/// ���� : ���� ����
/// fmod.hpp�� FMOD (C++ ��Ÿ��) ���
/// ����� : ���ؿ�
/// </summary>
class SoundManager
{
	SoundManager();
	~SoundManager();

	friend class GameProcess;

private:
	map<wstring, Sound*> m_mapSfx;

	// FMOD ���� ����
	// FMOD : ����� ó�� ���̺귯��
	FMOD::System * m_system;
	FMOD::Channel* m_channel[(int)SOUND_TYPE::END];
	FMOD::ChannelGroup* m_ovenGroup;

public:
	void Initialize(PathManager* pathManager);

	// �̸��� ��θ� ���ڷ� �޾� ���� ������ �ε��ϰ� map�� ����
	void LoadSound(PathManager* pathManager, const wstring& _key, const string& _relativePath, bool _isLoop = false);
	void PlaySound(const wstring& _key, SOUND_TYPE _type, float _volume = 0.25f);
	void SetLoopCount(SOUND_TYPE _type, int _loopCount);

	// Pause Func
	void Player_Pause();
	void Boss_Pause();
	void Order_Pause();
	void Oven1_Pause();
	void Oven2_Pause();
	void Oven3_Pause();
	void BGM_Pause();
	void UI_Pause();
	void Page_Pause();
	void OverCook_Pause();
	void GameOver_Pause();

	bool CheckUsingChannel(SOUND_TYPE _type);
};