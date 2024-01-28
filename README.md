![license](https://img.shields.io/github/license/JerryALT/iw3sp_mod.svg)
[![Build](https://github.com/xoxor4d/iw3sp-mod-rtx/workflows/Build/badge.svg)](https://github.com/xoxor4d/iw3sp-mod-rtx/actions)

# IW3SP-MOD-RTX - nvidia rtx-remix compatibility mod


<div align="center" markdown="1"> 

This singleplayer client modification for Call of Duty 4: Modern Warfare (IW3) is made to make   
the game compatible with nvidia's [rtx-remix](https://github.com/NVIDIAGameWorks/rtx-remix). It is a fork of [IW3SP_Mod](https://github.com/JerryALT/iw3sp_mod) and originally based on the [IW4x Client](https://github.com/iw4x/iw4x-client)  
It does __not__ come with a 'rtx mod' -> meaning no custom models nor materials.  

There is also a client mod for [CoD4-MP](https://github.com/xoxor4d/iw3xo-dev/tree/rtx) and a client mod for [CoD5-WaW SP/MP](https://github.com/xoxor4d/t4-rtx)

<br>
</div>

<p align="center">
  <img src="assets/github/banner-rtx.png?raw=true"/>
</p>

# Usage

#### A. Easy way but might not feature the latest and greatest:
  1. Download the latest [release](https://github.com/xoxor4d/iw3sp-mod-rtx/releases) and extract the contents into your cod4 root directory.  

2) Read the `Dvars / Console Commands` and `Current issues` sections

<br>

#### B. Manual way but always up-to-date:

1) Install the latest full rtx-remix release (0.4.0 at this time)   
https://github.com/NVIDIAGameWorks/rtx-remix/tags

<br>

2) (Optional) Install the latest `github action` builds of:  
remix bridge - https://github.com/NVIDIAGameWorks/bridge-remix/actions  
remix runtime - https://github.com/NVIDIAGameWorks/dxvk-remix/actions  

<br>

3) Download the latest iw3sp-mod-rtx `github actions` build:  
  Release-binaries-rtx - https://github.com/xoxor4d/iw3sp-mod-rtx/actions 
  - Drop `game.dll` and the contents of the `assets-remix` folder into your cod4 root directory and start the game.   
  (You'll need either `iw3sp_mod.exe` (from the latest full release) or another way to load `game.dll`  

<br>

4) Read the `Dvars / Console Commands` and `Current issues` sections


<br>
<br>

## Dvars / Console Commands to further tweak the game to your liking:

> 🔸 skybox and fog settings can be tweaked per map by using `map_settings.ini` found in the `iw3sp_data` folder 🔸

- use console commands starting with `rtx_sky_` to spawn/change the skybox
- `r_forceLod` :: force all models to a specific LOD (highest by default)
- `r_lodScaleRigid` :: adjust static model draw distances (lower values increase draw distance)
- `r_aspectRatio` :: **AUTO** is truly auto now and supports any aspect ratio
- use console commands `borderless` & `windowed` to switch between borderless and normal windowed mode  


<br>
<br>

## Advanced Settings:

- A. Commandline Arguments:  
  - `-no_default_sky` :: disable spawning of a default sky on each map
  - `-stock_effects` :: render effects using shaders
  - `-no_fog` :: disable fog
  - `-no_forced_lod` :: do not set `r_forceLod` to `high` by default  
  
  > eg: &ensp;`"c:\path\iw3sp_mod.exe" -no_default_sky -no_forced_lod ...` 

  - Notes: 
	- `rtx_disable_world_culling` :: tweakable culling - _(set to `less` (1) by default)_
	- `rtx_disable_entity_culling` :: tweakable culling of script objects (game entities) - _(enabled by default)_
	- `rtx_extend_smodel_drawing` :: disable static model draw limit (max amount of static models drawn at once)
	- `rtx_hacks` :: replace the skybox with a blue-gray texture _(enabled by default)_

<br>

- B. Console commands:  
  - `mapsettings_update` :: reload and apply settings from `iw3sp_data/map_settings.ini`

<br>


## ⚠️ Current issues:
- changing resolution or anything else that requires the game to restart the renderer might crash the game
  > use commandline arguments like `c:\path\iw3sp_mod.exe +set r_fullscreen 0` or edit your player config (`cod4/players/yourprofile`) before starting the game
- effects slow down the game a bit (really depends on the effect and the amount - use `fx_enable 0` or `fx_cull_` dvars to adjust to your liking - marking certain effect textures as _ignore_ might help as well) 
- objects might flicker due to disabled culling on LARGE maps (there is just too much going on and the iw3 engine just cant keep up)
- ^ this can also affect the spawned skybox (looks like it moves with the camera)

<br>

<p align="center">
  <img src="assets/github/rtx01.jpg?raw=true"/>
</p>


### Questions? 
> visit the [rtx-remix showcase](https://discord.gg/j6sh7JD3v9) discord and check out the cod4 thread (`remix-projects` channel)
- join the [iw3xo](https://discord.gg/t5jRGbj) discord if you have questions related to this modification
- join the [iw3sp-mod](https://discord.gg/YzNZSEMAnf) discord if you have questions related to iw3sp_mod

<br>

# Credits rtx-fork
- [people of the showcase discord](https://discord.gg/j6sh7JD3v9) (for testing, feedback and ideas)
- [nvidia - rtx-remix](https://github.com/NVIDIAGameWorks/rtx-remix)

# Credits iw3sp_mod
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

<br>

# Disclaimer
This software has been created purely for the purposes of academic research. Project maintainers are not responsible or liable for misuse of the software. Use responsibly.

<br>

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

<br>

# How to install modification? (For regular users)
**NOTE**: You must legally own [Steam version](https://store.steampowered.com/app/7940/Call_of_Duty_4_Modern_Warfare_2007/) of Call of Duty® 4: Modern Warfare® (2007) to run this mod. Cracked/Pirated/CDs versions of the game are **NOT** supported.
1. Download the latest [release](https://github.com/xoxor4d/iw3sp-mod-rtx/releases/latest)
2. Place the .zip contents into your cod4 root folder
3. Start the **iw3sp_mod.exe**

<br>

# How to compile from source? (For advanced users)
1. Clone the this repository. I recommend use the [git-scm](https://git-scm.com/downloads).
</br>P.S: (**Downloading the zip does not include deps!**)</br>
2. Find any directory which you want the placed the content of repository, then open context menu and find "Git Bash Here" and write next line in the cmd: `git clone https://github.com/xoxor4d/iw3sp-mod-rtx.git`
3. After cloning click the generate.bat and wait when all files will be ready.
4. Open build folder and find the iw3sp_dev.sln file and compile.
