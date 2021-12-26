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

#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <MtlCollection.hpp>

const std::map<std::string, MtlCollection::IllumModel> MtlCollection::s_illumModelMap{
    { "1", IllumModel::ColorAndAmbient },
    { "2", IllumModel::HighlightOn }
};

MtlCollection::MtlCollection(const std::string& rawPath)
{
    // Resolve absolute path
    m_parentPath = std::filesystem::absolute({ rawPath }).parent_path();

    std::ifstream fileStream{ rawPath };
    std::string line;

    std::string currentMaterialName;
    while (std::getline(fileStream, line)) {
        if (line.empty())
            continue;

        std::istringstream lineStream{ line };
        std::string keyword;

        if (!(lineStream >> keyword) || keyword[0] == '#')
            continue;

        if (keyword == "newmtl") {
            assert(lineStream >> currentMaterialName);
            m_collection[currentMaterialName] = Material{ m_parentPath };
        } else if (k_keywordMap.count(keyword)) {
            // Obtain parameters
            std::vector<std::string> parts;
            std::string part;
            while (lineStream >> part)
                parts.push_back(part);

            k_keywordMap.at(keyword)(m_collection[currentMaterialName], parts);
        }
#ifdef _DEBUG
        else {
            std::cerr << "WARN: unimplemented material parameter " << keyword << std::endl;
        }
#endif
    }
}
