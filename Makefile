####################################
# ARINC653-simulator head Makefile #
####################################
CC 	:= arm-linux-gnueabi-g++
#CC 		:= g++
INCLUDE_DIR	:= $(shell pwd)/include/libApexArinc653 #$(shell pwd)/include/libApexArinc653Jni
CC_FLAGS_ALL	:= -fPIC -Wall -pedantic
LIBS		:= -lpthread -L$(shell pwd)/lib/ -lApexArinc653 -Wl,-rpath ./
LIBAPEXARINC653_DIR	:= sources/libApexArinc653
#LIBAPEXARINC653JNI_DIR	:= sources/libApexArinc653Jni

define SRC_2_OBJ
    $(foreach src,$(1),$(patsubst sources/%,build/%,$(src)))
endef

define SRC_2_BIN
    $(foreach src,$(1),$(patsubst sources/%,binary/%,$(src)))
endef

define SRC_2_SYM
    $(foreach src,$(1),$(patsubst sources/%.sym,binary/%,$(src)))
endef

all: lib targets
#	cp -f ./lib/libApexArinc653.so  ./binary/UseCases/Test_Drone/libApexArinc653.so
#	cp -f ./binary/UseCases/Test_Drone/f2/f2.out ./binary/UseCases/Test_Drone/f2.out

build/%.o: sources/%.cpp
	@echo "  [CC]    $< -> $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CC_FLAGS_ALL) $(CC_FLAGS_SPEC) -I$(INCLUDE_DIR) -o $@ -c $<

%.sym :
	for path in $^ ; do \
	    echo ln -fs $(shell pwd)/$$path $(call SRC_2_SYM, $@)/; \
	    ln -fs $(shell pwd)/$$path $(call SRC_2_SYM, $@)/; \
	done

binary/%.out:
	@echo "  [CC]    $< -> $@"
	@mkdir -p $(dir $@)
	@echo -I$(INCLUDE_DIR)  -o $@ $^ $(LIBS)
	@$(CC) -I$(INCLUDE_DIR)  -o $@ $^ $(LIBS)

# Overriden in rules.mk
TARGETS :=
OBJECTS :=
SYMLINKS :=

dir	:= sources
include	$(dir)/rules.mk

targets: link $(patsubst sources/%, binary/%, $(TARGETS)) symlinks

clean:
	@rm -f $(TARGETS) $(OBJECTS)
#	@rm */*.so

mrproper :
	@rm -f $(TARGETS) $(OBJECTS)
	@rm -rf lib
	@find -name *~ | xargs rm -f
	@find -name "*.fifo" | xargs rm -f
	@(cd $(shell pwd)/sources/libApexArinc653/ && $(MAKE) $@)
#	@(cd $(shell pwd)/sources/libApexArinc653Jni/ && $(MAKE) $@)

symlinks: $(SYMLINKS)
	@echo symlinks created

info:
	@echo Targets [$(TARGETS)]
	@echo Objects [$(OBJECTS)]
	@echo Symlinks [$(SYMLINKS)]

link :
	@export LD_LIBRARY_PATH=$(shell pwd)/lib/


lib:
	@(cd $(shell pwd)/$(LIBAPEXARINC653_DIR) && $(MAKE) $@)
