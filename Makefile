CC	= /opt/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j

BINS	= SGJ.gb
SOURCE =  src/math_utility.o src/main.o src/game_screen.o
ASSETS = data/bg.o data/fire_anim.o data/player_head.o data/others.o data/follower.o
MUSIC = music_output.c src/gbt_player.o src/gbt_player_bank1.o
all:	  $(SOURCE) $(ASSETS) $(MUSIC) $(BINS)

SGJ.gb:
	$(CC) -Wl-yt3 -Wl-yo8 -Wl-ya1 -o $(BINS) $(SOURCE) $(ASSETS) $(MUSIC)

%.o:	%.c
	$(CC) -c -o $@ $<

src/gbt_player.o: src/gbt_player.s
		$(CC) -Wa-l -c -o src/gbt_player.o src/gbt_player.s

src/gbt_player_bank1.o: src/gbt_player_bank1.s
		$(CC) -Wa-l -Wf-bo4 -c -o src/gbt_player_bank1.o src/gbt_player_bank1.s

music_output.c:
	./tools/mod2gbt/mod2gbt data/main_theme.mod song -c 2

music_output.o:  music_output.c
		$(CC) -Wa-l -Wf-bo2 -c -o music_output.o music_output.c


clean:
	rm -f *.o */*.o */*/*.o *.lst *.map *.gb *~ *.rel *.cdb *.ihx *.lnk *.sym *.asm music_output.c
