#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "utils.h"
#include "manager.h"

#include <SFML/Window.hpp>

enum MouseState
{
    MOUSE_UP = 0,
    MOUSE_DRAGGED,
    MOUSE_CLICKED,
    MOUSE_RELEASED,
    MOUSE_DOUBLE,
    MOUSE_LONG
};

enum MouseButton
{
    MOUSE_LEFT,
    MOUSE_RIGHT,
    MOUSE_MIDDLE
};

enum KeyCode
{
    KEY_A = 0,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_ESC,
    KEY_LCTRL,
    KEY_LSHIFT,
    KEY_LALT,
    KEY_LSYSTEM,      ///< OS specific key (left side) : windows (Win and Linux), apple (MacOS), ...
    KEY_RCTRL,
    KEY_RSHIFT,
    KEY_RALT,
    KEY_RSYSTEM,      ///< OS specific key (right side) : windows (Win and Linux), apple (MacOS), ...
    KEY_MMENU,
    KEY_LBRACKET,     ///< [
    KEY_RBRACKET,     ///< ]
    KEY_SEMICOLON,    ///< ;
    KEY_COMMA,        ///< ,
    KEY_PERIOD,       ///< .
    KEY_QUOTE,        ///< '
    KEY_SLASH,        ///< /
    KEY_BACKSLASH,
    KEY_TILDE,        ///< ~
    KEY_EQUAL,        ///< =
    KEY_DASH,         ///< -
    KEY_SPACE,
    KEY_RETURN,
    KEY_BACK,
    KEY_TAB,
    KEY_PAGEUP,
    KEY_PAGEDOWN,
    KEY_END,
    KEY_HOME,
    KEY_INSERT,
    KEY_DELETE,
    KEY_ADD,          ///< +
    KEY_SUBSTRACT,     ///< -
    KEY_MILTIPLY,     ///< *
    KEY_DIVIDE,       ///< /
    KEY_LEFT,         ///< Left arrow
    KEY_RIGHT,        ///< Right arrow
    KEY_UP,           ///< Up arrow
    KEY_DOWN,         ///< Down arrow
    KEY_NUM0,
    KEY_NUM1,
    KEY_NUM2,
    KEY_NUM3,
    KEY_NUM4,
    KEY_NUM5,
    KEY_NUM6,
    KEY_NUM7,
    KEY_NUM8,
    KEY_NUM9,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_F13,
    KEY_F14,
    KEY_F15,
    KEY_PAUSE
};

/// Handles inputs (keyboard and mouse)
class InputManager : public Manager<InputManager>
{
friend class Manager<InputManager>;
public :

    /// Initializes this manager.
    /** \return 'true' if everything went fine
    */
    void            Initialize(float fScreenWidth, float fScreenHeight, sf::Window* pWindow);

    /// Reads configuration.
    /** \return 'false' if something went wrong
    */
    void            ReadConfig();

    void            ClearKeys();

    void            NotifyKeyPushed(KeyCode mKey);

    void            NotifyMouseWheelMoved(const int& iMovement);

    /// Updates input (keyboard and mouse).
    void            Update(float fDelta);

    /// Allows a particular input group to receive click events.
    /** \param sGroupName The name of the group to enable
    */
    void            AllowClicks(const std::string& sGroupName);

    /// Prevents a particular input group from receiving click events.
    /** \param sGroupName The name of the group to disable
    */
    void            BlockClicks(const std::string& sGroupName);

    /// Checks if a particular input group can receive click events.
    /** \param sGroupName The name of the group to check
    *   \return 'true' if the group can receive click events
    */
    bool          CanGroupReceiveClicks(const std::string& sGroupName) const;

    /// Makes sure a particular input group receives click events.
    /** \param sGroupName The name of the group to force
    *   \param bForce     'true' to force input
    *   \note Even if you call BlockClicks() with the same group name,
    *         CanGroupReceiveClicks() will return true.
    */
    void            ForceClicksAllowed(const std::string& sGroupName, bool bForce);

    /// Checks if a key has been pressed.
    /** \param bForce 'true' to bypass focus (see SetFocus())
    *   \return 'true' if a key has been pressed
    */
    bool          GetKey(bool bForce = false) const;

    /// Checks if a key is being pressed.
    /** \param mKey   The ID code of the key you're interested in
    *   \param bForce 'true' to bypass focus (see SetFocus())
    *   \return 'true' if the key is being pressed
    */
    bool          KeyIsDown(KeyCode mKey, bool bForce = false) const;

    /// Checks if a key is being pressed for a long time.
    /** \param mKey   The ID code of the key you're interested in
    *   \param bForce 'true' to bypass focus (see SetFocus())
    *   \return 'true' if the key is being pressed for a long time
    */
    bool          KeyIsDownLong(KeyCode mKey, bool bForce = false) const;

    /// Returns elapsed time since the key has been pressed.
    /** \param mKey The ID code of the key you're interested in
    *   \return Elapsed time since the key has been pressed
    */
    float GetKeyDownDuration(KeyCode mKey) const;

    /// Checks if a key has been pressed.
    /** \param mKey   The ID code of the key you're interested in
    *   \param bForce 'true' to bypass focus (see SetFocus())
    *   \return 'true' if the key has been pressed
    *   \note Happens just when the key is pressed.
    */
    bool          KeyIsPressed(KeyCode mKey, bool bForce = false) const;

    /// Checks if a key has been released.
    /** \param mKey   The ID code of the key you're interested in
    *   \param bForce 'true' to bypass focus (see SetFocus())
    *   \return 'true' if the key has been released
    *   \note Happens just when the key is released.
    */
    bool          KeyIsReleased(KeyCode mKey, bool bForce = false) const;

    /// Returns the string associated with a key.
    /** \param mKey   The ID code of the key you're interested in
    *   \return The string associated with a key
    *   \note Untested function...
    */
    std::string           GetKeyString(KeyCode mKey) const;

    /// Returns the list of keys that have been released during this frame.
    /** \return The list of keys that have been released during this frame.
    */
    const std::vector<uint_t>& GetReleasedList() const;

    /// Returns the list of keys that have been pressed during this frame.
    /** \return The list of keys that have been pressed during this frame.
    */
    const std::vector<uint_t>& GetPressedList() const;

    /// Checks if Alt is beeing pressed.
    /** \return 'true' if Alt is beeing pressed
    */
    bool   AltPressed() const;

    /// Checks if Shift is beeing pressed.
    /** \return 'true' if Shift is beeing pressed
    */
    bool   ShiftPressed() const;

    /// Checks if Control (Ctrl) is beeing pressed.
    /** \return 'true' if Control (Ctrl) is beeing pressed
    */
    bool   CtrlPressed() const;

    /// Checks if a mouse button is being pressed.
    /** \param mID    The ID code of the mouse button you're interested in
    *   \param bForce 'true' to bypass focus (see SetFocus())
    *   \return 'true' if the mouse button is being pressed
    */
    bool          MouseIsDown(MouseButton mID, bool bForce = false) const;

    /// Checks if a mouse button is being pressed for a long time.
    /** \param mID    The ID code of the mouse button you're interested in
    *   \param bForce 'true' to bypass focus (see SetFocus())
    *   \return 'true' if the mouse button is being pressed for a long time
    */
    bool          MouseIsDownLong(MouseButton mID, bool bForce = false) const;

    /// Returns elapsed time since the mouse button has been pressed.
    /** \param mKey The ID code of the mouse button you're interested in
    *   \return Elapsed time since the mouse button has been pressed
    */
    float GetMouseDownDuration(MouseButton mKey) const;

    /// Checks if a mouse button has been pressed.
    /** \param mID    The ID code of the mouse button you're interested in
    *   \param bForce 'true' to bypass focus (see SetFocus())
    *   \return 'true' if the mouse button has been pressed
    *   \note Happens just when the mouse button is pressed.
    */
    bool          MouseIsPressed(MouseButton mID, bool bForce = false) const;

    /// Checks if a mouse button has been released.
    /** \param mID    The ID code of the mouse button you're interested in
    *   \param bForce 'true' to bypass focus (see SetFocus())
    *   \return 'true' if the mouse button has been released
    *   \note Happens just when the mouse button is released.
    */
    bool          MouseIsReleased(MouseButton mID, bool bForce = false) const;

    /// Checks if a mouse button has been double clicked.
    /** \param mID    The ID code of the mouse button you're interested in
    *   \param bForce 'true' to bypass focus (see SetFocus())
    *   \return 'true' if the mouse button has been double clicked
    */
    bool          MouseIsDoubleClicked(MouseButton mID, bool bForce = false) const;

    /// Checks if the mouse wheel has been rolled.
    /** \param bForce 'true' to bypass focus (see SetFocus())
    *   \return 'true' if the mouse wheel has been rolled
    */
    bool          WheelIsRolled(bool bForce = false) const;

    /// Checks if the mouse has just started beeing dragged.
    /** \return 'true' if the mouse has just started beeing dragged
    */
    bool   MouseLastDragged() const;

    /// Returns a mouse button's state.
    /** \param mID The ID code of the mouse button you're interested in
    *   \return The mouse button's state
    */
    MouseState      GetMouseState(MouseButton mID) const;

    /// Returns the horizontal position of the mouse.
    /** \return The horizontal position of the mouse
    */
    float  GetMousePosX() const;

    /// Returns the vertical position of the mouse.
    /** \return The vertical position of the mouse
    */
    float  GetMousePosY() const;

    /// Returns the horizontal position variation of the mouse.
    /** \return The horizontal position variation of the mouse
    *   \note This function returns values just as they are given
    *         by the mouse.
    */
    float  GetMouseRawDX() const;

    /// Returns the vertical position variation of the mouse.
    /** \return The vertical position variation of the mouse
    *   \note This function returns values just as they are given
    *         by the mouse.
    */
    float  GetMouseRawDY() const;

    /// Returns the horizontal position variation of the mouse.
    /** \return The horizontal position variation of the mouse
    *   \note This function returns the same thing as GetMouseRawDX(),
    *         but this time, the game's sensibility factor is applied.
    */
    float  GetMouseDX() const;

    /// Returns the vertical position variation of the mouse.
    /** \return The vertical position variation of the mouse
    *   \note This function returns the same thing as GetMouseRawDY(),
    *         but this time, the game's sensibility factor is applied.
    */
    float  GetMouseDY() const;

    /// Returns the rolling ammount of the mouse wheel.
    /** \return The rolling ammount of the mouse wheel
    */
    const int&    GetMouseWheel() const;

    /// Returns the string associated to a mouse button.
    /** \param mID The ID code of the mouse button you're interested in
    *   \return The string associated with the mouse button
    */
    std::string           GetMouseButtonString(MouseButton mID) const;

    /// Sets the double click maximum time.
    /** \param dDoubleClickTime
    */
    void            SetDoubleClickTime(float dDoubleClickTime);

    /// Returns the double click maximum time.
    /** \return The double click maximum time
    */
    float GetDoubleClickTime() const;

    /// Sets the ammount of mouse movement to be buffered.
    /** \param dMouseHistoryMaxLength The maximum buffer length (in seconds)
    *   \note If you experience jerky mouse movement, you can try to increase
    *         this value (default : 0.1s).<br> On the contrary, if you feel
    *         your mouse is not responsive enough, try to decrease it.
    */
    void            SetMouseBufferDuration(float dMouseHistoryMaxLength);

    /// Returns the ammount of mouse movement to be buffered.
    /** \return The ammount of mouse movement to be buffered
    */
    float GetMouseBufferDuration() const;

    /// Sets the mouse movement factor.
    /** \param fMouseSensibility The new movement factor
    *   \note Increase this parameter to make mouse controlled movement faster.
    */
    void            SetMouseSensibility(float fMouseSensibility);

    /// Returns the mouse movement factor.
    /** \return The mouse movement factor
    */
    float  GetMouseSensibility() const;

    /// Sets whether input should be stopped.
    /** \param bFocus 'true' to stop inputs
    *   \note This function is usefull if you need to implement
    *         an edit box : the user can type letters binded to
    *         actions in the game, and you should prevent them
    *         from happening. So, you just have to call this function
    *         and use the second argument of all input functions to
    *         force focus in your edit box.
    */
    void            SetFocus(bool bFocus);

    static const std::string CLASS_NAME;

protected :

    /// Default constructor.
    /** \note This is a singleton class. That means that there can
    *         only be a single instance at the same time. That's why
    *         the constructor is not in public scope.<br>
    *         If you need to get a pointer to that unique instance,
    *         call InputManager::GetSingleton().<br>
    *         Note that, if GetSingleton() has been called at least once,
    *         you'll have to delete the returned pointer when you close
    *         your program by calling InputManager::Delete() (this is
    *         automatically done by Engine).
    */
    InputManager();

    /// Destructor.
    /** \note If GetSingleton() has been called at least once,
    *         you'll have to delete the returned pointer when
    *         you close your program by calling Delete() (this is
    *         automatically done by Engine).
    */
    ~InputManager();

    /// Copy constructor.
    InputManager(const InputManager& mMgr);

    /// Assignment operator.
    InputManager& operator = (const InputManager& mMgr);

private :

    sf::Window* pWindow_;

    bool bFocus_ = false;

    // Keyboard
    std::map<KeyCode, float> lKeyDelay_;
    std::map<KeyCode, bool>  lKeyLong_;
    std::map<KeyCode, bool>  lKeyBuf_;
    std::map<KeyCode, bool>  lKeyBufOld_;

    bool bCtrlPressed_ = false;
    bool bShiftPressed_ = false;
    bool bAltPressed_ = false;
    bool bKey_ = false;
    char cChar_ = 0;

    std::vector<uint_t> lDownStack_;
    std::vector<uint_t> lUpStack_;

    // Mouse
    float               fDoubleClickTime_ = 0.0;
    std::array<float,3>    lDoubleClickDelay_;
    std::array<float,3>    lMouseDelay_;
    std::array<bool,3>     lMouseLong_;
    std::array<bool,3>     lMouseBuf_;
    std::array<bool,3>     lMouseBufOld_;
    std::array<MouseState,3> lMouseState_;

    std::map<std::string, bool> lClickGroupList_;
    std::map<std::string, bool> lForcedClickGroupList_;

    float fMX_ = 0.0, fMY_ = 0.0;
    float fDMX_ = 0.0, fDMY_ = 0.0;
    float fRawDMX_ = 0.0, fRawDMY_ = 0.0;
    float fMouseSensibility_ = 0.0;
    float fMouseHistoryMaxLength_ = 0.0;
    std::vector< std::pair< float, std::array<float,3> > > lMouseHistory_;
    int   iMWheel_ = 0;
    bool  bWheelRolled_ = false;
    std::string   sMouseButton_;
    bool  bLastDragged_ = false;

    float fScreenWidth_ = 0.0, fScreenHeight_ = 0.0;
};

#endif
