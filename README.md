## IW3SP_MOD - Modification for Call of Duty 4: Modern Warfare

SP client modification for Call of Duty 4: Modern Warfare (IW3). This client based on the [IW4x Client](https://github.com/XLabsProject/iw4x-client).

<p align="center">
  <img src="assets/github/banner.png?raw=true"/>
</p>

___

# Credits
- [Bogdan Konstantinou AKA Nikolai Belinski](https://github.com/Nikolai2390)
- [xoxor4d - IW3xo](https://github.com/xoxor4d/iw3xo-dev)
- [X Labs Team](https://github.com/XLabsProject)
- [Nukem9 - LinkerMod](https://github.com/Nukem9/LinkerMod)
- [CoD4X Server/Client](https://github.com/callofduty4x)

## Disclaimer
This software has been created purely for the purposes of academic research. Project maintainers are not responsible or liable for misuse of the software. Use responsibly.
___

# How to install modification? (For regular users)
1. Download the .zip archive with .exe game and .dll file.
2. Place .exe and .dll in your game directory.
3. Launch the game and ENJOOOY!

# How to compile from source? (For advanced users)
1. Clone the this repository. I recommend use the [git-scm](https://git-scm.com/downloads).
<br>P.S: (**Downloading the zip does not include deps!**)</br>
2. Find any directory which you want the placed the content of repository, then open context menu and find "Git Bash Here" and write next line in the cmd: `git clone https://github.com/JerryALT/iw3sp_mod.git`
3. After cloning click the generate.bat and wait when all files will be ready.
4. Open build folder and find the iw3sp_dev.sln file and compile.

## Features
- Increased asset limits for IW3
  - FX: 1200
  - Image: 7168
  - Loaded Sound: 2700
  - Streamed Sound: 24000
  - Material: 8192
  - Weapon: 2400
  - Xmodel: 5125
- Adding the custom methods and functions of GSC.
- Adding the new dvars of all types in the game.
- Loading additional .ff files `[level name]_patch.ff`.
- Adding FOV logic from multi player to single player.
- Map entities editing on the stock map.
