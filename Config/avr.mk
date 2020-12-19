export AR=avr-ar
export CXX=avr-g++
export CC=avr-gcc
export CFLAGS=\
	-funsigned-bitfields                                                  \
	-fdata-sections                                                       \
	-fpack-struct                                                         \
	-fshort-enums                                                         \
	-g2                                                                   \
	-Wall                                                                 \
	-mmcu=atmega328p                                                      \
	-Os                                                                   \
	-ffunction-sections                                                   \
	-DF_CPU=8000000L                                                      \
	-fno-threadsafe-statics                                               

