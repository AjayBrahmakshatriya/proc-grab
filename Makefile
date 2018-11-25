SRC_DIR=$(shell pwd)/src
SAMPLE_DIR=$(shell pwd)/sample
BUILD_DIR=$(shell pwd)/build
INC_DIR=$(shell pwd)/include

SOURCES := $(wildcard $(SRC_DIR)/*.c)
SOURCES := $(filter-out $(SRC_DIR)/grab.c, $(SOURCES))
SOURCES := $(filter-out $(SRC_DIR)/release.c, $(SOURCES))
SOURCES := $(filter-out $(SRC_DIR)/asm_offset.c, $(SOURCES))


HEADERS=$(wildcard $(INC_DIR)/*.h)
OBJECTS=$(subst $(SRC_DIR),$(BUILD_DIR),$(SOURCES:.c=.o))


TARGET=$(BUILD_DIR)/grab $(BUILD_DIR)/release
SAMPLES=$(BUILD_DIR)/sample

DEFINES=-D_RAW_BINARY_PATH=$(BUILD_DIR)/raw_binary.o

$(shell mkdir -p $(BUILD_DIR))

all: $(TARGET) $(SAMPLES) $(BUILD_DIR)/raw_binary.o

$(BUILD_DIR)/asm_offset.h: $(SRC_DIR)/asm_offset.c $(HEADERS)
	$(CC) $< -I$(INC_DIR) $(DEFINES) -o $(BUILD_DIR)/asm_offset_gen
	$(BUILD_DIR)/asm_offset_gen > $@


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC)  -c $< -o $@ -I$(INC_DIR) $(DEFINES)

$(BUILD_DIR)/release_asm.o: $(SRC_DIR)/release_asm.s
	$(CC) -c $< -o $@

$(BUILD_DIR)/raw_binary.o: $(SRC_DIR)/raw_binary_asm.S $(BUILD_DIR)/asm_offset.h
	$(CC) -c $< -o $@ -I$(BUILD_DIR)
	objcopy -O binary $@

$(BUILD_DIR)/release: $(OBJECTS) $(BUILD_DIR)/release.o $(BUILD_DIR)/release_asm.o
	$(CC) $^ -o $@

$(BUILD_DIR)/grab: $(OBJECTS) $(BUILD_DIR)/grab.o
	$(CC) $^ -o $@


$(BUILD_DIR)/sample: $(SAMPLE_DIR)/sample.c
	$(CC) -o $@ $<

clean:
	rm -rf build

