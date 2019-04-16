#include "application.h"
#include "inputmanager.h"
#include "texturemanager.h"
#include "sprite.h"
#include "menu.h"
#include "board.h"
#include "button.h"

const std::string Application::CLASS_NAME = "Application";

void ReturnGame(Application& mApp)
{
    mApp.SetState(Application::STATE_GAME);
}

void SaveGame(Application& mApp)
{
    mApp.SetState(Application::STATE_SAVE);
}

void NewGame(Application& mApp)
{
    mApp.SetState(Application::STATE_NEWGAME);
}

void LoadGame(Application& mApp)
{
    mApp.SetState(Application::STATE_LOAD);
}

void BestScores(Application& mApp)
{

}

void Options(Application& mApp)
{

}

void Quit(Application& mApp)
{
    mApp.SetState(Application::STATE_EXIT);
}

template<uint_t N>
void SaveSlot(Application& mApp)
{
    mApp.SaveOnSlot(N);
}

Application* Application::MAIN_APP = nullptr;

Application::Application() : uiScreenWidth_(1024u), uiScreenHeight_(768u), sLanguage_("en")
{
    MAIN_APP = this;

    std::string sHelp;
    std::string sMainMenu;
    std::string sNewGame;
    std::string sLoadGame;
    std::string sQuit;
    if (sLanguage_ == "fr") {
        sMainMenu = "Menu principal";
        sNewGame = "Nouvelle partie";
        sLoadGame = "Charger partie";
        sQuit = "Quitter";
        sHelp = "Appuyez sur [Echap] pour revenir au menu.";
    }
    else if (sLanguage_ == "en")
    {
        sMainMenu = "Main menu";
        sNewGame = "New game";
        sLoadGame = "Load game";
        sQuit = "Exit";
        sHelp = "Press [Escape] to go back to the main menu.";
    }

    mWindow_.create(sf::VideoMode(uiScreenWidth_, uiScreenHeight_, 32), "Orb");
    mWindow_.setMouseCursorVisible(false);
    mWindow_.setFramerateLimit(60);
    InputManager::GetSingleton()->Initialize(float(uiScreenWidth_), float(uiScreenHeight_), &mWindow_);

    pCursor_ = std::unique_ptr<Sprite>(new Sprite("cursor.png"));

    pOrbTitle_ = std::unique_ptr<Text>(new Text("ravie.ttf", 42));
    pOrbTitle_->SetText("|cFF4876C2O|cFF51A3E3R|cFF84E2E8B");
    pOrbTitle_->SetAlignment(Text::ALIGN_CENTER);

    pHelpText_ = std::unique_ptr<Text>(new Text("ravie.ttf", 14));
    pHelpText_->SetText(sHelp);
    pHelpText_->SetAlignment(Text::ALIGN_CENTER);

    pMainMenu_ = std::unique_ptr<Menu>(new Menu(sMainMenu, *this));
    pMainMenu_->AddItem(0, sNewGame, &NewGame);
    pMainMenu_->AddItem(3, sLoadGame, &LoadGame);
    pMainMenu_->AddItem(4, sQuit, &Quit);

    // TODO
    /*lSaveSlotList_[i] = std::unique_ptr<Button>(new Button(
        Vector2D(), "save_button", "Slot "+i+".", &SaveSlot<1>, *this
    ));

    for (uint_t i; i < 5; ++i)
    {
        Button* pButton =

        pButton->GetText()->SetAlignment(ALIGN_LEFT);
    }*/

    mState_ = STATE_MENU;

    Loop_();
}

Application::~Application()
{
    TextureManager::Delete();
    InputManager::Delete();
}

void Application::SetState(State mState)
{
    mState_ = mState;
}

std::string Application::GetLanguage() const
{
    return sLanguage_;
}

void Application::Loop_()
{
    InputManager* pInputMgr = InputManager::GetSingleton();

    bool bRunning = true;
    float fDelta = 0.0f;
    sf::Clock mClock;

    while (bRunning)
    {
        pInputMgr->ClearKeys();
        sf::Event mEvent;
        while (mWindow_.pollEvent(mEvent))
        {
            // Window closed
            if (mEvent.type == sf::Event::Closed)
                bRunning = false;

            // Feed the input manager
            if (mEvent.type == sf::Event::KeyPressed)
                pInputMgr->NotifyKeyPushed((KeyCode)mEvent.key.code);

            if (mEvent.type == sf::Event::MouseWheelMoved)
                pInputMgr->NotifyMouseWheelMoved(mEvent.mouseWheel.delta);
        }

        pInputMgr->Update(fDelta);

        mWindow_.clear();

        switch (mState_)
        {
            case STATE_SAVE :
            {
                pOrbTitle_->Render(512, 90);

                pMainMenu_->Render();

                sf::RectangleShape mRect(sf::Vector2f(uiScreenWidth_, uiScreenHeight_));
                mRect.setPosition(sf::Vector2f(0.0, 0.0));
                mRect.setFillColor(sf::Color(0, 0, 0, 180));
                mWindow_.draw(mRect);

                if (pInputMgr->KeyIsPressed(KEY_ESC))
                    SetState(STATE_MENU);

                break;
            }
            case STATE_LOAD :
            {
                pOrbTitle_->Render(512, 90);

                pMainMenu_->Render();

                sf::RectangleShape mRect(sf::Vector2f(uiScreenWidth_, uiScreenHeight_));
                mRect.setPosition(sf::Vector2f(0.0, 0.0));
                mRect.setFillColor(sf::Color(0, 0, 0, 180));
                mWindow_.draw(mRect);

                if (pInputMgr->KeyIsPressed(KEY_ESC))
                    SetState(STATE_MENU);

                break;
            }
            case STATE_MENU :
            {
                pOrbTitle_->Render(512, 90);

                pMainMenu_->Update(fDelta);
                pMainMenu_->Render();

                if (pInputMgr->KeyIsPressed(KEY_ESC))
                    bRunning = false;

                break;
            }
            case STATE_NEWGAME :
            {
                pBoard_ = std::unique_ptr<Board>(new Board(Vector2D(
                    (float(uiScreenWidth_) - 576)/2.0f + 130.0f,
                    (float(uiScreenHeight_) - 576)/2.0f
                ), *this));

                std::string sContinue;
                std::string sSaveGame;
                if (sLanguage_ == "fr")
                {
                    sContinue = "Continuer";
                    sSaveGame = "Sauver partie";
                }
                else if (sLanguage_ == "en")
                {
                    sContinue = "Continue";
                    sSaveGame = "Save game";
                }

                pMainMenu_->AddItem(1, sContinue, &ReturnGame);
                pMainMenu_->AddItem(2, sSaveGame, &SaveGame);

                pBoard_->Render();

                pHelpText_->Render(512+130, 730);

                mState_ = STATE_GAME;

                break;
            }
            case STATE_GAME :
            {
                if (pInputMgr->KeyIsPressed(KEY_ESC))
                {
                    SetState(STATE_MENU);
                    break;
                }

                pBoard_->Update(fDelta);
                pBoard_->Render();

                pHelpText_->Render(512+130, 730);

                break;
            }
            case STATE_EXIT :
            {
                bRunning = false;

                break;
            }
        }

        pCursor_->RenderEx(pInputMgr->GetMousePosX(), pInputMgr->GetMousePosY(), 0.0f, 0.5f, 0.5f);

        mWindow_.display();

        fDelta = mClock.getElapsedTime().asSeconds();
        mClock.restart();
    }
}

sf::RenderWindow* Application::GetRenderWindow()
{
    return &mWindow_;
}

Application* Application::GetMainApp()
{
    return MAIN_APP;
}

const uint_t& Application::GetScreenWidth() const
{
    return uiScreenWidth_;
}

const uint_t& Application::GetScreenHeight() const
{
    return uiScreenHeight_;
}

Board* Application::GetBoard()
{
    return pBoard_.get();
}

void Application::SaveSlot_( const uint_t& uiSlot, const std::string& sName )
{

}

void Application::LoadSlot_( const uint_t& uiSlot )
{

}
