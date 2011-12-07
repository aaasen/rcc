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
DATEFORMAT := %d-%m-%y-%s
ARCHIVEID := `date +$(DATEFORMAT)`
ARCHIVENAME := $(OUTPUT)$(ARCHIVEID)
ARCHIVEDEST := $(ARCHIVEDIR)/$(ARCHIVENAME)

help:
	@echo "RCC makefile"
	@echo "    Only compatible with Unix/Linux based operating systems\n"
	@echo "METHODS"
	@echo "    make objects - compile all C source code into objects"
	@echo "    make exe - link all objects to create an executable"
	@echo "    make clean - remove all object files"
	@echo "    make all - make objects, exe and clean"
	@echo "    make run - make all then run"
	@echo "    make leak - make all and then run valgrind on the output"
	@echo "    make leaklog - make leak but output to a file instead of stdout"
	@echo "    make arcprep - make all and check that ARCHIVEDIR exists"
	@echo "    make arcbin - make arcprep, copy executable to ARCHIVEDIR,"
	@echo "      give it a unique name and gzip"
	@echo "    make arcsrc - make arcprep, copy source code to ARCHIVEDIR,"
	@echo "      give it a unique name, tar directory and gzip"
	@echo "    make arcclean - deletes all non-svn files in ARCHIVEDIR"

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
	@echo "copying source code..."
	@mkdir $(ARCHIVEDEST)
	@cp $(FILES) $(ARCHIVEDEST)
	@echo "creating archive..."
	@tar -c -f "$(ARCHIVEDEST).tar" $(ARCHIVEDEST)
	@echo "compressing archive..."
	@gzip "$(ARCHIVEDEST).tar"
	@echo "cleaning up..."
	@rm -r "$(ARCHIVEDEST)"
	@echo "compressed archive available at:\n    '$(ARCHIVEDEST).tar.gz'"

arcclean:
	@python arcclean.py $(ARCHIVEDIR)

