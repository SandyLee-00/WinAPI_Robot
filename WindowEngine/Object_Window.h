#pragma once
#include "Object.h"

enum class WINDOW_STATE
{
    IDLE, // ������� ������ �ʴ� ����
    WALK, // ������� ���������� �ɾ�� �ִϸ��̼�
    UP,  // ������� �����ϴ� �ִϸ��̼�
};

/// <summary>
/// â�� Ŭ������ ������� �����ϴ� �ִϸ��̼��� ����� ���
///
///
/// 2023.05.23 �̱���
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

    /// ������� ���������� �������� �ִϸ��̼� ���
    void PlayWalkAnimaion(SoundManager* soundManager);

    // ������� �����ϴ� �ִϸ��̼� ���
    void PlayUpAnimaion(SoundManager* soundManager);

};

