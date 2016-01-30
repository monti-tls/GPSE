# Tools
CC  = g++
LD  = g++
FMT = clang-format

# Directories
SRC_DIR = src
INC_DIR = inc
TMP_DIR = obj
BIN_DIR = bin

# Configuration
PRODUCT  = gpse
DEFINES  =
CC_FLAGS =
AS_FLAGS =
LD_FLAGS =

# Mandatory CC flags
CC_FLAGS += -std=gnu++11 -g -O0
CC_FLAGS += -Wall -Wextra
CC_FLAGS += $(DEFINES)
CC_FLAGS += -I$(INC_DIR) -I$(SRC_DIR)

# Format flags
FMT_FLAGS = -i -style=file

# Sources management
C_SUB  = $(shell find $(SRC_DIR) -type d)
H_SUB  = $(shell find $(INC_DIR) -type d)

C_SRC  = $(wildcard $(addsuffix /*.cpp,$(C_SUB)))

C_OBJ  = $(patsubst $(SRC_DIR)/%.cpp,$(TMP_DIR)/%.o,$(C_SRC))
C_DEP  = $(patsubst $(SRC_DIR)/%.cpp,$(TMP_DIR)/%.d,$(C_SRC))

C_FMT  = $(foreach d,$(C_SUB),$(patsubst $(d)/%.cpp,$(d)/fmt-%,$(wildcard $(d)/*.cpp)))
H_FMT  = $(foreach d,$(H_SUB),$(patsubst $(d)/%.hpp,$(d)/fmt-%,$(wildcard $(d)/*.hpp)))

# Product files
BINARY = $(BIN_DIR)/$(PRODUCT)

# Top-level
all: gpse

gpse: $(BINARY)

.PHONY: clean
clean:
	@rm -rf $(BIN_DIR) $(TMP_DIR)

.PHONY: format
format: $(C_FMT) $(H_FMT)

# Dependencies
-include $(C_DEPS)

# Translation
$(BINARY): $(C_OBJ)
	@mkdir -p $(@D)
	@echo "(LD)      $@"
	@$(LD) -o $@ $^ $(LD_FLAGS)

$(TMP_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	@echo "(CC)      $<"
	@$(CC) $(CC_FLAGS) -MMD -c $< -o $@

# Format
fmt-%: %.cpp
	@$(FMT) $(FMT_FLAGS) $<
	@echo "(FMT)     $<"

fmt-%: %.hpp
	@$(FMT) $(FMT_FLAGS) $<
	@echo "(FMT)     $<"
