# Paths
TARGET_NAME = test_nn
CODEGENDIR = .
INPUTDIR = ./emnist_byclass_testset_0_999
TEDIR = ./TinyEngine
CMSIS_NN_DIR = ${TEDIR}/third_party/CMSIS_NN

# For code in current path
IDIR = .
ODIR = obj
SOURCES := $(shell find ./ -name '*.c' -maxdepth 1)
DEPS := $(shell find $(IDIR) -name '*.h' -o -name '*.hh' -o -name '*.hpp' -maxdepth 1)
DEPS += $(shell find $(INPUTDIR) -name '*.h' -o -name '*.hh' -o -name '*.hpp' -maxdepth 1)
_OBJ = $(TARGET_NAME).o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

# For model inference code in Include and Source
CODEGEN_IDIR = $(CODEGENDIR)/Include/
CODEGEN_ODIR = obj/genModel
CODEGEN_SOURCES := $(shell find $(CODEGENDIR)/Source/ ! -name 'genModel.c' -name '*.c' -maxdepth 1)
CODEGEN_DEPS := $(shell find $(CODEGEN_IDIR) -name '*.h' -maxdepth 1)
_CODEGEN_OBJ = $(CODEGEN_SOURCES:%.c=%.o)
CODEGEN_OBJ := $(patsubst $(CODEGENDIR)/%, $(CODEGEN_ODIR)/%, $(_CODEGEN_OBJ))

# For TinyEngine
TE_IDIR += $(TEDIR)/include
TE_ODIR = obj/TinyEngine
TE_SOURCES := $(shell find $(TEDIR)/src -name '*.c' -maxdepth 1)
TE_DEPS := $(shell find $(TE_IDIR) -name '*.h' -maxdepth 1)
_TE_OBJ := $(TE_SOURCES:%.c=%.o)
TE_OBJ := $(patsubst $(TEDIR)/%, $(TE_ODIR)/%, $(_TE_OBJ))

# For CMSIS_NN
CMSIS_NN_IDIR = $(CMSIS_NN_DIR)/Include 
CMSIS_NN_ODIR = obj/cmsis_nn
CMSIS_NN_SOURCES := $(shell find $(CMSIS_NN_DIR)/Source -name '*.c' -maxdepth 1)
CMSIS_NN_DEPS := $(shell find $(CMSIS_NN_DIR)/Include -name '*.h' -maxdepth 1)

# Compiler and flags
GCC = gcc
INC = -I$(IDIR) -I$(CODEGEN_IDIR) -I$(TE_IDIR) -I$(CMSIS_NN_IDIR)
CFLAGS = -O2 -g -std=c17
_CMSIS_NN_OBJ := $(CMSIS_NN_SOURCES:%.c=%.o)
CMSIS_NN_OBJ := $(patsubst $(CMSIS_NN_DIR)/%, $(CMSIS_NN_ODIR)/%, $(_CMSIS_NN_OBJ))

############################

$(ODIR)/%.o: %.c $(DEPS)
	@mkdir -p $(@D)
	$(GCC) -c -o $@ $< $(INC) $(CFLAGS) 

$(CODEGEN_ODIR)/%.o: $(CODEGEN_SOURCES) $(CODEGEN_DEPS)
	@mkdir -p $(@D)
	$(GCC) -c -o $@ $(CODEGENDIR)/$*.c $(INC) $(CFLAGS)

$(TE_ODIR)/%.o: $(TE_SOURCES) $(TE_DEPS)
	@mkdir -p $(@D)
	$(GCC) -c -o $@ $(TEDIR)/$*.c $(INC) $(CFLAGS)

$(CMSIS_NN_ODIR)/%.o: $(CMSIS_NN_SOURCES) $(CMSIS_NN_DEPS)
	@mkdir -p $(@D)
	$(GCC) -c -o $@ $(CMSIS_NN_DIR)/$*.c $(INC) $(CFLAGS) 

$(TARGET_NAME): $(OBJ) $(CODEGEN_OBJ) $(CMSIS_NN_OBJ) $(TE_OBJ) 
	$(GCC) $^ -o $@ $(INC) $(CFLAGS)

.PHONY: clean

clean:
	rm -rf $(ODIR)
	rm $(TARGET_NAME)
