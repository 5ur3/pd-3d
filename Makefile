HEAP_SIZE      = 8388208
STACK_SIZE     = 61800

PRODUCT = 3d.pdx

# Locate the SDK
SDK = ${PLAYDATE_SDK_PATH}
ifeq ($(SDK),)
	SDK = $(shell egrep '^\s*SDKRoot' ~/.Playdate/config | head -n 1 | cut -c9-)
endif

ifeq ($(SDK),)
$(error SDK path not found; set ENV value PLAYDATE_SDK_PATH)
endif

######
# IMPORTANT: You must add your source folders to VPATH for make to find them
# ex: VPATH += src1:src2
######

VPATH += src

# List C source files here
SRC = src/main.c \
		src/screen/screen.c \
		src/screen/dither.c \
		src/screen/triangle.c \
		src/vector/vector3.c \
		src/geometry/geometry.c \
		src/camera/camera.c \
		src/object/object.c \
		src/scene/scene.c \
		src/renderer/renderer.c \
		src/rotation/quaternion.c	

# List all user directories here
UINCDIR = 

# List user asm files
UASRC = 

# List all user C define here, like -D_DEBUG=1
UDEFS = 

# Define ASM defines here
UADEFS = 

# List the user directory to look for the libraries here
ULIBDIR = src

# List all user libraries here
ULIBS = 

include $(SDK)/C_API/buildsupport/common.mk

