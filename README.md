# Start Menu Icons and expansion lmao
1. If you want more icons, add them to graphics/interface/start_menu_icons.png in the right order. Grayscale in the first batch and then coloured in the latter part.

2. You'll need to adjust some entries in start_menu.c if you want your icon to show such as sStartMenuIconFrames, HandleStartMenuInput, DynamicallyLoadStartMenuIcon, etc. You'll need to read through it to know exactly what you need.

3. DNS may darken the icons when late. Its up to you to make an exception for that.. This line might be of use "oam.paletteNum = LoadSpritePalette(&palSheet);"

4. The spritesheet is divided into 2 portions(Grayscale and Coloured) and only the index of the icon in the first portion is needed. It will be appropriately adjusted when needed.

5. Remember to check the defines with comments in start_menu.c and start_menu.h.

6. The start menu's in places like the Battle Pyramid, Safari Zone, etc may look wonky. Changing the placement of options should help.


# Start Menu Clock
This is a feature branch for [pokeemerald-expansion](https://github.com/rh-hideout/pokeemerald-expansion) that adds a clock that tracks the in-game time to the start menu, as seen below.

My implementation is a modified version of part of this [commit](https://github.com/BluRosie/em-remake/commit/4999f9ca4efe5b4b8fec8172cf2a7c364e0ad07e) from BluRosie that was shared a few years ago, that put playtime in the same location. Please credit them rather than me, they did all the difficult work.

![image](https://github.com/Pawkkie/pokeemerald-expansion/assets/61265402/42034ce7-713b-44f4-bb9e-99ff269fb4ba)

While the branch is intended to be used in cases where the in-game clock has been sped up and is no longer synced with the real time clock, it can certainly be used in both cases if you'd like to include a portable clock display.

I don't have a setting for it, but you can manually change the clock to a 24-hour mode if you prefer. Just change the conditional at the top of `ShowTimeWindow` in `start_menu.c`.

Lastly, if you do speed up the in-game clock, it's possible the start menu clock won't refresh as fast as you'd like it to. You can increase the refresh rate by adding a conditional just before the the last return statement in `HandleStartMenuInput`, here:
https://github.com/Pawkkie/pokeemerald-expansion/blob/f9cceba5e07af02719079d869a3e6d57fd3f5269/src/start_menu.c#L699


    if (!GetSafariZoneFlag() && !InBattlePyramid() && gSaveBlock2Ptr->playTimeSeconds == 0) 
    {
        RemoveExtraStartMenuWindows();
        UpdateClockDisplay();
    }

By default the clock refreshes every time playTimeSeconds hits 0. Feel free to change this as you see fit, I have a dramatically sped up in-game clock and have  `&& gSaveBlock2Ptr->playTimeSeconds == 0` removed entirely.

Feel free to reach out to Pawkkie on Discord if you've got any concerns, but please do so in a romhacking channel so others can also benefit :)
