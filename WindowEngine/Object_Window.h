#pragma once
#include "Object.h"

enum class WINDOW_STATE
{
    IDLE, // 사장님이 보이지 않는 상태
    WALK, // 사장님이 오른쪽으로 걸어가는 애니메이션
    UP,  // 사장님이 감시하는 애니메이션
};

/// <summary>
/// 창문 클래스로 사장님이 감시하는 애니메이션을 출력을 담당
///
///
/// 2023.05.23 이기태
/// </summary>
class Object_Window :
    public Object
{
private:
    WINDOW_STATE m_state;
    float m_lookTime;
    float m_sumLookTime;

public:

    Object_Window(GameProcess* _gameprocess);
    ~Object_Window();

    void Render(HDC _dc) override;
    void Update(GameProcess* _gameProcess) override;

    /// 사장님이 오른쪽으로 지나가는 애니메이션 재생
    void PlayWalkAnimaion(SoundManager* soundManager);

    // 사장님이 빼곰하는 애니메이션 재생
    void PlayUpAnimaion(SoundManager* soundManager);

};

