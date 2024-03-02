CXX = xcc
TARGET = XU316-1024-QF60A-C24
SRC_DIR = .
LIB_DIR = ./lib
BUILD_DIR = ./build

# Get all source files in the current directory with the specified extensions
SRC_FILES := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.xc)

# Recursively get all source files in the /lib directory with the specified extensions
SRC_FILES += $(wildcard $(LIB_DIR)/**/*.c) $(wildcard $(LIB_DIR)/**/*.cpp) $(wildcard $(LIB_DIR)/**/*.xc)

# Create a list of object files from source files
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(filter %.c,$(SRC_FILES)))
OBJ_FILES += $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(filter %.cpp,$(SRC_FILES)))
OBJ_FILES += $(patsubst $(SRC_DIR)/%.xc,$(BUILD_DIR)/%.o,$(filter %.xc,$(SRC_FILES)))

# Add main.cpp as a dependency
OBJ_FILES += $(BUILD_DIR)/main.o

# Create a list of header files in the current directory
HEADER_FILES := $(wildcard $(SRC_DIR)/*.h)

# Recursively get all header files in the /lib directory
HEADER_FILES += $(wildcard $(LIB_DIR)/**/*.h)

# Compiler flags
CXXFLAGS = -target=$(TARGET)

# Linking flags
LDFLAGS =

# Output executable
EXECUTABLE = my_program

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Rule for building main.o
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(HEADER_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule for building other object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADER_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.xc $(HEADER_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)
