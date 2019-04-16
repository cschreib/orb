#ifndef MANAGER_H
#define MANAGER_H

#include "utils.h"

/// Abstract singleton pattern
template<class T>
class Manager
{
public :

    /// Returns a pointer to the singleton
    /** \note The singleton will be created on the very first call.
    */
    static T* GetSingleton()
    {
        if (!pMgr_)
            pMgr_ = new T();

        return pMgr_;
    }

    /// Deletes the singleton
    /** \note You should call that method when you're not using the
    *         singleton anymore, or if you want to reset it.<br>
    *         Use only if you've called GetSingleton at least once.
    */
    static void Delete()
    {
        delete pMgr_;
        pMgr_ = nullptr;
    }

protected :

    /// Default constructor
    /** \note This is a singleton class. That means that there can
    *         only be a single instance at the same time. That's why
    *         the constructor is not in public scope.<br>
    *         If you need to get a pointer to that unique instance,
    *         call Manager::GetSingleton().<br>
    *         Note that, if GetSingleton() has been called at least once,
    *         you'll have to delete the returned pointer when you close
    *         your program by calling Manager::Delete().
    */
    Manager() {}

    /// Destructor
    /** \note If GetSingleton() has been called at least once,
    *         you'll have to delete the returned pointer when you close your
    *         program by calling Manager::Delete() (this is automatically done
    *         by Engine).
    */
    ~Manager() {}

    /// Copy constructor
    Manager(const Manager& mMgr);

    /// Assignment operator
    Manager& operator = (const Manager& mMgr);

private :

    static T* pMgr_;

};

template<class T>
T* Manager<T>::pMgr_ = nullptr;

#endif
