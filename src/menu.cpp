#include "menu.h"
#include "button.h"
#include "sprite.h"
#include "text.h"
#include "inputmanager.h"

Menu::Menu( const std::string& sTitle, Application& mApp ) : mApp_(mApp)
{
    pTextTitle_ = std::unique_ptr<Text>(new Text("ravie.ttf", 24));
    pTextTitle_->SetText(sTitle);
    pTextTitle_->SetAlignment(Text::ALIGN_CENTER);
}

Menu::~Menu()
{
}

void Menu::AddItem( const uint_t& uiID, const std::string& sContent, OnClickFunc pFunc )
{
    lItemList_[uiID] = std::unique_ptr<Button>(new Button(Vector2D(), "menu_button", sContent, pFunc, mApp_));

    bUpdatePosition_ = true;
}

void Menu::Render()
{
    pTextTitle_->Render(512, 160);

    if (bUpdatePosition_)
    {
        float fStartPos = (768.0f - 70.0f*float(lItemList_.size()-1))/2.0f + 84;
        float fY = 0.0;

        for (auto& pButton : lItemList_)
        {
            pButton.second->SetPosition(Vector2D(512, fStartPos + fY));
            fY += 70.0f;
        }

        bUpdatePosition_ = false;
    }

    for (auto& pButton : lItemList_)
    {
        pButton.second->Render();
    }
}

void Menu::Update( float fDelta )
{
    Vector2D mMouse(InputManager::GetSingleton()->GetMousePosX(), InputManager::GetSingleton()->GetMousePosY());

    for (auto& pButton : lItemList_)
    {
        pButton.second->Update(fDelta, mMouse,
            InputManager::GetSingleton()->MouseIsDown(MOUSE_LEFT),
            InputManager::GetSingleton()->MouseIsReleased(MOUSE_LEFT)
        );
    }
}
