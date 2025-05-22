# Variables
SRC_DIR = ./src
OUTPUT_DIR = ./docs
TEMPLATE_DIR = ./template
INCLUDE_DIR = ./include
PRELOAD_DIR = ./assets

SOURCE_FILES = $(SRC_DIR)/*.c
OUTPUT_FILE = $(OUTPUT_DIR)/index.html
SHELL_FILE = $(TEMPLATE_DIR)/shell.html

# Flags de compilación
EMCC_FLAGS = -I/usr/local/include \
	-s USE_GLFW=3 \
	-s MIN_WEBGL_VERSION=2 \
	-s MAX_WEBGL_VERSION=2 \
	-s FULL_ES3=1 \
	-s WASM=1 \
	-s ALLOW_MEMORY_GROWTH=1 \
	-s GL_PREINITIALIZED_CONTEXT=1 \
	-s EXPORTED_FUNCTIONS='["_main", "_showHideObj"]' \
	-s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
	--preload-file $(PRELOAD_DIR)/ \
	-O2 \
	--shell-file $(SHELL_FILE) \
    -I$(INCLUDE_DIR) \

# Destino por defecto
all: setup compile copy

# Configurar directorios
setup:
	@mkdir -p $(OUTPUT_DIR)

# Compilar con emcc - con salida silenciada
compile:
	@echo "Compilando..."
	@EMSDK_QUIET=1 emcc $(SOURCE_FILES) -o $(OUTPUT_FILE) $(EMCC_FLAGS)

# Copiar archivos adicionales
copy:
	@cp -f $(TEMPLATE_DIR)/* $(OUTPUT_DIR)/ > /dev/null 2>&1
	@rm -f $(OUTPUT_DIR)/shell.html
	@echo "Compilación completada."

# Limpiar archivos generados
clean:
	@rm -rf $(OUTPUT_DIR)
	@echo "Limpieza completada."

# Declaración de phony targets
.PHONY: all setup compile copy clean