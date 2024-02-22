#include "pch.h"
#include "SoundManager.h"
#include "PathManager.h"

SoundManager::SoundManager()
	:m_system(nullptr)
	,m_channel{}
	,m_ovenGroup()
{
	// Fmod �ý��� ���� �� �ʱ�ȭ
	System_Create(&m_system, FMOD_VERSION);
	m_system->init((int)SOUND_TYPE::END, FMOD_INIT_NORMAL, nullptr);
	m_system->createChannelGroup("OvenGroup", &m_ovenGroup);
}

SoundManager::~SoundManager()
{
	// ���� ��� ��ȸ
	for (const auto& pair : m_mapSfx) {
		pair.second->release();
	}

	// �ڿ� ��ȯ
	m_system->close();
	m_system->release();
}

void SoundManager::Initialize(PathManager* pathManager)
{
	/// ���� ���� �ε�

	// PLAYER
	LoadSound(pathManager, L"SE_action", "\\Sound\\SE_action.mp3");
	LoadSound(pathManager, L"SE_Pizza", "\\Sound\\SE_Pizza.mp3");
	LoadSound(pathManager, L"SE_Trash", "\\Sound\\SE_Trash.wav");


	// BOSS
	LoadSound(pathManager, L"SE_Window", "\\Sound\\SE_Window.mp3");
	LoadSound(pathManager, L"SE_Walk", "\\Sound\\SE_Walk.mp3", true);
	LoadSound(pathManager, L"SE_Hmm", "\\Sound\\SE_hmm.wav");

	// ORDER
	LoadSound(pathManager, L"SE_Success", "\\Sound\\SE_Success.mp3");
	LoadSound(pathManager, L"SE_Fail", "\\Sound\\SE_Fail.mp3");


	// OVEN 1 , 2 , 3
	LoadSound(pathManager, L"SE_Oven", "\\Sound\\SE_Oven.mp3");


	// BGM
	LoadSound(pathManager, L"BGM_Title", "\\Sound\\BGM_Title.mp3", true);
	LoadSound(pathManager, L"BGM_In_game", "\\Sound\\BGM_In_game.mp3", true);
	LoadSound(pathManager, L"BGM_start_comic", "\\Sound\\BGM_start_comic.wav", true);
	LoadSound(pathManager, L"BGM_Credit", "\\Sound\\BGM_Credit.wav", true);
	LoadSound(pathManager, L"BGM_Ending", "\\Sound\\BGM_Ending.mp3", true);

	// UI
	LoadSound(pathManager, L"SE_UI", "\\Sound\\SE_UI.wav");
	LoadSound(pathManager, L"SE_UI_Touch", "\\Sound\\SE_UI_Touch.wav");

	// PAGE
	LoadSound(pathManager, L"SE_pages", "\\Sound\\SE_pages.wav");

	// OverCook
	LoadSound(pathManager, L"SE_Pizza_overcook", "\\Sound\\SE_Pizza_overcook.mp3");

	// GAMEOVER
	LoadSound(pathManager, L"SE_Gameover", "\\Sound\\SE_Gameover.mp3");
}

void SoundManager::LoadSound(PathManager* pathManager, const wstring& _key, const string& _relativePath, bool _isLoop)
{
	// ���ҽ� ������ ��θ� ���� ����
	string resFilePath = pathManager->GetSoundResPath();
	resFilePath += _relativePath;

	// ���� ���� ����
	FMOD::Sound* pSfx;

	// Loop Type ����
	UINT loopType;

	if (_isLoop)
		loopType = FMOD_LOOP_NORMAL;
	else
		loopType = FMOD_INIT_NORMAL;

	// ���� ����(������� ���� ����)
	m_system->createSound(resFilePath.c_str(), loopType, nullptr, &pSfx);

	// map�� ����
	m_mapSfx.insert(make_pair(_key, pSfx));
}

void SoundManager::PlaySound(const wstring& _key, SOUND_TYPE _type, float _volume)
{
	// find() �Լ��� ����Ͽ� �˻�
	auto it = m_mapSfx.find(_key);

	// Ű�� ã�� ���
	if (it != m_mapSfx.end())
	{
		switch (_type)
		{
		case SOUND_TYPE::PLAYER:
			Player_Pause();
			m_system->playSound(it->second, nullptr, false, &m_channel[(int)SOUND_TYPE::PLAYER]);
			m_channel[(int)SOUND_TYPE::PLAYER]->setVolume(_volume);
			break;
		case SOUND_TYPE::BOSS:
			Boss_Pause();
			m_system->playSound(it->second, nullptr, false, &m_channel[(int)SOUND_TYPE::BOSS]);
			m_channel[(int)SOUND_TYPE::BOSS]->setVolume(_volume);
			break;
		case SOUND_TYPE::ORDER:
			Order_Pause();
			m_system->playSound(it->second, nullptr, false, &m_channel[(int)SOUND_TYPE::ORDER]);
			m_channel[(int)SOUND_TYPE::ORDER]->setVolume(_volume);
			break;
		case SOUND_TYPE::OVEN1:
			Oven1_Pause();
			m_system->playSound(it->second, m_ovenGroup, false, &m_channel[(int)SOUND_TYPE::OVEN1]);
			m_channel[(int)SOUND_TYPE::OVEN1]->setVolume(_volume);
			break;
		case SOUND_TYPE::OVEN2:
			Oven2_Pause();
			m_system->playSound(it->second, m_ovenGroup, false, &m_channel[(int)SOUND_TYPE::OVEN2]);
			m_channel[(int)SOUND_TYPE::OVEN2]->setVolume(_volume);
			break;
		case SOUND_TYPE::OVEN3:
			Oven3_Pause();
			m_system->playSound(it->second, m_ovenGroup, false, &m_channel[(int)SOUND_TYPE::OVEN3]);
			m_channel[(int)SOUND_TYPE::OVEN3]->setVolume(_volume);
			break;
		case SOUND_TYPE::BGM:
			BGM_Pause();
			m_system->playSound(it->second, nullptr, false, &m_channel[(int)SOUND_TYPE::BGM]);
			m_channel[(int)SOUND_TYPE::BGM]->setVolume(_volume);
			break;
		case SOUND_TYPE::UI:
			UI_Pause();
			m_system->playSound(it->second, nullptr, false, &m_channel[(int)SOUND_TYPE::UI]);
			m_channel[(int)SOUND_TYPE::UI]->setVolume(_volume);
			break;
		case SOUND_TYPE::PAGE:
			Page_Pause();
			m_system->playSound(it->second, nullptr, false, &m_channel[(int)SOUND_TYPE::PAGE]);
			m_channel[(int)SOUND_TYPE::PAGE]->setVolume(_volume);
			break;
		case SOUND_TYPE::OVERCOOK:
			OverCook_Pause();
			m_system->playSound(it->second, nullptr, false, &m_channel[(int)SOUND_TYPE::OVERCOOK]);
			m_channel[(int)SOUND_TYPE::OVERCOOK]->setVolume(_volume);
			break;
		case SOUND_TYPE::GAMEOVER:
			GameOver_Pause();
			m_system->playSound(it->second, nullptr, false, &m_channel[(int)SOUND_TYPE::GAMEOVER]);
			m_channel[(int)SOUND_TYPE::GAMEOVER]->setVolume(_volume);
			break;
		default:
			break;
		}
	}
	else
	{
		// Ű�� ã�� ���� ���
		return;
	}
}

void SoundManager::SetLoopCount(SOUND_TYPE _type, int loopCount)
{
	// Walk���� ���
	m_channel[(int)_type]->setLoopCount(loopCount);
}

void SoundManager::Player_Pause()
{
	m_channel[(int)SOUND_TYPE::PLAYER]->stop();
}

void SoundManager::Boss_Pause()
{
	m_channel[(int)SOUND_TYPE::BOSS]->stop();
}

void SoundManager::Order_Pause()
{
	m_channel[(int)SOUND_TYPE::ORDER]->stop();
}

void SoundManager::Oven1_Pause()
{
	m_channel[(int)SOUND_TYPE::OVEN1]->stop();
}

void SoundManager::Oven2_Pause()
{
	m_channel[(int)SOUND_TYPE::OVEN2]->stop();
}

void SoundManager::Oven3_Pause()
{
	m_channel[(int)SOUND_TYPE::OVEN3]->stop();
}

void SoundManager::BGM_Pause()
{
	m_channel[(int)SOUND_TYPE::BGM]->stop();
}

void SoundManager::UI_Pause()
{
	m_channel[(int)SOUND_TYPE::UI]->stop();
}

void SoundManager::Page_Pause()
{
	m_channel[(int)SOUND_TYPE::PAGE]->stop();
}

void SoundManager::OverCook_Pause()
{
	m_channel[(int)SOUND_TYPE::OVERCOOK]->stop();
}

void SoundManager::GameOver_Pause()
{
	m_channel[(int)SOUND_TYPE::GAMEOVER]->stop();
}

bool SoundManager::CheckUsingChannel(SOUND_TYPE _type)
{
	bool isPlaying; // �ش� ä���� ���带 ���������?
	m_channel[(int)_type]->isPlaying(&isPlaying);

	return isPlaying;
}