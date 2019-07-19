#include <gb/gb.h>
#include "../data/test.h"
#include "../data/fire_anim.h"
#include "game_screen.h"
#include "globals.h"

struct Player
{
  UBYTE pos_x;
  UBYTE pos_y;
  UBYTE v_x;
  UBYTE v_y;
  UBYTE anim_index;
  UBYTE counter;
  UBYTE bitwise;//current_dir = 0, previous_dir = 2
};


struct Player player;

void animate()
{
  player.counter++;
  if(player.counter == 10U)
  {
    player.anim_index = 1U;
  }
  if(player.counter == 20U)
  {
    player.anim_index =2U;
  }
  if(player.counter == 30U)
  {
    player.anim_index = 0u;
    player.counter = 0u;
  }
  set_sprite_tile(0,(player.anim_index<<2)+0);
  set_sprite_tile(1,(player.anim_index<<2)+1);
  set_sprite_tile(2,(player.anim_index<<2)+2);
  set_sprite_tile(3,(player.anim_index<<2)+3);
  move_sprite( 0, SCSZX>>1,    SCSZY>>1);
  move_sprite( 1, SCSZX>>1,    (SCSZY>>1)+8U);
  move_sprite( 2, (SCSZX>>1)+8U, SCSZY>>1);
  move_sprite( 3, (SCSZX>>1)+8U, (SCSZY>>1)+8U);
}

void init_game()
{
    wait_vbl_done();
  	disable_interrupts();
  	HIDE_SPRITES;
  	HIDE_WIN;
  	HIDE_BKG;
  	DISPLAY_OFF;

    set_bkg_data(0x0, 0x04, TileLabel);
    set_sprite_data(0x0, 12U, FireAnim);
    set_sprite_tile(0,0);
    set_sprite_tile(1,1);
    set_sprite_tile(2,2);
    set_sprite_tile(3,3);
    /*
     * Draw the frame in the window
     *
     * Width  = 0x80 = 0x10 * 8
     * Height = 0x50 = 0x0A * 8
     */
     set_bkg_tiles(0, 0, 16, 16, TileMap);
     set_bkg_tiles(16, 0, 16, 16, &TileMap[(UWORD)256]);
     set_bkg_tiles(0, 16, 16, 16, &TileMap[(UWORD)512]);
     set_bkg_tiles(16, 16, 16, 16, &TileMap[(UWORD)768]);

    ENABLE_RAM_MBC1;
    DISPLAY_ON;
  	SHOW_BKG;
  	SHOW_SPRITES;
    //SHOW_WIN;
}


void game_screen()
{
  	UBYTE keys = 0;
    //init player
    player.pos_x = 0;
    player.pos_y = 0;
    player.anim_index = 0;
    player.counter = 0;

    enable_interrupts();
  	while(1)
  	{
  		wait_vbl_done();
  		keys = joypad();

  		if (keys & J_LEFT)
      {
        player.v_x = -1;
      }
      if (keys & J_RIGHT)
      {
        player.v_x = 1;
      }
      if(keys & J_UP)
      {
        player.v_y = -1;
      }

      if(keys & J_DOWN)
      {
        player.v_y = 1;
      }

      SCX_REG = player.pos_x-(SCSZX>>1);
      SCY_REG = player.pos_y-(SCSZY>>1);
      animate();

    }
}
