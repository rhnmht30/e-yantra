
#include <avr/io.h>
#include <util/delay.h>

#include "mcp23017.h"
#include "i2c.h"

/*
 * initialize
 */
void mcp23017_init() {
	#if MCP23017_I2CINIT == 1
	//init i2c
	i2c_init();
	_delay_us(10);
	#endif

	//initialize chip array
	uint8_t i = 0;
	for(i=0; i<MCP23017_NUM; i++) {
		mcp23017_array[i].id = i;
		mcp23017_array[i].modeA = MCP23017_MODEINPUTALL;
		mcp23017_array[i].modeB = MCP23017_MODEINPUTALL;
		mcp23017_array[i].pinstatusA = 0x00;
		mcp23017_array[i].pinstatusB = 0x00;
	}

	//initialize chips
	for(i=0; i<MCP23017_NUM; i++) {
		mcp23017_writebyte(mcp23017_array[i].id, MCP23017_IODIRA, mcp23017_array[i].modeA);
		mcp23017_writebyte(mcp23017_array[i].id, MCP23017_IODIRB, mcp23017_array[i].modeB);
	}
}

/*
 * set mode port A
 */
void mcp23017_setmodeA(uint8_t addr, uint8_t mode) {
	if(addr > MCP23017_NUM)
		return;

	mcp23017_array[addr].modeA = mode;
	mcp23017_writebyte(mcp23017_array[addr].id, MCP23017_IODIRA, mcp23017_array[addr].modeA);
}

/*
 * set mode port B
 */
void mcp23017_setmodeB(uint8_t addr, uint8_t mode) {
	if(addr > MCP23017_NUM)
		return;

	mcp23017_array[addr].modeB = mode;
	mcp23017_writebyte(mcp23017_array[addr].id, MCP23017_IODIRB, mcp23017_array[addr].modeB);
}

/*
 * set mode for pin port A
 */
void mcp23017_setmodepinA(uint8_t addr, uint8_t pin, uint8_t mode) {
	if(addr > MCP23017_NUM)
		return;

	uint8_t data = mcp23017_array[addr].modeA;
	data &= ~(1<<pin);
	if(mode)
		data |= (1<<pin);
	mcp23017_array[addr].modeA = data;
	mcp23017_writebyte(mcp23017_array[addr].id, MCP23017_IODIRA, mcp23017_array[addr].modeA);
}

/*
 * set mode for pin port B
 */
void mcp23017_setmodepinB(uint8_t addr, uint8_t pin, uint8_t mode) {
	if(addr > MCP23017_NUM)
		return;

	uint8_t data = mcp23017_array[addr].modeB;
	data &= ~(1<<pin);
	if(mode)
		data |= (1<<pin);
	mcp23017_array[addr].modeB = data;
	mcp23017_writebyte(mcp23017_array[addr].id, MCP23017_IODIRB, mcp23017_array[addr].modeB);
}

/*
 * write pin status on port A
 */
void mcp23017_writepinA(uint8_t addr, uint8_t pin, uint8_t state) {
	if(addr > MCP23017_NUM)
		return;

#if MCP23017_READDATAFORPINSTATUS == 1
	uint8_t data = mcp23017_readbyte(mcp23017_array[addr].id, MCP23017_OLATA);
#else
	uint8_t data = mcp23017_array[addr].pinstatusA;
#endif
	data &= ~(1<<pin);
	if(!state)
		data |= (1<<pin);
	mcp23017_array[addr].pinstatusA = data;
	mcp23017_writebyte(mcp23017_array[addr].id, MCP23017_GPIOA, mcp23017_array[addr].pinstatusA);
}

/*
 * write pin status on port B
 */
void mcp23017_writepinB(uint8_t addr, uint8_t pin, uint8_t state) {
	if(addr > MCP23017_NUM)
		return;

#if MCP23017_READDATAFORPINSTATUS == 1
	uint8_t data = mcp23017_readbyte(mcp23017_array[addr].id, MCP23017_OLATB);
#else
	uint8_t data = mcp23017_array[addr].pinstatusB;
#endif
	data &= ~(1<<pin);
	if(!state)
		data |= (1<<pin);
	mcp23017_array[addr].pinstatusB = data;
	mcp23017_writebyte(mcp23017_array[addr].id, MCP23017_GPIOB, mcp23017_array[addr].pinstatusB);
}

/*
 * write pins status on port A
 */
void mcp23017_writepinsA(uint8_t addr, uint8_t state) {
	if(addr > MCP23017_NUM)
		return;

	mcp23017_array[addr].pinstatusA = state;
	mcp23017_writebyte(mcp23017_array[addr].id, MCP23017_GPIOA, mcp23017_array[addr].pinstatusA);
}

/*
 * write pins status on port B
 */
void mcp23017_writepinsB(uint8_t addr, uint8_t state) {
	if(addr > MCP23017_NUM)
		return;

	mcp23017_array[addr].pinstatusB = state;
	mcp23017_writebyte(mcp23017_array[addr].id, MCP23017_GPIOB, mcp23017_array[addr].pinstatusB);
}

/*
 * read pin status on port A
 */
uint8_t mcp23017_readpinA(uint8_t addr, uint8_t pin) {
	if(addr > MCP23017_NUM)
		return 0x00;

	uint8_t data = mcp23017_readbyte(mcp23017_array[addr].id, MCP23017_GPIOA);
	return ((data>>pin) & 0b00000001);
}

/*
 * read pin status on port B
 */
uint8_t mcp23017_readpinB(uint8_t addr, uint8_t pin) {
	if(addr > MCP23017_NUM)
		return 0x00;

	uint8_t data = mcp23017_readbyte(mcp23017_array[addr].id, MCP23017_GPIOB);
	return ((data>>pin) & 0b00000001);
}

/**
 * Read pins status on port A
 */
uint8_t mcp23017_readpinsA(uint8_t addr) {
	if(addr > MCP23017_NUM)
		return 0x00;

	return mcp23017_readbyte(mcp23017_array[addr].id, MCP23017_GPIOA);
}

/*
 * read pins status on port B
 */
uint8_t mcp23017_readpinsB(uint8_t addr) {
	if(addr > MCP23017_NUM)
		return 0x00;

	return mcp23017_readbyte(mcp23017_array[addr].id, MCP23017_GPIOB);
}

/*
 * write a byte
 */
void mcp23017_writebyte(uint8_t address, uint8_t reg, uint8_t data) {
	i2c_start_wait(I2C_WRITE + MCP23017_BASEADDRESS + address);
	i2c_write(reg);
	i2c_write(data);
	i2c_stop();
}

/*
 * write a word
 */
void mcp23017_writeword(uint8_t address, uint8_t reg, uint16_t data) {
	i2c_start_wait(I2C_WRITE + MCP23017_BASEADDRESS + address);
	i2c_write(reg);
	i2c_write((uint8_t)data);
	i2c_write((uint8_t)(data>>8));
	i2c_stop();
}

/*
 * read a byte
 */
uint8_t mcp23017_readbyte(uint8_t address, uint8_t reg) {
	i2c_start_wait(I2C_WRITE + MCP23017_BASEADDRESS + address);
	i2c_write(reg);
	i2c_rep_start(I2C_READ + MCP23017_BASEADDRESS + address);
	uint8_t data = i2c_readNak();
	i2c_stop();
	return data;
}

/*
 * set interrupts for port A
 */
void mcp23017_setupinterruptsA(uint8_t addr, uint8_t mirroring, uint8_t opendrain, uint8_t polarity) {
	if(addr > MCP23017_NUM)
		return;

	uint8_t data = mcp23017_readbyte(mcp23017_array[addr].id, MCP23017_IOCONA);
	data &= ~(1<<6);
	data |= (1<<mirroring);
	data &= ~(1<<2);
	data |= (1<<opendrain);
	data &= ~(1<<1);
	data |= (1<<polarity);
	mcp23017_writebyte(mcp23017_array[addr].id, MCP23017_IOCONA, data);
}

/*
 * set interrupts for port B
 */
void mcp23017_setupinterruptsB(uint8_t addr, uint8_t mirroring, uint8_t opendrain, uint8_t polarity) {
	if(addr > MCP23017_NUM)
		return;

	uint8_t data = mcp23017_readbyte(mcp23017_array[addr].id, MCP23017_IOCONB);
	data &= ~(1<<6);
	data |= (1<<mirroring);
	data &= ~(1<<2);
	data |= (1<<opendrain);
	data &= ~(1<<1);
	data |= (1<<polarity);
	mcp23017_writebyte(mcp23017_array[addr].id, MCP23017_IOCONB, data);
}

/*
 * set interrupt pin for port A
 */
void mcp23017_setinterruptpinA(uint8_t addr, uint8_t pin, uint8_t ischange, uint8_t isrising, uint8_t enabled) {
	if(addr > MCP23017_NUM)
		return;

	uint8_t data = 0;

	//set the compare mode
	data = mcp23017_readbyte(mcp23017_array[addr].id, MCP23017_INTCONA);
	data &= ~(1<<pin);
	if(!ischange)
		data |= (1<<pin); //compare against a given value
	mcp23017_writebyte(mcp23017_array[addr].id, MCP23017_INTCONA, data);

	//set the edge mode
	data = mcp23017_readbyte(mcp23017_array[addr].id, MCP23017_DEFVALA);
	data &= ~(1<<pin);
	if(!isrising)
		data |= (1<<pin);
	mcp23017_writebyte(mcp23017_array[addr].id, MCP23017_DEFVALA, data);

	//enable or disable the interrupt
	data = mcp23017_readbyte(mcp23017_array[addr].id, MCP23017_GPINTENA);
	data &= ~(1<<pin);
	if(enabled)
		data |= (1<<pin);
	mcp23017_writebyte(mcp23017_array[addr].id, MCP23017_GPINTENA, data);
}

/*
 * set interrupt pin for port B
 */
void mcp23017_setinterruptpinB(uint8_t addr, uint8_t pin, uint8_t ischange, uint8_t isrising, uint8_t enabled) {
	if(addr > MCP23017_NUM)
		return;

	uint8_t data = 0;

	//set the compare mode
	data = mcp23017_readbyte(mcp23017_array[addr].id, MCP23017_INTCONB);
	data &= ~(1<<pin);
	if(!ischange)
		data |= (1<<pin); //compare against a given value
	mcp23017_writebyte(mcp23017_array[addr].id, MCP23017_INTCONB, data);

	//set the edge mode
	data = mcp23017_readbyte(mcp23017_array[addr].id, MCP23017_DEFVALB);
	data &= ~(1<<pin);
	if(!isrising)
		data |= (1<<pin);
	mcp23017_writebyte(mcp23017_array[addr].id, MCP23017_DEFVALB, data);

	//enable or disable the interrupt
	data = mcp23017_readbyte(mcp23017_array[addr].id, MCP23017_GPINTENB);
	data &= ~(1<<pin);
	if(enabled)
		data |= (1<<pin);
	mcp23017_writebyte(mcp23017_array[addr].id, MCP23017_GPINTENB, data);
}
