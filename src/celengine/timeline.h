// timeline.h
//
// Object timelines.
//
// Copyright (C) 2008, the Celestia Development Team
// Initial version by Chris Laurel, claurel@gmail.com
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#pragma once

#include <memory>
#include <vector>
#include "timelinephase.h"

class Timeline
{
public:
    Timeline() = default;
    ~Timeline();

    const TimelinePhase::SharedConstPtr& findPhase(double t) const;
    bool appendPhase(TimelinePhase::SharedConstPtr&);
    const TimelinePhase::SharedConstPtr& getPhase(unsigned int n) const;
    unsigned int phaseCount() const;

    double startTime() const;
    double endTime() const;
    bool includes(double t) const;

    void markChanged();

private:
    std::vector<TimelinePhase::SharedConstPtr> phases;
};
