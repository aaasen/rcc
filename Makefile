#RCC makefile
#Only compatible with Unix/Linux based operating systems

CFLAGS := -Wall -lm
FILES := main.c rtree.c point.c rect.c qdbmp.c hmap.c parray.c\
	 sphere.c cylinder.c rectarray.c
OBJECTS := main.o rtree.o point.o rect.o qdbmp.o hmap.o parray.o\
	 sphere.o cylinder.o rectarray.o
MAIN := main
CC := gcc
OUTPUT := rcc
LEAKFILE := leaklog.txt
ARCHIVEDIR := arc
DATEFORMAT := %d-%m-%y:%s
ARCHIVEID := `date +$(DATEFORMAT)`
ARCHIVENAME := $(OUTPUT)$(ARCHIVEID)
ARCHIVEDEST := $(ARCHIVEDIR)/$(ARCHIVENAME)

help:
	@echo "RCC makefile"
	@echo "\tOnly compatible with Unix/Linux based operating systems\n"
	@echo "METHODS"
	@echo "\tmake objects - compile all c files to objects"
	@echo "\tmake exe - compile all objects to an executable"
	@echo "\tmake clean - remove all object files"
	@echo "\tmake all - compile objects, make executable, clean directory"
	@echo "\tmake run - do full compile and clean, then run"
	@echo "\tmake leak - make all and then run a"
	@echo "\t  full valgrind leak check on the output"
	@echo "\tmake leaklog - make leak but output to a file instead of stdout"
	@echo "\tmake archive - compress the output (.gz),"
	@echo "\t  give it a meaningful name and"
	@echo "\t  move it into the ARCHIVEDIR"
	@echo "\t  -unzip using gunzip -r [archivename]"
	@echo "\tmake cleanarchive - deletes all files in ARCHIVEDIR"

objects:
	@echo "compiling source..."
	@$(CC) -g -c $(FILES)			# -g for debugging
	
exe:
	@echo "linking objects..."
	@$(CC) -g $(CFLAGS) $(OBJECTS) -o $(OUTPUT)
	
clean:
	@echo "removing object files..."
	@rm $(OBJECTS)

all:
	@make -s objects
	@make -s exe
	@make -s clean
	@echo "compilation of $(OUTPUT) complete!"

run:
	@make -s all
	@./$(OUTPUT)

leak:
	@make -s all
	@echo "\nchecking $(OUTPUT) for leaks..."
	@echo "WARNING: SHITSTORM IMMINENT!!!\n"
	valgrind --leak-check=full ./$(OUTPUT)
	
leaklog:
	@make -s all
	@echo "\nchecking $(OUTPUT) for leaks and outputting to $(LEAKFILE)\n"
	valgrind --leak-check=full --log-file=$(LEAKFILE) ./$(OUTPUT)	

arcprep:
	@make -s all
	@echo "\nmake sure '$(ARCHIVEDIR)' is an existing directory!\n"
	
arcbin:
	@make -s arcprep
	@echo "copying '$(OUTPUT)' into '$(ARCHIVEDEST)'"
	@cp $(OUTPUT) $(ARCHIVEDEST)
	@echo "creating compressed archive: '$(ARCHIVENAME).gz'"
	@gzip $(ARCHIVEDEST)
	
arcsrc:
	@make -s arcprep
	@echo "copying source code into '$(ARCHIVEDEST)'"
	@mkdir $(ARCHIVEDEST)
	@cp $(FILES) $(ARCHIVEDEST)
	@echo "gzipping all files in '$(ARCHIVEDEST)'"
	@gzip -r $(ARCHIVEDEST)

cleanarchive:
	@rm -r $(ARCHIVEDIR)
	@mkdir $(ARCHIVEDIR)

