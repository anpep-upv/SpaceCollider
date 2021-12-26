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

#include <cassert>
#include <filesystem>
#include <functional>
#include <map>
#include <string>
#include <vector>

#include <Texture.hpp>
#include <Vec2.hpp>
#include <Vec3.hpp>

struct MtlCollection {
    struct Material {
        std::vector<float> vertexBuffer {};

        float specularExponent {};
        float ambientReflectivity[4] {};
        float diffuseReflectivity[4] {};
        float specularReflectivity[4] {};
        float emissiveReflectivity[4] {};
        float dissolveFactor {};

        const std::filesystem::path &parentPath;
        Texture* texture { nullptr };

        explicit Material(const std::filesystem::path& parentPath)
            : parentPath(parentPath)
        {
        }

        ~Material()
        {
            delete texture;
            texture = nullptr;
        }
    };

    explicit MtlCollection(const std::string& rawPath);
    ~MtlCollection() = default;

    const std::map<std::string, Material>& getCollection() const
    {
        return m_collection;
    }

    Material* getMaterialPtr(const std::string& name)
    {
        return &m_collection.at(name);
    }

private:
    // Path of the parent directory containing this material library file
    // This is used to resolve absolute texture paths
    std::filesystem::path m_parentPath;

    std::map<std::string, Material> m_collection;

    // Parameter setting function type alias
    using SetParamFn = std::function<void(Material&, std::vector<std::string>)>;

    // Keyword-parameter setting function mapping
    const std::map<std::string, SetParamFn> k_keywordMap {
        // Specular exponent (shininess)
        { "Ns", [](auto& material, auto params) {
             assert(params.size() == 1);
             material.specularExponent = std::stof(params[0]);
         } },
        // Ambient reflectivity (RGB)
        { "Ka", [](auto& material, auto params) {
             assert(params.size() == 3);
             material.ambientReflectivity[0] = std::stof(params[0]);
             material.ambientReflectivity[1] = std::stof(params[1]);
             material.ambientReflectivity[2] = std::stof(params[2]);
             material.ambientReflectivity[3] = 1.0f;
         } },
        // Diffuse reflectivity (RGB)
        { "Kd", [](auto& material, auto params) {
             assert(params.size() == 3);
             material.diffuseReflectivity[0] = std::stof(params[0]);
             material.diffuseReflectivity[1] = std::stof(params[1]);
             material.diffuseReflectivity[2] = std::stof(params[2]);
             material.diffuseReflectivity[3] = 1.0f;
         } },
        // Specular reflectivity (RGB)
        { "Ks", [](auto& material, auto params) {
             assert(params.size() == 3);
             material.specularReflectivity[0] = std::stof(params[0]);
             material.specularReflectivity[1] = std::stof(params[1]);
             material.specularReflectivity[2] = std::stof(params[2]);
             material.specularReflectivity[3] = 1.0f;
         } },
        // Emissive reflectivity (RGB)
        { "Ke", [](auto& material, auto params) {
             assert(params.size() == 3);
             material.emissiveReflectivity[0] = std::stof(params[0]);
             material.emissiveReflectivity[1] = std::stof(params[1]);
             material.emissiveReflectivity[2] = std::stof(params[2]);
             material.emissiveReflectivity[3] = 1.0f;
         } },
        // Dissolve factor (opacity)
        { "d", [](auto& material, auto params) {
             assert(params.size() == 1);
             material.dissolveFactor = std::stof(params[0]);
         } },
        // Texture map
        { "map_Kd", [](auto& material, auto params) {
             assert(params.size() == 1);
             assert(material.texture == nullptr);

             // Obtain full file path to the texture file
             const std::filesystem::path filePath { params[0] };
             const auto fullPath = material.parentPath / filePath;

             material.texture = new Texture { fullPath.generic_string() };
         } }
    };
};
