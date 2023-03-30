#pragma once
#include <concepts>

class Length
{
public:
    Length(long double length, double unitLength) : _len(length), _unitsPerFoot(unitLength) {}

    template <typename T>
    requires std::derived_from<T, Length>
    T convertTo() const;

    void setLength(long double len) { _len = len; }

    double getUnitsPerFoot() const { return _unitsPerFoot; };
    long double getLength() const { return _len; }

private:
    long double _len;
    const double _unitsPerFoot;

    template <typename T, typename Y>
    requires std::derived_from<T, Length> && std::derived_from<Y, Length>
    friend T operator+(const T& lhs, const Y& rhs);

};

class Inches : public Length 
{
public:
    Inches(long double length = 0.0) : Length(length, 12.0) {}
};

class Feet : public Length 
{
public:
    Feet(long double length = 1.0) : Length(length, 1.0) {}
};

class Miles : public Length 
{
public:
    Miles(long double length = 0.0) : Length(length, 1.0 / 5280.0) {}
};

class Meters : public Length 
{
public:
    Meters(long double length = 0.0) : Length(length, 1.0 / 3.28084) {}
};

class Centimeters : public Length 
{
public:
    Centimeters(long double length = 0.0) : Length(length, 1.0 / 0.0328084) {}
};

class Kilometers : public Length 
{
public:
    Kilometers(long double length = 0.0) : Length(length, 1.0 / 3280.84) {}
};

Inches operator ""_in(long double l) { return Inches(l); }
Feet operator ""_ft(long double l) { return Feet(l); }
Miles operator ""_mi(long double l) { return Miles(l); }
Meters operator ""_m(long double l) { return Meters(l); }  
Centimeters operator ""_cm(long double l) { return Centimeters(l); }
Kilometers operator ""_km(long double l) { return Kilometers(l); }

template <typename T>
requires std::derived_from<T, Length>
T Length::convertTo() const
{
    T ret;
    ret.setLength(_len / _unitsPerFoot * ret.getUnitsPerFoot());
    return ret;
}

template <typename T, typename Y>
requires std::derived_from<T, Length> && std::derived_from<Y, Length>
inline T operator+(const T &lhs, const Y &rhs)
{
    return lhs + rhs.template convertTo<T>();
}
template <typename T>
requires std::derived_from<T, Length>
inline T operator+(const T &lhs, const T &rhs)
{
    return T(lhs.getLength() + rhs.getLength());
}
