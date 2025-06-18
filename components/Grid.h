#pragma once

#define GRID MAP.component<Grid>()
#define TILESET MAP.component<Tileset>()

#define load_grid(MAP, tilesize, scale) [&] { MAP = MAKE_ENTITY; MAP.add<Grid>(); MAP.add<Tileset>(); GRID->load(tilesize, scale); }()
#define load_tileset(texture_alias) TILESET->load(texture_alias, texture(texture_alias)->width(), texture(texture_alias)->height(), GRID->tilesize)

//SCALED TILESIZE
#define TILESIZE GRID->scale * GRID->tilesize

#define TILE(index) MAP.component<Tileset>()->tile(index)
#define BLIT_TILE(index, x, y) BLIT(texture(TILESET->alias), x, y, GRID->scale, TILE(index)); 

/*/ ! \ must create an entity called map and also add a tileset to use this / ! \*/
struct Grid
{
    int tilesize;
    int scale;

    Grid() {}

    void load(int _tilesize, int _scale)
    {
        tilesize = _tilesize;
        scale = _scale;

        //...
    }
};