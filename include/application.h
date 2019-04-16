#ifndef APPLICATION_H
#define APPLICATION_H

#include "utils.h"
#include "manager.h"

#include <SFML/Graphics.hpp>

class Sprite;
class Text;
class Menu;
class Board;
class Button;

class Application
{
public :

    enum State
    {
        STATE_MENU,
        STATE_NEWGAME,
        STATE_SAVE,
        STATE_LOAD,
        STATE_GAME,
        STATE_EXIT
    };

    Application();
    ~Application();

    sf::RenderWindow* GetRenderWindow();

    const uint_t& GetScreenWidth() const;
    const uint_t& GetScreenHeight() const;

    void SetState(State mState);

    void SaveOnSlot(const uint_t& uiSlot);

    Board* GetBoard();

    std::string GetLanguage() const;

    static Application* GetMainApp();

    static const std::string CLASS_NAME;

private :

    static Application* MAIN_APP;

    void Loop_();
    void SaveSlot_(const uint_t& uiSlot, const std::string& sName);
    void LoadSlot_(const uint_t& uiSlot);

    State mState_;
    sf::RenderWindow mWindow_;
    std::unique_ptr<Text> pOrbTitle_;
    std::unique_ptr<Text> pHelpText_;
    std::unique_ptr<Sprite> pCursor_;
    std::unique_ptr<Menu> pMainMenu_;
    std::unique_ptr<Board> pBoard_;

    std::array<std::unique_ptr<Button>,5> lSaveSlotList_;

    uint_t uiScreenWidth_;
    uint_t uiScreenHeight_;
    std::string sLanguage_;
};

#endif
