#ifndef POINT_H
#define POINT_H

#include "utils.h"

/// A simple 2d coordinate storage.
/** This class is used to store a two dimensions
*   set of coordinates (x, y). It provides a few
*   basic operations (+, -, (in)equality).<br>
*   You have to choose the coordinate value type.
*/
template<class T>
class Point
{
public :

    Point()
    {
    }

    /** \param mX The horizontal coordinate
    *   \param mY The vertical coordinate
    */
    Point(const T& mX, const T& mY) : mX_(mX), mY_(mY)
    {
    }

    ~Point()
    {
    }

    /// Sets the point's coordinates.
    /** \param mX The new horizontal coordinate
    *   \param mY The new vertical coordinate
    */
    void Set(const T& mX, const T& mY)
    {
        mX_ = mX; mY_ = mY;
    }

    T GetNorm() const
    {
        return T(std::sqrt(mX_*mX_ + mY_*mY_));
    }

    T GetNormSquared() const
    {
        return mX_*mX_ + mY_*mY_;
    }

    void Normalize()
    {
        T mNorm = T(std::sqrt(mX_*mX_ + mY_*mY_));
        mX_ = mX_/mNorm;
        mY_ = mY_/mNorm;
    }

    Point GetUnit() const
    {
        T mNorm = T(std::sqrt(mX_*mX_ + mY_*mY_));
        return Point(mX_/mNorm, mY_/mNorm);
    }

    void Rotate(float fAngle)
    {
        Point p;

        p.mX_ = mX_*T(std::cos(fAngle)) - mY_*T(std::sin(fAngle));
        p.mY_ = mY_*T(std::sin(fAngle)) + mY_*T(std::cos(fAngle));

        mX_ = p.mX_;
        mY_ = p.mY_;
    }

    Point GetRotation(float fAngle) const
    {
        return Point(
            mX_*T(std::cos(fAngle)) - mY_*T(std::sin(fAngle)),
            mY_*T(std::sin(fAngle)) + mY_*T(std::cos(fAngle))
        );
    }

    Point operator + (const Point& mPoint)  const
    {
        return Point(mX_ + mPoint.mX_, mY_ + mPoint.mY_);
    }
    void operator += (const Point& mPoint)
    {
        mX_ += mPoint.mX_; mY_ += mPoint.mY_;
    }

    Point operator - () const
    {
        return Point(-mX_, -mY_);
    }

    Point operator - (const Point& mPoint) const
    {
        return Point(mX_ - mPoint.mX_, mY_ - mPoint.mY_);
    }
    void operator -= (const Point& mPoint)
    {
        mX_ -= mPoint.mX_; mY_ -= mPoint.mY_;
    }

    bool operator == (const Point& mPoint) const
    {
        return ( (mX_ == mPoint.mX_) && (mY_ == mPoint.mY_) );
    }
    bool operator != (const Point& mPoint) const
    {
        return !( (mX_ == mPoint.mX_) && (mY_ == mPoint.mY_) );
    }

    Point operator * (const T& mValue) const
    {
        return Point(mX_*mValue, mY_*mValue);
    }

    Point operator / (const T& mValue) const
    {
        return Point(mX_/mValue, mY_/mValue);
    }

    T operator * (const Point& mPoint) const
    {
        return mX_*mPoint.mX_ + mY_*mPoint.mY_;
    }

    const T& X() const
    {
        return mX_;
    }
    const T& Y() const
    {
        return mY_;
    }

    T& X()
    {
        return mX_;
    }
    T& Y()
    {
        return mY_;
    }

private :

    /// The horizontal coordinate
    T mX_ = 0;
    /// The vertical coordinate
    T mY_ = 0;
};

template<class T>
Point<T> operator * (const T& mValue, const Point<T>& mVec)
{
    return Point<T>(mVec.X()*mValue, mVec.Y()*mValue);
}

template<class T>
Point<T> operator / (const T& mValue, const Point<T>& mVec)
{
    return Point<T>(mVec.X()/mValue, mVec.Y()/mValue);
}

using Vector2D = Point<float>;

#endif
