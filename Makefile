CC	= /opt/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j

BINS	= SGJ.gb
SOURCE =  src/math_utility.o src/main.o src/game_screen.o
ASSETS = data/test.o data/fire_anim.o

all:	  $(SOURCE) $(ASSETS) $(BINS)

SGJ.gb:
	$(CC) -Wl-yt3 -Wl-yo16 -Wl-ya1 -o $(BINS) $(SOURCE) $(ASSETS)

%.o:	%.c
	$(CC) -c -o $@ $<


#data/music_output.c:
#	./tools/mod2gbt/mod2gbt data/template.mod song -c 2

clean:
	rm -f *.o */*.o */*/*.o *.lst *.map *.gb *~ *.rel *.cdb *.ihx *.lnk *.sym *.asm
