// univcoord.h
//
// Copyright (C) 2001-2009, the Celestia Development Team
// Original version by Chris Laurel <claurel@gmail.com>
//
// Universal coordinate is a high-precision fixed point coordinate for
// locating objects in 3D space on scales ranging from millimeters to
// thousands of light years.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#pragma once

#include <Eigen/Core>

#include <celastro/astro.h>
#include <celutil/r128.h>
#include <celutil/r128util.h>


class UniversalCoord
{
 public:
    UniversalCoord() = default;

    UniversalCoord(R128 _x, R128 _y, R128 _z) :
        x(_x), y(_y), z(_z)
    {
    }

    UniversalCoord(double _x, double _y, double _z) :
        x(_x), y(_y), z(_z)
    {
    }

    explicit UniversalCoord(const Eigen::Vector3d& v) :
        x(v.x()), y(v.y()), z(v.z())
    {
    }

    friend UniversalCoord operator+(const UniversalCoord&, const UniversalCoord&);
    friend UniversalCoord operator-(const UniversalCoord&, const UniversalCoord&);

    /** Compute a universal coordinate that is the sum of this coordinate and
      * an offset in kilometers.
      */
    UniversalCoord offsetKm(const Eigen::Vector3d& v)
    {
        Eigen::Vector3d vUly = v * celestia::astro::kilometersToMicroLightYears(1.0);
        return *this + UniversalCoord(vUly);
    }

    /** Compute a universal coordinate that is the sum of this coordinate and
      * an offset in micro-light years.
      *
      * This method is only here to help in porting older code; it shouldn't be
      * necessary to use it in new code, where the use of the rather the rather
      * obscure unit micro-light year isn't necessary.
      */
    UniversalCoord offsetUly(const Eigen::Vector3d& vUly)
    {
        return *this + UniversalCoord(vUly);
    }

    /** Get the offset in kilometers of this coordinate from another coordinate.
      * The result is double precision, calculated as (this - uc) * scale, where
      * scale is a factor that converts from Celestia's internal units to kilometers.
      */
    Eigen::Vector3d offsetFromKm(const UniversalCoord& uc) const
    {
        return Eigen::Vector3d(static_cast<double>(x - uc.x),
                               static_cast<double>(y - uc.y),
                               static_cast<double>(z - uc.z)) * celestia::astro::microLightYearsToKilometers(1.0);
    }

    /** Get the offset in light years of this coordinate from a point (also with
      * units of light years.) The difference is calculated at high precision and
      * the reduced to single precision.
      */
    Eigen::Vector3f offsetFromLy(const Eigen::Vector3f& v) const
    {
        Eigen::Vector3f vUly = v * 1.0e6f;
        Eigen::Vector3f offsetUly(static_cast<float>(static_cast<double>(x - R128(vUly.x()))),
                                  static_cast<float>(static_cast<double>(y - R128(vUly.y()))),
                                  static_cast<float>(static_cast<double>(z - R128(vUly.z()))));
        return offsetUly * 1.0e-6f;
    }

    /** Get the offset in light years of this coordinate from a point (also with
      * units of light years.) The difference is calculated at high precision and
      * the reduced to single precision.
      *
      * This method is only here to help in porting older code; it shouldn't be
      * necessary to use it in new code, where the use of the rather the rather
      * obscure unit micro-light year isn't necessary.
      */
    Eigen::Vector3d offsetFromUly(const UniversalCoord& uc) const
    {
        return Eigen::Vector3d(static_cast<double>(x - uc.x),
                               static_cast<double>(y - uc.y),
                               static_cast<double>(z - uc.z));
    }

    /** Get the value of the coordinate in light years. The result is truncated to
      * double precision.
      */
    Eigen::Vector3d toLy() const
    {
        return Eigen::Vector3d(static_cast<double>(x),
                               static_cast<double>(y),
                               static_cast<double>(z)) * 1.0e-6;
    }

    double distanceFromKm(const UniversalCoord& uc) const
    {
        return offsetFromKm(uc).norm();
    }

    double distanceFromLy(const UniversalCoord& uc) const
    {
        return celestia::astro::kilometersToLightYears(offsetFromKm(uc).norm());
    }

    static UniversalCoord Zero()
    {
        // Default constructor returns zero, but this static method is clearer
        return UniversalCoord();
    }

    /** Convert double precision coordinates in kilometers to high precision
      * universal coordinates.
      */
    static UniversalCoord CreateKm(const Eigen::Vector3d& v)
    {
        Eigen::Vector3d vUly = v * celestia::astro::microLightYearsToKilometers(1.0);
        return UniversalCoord(vUly.x(), vUly.y(), vUly.z());
    }

    /** Convert double precision coordinates in light years to high precision
      * universal coordinates.
      */
    static UniversalCoord CreateLy(const Eigen::Vector3d& v)
    {
        Eigen::Vector3d vUly = v * 1.0e6;
        return UniversalCoord(vUly.x(), vUly.y(), vUly.z());
    }

    /** Convert double precision coordinates in micro-light years to high precision
      * universal coordinates. This method is intended only for porting older code;
      * it should not be used by new code.
      */
    static UniversalCoord CreateUly(const Eigen::Vector3d& v)
    {
        Eigen::Vector3d vUly = v;
        return UniversalCoord(vUly.x(), vUly.y(), vUly.z());
    }

    bool isOutOfBounds() const
    {
        using celestia::util::isOutOfBounds;
        return isOutOfBounds(x) || isOutOfBounds(y) || isOutOfBounds(z);
    }

public:
    R128 x { 0 };
    R128 y { 0 };
    R128 z { 0 };

};

inline UniversalCoord operator+(const UniversalCoord& uc0, const UniversalCoord& uc1)
{
    return UniversalCoord(uc0.x + uc1.x, uc0.y + uc1.y, uc0.z + uc1.z);
}

inline UniversalCoord operator-(const UniversalCoord& uc0, const UniversalCoord& uc1)
{
    return UniversalCoord(uc0.x - uc1.x, uc0.y - uc1.y, uc0.z - uc1.z);
}
