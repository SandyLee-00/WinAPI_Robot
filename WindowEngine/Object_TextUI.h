#pragma once
#include "Object_UI.h"
class Object_TextUI :
    public Object_UI
{
private:
    wstring m_text; 


    void Render(HDC _dc);

public:
    const wstring& GetText() { return m_text;}
    void SetText(const wchar_t* _char);

    Object_TextUI(GameProcess* gameProcess);
    ~Object_TextUI() override;

};

