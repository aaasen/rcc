# Make file by Lane Aasen and Dylan Swiggett
# Only compatible with Unix/Linux based operating systems

# make objects - compile all c files to objects
# make exe - compile all objects to an executable
# make clean - remove all object files
# make all - compile objects, make executable, clean directory
# make run - do full compile and clean, then run
# make leak - make all and then run a full valgrind leak check on the output
# make leaklog - make leak but output to a file instead of stdout
# make archive - compress the output (.gz), give it a meaningful name and
#	move it into the ARCHIVEDIR
# make cleanarchive - deletes all files in ARCHIVEDIR

CFLAGS = -Wall -lm
FILES = main.c rtree.c point.c rect.c qdbmp.c hmap.c parray.c\
	 sphere.c cylinder.c rectarray.c
OBJECTS = main.o rtree.o point.o rect.o qdbmp.o hmap.o parray.o\
	 sphere.o cylinder.o rectarray.o
MAIN = main
CC = gcc
OUTPUT = rcc
LEAKFILE = leaklog.txt
ARCHIVEDIR = arc
DATEFORMAT = %d-%m-%y:%s 
ARCHIVEID = `date +$(DATEFORMAT)`
ARCHIVENAME = $(OUTPUT)$(ARCHIVEID)
ARCHIVEDEST = $(ARCHIVEDIR)/$(ARCHIVENAME)

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
	
archive:
	@make -s all
	@echo "\nmake sure '$(ARCHIVEDIR)' is an existing directory!\n"
	@echo "copying '$(OUTPUT)' into '$(ARCHIVEDIR)'"
	@cp $(OUTPUT) $(ARCHIVEDEST)
	@echo "creating compressed archive: '$(ARCHIVENAME).gz'"
	@gzip $(ARCHIVEDEST)
	
cleanarchive:
	@rm -r $(ARCHIVEDIR)
	@mkdir $(ARCHIVEDIR)


