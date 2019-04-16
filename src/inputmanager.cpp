#include "inputmanager.h"
#include "inputmanager.h"
#include "log.h"

#define INPUT_LONGPRESS_DELAY 0.7f

const std::string InputManager::CLASS_NAME = "InputManager";

InputManager::InputManager()
{
    fDoubleClickTime_ = 0.25;
    fMouseHistoryMaxLength_ = 0.03;
    fMouseSensibility_ = 1.0f;
}

InputManager::~InputManager()
{
    Log("Closing "+CLASS_NAME+"...");
}

void InputManager::Initialize( float fScreenWidth, float fScreenHeight, sf::Window* pWindow )
{
    pWindow_ = pWindow;
    fScreenWidth_ = fScreenWidth;
    fScreenHeight_ = fScreenHeight;
}

void InputManager::ReadConfig()
{
}

void InputManager::AllowClicks( const std::string& sGroupName )
{
    lClickGroupList_[sGroupName] = true;
}

void InputManager::BlockClicks( const std::string& sGroupName )
{
    lClickGroupList_[sGroupName] = false;
}

bool InputManager::CanGroupReceiveClicks( const std::string& sGroupName ) const
{
    auto iter = lClickGroupList_.find(sGroupName);
    if (iter != lClickGroupList_.end())
    {
        if (!iter->second)
        {
            iter = lForcedClickGroupList_.find(sGroupName);
            if (iter != lForcedClickGroupList_.end())
            {
                return iter->second;
            }
            else
                return false;
        }
        else
            return true;
    }
    else
        return true;
}

void InputManager::ForceClicksAllowed( const std::string& sGroupName, bool bForce )
{
    lForcedClickGroupList_[sGroupName] = bForce;
}

bool InputManager::GetKey( bool bForce ) const
{
    if (!bForce && bFocus_)
        return false;
    else
        return bKey_;
}

std::string InputManager::GetKeyString( KeyCode mKey ) const
{
    return "";
}

const std::vector<uint_t>& InputManager::GetPressedList() const
{
    return lDownStack_;
}

const std::vector<uint_t>& InputManager::GetReleasedList() const
{
    return lUpStack_;
}

bool InputManager::KeyIsDown( KeyCode mKey, bool bForce ) const
{
    if (!bForce && bFocus_)
        return false;
    else
    {
        auto iter = lKeyBuf_.find(mKey);
        if (iter == lKeyBuf_.end())
            return false;
        else
            return iter->second;
    }
}

bool InputManager::KeyIsDownLong( KeyCode mKey, bool bForce ) const
{
    if (!bForce && bFocus_)
        return false;
    else
    {
        auto iter1 = lKeyBuf_.find(mKey);
        auto iter2 = lKeyLong_.find(mKey);
        if (iter1 == lKeyBuf_.end())
            return false;
        else
            return iter1->second && iter2->second;
    }
}

float InputManager::GetKeyDownDuration( KeyCode mKey ) const
{
    auto iter = lKeyDelay_.find(mKey);
    if (iter == lKeyDelay_.end())
        return std::numeric_limits<float>::quiet_NaN();
    else
        return iter->second;
}

bool InputManager::KeyIsPressed( KeyCode mKey, bool bForce ) const
{
    if (!bForce && bFocus_)
        return false;
    else
    {
        auto iter1 = lKeyBuf_.find(mKey);
        auto iter2 = lKeyBufOld_.find(mKey);
        if (iter1 == lKeyBuf_.end())
            return false;
        else
            return iter1->second && !iter2->second;
    }
}

bool InputManager::KeyIsReleased( KeyCode mKey, bool bForce ) const
{
    if (!bForce && bFocus_)
        return false;
    else
    {
        auto iter1 = lKeyBuf_.find(mKey);
        auto iter2 = lKeyBufOld_.find(mKey);
        if (iter1 == lKeyBuf_.end())
            return false;
        else
            return !iter1->second && iter2->second;
    }
}


bool InputManager::MouseIsDown( MouseButton mID, bool bForce ) const
{
    if (!bForce && bFocus_)
        return false;
    else
        return lMouseBuf_[mID];
}

bool InputManager::MouseIsDownLong( MouseButton mID, bool bForce ) const
{
    if (!bForce && bFocus_)
        return false;
    else
        return (lMouseBuf_[mID] && lMouseLong_[mID]);
}

float InputManager::GetMouseDownDuration( MouseButton mID ) const
{
    return lMouseDelay_[mID];
}

bool InputManager::MouseIsPressed( MouseButton mID, bool bForce ) const
{
    if (!bForce && bFocus_)
        return false;
    else
        return (lMouseBuf_[mID] && !lMouseBufOld_[mID]);
}

bool InputManager::MouseIsReleased( MouseButton mID, bool bForce ) const
{
    if (!bForce && bFocus_)
        return false;
    else
        return (!lMouseBuf_[mID] && lMouseBufOld_[mID]);
}

bool InputManager::MouseIsDoubleClicked( MouseButton mID, bool bForce ) const
{
    if (!bForce && bFocus_)
        return false;
    else
    {
        return (MouseIsPressed(mID) && lDoubleClickDelay_[mID] > 0.0);
    }
}

bool InputManager::WheelIsRolled( bool bForce ) const
{
    if (!bForce && bFocus_)
        return false;
    else
    {
        return bWheelRolled_;
    }
}

void InputManager::ClearKeys()
{
    lKeyBufOld_ = lKeyBuf_;
    iMWheel_ = 0;

    for (auto& p : lKeyBuf_)
    {
        p.second = false;
    }
}

void InputManager::NotifyKeyPushed( KeyCode mKey )
{
    lKeyBuf_[mKey] = true;
}

void InputManager::NotifyMouseWheelMoved( const int& iMovement )
{
    iMWheel_ = iMovement;
}

void InputManager::Update( float fTempDelta )
{
    // Control extreme delta time after loading/at startup etc
    float fDelta = fTempDelta;
    if ( (fDelta < 0.0) || (fDelta > 1.0) )
        fDelta = 0.05;

    // Update keys
    bKey_ = false;

    std::map<KeyCode, bool>::iterator iterKey;
    for (auto& mKey : lKeyBuf_)
    {
        KeyCode i = mKey.first;

        // Update delays
        if (lKeyBufOld_[i])
        {
            lKeyDelay_[i] += fDelta;
            if (lKeyDelay_[i] >= INPUT_LONGPRESS_DELAY)
                lKeyLong_[i] = true;
        }
        else
        {
            lKeyDelay_[i] = 0.0;
            lKeyLong_[i] = false;
        }

        if (lKeyBuf_[i])
        {
            bKey_ = true;
            if (!lKeyBufOld_[i])
            {
                // Key is pressed
                lDownStack_.push_back(i);
            }
        }
        else if (lKeyBufOld_[i])
        {
            // Key is released
            lUpStack_.push_back(i);
        }
    }

    // Handle modifier keys
    bCtrlPressed_ = KeyIsDown(KEY_LCTRL, true) || KeyIsDown(KEY_RCTRL, true);
    bShiftPressed_ = KeyIsDown(KEY_LSHIFT, true) || KeyIsDown(KEY_RSHIFT, true);
    bAltPressed_ = KeyIsDown(KEY_LALT, true) || KeyIsDown(KEY_RALT, true);

    // Update mouse state
    sf::Vector2i mMousePos = sf::Mouse::getPosition(*pWindow_);
    float fTempMX = mMousePos.x;
    float fTempMY = mMousePos.y;

    bool bNewDragged = false;
    bool bMouseState = false, bOldMouseState = false;
    for (uint_t i = 0; i < 3; ++i)
    {
        bOldMouseState = lMouseBufOld_[i] = lMouseBuf_[i];

        // Handle double clicking
        lDoubleClickDelay_[i] -= fDelta;

        if (bOldMouseState)
            lDoubleClickDelay_[i] = fDoubleClickTime_;

        // Update delays
        if (bOldMouseState)
        {
            lMouseDelay_[i] += fDelta;
            if (lMouseDelay_[i] >= INPUT_LONGPRESS_DELAY)
                lMouseLong_[i] = true;
        }
        else
        {
            lMouseDelay_[i] = 0.0;
            lMouseLong_[i] = false;
        }

        // Update state
        bMouseState = lMouseBuf_[i] = sf::Mouse::isButtonPressed((sf::Mouse::Button)i);

        // Handle dragging
        bool bDragStartTest = true;
        if (lMouseState_[i] == MOUSE_DRAGGED)
            bDragStartTest = false;

        if (bMouseState)
        {
            if (!bOldMouseState)
            {
                lMouseState_[i] = MOUSE_CLICKED; // single pressed

                if (lDoubleClickDelay_[i] > 0.0)
                {
                    lMouseState_[i] = MOUSE_DOUBLE; // double clicked
                }
                else
                {
                    bNewDragged = true;
                    if (bDragStartTest)
                        bLastDragged_ = true;

                    lMouseState_[i] = MOUSE_DRAGGED; // dragged
                }
            }
            else
            {
                bNewDragged = true;
                if (bDragStartTest)
                    bLastDragged_ = true;

                lMouseState_[i] = MOUSE_DRAGGED; // dragged
            }
        }
        else if (bOldMouseState)
        {
            lMouseState_[i] = MOUSE_RELEASED; // released
        }
        else
        {
            lMouseState_[i] = MOUSE_UP; // no input
        }
    }

    if (!bNewDragged)
        bLastDragged_ = false;

    // Update mouse position
    fRawDMX_ = fDMX_ = fTempMX - fMX_;
    fRawDMY_ = fDMY_ = fTempMY - fMY_;

    fMX_ = fTempMX;
    fMY_ = fTempMY;

    fDMX_ *= fMouseSensibility_;
    fDMY_ *= fMouseSensibility_;

    if (iMWheel_ == 0u)
        bWheelRolled_ = false;
    else
        bWheelRolled_ = true;
}

void InputManager::SetDoubleClickTime( float fDoubleClickTime )
{
    fDoubleClickTime_ = fDoubleClickTime;
}

float InputManager::GetDoubleClickTime() const
{
    return fDoubleClickTime_;
}

void InputManager::SetMouseBufferDuration( float fMouseHistoryMaxLength )
{
    fMouseHistoryMaxLength_ = fMouseHistoryMaxLength;
}

float InputManager::GetMouseBufferDuration() const
{
    return fMouseHistoryMaxLength_;
}

void InputManager::SetFocus( bool bFocus )
{
    bFocus_ = bFocus;
}

bool InputManager::AltPressed() const
{
    return bAltPressed_;
}

bool InputManager::ShiftPressed() const
{
    return bShiftPressed_;
}

bool InputManager::CtrlPressed() const
{
    return bCtrlPressed_;
}

bool InputManager::MouseLastDragged() const
{
    return bLastDragged_;
}

MouseState InputManager::GetMouseState( MouseButton mID ) const
{
    return lMouseState_[mID];
}

float InputManager::GetMousePosX() const
{
    return fMX_;
}

float InputManager::GetMousePosY() const
{
    return fMY_;
}

float InputManager::GetMouseRawDX() const
{
    return fRawDMX_;
}

float InputManager::GetMouseRawDY() const
{
    return fRawDMY_;
}

float InputManager::GetMouseDX() const
{
    return fDMX_;
}

float InputManager::GetMouseDY() const
{
    return fDMY_;
}

const int& InputManager::GetMouseWheel() const
{
    return iMWheel_;
}

void InputManager::SetMouseSensibility(float fMouseSensibility)
{
    fMouseSensibility_ = fMouseSensibility;
}

float InputManager::GetMouseSensibility() const
{
    return fMouseSensibility_;
}

std::string InputManager::GetMouseButtonString( MouseButton mID ) const
{
    switch (mID)
    {
        case MOUSE_LEFT : return "LeftButton";
        case MOUSE_RIGHT : return "RightButton";
        case MOUSE_MIDDLE : return "MiddleButton";
        default : return "";
    }
}
