// SpaceCollider -- SGI Lab Task 9
// Copyright (c) 2021-2022 Ángel Pérez <aperpor@upv.edu.es>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#pragma once

#include <filesystem>
#include <functional>
#include <map>
#include <string>
#include <vector>

#include <GL/freeglut.h>
#include <MtlCollection.hpp>
#include <Util.hpp>
#include <Vec2.hpp>
#include <Vec3.hpp>

struct Model {
    Model(const std::string& rawPath, const Vec3<float>& position = {}, const Vec3<float>& scale = { 1, 1, 1 }, float rotationAngle = 0, const Vec3<float>& rotationAxis = {});

    virtual void update(float dt);
    virtual void render() const;

    const Vec3<float>& getPosition() const { return m_position; }
    const Vec3<float>& getScale() const { return m_scale; }
    float getRotationAngle() const { return m_rotationAngle; }
    const Vec3<float>& getRotationAxis() const { return m_rotationAxis; }

    std::shared_ptr<MtlCollection> getMaterialCollection() { return m_mtlCollection; }

    void setPosition(const Vec3<float>& position) { m_position = position; }
    void setScale(const Vec3<float>& scale) { m_scale = scale; }
    void setRotation(float angle, const Vec3<float>& axis)
    {
        m_rotationAngle = angle;
        m_rotationAxis = axis;
    }

protected:
    Vec3<float> m_position;
    Vec3<float> m_scale;
    float m_rotationAngle;
    Vec3<float> m_rotationAxis;

    void renderNoTransform() const;

private:
    std::vector<Vec3<float>> m_vertices;
    std::vector<Vec3<float>> m_normals;
    std::vector<Vec2<float>> m_textureCoordinates;

    // Path of the parent directory containing this model file
    // This is used to resolve absolute material library paths
    std::filesystem::path m_parentPath;

    // Material collection (if any)
    std::shared_ptr<MtlCollection> m_mtlCollection;
    MtlCollection::Material* m_currentMaterial { nullptr };

    // Parameter setting function type alias
    using SetParamFn = std::function<void(Model&, std::vector<std::string>)>;

    // Keyword-parameter setting function mapping
    const std::map<std::string, SetParamFn> k_keywordMap {
        // Load material library
        { "mtllib", [](auto& model, auto params) {
             assert(params.size() == 1);
             assert(!model.m_mtlCollection);

             // Obtain full file path to the material library
             const std::filesystem::path filePath { params[0] };
             const auto fullPath = model.m_parentPath / filePath;

             // Load and parse the material library
             model.m_mtlCollection = std::make_unique<MtlCollection>(fullPath.generic_string());
         } },

        // Use material
        { "usemtl", [](auto& model, auto params) {
             assert(params.size() == 1);
             assert(model.m_mtlCollection);
             model.m_currentMaterial = &model.m_mtlCollection->getMaterial(params[0]);
         } },

        // Vertex positions
        { "v", [](auto& model, auto params) {
             assert(params.size() == 3);
             model.m_vertices.emplace_back(std::stof(params[0]), std::stof(params[1]), std::stof(params[2]));
         } },

        // Vertex normals
        { "vn", [](auto& model, auto params) {
             assert(params.size() == 3);
             model.m_normals.emplace_back(std::stof(params[0]), std::stof(params[1]), std::stof(params[2]));
         } },

        // Texture coordinates
        { "vt", [](auto& model, auto params) {
             assert(params.size() == 2);
             model.m_textureCoordinates.emplace_back(std::stof(params[0]), std::stof(params[1]));
         } },

        // Faces
        { "f", [](auto& model, auto params) {
             // See <https://webglfundamentals.org/webgl/lessons/webgl-load-obj.html>
             // (1) f 1 2 3              # indices for positions only
             // (2) f 1/1 2/2 3/3        # indices for positions and texcoords
             // (3) f 1/1/1 2/2/2 3/3/3  # indices for positions, texcoords, and normals
             // (4) f 1//1 2//2 3//3     # indices for positions and normals

             // TODO: support for n > 3 vertices? (GL_QUADS, GL_POLYGON)
             assert(params.size() == 3);
             assert(model.m_currentMaterial != nullptr);

             for (const auto& param : params) {
                 std::vector<std::string> indices = Util::splitString(param, '/');
                 // TODO: support for incomplete face indices (cases 1, 2, 4)
                 assert(indices.size() == 3);
                 assert(!indices[0].empty() && !indices[1].empty() && !indices[2].empty());

                 // Heads up! OBJ indices are 1-based, not 0-based like std::vector
                 const auto textureCoordinateIndex = std::stoi(indices[1]) - 1;
                 model.m_currentMaterial->vertexBuffer.push_back(model.m_textureCoordinates[textureCoordinateIndex].x);
                 model.m_currentMaterial->vertexBuffer.push_back(model.m_textureCoordinates[textureCoordinateIndex].y);

                 const auto normalIndex = std::stoi(indices[2]) - 1;
                 model.m_currentMaterial->vertexBuffer.push_back(model.m_normals[normalIndex].x);
                 model.m_currentMaterial->vertexBuffer.push_back(model.m_normals[normalIndex].y);
                 model.m_currentMaterial->vertexBuffer.push_back(model.m_normals[normalIndex].z);

                 const auto positionIndex = std::stoi(indices[0]) - 1;
                 model.m_currentMaterial->vertexBuffer.push_back(model.m_vertices[positionIndex].x);
                 model.m_currentMaterial->vertexBuffer.push_back(model.m_vertices[positionIndex].y);
                 model.m_currentMaterial->vertexBuffer.push_back(model.m_vertices[positionIndex].z);
             }
         } }
    };
};
