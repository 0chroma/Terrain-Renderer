SOURCE_DIRS	=	\
			./src

all: project

project: $(SOURCE_DIRS) finish

$(SOURCE_DIRS): FORCE
	cd $@; $(MAKE)


finish: 
	@echo ""
	@cp -vf ./src/renderer .
	@chmod +x ./renderer
	@echo ""
	-ls --color ./renderer ./src
	@echo ""
	@echo "Now run renderer!"
	@echo ""

clean:
	for i in $(PROJECT_DIRS); do ( cd $$i ; $(MAKE) clean ) ; done
	
FORCE:
