#include "global.h"
#include "decompress.h"
#include "graphics.h"
#include "item_icon.h"
#include "malloc.h"
#include "sprite.h"
#include "constants/items.h"
#include "pokeball.h"
#include "item.h"
#include "battle_main.h"

// EWRAM vars
EWRAM_DATA u8 *gItemIconDecompressionBuffer = NULL;
EWRAM_DATA u8 *gItemIcon4x4Buffer = NULL;

// const rom data
#include "data/item_icon_table.h"

static const struct OamData sOamData_ItemIcon =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 2,
    .affineParam = 0
};

static const struct OamData sOamData_BallIcon =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = 0,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 2,
    .affineParam = 0
};

static const union AnimCmd sSpriteAnim_ItemIcon[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_ItemIcon[] =
{
    sSpriteAnim_ItemIcon
};

const struct SpriteTemplate gItemIconSpriteTemplate =
{
    .tileTag = 0,
    .paletteTag = 0,
    .oam = &sOamData_ItemIcon,
    .anims = sSpriteAnimTable_ItemIcon,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct SpriteTemplate gBallIconSpriteTemplate =
{
    .tileTag = 0,
    .paletteTag = 0,
    .oam = &sOamData_BallIcon,
    .anims = sSpriteAnimTable_ItemIcon,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

// code
bool8 AllocItemIconTemporaryBuffers(void)
{
    gItemIconDecompressionBuffer = Alloc(0x120);
    if (gItemIconDecompressionBuffer == NULL)
        return FALSE;

    gItemIcon4x4Buffer = AllocZeroed(0x200);
    if (gItemIcon4x4Buffer == NULL)
    {
        Free(gItemIconDecompressionBuffer);
        return FALSE;
    }

    return TRUE;
}

void FreeItemIconTemporaryBuffers(void)
{
    Free(gItemIconDecompressionBuffer);
    Free(gItemIcon4x4Buffer);
}

void CopyItemIconPicTo4x4Buffer(const void *src, void *dest)
{
    u8 i;

    for (i = 0; i < 3; i++)
        CpuCopy16(src + i * 96, dest + i * 128, 0x60);
}

u8 AddItemIconSprite(u16 tilesTag, u16 paletteTag, u16 itemId)
{
    if (!AllocItemIconTemporaryBuffers())
    {
        return MAX_SPRITES;
    }
    else
    {
        u8 spriteId;
        struct SpriteSheet spriteSheet;
        struct CompressedSpritePalette spritePalette;
        struct SpriteTemplate *spriteTemplate;

        LZDecompressWram(GetItemIconPic(itemId), gItemIconDecompressionBuffer);
        CopyItemIconPicTo4x4Buffer(gItemIconDecompressionBuffer, gItemIcon4x4Buffer);
        spriteSheet.data = gItemIcon4x4Buffer;
        spriteSheet.size = 0x200;
        spriteSheet.tag = tilesTag;
        LoadSpriteSheet(&spriteSheet);

        spritePalette.data = GetItemIconPalette(itemId);
        spritePalette.tag = paletteTag;
        LoadCompressedSpritePalette(&spritePalette);

        spriteTemplate = Alloc(sizeof(*spriteTemplate));
        CpuCopy16(&gItemIconSpriteTemplate, spriteTemplate, sizeof(*spriteTemplate));
        spriteTemplate->tileTag = tilesTag;
        spriteTemplate->paletteTag = paletteTag;
        spriteId = CreateSprite(spriteTemplate, 0, 0, 0);

        FreeItemIconTemporaryBuffers();
        Free(spriteTemplate);

        return spriteId;
    }
}

u8 AddCustomItemIconSprite(const struct SpriteTemplate *customSpriteTemplate, u16 tilesTag, u16 paletteTag, u16 itemId)
{
    if (!AllocItemIconTemporaryBuffers())
    {
        return MAX_SPRITES;
    }
    else
    {
        u8 spriteId;
        struct SpriteSheet spriteSheet;
        struct CompressedSpritePalette spritePalette;
        struct SpriteTemplate *spriteTemplate;

        LZDecompressWram(GetItemIconPic(itemId), gItemIconDecompressionBuffer);
        CopyItemIconPicTo4x4Buffer(gItemIconDecompressionBuffer, gItemIcon4x4Buffer);
        spriteSheet.data = gItemIcon4x4Buffer;
        spriteSheet.size = 0x200;
        spriteSheet.tag = tilesTag;
        LoadSpriteSheet(&spriteSheet);

        spritePalette.data = GetItemIconPalette(itemId);
        spritePalette.tag = paletteTag;
        LoadCompressedSpritePalette(&spritePalette);

        spriteTemplate = Alloc(sizeof(*spriteTemplate));
        CpuCopy16(customSpriteTemplate, spriteTemplate, sizeof(*spriteTemplate));
        spriteTemplate->tileTag = tilesTag;
        spriteTemplate->paletteTag = paletteTag;
        spriteId = CreateSprite(spriteTemplate, 0, 0, 0);

        FreeItemIconTemporaryBuffers();
        Free(spriteTemplate);

        return spriteId;
    }
}

const void *GetItemIconPic(u16 itemId)
{
    if (itemId == ITEM_LIST_END)
        return gItemIcon_ReturnToFieldArrow; // Use last icon, the "return to field" arrow
    if (itemId >= ITEMS_COUNT)
        return gItemsInfo[0].iconPic;
    if (itemId >= ITEM_TM01 && itemId < ITEM_HM01 + NUM_HIDDEN_MACHINES)
    {
        if (itemId < ITEM_TM01 + NUM_TECHNICAL_MACHINES)
            return gItemIcon_TM;
        return gItemIcon_HM;
    }

    return gItemsInfo[itemId].iconPic;
}

const void *GetItemIconPalette(u16 itemId)
{
    if (itemId == ITEM_LIST_END)
        return gItemIconPalette_ReturnToFieldArrow;
    if (itemId >= ITEMS_COUNT)
        return gItemsInfo[0].iconPalette;
    if (itemId >= ITEM_TM01 && itemId < ITEM_HM01 + NUM_HIDDEN_MACHINES)
        return gTypesInfo[gMovesInfo[gItemsInfo[itemId].secondaryId].type].paletteTMHM;

    return gItemsInfo[itemId].iconPalette;
}



const u32 *const gBallIconTable[][2] =
{
   [BALL_POKE] = {gBallIcon_Poke, gBallIconPalette_Poke},
   [BALL_GREAT] = {gBallIcon_Great, gBallIconPalette_Great},
   [BALL_ULTRA] = {gBallIcon_Ultra, gBallIconPalette_Ultra},
   [BALL_MASTER] = {gBallIcon_Master, gBallIconPalette_Master},
   [BALL_PREMIER] = {gBallIcon_Premier, gBallIconPalette_Premier},
   [BALL_HEAL] = {gBallIcon_Heal, gBallIconPalette_Heal},
   [BALL_NET] = {gBallIcon_Net, gBallIconPalette_Net},
   [BALL_NEST] = {gBallIcon_Nest, gBallIconPalette_Nest},
   [BALL_DIVE] = {gBallIcon_Dive, gBallIconPalette_Dive},
   [BALL_DUSK] = {gBallIcon_Dusk, gBallIconPalette_Dusk},
   [BALL_TIMER] = {gBallIcon_Timer, gBallIconPalette_Timer},
   [BALL_QUICK] = {gBallIcon_Quick, gBallIconPalette_Quick},
   [BALL_REPEAT] = {gBallIcon_Repeat, gBallIconPalette_Repeat},
   [BALL_LUXURY] = {gBallIcon_Luxury, gBallIconPalette_Luxury},
   [BALL_LEVEL] = {gBallIcon_Level, gBallIconPalette_Level},
   [BALL_LURE] = {gBallIcon_Lure, gBallIconPalette_Lure},
   [BALL_MOON] = {gBallIcon_Moon, gBallIconPalette_Moon},
   [BALL_FRIEND] = {gBallIcon_Friend, gBallIconPalette_Friend},
   [BALL_LOVE] = {gBallIcon_Love, gBallIconPalette_Love},
   [BALL_FAST] = {gBallIcon_Fast, gBallIconPalette_Fast},
   [BALL_HEAVY] = {gBallIcon_Heavy, gBallIconPalette_Heavy},
   [BALL_DREAM] = {gBallIcon_Dream, gBallIconPalette_Dream},
   [BALL_SAFARI] = {gBallIcon_Safari, gBallIconPalette_Safari},
   [BALL_SPORT] = {gBallIcon_Sport, gBallIconPalette_Sport},
   [BALL_PARK] = {gBallIcon_Sport, gBallIconPalette_Sport},    // We don't have a sprite for Park Ball
   [BALL_BEAST] = {gBallIcon_Beast, gBallIconPalette_Beast},
   [BALL_CHERISH] = {gBallIcon_Cherish, gBallIconPalette_Cherish},
};

u8 AddBallIconSprite(u16 tilesTag, u16 paletteTag, u8 ballId)
{
    if (!AllocItemIconTemporaryBuffers())
    {
        return MAX_SPRITES;
    }
    else
    {
        u8 spriteId;
        struct SpriteSheet spriteSheet;
        struct CompressedSpritePalette spritePalette;
        struct SpriteTemplate *spriteTemplate;

        if (ballId > ARRAY_COUNT(gBallIconTable) - 1)
            ballId = 0;

        LZDecompressWram(gBallIconTable[ballId][0], gItemIconDecompressionBuffer);
        CpuCopy16(gItemIconDecompressionBuffer, gItemIcon4x4Buffer, 0x100);
        spriteSheet.data = gItemIcon4x4Buffer;
        spriteSheet.size = 0x100;
        spriteSheet.tag = tilesTag;
        LoadSpriteSheet(&spriteSheet);
        spritePalette.data = gBallIconTable[ballId][1];
        spritePalette.tag = paletteTag;
        LoadCompressedSpritePalette(&spritePalette);
        spriteTemplate = Alloc(sizeof(*spriteTemplate));
        CpuCopy16(&gBallIconSpriteTemplate, spriteTemplate, sizeof(*spriteTemplate));
        spriteTemplate->tileTag = tilesTag;
        spriteTemplate->paletteTag = paletteTag;
        spriteId = CreateSprite(spriteTemplate, 0, 0, 0);
        FreeItemIconTemporaryBuffers();
        Free(spriteTemplate);
        return spriteId;
    }
}
