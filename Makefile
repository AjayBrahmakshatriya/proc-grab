SRC_DIR=$(shell pwd)/src
SAMPLE_DIR=$(shell pwd)/sample
BUILD_DIR=$(shell pwd)/build
INC_DIR=$(shell pwd)/include

SOURCES=$(wildcard $(SRC_DIR)/*.c)
HEADERS=$(wildcard $(INC_DIR)/*.h)
OBJECTS=$(subst $(SRC_DIR),$(BUILD_DIR),$(SOURCES:.c=.o))


TARGET=$(BUILD_DIR)/grab $(BUILD_DIR)/release
SAMPLES=$(BUILD_DIR)/sample


$(shell mkdir -p $(BUILD_DIR))

all: $(TARGET) $(SAMPLES)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC)  -c $< -o $@ -I$(INC_DIR)




$(BUILD_DIR)/release: $(OBJECTS)

$(BUILD_DIR)/grab: $(OBJECTS)
	$(CC) $^ -o $@
$(BUILD_DIR)/sample: $(SAMPLE_DIR)/sample.c
	$(CC) -o $@ $<

clean:
	rm -rf build

