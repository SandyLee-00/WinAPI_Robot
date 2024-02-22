#pragma once

/// <summary>
/// 역할 : 파일 경로 제어 (리소스 파일의 경로를 상대경로로 나타냄)
/// 담당자 : 공준열
/// </summary>
class PathManager
{
	PathManager();
	~PathManager();

	friend class GameProcess;

private: // member variable

	// sz : NULL로 끝나는 문자열 (헝가리안 표기법)
	// wchar_t : 2바이트의 크기, 유니코드 표현가능 (경로에 한글이 포함되어 있을 수 있기 때문..?)
	wchar_t m_szContentPath[256];
	char m_szSoundResPath[256]; // 사운드 경로 변수

public: // member func

	// Path Manager 초기화 함수
	void Initialize();

	// m_szContentPath의 getter
	const wchar_t* GetContentPath() { return m_szContentPath; }
	const char* GetSoundResPath() { return m_szSoundResPath; }
};