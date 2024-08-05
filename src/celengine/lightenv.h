// lightenv.h
//
// Structures that describe the lighting environment for rendering objects
// in Celestia.
//
// Copyright (C) 2006, Chris Laurel <claurel@shatters.net>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#pragma once

#include <celutil/color.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <vector>

constexpr unsigned int MaxLights = 8;

class Body;
class RingSystem;

class DirectionalLight
{
public:
    Color color;
    float irradiance;
    Eigen::Vector3f direction_eye;
    Eigen::Vector3f direction_obj;

    // Required for eclipse shadows only--may be able to use
    // distance instead of position.
    Eigen::Vector3d position;  // position relative to the lit object
    float apparentSize;
    bool castsShadows;
};

class EclipseShadow
{
public:
    const Body* caster;
    Eigen::Quaternionf casterOrientation;
    Eigen::Vector3f origin;
    Eigen::Vector3f direction;
    float penumbraRadius;
    float umbraRadius;
    float maxDepth;
};

class RingShadow
{
public:
    RingSystem* ringSystem;
    Eigen::Quaternionf casterOrientation;
    Eigen::Vector3f origin;
    Eigen::Vector3f direction;
    float texLod;
};

class LightingState
{
public:
    using EclipseShadowVector = std::vector<EclipseShadow>;

    LightingState() :
        nLights(0),
        shadowingRingSystem(nullptr),
        eyeDir_obj(-Eigen::Vector3f::UnitZ()),
        eyePos_obj(-Eigen::Vector3f::UnitZ())
    {
        shadows[0] = nullptr;
        for (auto &ringShadow : ringShadows)
        {
            ringShadow.ringSystem = nullptr;
        }
    };

    unsigned int nLights;
    DirectionalLight lights[MaxLights];
    EclipseShadowVector* shadows[MaxLights];
    RingShadow ringShadows[MaxLights];
    RingSystem* shadowingRingSystem; // nullptr when there are no ring shadows
    Eigen::Vector3f ringPlaneNormal;
    Eigen::Vector3f ringCenter;

    Eigen::Vector3f eyeDir_obj;
    Eigen::Vector3f eyePos_obj;

    Eigen::Vector3f ambientColor;
};
