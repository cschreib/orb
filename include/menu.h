#ifndef MENU_H
#define MENU_H

#include "utils.h"

#include "sprite.h"
#include "text.h"
#include "application.h"

class Sprite;
class Text;
class Button;

typedef Point<float> Vector2D;

typedef void (*OnClickFunc)(Application&);

class Menu
{
public :

    Menu(const std::string& sTitle, Application& mApp);
    virtual ~Menu();

    void AddItem(const uint_t& uiID, const std::string& sContent, OnClickFunc pFunc = nullptr);

    virtual void Render();
    virtual void Update(float fDelta);

private:

    Application& mApp_;

    std::map< uint_t, std::unique_ptr<Button> > lItemList_;

    std::unique_ptr<Text> pTextTitle_;

    bool bUpdatePosition_ = false;

};

#endif
