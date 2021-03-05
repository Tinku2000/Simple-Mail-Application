all: ms.c mc.c
	gcc    ms.c -o ms
	gcc    mc.c -o mc
clean:
	rm ms mc
	
