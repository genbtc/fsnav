## FSNav (for Windows) - ported by genBTC - active @ December 2017
This is a Windows fork of the Linux program FSNav. [This Repo URL](https://github.com/genbtc/fsnav) <br>
This repo [is a fork of](https://github.com/jtsiomb/fsnav/) which is a clone of [the original fsv](http://fsv.sourceforge.net/). <br>
The original license holder is [John Tsiombikas <nuclear@member.fsf.org>](https://github.com/jtsiomb) and his code has been modified under the GPLv3. <br>
Included are original source versions of jtsiomb's libraries ["Vmath"](https://github.com/jtsiomb/libvmath) and ["Imago"](https://github.com/jtsiomb/libimago) both also available from the [jtsiomb github](https://github.com/jtsiomb/), but were also distributed as part of his [fsnav](https://github.com/jtsiomb/fsnav/) github repo. <br>

**Screenshots**

![shot1](http://nuclear.mutantstargoat.com/sw/misc/fsnav1_thumb.png)
![shot2](http://nuclear.mutantstargoat.com/sw/misc/fsnav2_thumb.png)

About FSNav
------------

3D filesystem navigator inspired by the early 90s fsn program on SGI workstations.

Visualizes the filesystem starting from the working directory, or any directory passed as a commandline argument to the program. Also using the -s option enables stereoscopic rendering, which requires quad-buffer stereo visuals, or a stereoscopic wrapper such as: http://github.com/jtsiomb/stereowrap

Double-click to move to any directory box, rotate view by dragging with the left mouse button, and zoom by dragging with the right mouse button. 
Clicking on files or holding the spacebar while hovering over them displays file attributes.

**Windows Install Instructions**

1. Install MSYS2 (MingW64), set it up and update it several times with `pacman -Syu`, making sure you end task or restart it like it says.<br>
2a. Install basic dependencies (required): `pacman -S unzip base-devel make` <br>
2b. Install platform speficic mingw64 dependencies (required): `pacman -S mingw-w64-x86_64-{toolchain,freeglut,freetype,libpng,libjpeg-turbo}` <br>
This expands to something like: mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-freeglut mingw-w64-x86_64-freetype mingw-w64-x86_64-libpng mingw-w64-x86_64-libjpeg-turbo <br>
3a. Change Dir (cd) into your home directory: `cd ~/` <br>
3b. Clone the github repository: `git clone git@github.com:genbtc/fsnav.git` <br>
3c. Change Dir (cd) into the new fsnav dir: `cd fsnav` <br>
4. Compile/Install FSNav Instructions (short version):
	- `make -j12`       &nbsp;&nbsp;&nbsp;&nbsp;(parallel build with 12 threads)
	- `./fsnav.exe`
    - `make install` and `make clean` also works.
5. Note: The `data` folder has to be next to the executable for it to find the fonts.

***Has also been set up to use CMake - check CMakeLists.txt*** <br>
***Also works with Visual Studio 2017's Open Folder w/ CppProperties.json using CMake.*** <br>
***Attempted to set up for Visual Studio code using the .vscode dir but failed. Revisit when VSCode is more documented for MingW*** <br>

**More Details on my blog**
A More Step by step Tutorial on Compiling Insructions Can be found @ [this location - my blog](https://mrlithium.blogspot.com/2017/07/compiling-fsv-jurassic-park-program-on.html) <br>
However it was written for FSV not FSnav but its almost the same. Just substitute in this program. This one is easier, just "make".

**Picture Proving it works**
![MingWCompiles](/BuildingFSNav-with-Cmake-MingW.png?raw=true "MingW Compiles on Windows OK") <br>

License
------------

Copyright (C) 2009 John Tsiombikas nuclear@member.fsf.org This program is free software. Feel free to use, modify, and/or redistribute it under the terms of the GNU General Public License version 3, or at your option, any later version published by the Free Software Foundation. See COPYING for details.