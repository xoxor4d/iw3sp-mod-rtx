![license](https://img.shields.io/github/license/JerryALT/iw3sp_mod.svg)
[![Build](https://github.com/JerryALT/iw3sp_mod/workflows/Build/badge.svg)](https://github.com/JerryALT/iw3sp_mod/actions)
[![Discord](https://img.shields.io/discord/1091304681822752778?color=%237289DA&label=members&logo=discord&logoColor=%23FFFFFF)](https://discord.gg/YzNZSEMAnf)

# IW3SP_MOD - Modification for Call of Duty 4: Modern Warfare

Singleplayer client modification for Call of Duty 4: Modern Warfare (IW3). This client based on the [IW4x Client](https://github.com/XLabsProject/iw4x-client).

<p align="center">
  <img src="assets/github/banner.png?raw=true"/>
</p>

___

# Credits
- [Bogdan Konstantinou AKA Nikolai Belinski](https://github.com/Nikolai2390)
- [xoxor4d - IW3xo](https://github.com/xoxor4d/iw3xo-dev)
- [X Labs Team](https://github.com/XLabsProject)
- [fed - h2-mod](https://github.com/fedddddd/h2-mod)
- [Nukem9 - LinkerMod](https://github.com/Nukem9/LinkerMod)
- [CoD4X Server/Client](https://github.com/callofduty4x)

# Disclaimer
This software has been created purely for the purposes of academic research. Project maintainers are not responsible or liable for misuse of the software. Use responsibly.

# Features
- Increased asset limits for IW3
  - FX: 1200
  - Image: 7168
  - Loaded Sound: 2700
  - Streamed Sound: 24000
  - Material: 8192
  - Weapon: 2400
  - Xmodel: 5125
- Added the game achievements;
- Added the custom methods and functions for GSC;
- Added loading the additional .ff files `[level name]_patch.ff`;
- Added FOV logic from multiplayer to singleplayer;
- Added map entities editing for the stock maps and more...

# How to install modification? (For regular users)
**NOTE**: You must legally own Call of Duty® 4: Modern Warfare® (2007) to run this mod. Cracked/Pirated versions of the game are **NOT** supported.
1. Download the latest [release](https://github.com/JerryALT/iw3sp_mod/releases/latest)
2. Place the .zip contents into your cod4 root folder
3. Start the **iw3sp_mod.exe**

# How to compile from source? (For advanced users)
1. Clone the this repository. I recommend use the [git-scm](https://git-scm.com/downloads).
</br>P.S: (**Downloading the zip does not include deps!**)</br>
2. Find any directory which you want the placed the content of repository, then open context menu and find "Git Bash Here" and write next line in the cmd: `git clone https://github.com/JerryALT/iw3sp_mod.git`
3. After cloning click the generate.bat and wait when all files will be ready.
4. Open build folder and find the iw3sp_dev.sln file and compile.