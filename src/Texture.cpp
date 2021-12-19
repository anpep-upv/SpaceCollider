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

#include "Util.hpp"

#include <cassert>
#include <iostream>
#include <Texture.hpp>
#include <freeimage/FreeImage.h>

Texture::Texture(const std::string& path)
    : m_id{ 0 }
{
    // Generate texture
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    // Load image file
    const auto imageFormat = FreeImage_GetFileType(path.c_str(), 0);
    const auto imageDib = FreeImage_Load(imageFormat, path.c_str(), 0);
    assert(imageDib != NULL);
    const auto imageConverted = FreeImage_ConvertTo32Bits(imageDib);

    // Obtain image bounds and data
    const auto imageWidth = FreeImage_GetWidth(imageConverted);
    const auto imageHeight = FreeImage_GetHeight(imageConverted);
    const auto imageData = FreeImage_GetBits(imageConverted);

    // Load texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<int>(imageWidth), static_cast<int>(imageHeight), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, imageData);

    // Release FreeImage resources
    FreeImage_Unload(imageConverted);
    FreeImage_Unload(imageDib);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

void Texture::bind() const
{
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind() const
{
    glDisable(GL_TEXTURE_2D);
}
