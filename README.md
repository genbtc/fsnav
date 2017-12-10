## FSNav (for Windows) - ported by genBTC
This is a Windows fork of the Linux program FSNav
This repo [is a fork of](https://github.com/jtsiomb/fsnav/) which is a clone of [the original fsv](http://fsv.sourceforge.net/).
The original license holder is [John Tsiombikas <nuclear@member.fsf.org>](https://github.com/jtsiomb) and his code has been modified under the GPLv3.
He has included his own libraries "Vmath" and "Imago" both available from the jtsiomb github. 

**Windows Install Instructions**

1. Install MSYS2 (MingW64), set it up and update it several times with `pacman -Syu`, making sure you end task or restart it like it says.
2. Install dependencies (required): `pacman -S unzip base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-freeglut mingw-w64-x86_64-libpng mingw-w64-x86_64-libjpeg-turbo`
3. Clone the repository into your username's Home dir under MSYS2. for example: C:\Software\msys64\home\...\fsnav\ and `cd fsnav-master` into it
4. Compile/Install FSNav Instructions (short version):
	- make -j12
	- ./fsnav.exe
    - make install and make clean also works.
5. 
***Has also been set up to use Cmake and Visual Studio 2017 CppProperties.json.***
***Attempted to set up for Visual Studio code using the .vscode dir but failed. Revisit when VSCode is more documented for MingW***
    
**Full Tutorial on Compiling**
Insructions Can be found @ [this location](https://mrlithium.blogspot.com/2017/07/compiling-fsv-jurassic-park-program-on.html)
However it was written for FSV not FSnav but its almost the same. Just substitute in this program. This one is easier, just "make".

**About fsnav**

![shot1](http://nuclear.mutantstargoat.com/sw/misc/fsnav1_thumb.png) ![shot2](http://nuclear.mutantstargoat.com/sw/misc/fsnav2_thumb.png)

About FSNav
------------
3D filesystem navigator inspired by the early 90s fsn program on SGI workstations.

Visualizes the filesystem starting from the working directory, or any directory
passed as a commandline argument to the program. Also using the -s option
enables stereoscopic rendering, which requires quad-buffer stereo visuals, or a
stereoscopic wrapper such as: http://github.com/jtsiomb/stereowrap

Double-click to move to any directory box, rotate view by dragging with the left
mouse button, and zoom by dragging with the right mouse button. Clicking on files
or holding the spacebar while hovering over them displays file attributes.

License
-------
Copyright (C) 2009 John Tsiombikas <nuclear@member.fsf.org>
This program is free software. Feel free to use, modify, and/or redistribute it
under the terms of the GNU General Public License version 3, or at your option,
any later version published by the Free Software Foundation. See COPYING for
details.