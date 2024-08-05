// timelinephase.cpp
//
// Object timeline phase
//
// Copyright (C) 2008, the Celestia Development Team
// Initial version by Chris Laurel, claurel@gmail.com
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#include "timelinephase.h"

#include <cassert>

#include <celephem/orbit.h>
#include <celephem/rotation.h>
#include "body.h"
#include "frame.h"
#include "frametree.h"
#include "universe.h"


TimelinePhase::TimelinePhase(CreateToken,
                             Body* _body,
                             double _startTime,
                             double _endTime,
                             const ReferenceFrame::SharedConstPtr& _orbitFrame,
                             const std::shared_ptr<const celestia::ephem::Orbit>& _orbit,
                             const ReferenceFrame::SharedConstPtr& _bodyFrame,
                             const std::shared_ptr<const celestia::ephem::RotationModel>& _rotationModel,
                             FrameTree* _owner) :
    m_body(_body),
    m_startTime(_startTime),
    m_endTime(_endTime),
    m_orbitFrame(_orbitFrame),
    m_orbit(_orbit),
    m_bodyFrame(_bodyFrame),
    m_rotationModel(_rotationModel),
    m_owner(_owner)
{
    // assert(owner == orbitFrame->getCenter()->getFrameTree());
}

/*! Create a new timeline phase in the specified universe.
 */
TimelinePhase::SharedConstPtr
TimelinePhase::CreateTimelinePhase(Universe& universe,
                                   Body* body,
                                   double startTime,
                                   double endTime,
                                   const ReferenceFrame::SharedConstPtr& orbitFrame,
                                   const std::shared_ptr<const celestia::ephem::Orbit>& orbit,
                                   const ReferenceFrame::SharedConstPtr& bodyFrame,
                                   const std::shared_ptr<const celestia::ephem::RotationModel>& rotationModel)
{
    // Validate the time range.
    if (endTime <= startTime)
        return nullptr;

    // Get the frame tree to add the new phase to. Verify that the reference frame
    // center is either a star or solar system body.
    FrameTree* frameTree = nullptr;
    Selection center = orbitFrame->getCenter();
    if (center.body() != nullptr)
    {
        frameTree = center.body()->getOrCreateFrameTree();
    }
    else if (center.star() != nullptr)
    {
        const SolarSystem* solarSystem = universe.getOrCreateSolarSystem(center.star());
        frameTree = solarSystem->getFrameTree();
    }
    else
    {
        // Frame center is not a star or body.
        return nullptr;
    }

    auto phase = std::make_shared<TimelinePhase>(CreateToken(),
                                                 body,
                                                 startTime,
                                                 endTime,
                                                 orbitFrame,
                                                 orbit,
                                                 bodyFrame,
                                                 rotationModel,
                                                 frameTree);

    frameTree->addChild(phase);

    return phase;
}
