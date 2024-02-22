#pragma once

enum class SOUND_TYPE;

/// <summary>
/// 역학 : 사운드 관리
/// fmod.hpp의 FMOD (C++ 스타일) 사용
/// 담당자 : 공준열
/// </summary>
class SoundManager
{
	SoundManager();
	~SoundManager();

	friend class GameProcess;

private:
	map<wstring, Sound*> m_mapSfx;

	// FMOD 관련 변수
	// FMOD : 오디오 처리 라이브러리
	FMOD::System * m_system;
	FMOD::Channel* m_channel[(int)SOUND_TYPE::END];
	FMOD::ChannelGroup* m_ovenGroup;

public:
	void Initialize(PathManager* pathManager);

	// 이름과 경로를 인자로 받아 사운드 파일을 로드하고 map에 넣음
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