# SpaceCollider
> Final project for the Introduction to Interactive Graphics (SGI) course at ETSINF UPV.

![license: GNU GPL v2](https://img.shields.io/badge/license-GNU_GPL_v2-brightgreen.svg)
![works: in my machine](https://img.shields.io/badge/works-in_my_machine-brightgreen.svg)

![SpaceCollider screenshot](https://raw.githubusercontent.com/anpep/SpaceCollider/master/screenshot.png)

## Requirements
### GNU/Linux
- A recent GNU/Linux distribution with PulseAudio
- `libfreeimage`, `libGL`, `libGLU`, `libpulse-simple`
- A recent `g++` version with C++20 support
- GNU `make`

### Windows
- Microsoft Visual Studio 2019 or later
- Clone the repository with `--recursive` flag so the required `codebase` directory is downloaded.

## Build instructions
### GNU/Linux
```
# Clone the repository
git clone https://github.com/anpep/SpaceCollider
# Build the game
cd SpaceCollider && make -j8
# Enjoy!
./SpaceCollider
```

### Windows
```
# Clone the repository and submodules
git clone --recursive https://github.com/anpep/SpaceCollider
# Open in Visual Studio 2019 or greater
devenv SpaceCollider\SpaceCollider.sln
```

## License
SpaceCollider is licensed under the GNU GPL v2 open-source license.
```
SpaceCollider -- SGI Lab Task 9
Copyright (c) 2021-2022 Ángel Pérez <aperpor@upv.edu.es>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
```