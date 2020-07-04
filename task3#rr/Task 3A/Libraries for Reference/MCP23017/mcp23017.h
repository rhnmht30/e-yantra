
#include <avr/io.h>


#ifndef MCP23017_H_
#define MCP23017_H_


#define MCP23017_BASEADDRESS (0x40) //device base address

#include "i2c.h"
#define MCP23017_I2CINIT 1 //init i2c

//set how many ic are connected, max is 8
#define MCP23017_NUM 1

//struct definition
typedef struct {
	uint8_t id;
	uint8_t modeA;
	uint8_t modeB;
	uint8_t pinstatusA;
	uint8_t pinstatusB;
} mcp23017_arrayt;

//set array
volatile mcp23017_arrayt mcp23017_array[MCP23017_NUM];

//pin modes
#define MCP23017_MODEOUTPUT 0
#define MCP23017_MODEINPUT 1
#define MCP23017_MODEOUTPUTALL 0b00000000
#define MCP23017_MODEINPUTALL 0b11111111

//states for pin
#define MCP23017_PINSTATEON 1
#define MCP23017_PINSTATEOFF 0
#define MCP23017_PINSTATEONALL 0b11111111
#define MCP23017_PINSTATEOFFALL 0b00000000

//read data from chip for pin status or use internal structure
#define MCP23017_READDATAFORPINSTATUS 1

//registers
#define MCP23017_IODIRA 0x00
#define MCP23017_IODIRB 0x01
#define MCP23017_IPOLA 0x02
#define MCP23017_IPOLB 0x03
#define MCP23017_GPINTENA 0x04
#define MCP23017_GPINTENB 0x05
#define MCP23017_DEFVALA 0x06
#define MCP23017_DEFVALB 0x07
#define MCP23017_INTCONA 0x08
#define MCP23017_INTCONB 0x09
#define MCP23017_IOCONA 0x0A
#define MCP23017_IOCONB 0x0B
#define MCP23017_GPPUA 0x0C
#define MCP23017_GPPUB 0x0D
#define MCP23017_INTFA 0x0E
#define MCP23017_INTFB 0x0F
#define MCP23017_INTCAPA 0x10
#define MCP23017_INTCAPB 0x11
#define MCP23017_GPIOA 0x12
#define MCP23017_GPIOB 0x13
#define MCP23017_OLATA 0x14
#define MCP23017_OLATB 0x15


//functions
extern void mcp23017_writebyte(uint8_t address, uint8_t reg, uint8_t data);
extern void mcp23017_writeword(uint8_t address, uint8_t reg, uint16_t data);
extern uint8_t mcp23017_readbyte(uint8_t address, uint8_t reg);
extern void mcp23017_init();
extern void mcp23017_setmodeA(uint8_t addr, uint8_t mode);
extern void mcp23017_setmodeB(uint8_t addr, uint8_t mode);
extern void mcp23017_setmodepinA(uint8_t addr, uint8_t pin, uint8_t mode);
extern void mcp23017_setmodepinB(uint8_t addr, uint8_t pin, uint8_t mode);
extern void mcp23017_writepinA(uint8_t addr, uint8_t pin, uint8_t state);
extern void mcp23017_writepinB(uint8_t addr, uint8_t pin, uint8_t state);
extern void mcp23017_writepinsA(uint8_t addr, uint8_t state);
extern void mcp23017_writepinsB(uint8_t addr, uint8_t state);
extern uint8_t mcp23017_readpinA(uint8_t addr, uint8_t pin);
extern uint8_t mcp23017_readpinB(uint8_t addr, uint8_t pin);
extern uint8_t mcp23017_readpinsA(uint8_t addr);
extern uint8_t mcp23017_readpinsB(uint8_t addr);
extern void mcp23017_setupinterruptsA(uint8_t addr, uint8_t mirroring, uint8_t opendrain, uint8_t polarity);
extern void mcp23017_setupinterruptsB(uint8_t addr, uint8_t mirroring, uint8_t opendrain, uint8_t polarity);
extern void mcp23017_setinterruptpinA(uint8_t addr, uint8_t pin, uint8_t ischange, uint8_t isrising, uint8_t enabled);
extern void mcp23017_setinterruptpinB(uint8_t addr, uint8_t pin, uint8_t ischange, uint8_t isrising, uint8_t enabled);

#endif
