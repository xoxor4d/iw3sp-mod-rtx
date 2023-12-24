![license](https://img.shields.io/github/license/JerryALT/iw3sp_mod.svg)
[![Build](https://github.com/xoxor4d/iw3sp-mod-rtx/workflows/Build/badge.svg)](https://github.com/xoxor4d/iw3sp-mod-rtx/actions)
[![Discord](https://img.shields.io/discord/1091304681822752778?color=%237289DA&label=members&logo=discord&logoColor=%23FFFFFF)](https://discord.gg/YzNZSEMAnf)

# IW3SP-MOD-RTX - nvidia rtx-remix compatibility mod

Singleplayer client modification for Call of Duty 4: Modern Warfare (IW3) to make it compatible with nvidia's rtx-remix. This client is a fork of [IW3SP_Mod](https://github.com/JerryALT/iw3sp_mod) and originally based on the [IW4x Client](https://github.com/iw4x/iw4x-client).

<p align="center">
  <img src="assets/github/banner.png?raw=true"/>
</p>

## nvidia-remix-fork
Open [Github Actions](https://github.com/xoxor4d/iw3sp-mod-rtx/actions), select the latest successful build, grab the `Release-binaries-rtx` artifact and extract it`s contents into your cod4 root folder. 

<br>

### Commandline Arguments:
  - `-disable_culling` - disable all culling   
  ~~- `-fixed_function`- fixed-function rendering (static models, fps ++)~~ (not yet implemented)
  
> eg: &ensp;`"c:\path\iw3xo.exe" -disable_culling` 

### Additional settings
- Use console commands starting with `rtx_sky_` to spawn a skysphere (not yet working)

### General tips

- You might need to assign the sky category manually (if your map is black).   
Open remix -> Go to the game setup tab -> Step 1 -> Open Sky Texture -> Use your mouse to select the sky (in the world) and assign the sky category

___

# Credits
- [Jerry4LT - IW3SP_Mod](https://github.com/JerryALT/iw3sp_mod)
- [Bogdan Konstantinou](https://www.youtube.com/@BogdanKonstantinou)
- [xoxor4d - IW3xo](https://github.com/xoxor4d/iw3xo-dev)
- [X Labs Team](https://github.com/XLabsProject)
- [AlterWare](https://github.com/alterware)
- [fed - h2-mod](https://github.com/fedddddd/h2-mod)
- [Nukem9 - LinkerMod](https://github.com/Nukem9/LinkerMod)
- [CoD4X Server/Client](https://github.com/callofduty4x)
- [Vlad Loktionov](https://www.youtube.com/@ruvlad)

## [Dependencies]
- [ocornut - Dear ImGui](https://github.com/ocornut/imgui)
- [curl](https://github.com/curl/curl)
- [juliettef - imgui_markdown](https://github.com/juliettef/imgui_markdown)
- [discord-rpc](https://github.com/discord/discord-rpc)
- [nlohmann - json](https://github.com/nlohmann/json)
- [DirectX SDK](https://github.com/devKlausS/dxsdk)
- [LibTomCrypt](https://github.com/libtom/libtomcrypt)
- [LibTomMath](https://github.com/libtom/libtommath)

# Disclaimer
This software has been created purely for the purposes of academic research. Project maintainers are not responsible or liable for misuse of the software. Use responsibly.

# Features
[Mod compatibility list](https://github.com/JerryALT/iw3sp_mod/wiki/Mod-compatibility-list)

- Increased asset limits for IW3
  - FX: 1200
  - Image: 7168
  - Loaded Sound: 2700
  - Streamed Sound: 24000
  - Material: 8192
  - Weapon: 2400
  - Xmodel: 5125
- Added the game achievements;
- Added the [gamepad](https://github.com/JerryALT/iw3sp_mod/wiki/All-about-gamepad) support with aim assist;
- Added the [custom methods and functions](https://github.com/JerryALT/iw3sp_mod/wiki/GSC-Functions) for GSC;
- Added loading the additional .ff files `[level name]_patch.ff`;
- Added FOV logic from multiplayer to singleplayer;
- Added [map entities editing](https://github.com/JerryALT/iw3sp_mod/wiki/Changing-the-map-entities) for the stock maps and more...

# How to install modification? (For regular users)
**NOTE**: You must legally own [Steam version](https://store.steampowered.com/app/7940/Call_of_Duty_4_Modern_Warfare_2007/) of Call of Duty® 4: Modern Warfare® (2007) to run this mod. Cracked/Pirated/CDs versions of the game are **NOT** supported.
1. Download the latest [release](https://github.com/xoxor4d/iw3sp-mod-rtx/releases/latest)
2. Place the .zip contents into your cod4 root folder
3. Start the **iw3sp_mod.exe**

# How to compile from source? (For advanced users)
1. Clone the this repository. I recommend use the [git-scm](https://git-scm.com/downloads).
</br>P.S: (**Downloading the zip does not include deps!**)</br>
2. Find any directory which you want the placed the content of repository, then open context menu and find "Git Bash Here" and write next line in the cmd: `git clone https://github.com/xoxor4d/iw3sp-mod-rtx.git`
3. After cloning click the generate.bat and wait when all files will be ready.
4. Open build folder and find the iw3sp_dev.sln file and compile.
