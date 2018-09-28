######################################
# source
######################################
# C sources
APP_SRC =  \
		${wildcard ../Middlewares/LiteOS/examples/api/*.c} 
        C_SOURCES += $(APP_SRC)

# C defines
C_DEFS +=  \
-DLOS_KERNEL_DEBUG_OUT

# C includes
APP_INC = \
		-I ../OS_CONFIG \
		-I ../Middlewares/LiteOS/examples/include
        C_INCLUDES += $(APP_INC)