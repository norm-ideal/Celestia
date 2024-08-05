// model.h
//
// Copyright (C) 2004-2010, Celestia Development Team
// Original version by Chris Laurel <claurel@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#pragma once

#include <array>
#include <cstddef>
#include <vector>

#include <Eigen/Core>

#include "material.h"
#include "mesh.h"


namespace cmod
{

/*!
 * Model is the standard geometry object in Celestia.  A Model
 * consists of a library of materials together with a list of
 * meshes.  Each mesh object contains a pool of vertices and a
 * set of primitive groups.  A primitive groups consists of a
 * a primitive group type and a list of vertex indices.  This
 * structure is exactly the one used in Celestia model (.cmod)
 * files.
 */
class Model
{
 public:
    Model();
    ~Model() = default;

    const Material* getMaterial(unsigned int index) const;
    bool setMaterial(unsigned int index, Material&& material);
    unsigned int addMaterial(Material&& material);

    /*! Return the number of materials in the model
     */
    unsigned int getMaterialCount() const;

    /*! Return the total number of vertices in the model
     */
    unsigned int getVertexCount() const;

    /*! Return the total number of primitives in the model
     */
    unsigned int getPrimitiveCount() const;

    Mesh* getMesh(unsigned int index);

    /*! Return the mesh with the specified index, or nullptr if the
     *  index is out of range.
     */
    const Mesh* getMesh(unsigned int index) const;

    /*! Return the total number of meshes withing the model.
     */
    unsigned int getMeshCount() const;

    /*! Add a new mesh to the model; the return value is the
     *  total number of meshes in the model.
     */
    unsigned int addMesh(Mesh&& mesh);

    /** Find the closest intersection between the ray (given
     *  by origin and direction) and the model. If the ray
     *  intersects the model, return true and fill in the
     *  pick result record. If the result record is nullptr, it
     *  ignored, and the function just computes whether or
     *  not the ray intersected the model.
     */
    bool pick(const Eigen::Vector3d& rayOrigin,
              const Eigen::Vector3d& rayDirection,
              Mesh::PickResult* result) const;

    /** Find the closest intersection between the ray (given
     *  by origin and direction) and the model. If the ray
     *  intersects the model, return true and set distance;
     *  otherwise return false and leave distance unmodified.
     */
    bool pick(const Eigen::Vector3d& rayOrigin,
              const Eigen::Vector3d& rayDirection,
              double& distance) const;

    void transform(const Eigen::Vector3f& translation, float scale);

    /** Apply a uniform scale to the model so that it fits into
     *  a box with a center at centerOffset and a maximum side
     *  length of one.
     */
    void normalize(const Eigen::Vector3f& centerOffset);

    /** Return true if the specified texture map type is used at
     *  all within a mesh. This information is used to decide
     *  if multiple rendering passes are required.
     */
    bool usesTextureType(TextureSemantic) const;

    /** Return true if the model has no translucent components. */
    bool isOpaque() const
    {
        return opaque;
    }

    bool isNormalized() const
    {
        return normalized;
    }

    /*! Set the opacity flag based on material usage within the model */
    void determineOpacity();


    class MeshComparator
    {
    public:
        virtual ~MeshComparator() = default;

        virtual bool operator()(const Mesh&, const Mesh&) const = 0;
    };

    /*! Sort the model's meshes in place. */
    void sortMeshes(const MeshComparator&);

    /*! Optimize the model by eliminating all duplicated materials */
    void uniquifyMaterials();

    /*! This comparator will roughly sort the model's meshes by
     *  opacity so that transparent meshes are rendered last.  It's far
     *  from perfect, but covers a lot of cases.  A better method of
     *  opacity sorting would operate at the primitive group level, or
     *  even better at the triangle level.
     *
     *  Standard usage for this class is:
     *     model->sortMeshes(Model::OpacityComparator());
     *
     *  uniquifyMaterials() should be used before sortMeshes(), since
     *  the opacity comparison depends on material indices being ordered
     *  by opacity.
     */
    class OpacityComparator : public MeshComparator
    {
    public:
        OpacityComparator() = default;
        bool operator()(const Mesh&, const Mesh&) const override;
    };

 private:
    std::vector<Material> materials{ };
    std::vector<Mesh> meshes{ };

    std::array<bool, static_cast<std::size_t>(TextureSemantic::TextureSemanticMax)> textureUsage;
    bool opaque{ true };
    bool normalized{ false };
};

} // namespace
