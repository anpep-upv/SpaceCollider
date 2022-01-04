// SpaceCollider -- SGI Lab Task 9
// Copyright (c) 2021-2022 Ãngel PÃ©rez <aperpor@upv.edu.es>
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

#include <fstream>
#include <iostream>

#include <Model.hpp>

Model::Model(const std::string& rawPath, const Vec3<float>& position, const Vec3<float>& scale, const float rotationAngle, const Vec3<float>& rotationAxis)
    : m_position(position)
    , m_scale(scale)
    , m_rotationAngle(rotationAngle)
    , m_rotationAxis(rotationAxis)
    , m_mtlCollection(nullptr)
{
    // Resolve absolute path
    m_parentPath = std::filesystem::absolute({ rawPath }).parent_path();

    std::ifstream fileStream(rawPath);
    std::string line;

    while (std::getline(fileStream, line)) {
        if (line.empty())
            continue;

        std::istringstream lineStream { line };
        std::string keyword;

        if (!(lineStream >> keyword) || keyword[0] == '#')
            continue;

        if (k_keywordMap.count(keyword)) {
            // Obtain parameters
            std::vector<std::string> parts;
            std::string part;
            while (lineStream >> part)
                parts.push_back(part);

            k_keywordMap.at(keyword)(*this, parts);
        }
    }
}

void Model::update(float dt) { }

void Model::render() const
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    for (const auto& [_, material] : m_mtlCollection->getCollection()) {
        if (!material->isVisible)
            continue;

        glMaterialf(GL_FRONT, GL_SHININESS, material->specularExponent);
        glMaterialfv(GL_FRONT, GL_AMBIENT, material->ambientReflectivity);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material->diffuseReflectivity);
        glMaterialfv(GL_FRONT, GL_SPECULAR, material->specularReflectivity);
        glMaterialfv(GL_FRONT, GL_EMISSION, material->emissiveReflectivity);

        glPushMatrix();
        glTranslatef(m_position.x, m_position.y, m_position.z);
        glScalef(m_scale.x, m_scale.y, m_scale.z);
        glRotatef(m_rotationAngle, m_rotationAxis.x, m_rotationAxis.y, m_rotationAxis.z);

        if (material->texture) {
            glEnable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            material->texture->bind();
        }

        glTexCoordPointer(2, GL_FLOAT, 8 * sizeof(float), material->vertexBuffer.data());
        glNormalPointer(GL_FLOAT, 8 * sizeof(float), material->vertexBuffer.data() + 2);
        glVertexPointer(3, GL_FLOAT, 8 * sizeof(float), material->vertexBuffer.data() + 5);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(material->vertexBuffer.size()) / 8);

        if (material->texture)
            material->texture->unbind();

        glPopMatrix();
    }

    glDisable(GL_COLOR_MATERIAL);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
