# The Sea Siren Text Adventure Game

***  

## Overview
The Sea Siren is a text based adventure where you explore a sunken luxury cruise liner, collect key items, outsmart hazards, and make your escape. The ship is full of secrets, random shark encounters, and one very moody ghost.

***  

## Story
You slip through a porthole into the wreck of the Sea Siren, a ship lost on its maiden voyage. The laughter and light are gone, replaced by cold water, drifting uniforms, and restless spirits. Forward is the only way out.

***  

## Key Features
* Random shark spawns in five rooms each playthrough, never in Passenger Quarters or Dining Hall
* Look first survival loop that forces careful play on entry to each room
* Dining Hall ghost door trap that can be neutralized if you notice it in time
* Item based progression with Knife, Metal Coffee Cup, Crowbar, Nut and Bolt, Map Etched on Rotted Wood, and Ancient Artifact
* Observation Deck hinge repair event that requires Crowbar and Nut and Bolt
* Nonlinear room layout with multiple paths and reentry text for flavor
* Simple inventory system and clear command set

***  

## Commands
* Movement: `N` `S` `E` `W` `U` `D`
* Look: `L`
* Inventory: `I`
* Quit: `Q`

***  

## How to Win
Reach the Escape Boat while carrying the Ancient Artifact and the Map Etched on Rotted Wood. Without both items, the gangsters refuse to let you leave.

***  

## Survival Tips
* Look first in every room to reveal hazards and items
* Collect everything that looks useful
* In the Dining Hall, wedge the Metal Coffee Cup in the door after you look to block the ghost
* Repair the Observation Deck hinge when prompted if you have the required parts

***  

## Build and Run
You need a C++ compiler that supports C++11 or later.

```bash
# Windows with g++
g++ -std=c++11 -O2 -o sea_siren The-Sea-Siren-Text-Adventure-Game-v2.cpp
sea_siren.exe

# macOS or Linux
g++ -std=c++11 -O2 -o sea_siren The-Sea-Siren-Text-Adventure-Game-v2.cpp
./sea_siren

