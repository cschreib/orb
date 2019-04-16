#ifndef BUTTON_H
#define BUTTON_H

#include "utils.h"
#include "point.h"

class Application;
class Sprite;
class Text;

typedef void (*OnClickFunc)(Application&);
typedef Point<float> Vector2D;

class Button
{
public :

    enum State
    {
        STATE_NORMAL,
        STATE_PUSHED,
        STATE_DISABLED
    };

    Button(const Vector2D& mPosition, const std::string& sFile, const std::string& sCaption, OnClickFunc mFunc, Application& mApp);
    ~Button();

    void Disable();
    void Enable();

    void SetCaption(const std::string& sCaption);
    void SetPosition(const Vector2D& mPosition);

    void Update(float fDelta, const Vector2D& mMouse, bool bMouseDown, bool bMouseReleased);
    void Render() const;

    Text* GetText();

private :

    Application& mApp_;
    OnClickFunc mFunc_;

    State mState_;
    Vector2D mPosition_;

    float fTime_ = 0.0;

    std::unique_ptr<Sprite> pHighlight_;
    std::unique_ptr<Sprite> pButton_;
    std::unique_ptr<Sprite> pButtonDown_;
    std::unique_ptr<Sprite> pButtonDisabled_;
    std::unique_ptr<Text>   pCaption_;

    bool bMouseOver_ = false;
};

#endif
