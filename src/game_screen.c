#include <gb/gb.h>
#include "../data/player_head.h"
#include "../data/fire_anim.h"
#include "../data/bg.h"
#include "../data/others.h"
#include "../data/follower.h"
#include "game_screen.h"
#include "globals.h"
#include "math_utility.h"
#include "src/gbt_player.h"

struct Player
{
  UBYTE pos_x;
  UBYTE pos_y;
  BYTE v_x;
  BYTE v_y;
  UBYTE anim_index;
  UBYTE counter;
  UBYTE bitwise; //0 fire
};

struct Other
{
  UBYTE pos_x;
  UBYTE pos_y;
  BYTE v_x;
  BYTE v_y;
};
#define OTHERS_NMB 5
#define FOLLOWING 3 //bitwise
extern const unsigned char * song_Data[];

struct Player player;
struct Other others[OTHERS_NMB];
struct Other follower;
UBYTE o_x[OTHERS_NMB] =
{12,34, 56, 78, 128};
UBYTE o_y[OTHERS_NMB] =
{134,10,22,240,6};

void animate()
{
  UBYTE sprite_index = 0, i;
  UBYTE x,y;
  set_sprite_tile(0,0);
  move_sprite( 0, SCSZX>>1,    SCSZY>>1);
  for(i = 0; i != OTHERS_NMB;i++)
  {
    set_sprite_tile(i+1,0x04);
    move_sprite(i+1, others[i].pos_x-player.pos_x+(SCSZX>>1), others[i].pos_y-player.pos_y+(SCSZY>>1));
  }
  set_sprite_tile(5,5);
  for(i = 0; i != 4;i++)
  {
    set_sprite_tile(6+i, 6+((player.counter>>3)<<2)+i);
  }
  x = follower.pos_x-player.pos_x+(SCSZX>>1);
  y = follower.pos_y-player.pos_y+(SCSZY>>1);
  move_sprite(5, x, y);

  if((player.bitwise & (1<<FOLLOWING)) == (1<<FOLLOWING))
  {
    move_sprite(6, (SCSZX>>1)-4, (SCSZY>>1)-8);
    move_sprite(7, (SCSZX>>1)-4, (SCSZY>>1));
    move_sprite(8, (SCSZX>>1)+4, (SCSZY>>1)-8);
    move_sprite(9, (SCSZX>>1)+4, (SCSZY>>1));
  }
  else
  {
    move_sprite(6, SCSZX+8, SCSZY);
    move_sprite(7, SCSZX+8, SCSZY);
    move_sprite(8, SCSZX+8, SCSZY);
    move_sprite(9, SCSZX+8, SCSZY);
  }


}
UBYTE bg_tilemap[] = {0x00,0x01,0x02,0x03};
void init_game()
{
  UBYTE i,j;
    wait_vbl_done();
  	disable_interrupts();
  	HIDE_SPRITES;
  	HIDE_WIN;
  	HIDE_BKG;
  	DISPLAY_OFF;

    set_bkg_data(0x0, 0x04, TileLabel);
    set_sprite_data(0x0U,4U, PlayerHeadSprite);
    set_sprite_data(0x4,0x1, OthersTile);
    set_sprite_data(5U, 1U, FollowerTile);
    set_sprite_data(0x6U, 12U, FireAnim);
    for(i = 0; i != 8U; i++)
    {
      for(j = 0; j !=8;j++)
      {
        set_bkg_tiles(i<<2, j<<2, 0x04, 0x04, bg_tilemap);
      }
    }


    ENABLE_RAM_MBC1;
    DISPLAY_ON;
  	SHOW_BKG;
  	SHOW_SPRITES;
    //SHOW_WIN;

    //init player
    player.pos_x = 0;
    player.pos_y = 0;
    player.anim_index = 0;
    player.counter = 0;
    //init others
    seed = 11u;
    for(i = 0; i != OTHERS_NMB;i++)
    {
      others[i].pos_x = o_x[i];
      others[i].pos_y = o_y[i];
      others[i].v_x = 0u;
      others[i].v_y = 0u;
    }
    follower.pos_x = 128u;
    follower.pos_y = 128u;
    follower.v_x = 0;
    follower.v_y = 0;
}

UBYTE px [8];
UBYTE py [8];

void move_player()
{
  UBYTE i;
  player.counter++;
  if(player.counter == 24u)
  {
    player.counter = 0u;
  }

  for(i = 1; i != 8; i++)
  {
    px[i-1] = px[i];
    py[i-1] = py[i];
  }
  px[7] = player.pos_x;
  py[7] = player.pos_y;
  player.pos_x += player.v_x;
  player.pos_y += player.v_y;
}
#define OTHER_MAX_DISTANCE 32

void move_others()
{
  BYTE i;
  WORD dist_x;
  WORD dist_y;
  for(i = 0u; i != OTHERS_NMB;i++)
  {
    dist_x = (WORD)player.pos_x;
    dist_x -= (WORD)others[i].pos_x;


    dist_y = (WORD)player.pos_y;
    dist_y -= (WORD) others[i].pos_y;
    if(dist_x+dist_y < OTHER_MAX_DISTANCE && dist_x+dist_y > -OTHER_MAX_DISTANCE)
    {
      others[i].v_x = (BYTE)(dist_x < OTHER_MAX_DISTANCE + i) && (dist_x > -OTHER_MAX_DISTANCE - i) ? (dist_x < 0 ? i+1 : -1-i) : 0;
      others[i].v_y = (BYTE)(dist_y < OTHER_MAX_DISTANCE + i) && (dist_y > -OTHER_MAX_DISTANCE - i) ? (dist_y < 0 ? i+1 : -1-i) : 0;
    }
    else
    {
      others[i].v_x = 0;
      others[i].v_y = 0;
    }
    others[i].pos_x += others[i].v_x;
    others[i].pos_y += others[i].v_y;
  }
  dist_x = (WORD)player.pos_x;
  dist_x -= (WORD)follower.pos_x;


  dist_y = (WORD)player.pos_y;
  dist_y -= (WORD) follower.pos_y;
  if(dist_x+dist_y < OTHER_MAX_DISTANCE && dist_x+dist_y > -OTHER_MAX_DISTANCE)
  {
    if(dist_x+dist_y < 4 && dist_x+dist_y > -4)
    {

      player.bitwise = player.bitwise | (1 << FOLLOWING);
    }
    else
    {
      player.bitwise = player.bitwise & ~(1 << FOLLOWING);
    }
    follower.v_x = (BYTE)(dist_x < OTHER_MAX_DISTANCE + i) && (dist_x > -OTHER_MAX_DISTANCE - i) ? (dist_x < 0 ? -1 : 1) : 0;
    follower.v_y = (BYTE)(dist_y < OTHER_MAX_DISTANCE + i) && (dist_y > -OTHER_MAX_DISTANCE - i) ? (dist_y < 0 ? -1 : 1) : 0;
  }
  else
  {
    player.bitwise = player.bitwise & ~(1 << FOLLOWING);
    follower.v_x = 0;
    follower.v_y = 0;
  }
  if((player.counter & 0x01) == 0x01)
  follower.pos_x += follower.v_x;
  follower.pos_y += follower.v_y;
}


void game_screen()
{
  	UBYTE keys = 0;
	  SWITCH_ROM_MBC1(2);
    gbt_play(song_Data, 2, 7);
    gbt_loop(0);
    enable_interrupts();
  	while(1)
  	{
  		wait_vbl_done();
  		keys = joypad();

  		if ((keys & J_LEFT))
      {
        player.v_x = -1;
      }
      else if ((keys & J_RIGHT))
      {
        player.v_x = 1;
      }
      else
      {
        player.v_x = 0;
      }
      if((keys & J_UP))
      {
        player.v_y = -1;
      }
      else if((keys & J_DOWN))
      {
        player.v_y = 1;
      }
      else
      {
        player.v_y = 0;
      }
      move_player();
      move_others();
      SCX_REG = player.pos_x-(SCSZX>>1);
      SCY_REG = player.pos_y-(SCSZY>>1);
      animate();
      gbt_update();
    }
}
