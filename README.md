# Start Menu Icons and expansion lmao
1. If you want more icons, add them to graphics/interface/start_menu_icons.png in the right order. Grayscale in the first batch and then coloured in the latter part.

2. You'll need to adjust some entries in start_menu.c if you want your icon to show such as sStartMenuIconFrames, HandleStartMenuInput, DynamicallyLoadStartMenuIcon, etc. You'll need to read through it to know exactly what you need.

3. DNS may darken the icons when late. Its up to you to make an exception for that.. This line might be of use "oam.paletteNum = LoadSpritePalette(&palSheet);"

4. The spritesheet is divided into 2 portions(Grayscale and Coloured) and only the index of the icon in the first portion is needed. It will be appropriately adjusted when needed.

5. Remember to check the defines with comments in start_menu.c and start_menu.h.

6. The start menu's in places like the Battle Pyramid, Safari Zone, etc may look wonky. Changing the placement of options should help.


# Pokémon Emerald

## What is the pokeemerald Expansion?

The Pokeemerald Expansion is a collection of feature branches that can be integrated into existing [pokeemerald](https://github.com/pret/pokeemerald) projects.

## What features are included?
- Upgraded battle engine.
    - Fairy Type.
    - Physical/Special/Status Category Split.
    - New moves and abilities up to SwSh.
    - Options to change behaviors and data by generation.
    - Mega Evolution and Primal Reversion
    - Z-Moves
- Pokémon Species from newer Generations (with the option to disable them if needed).
    - Updates Hoenn's Regional Dex to match ORAS'.
    - Updates National Dex incorporating all the new species.
    - Option to change base stats by generation.
    - New evolution methods.
    - Hidden Abilities data (How to make them available is up to the user).
- Items from newer Generations and updated item effects for battle and field use.

Certain mechanics, moves, abilities and species sprites are missing. For more information, see [the project's milestones](https://github.com/rh-hideout/pokeemerald-expansion/milestones).

### [Documentation on features can be found here](https://github.com/rh-hideout/pokeemerald-expansion/wiki)

## Who maintains the project?

The project was originally started by DizzyEgg alongside other contributors.

The project has now gotten larger and DizzyEgg is now maintaining the project as part of the ROM Hacking Hideout community. Some members of this community are taking on larger roles to help maintain the project.

### Please consider crediting the entire [list of contributors](https://github.com/rh-hideout/pokeemerald-expansion/wiki/Credits) in your project, as they have all worked hard to develop this project :)

## Can I contribute even if I'm not a member of ROM Hacking Hideout?

Yes! Contributions are welcome via Pull Requests and they will be reviewed by maintainers. Don't feel discouraged if we take a bit to review your PR, we'll get to it.

## What is ROM Hacking Hideout?

A Discord-based ROM hacking community that has many members who hack using the disassembly and decompilation projects for Pokémon. Quite a few contributors to the original feature branches by DizzyEgg were members of ROM Hacking Hideout. You can call it RHH for short!

[Click here to join the RHH Discord Server!](https://discord.gg/6CzjAG6GZk)
