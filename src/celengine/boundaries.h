// boundaries.h
//
// Copyright (C) 2002-2019, the Celestia Development Team
// Original version by Chris Laurel <claurel@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#pragma once

#include <iosfwd>
#include <memory>
#include <vector>

#include <Eigen/Core>

class ConstellationBoundaries
{
 public:
    using Chain = std::vector<Eigen::Vector3f>;

    explicit ConstellationBoundaries(std::vector<Chain>&&);
    ~ConstellationBoundaries() = default;
    ConstellationBoundaries(const ConstellationBoundaries&)            = delete;
    ConstellationBoundaries(ConstellationBoundaries&&)                 = delete;
    ConstellationBoundaries& operator=(const ConstellationBoundaries&) = delete;
    ConstellationBoundaries& operator=(ConstellationBoundaries&&)      = delete;

    const std::vector<Chain>& getChains() const;

 private:
    std::vector<Chain> chains;
};

std::unique_ptr<ConstellationBoundaries> ReadBoundaries(std::istream&);
