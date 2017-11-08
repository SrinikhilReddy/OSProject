#include<sys/defs.h>
#include<sys/kprintf.h>
#include<sys/ahci.h>
uint16_t getVendorID(uint8_t bus, uint8_t slot,uint8_t fun);
void checkDevice(uint8_t bus, uint8_t device,uint8_t fun);



void sysOutLong(uint16_t port,uint32_t addr){
	__asm__ volatile ( "outl %0, %1" : : "a"(addr), "Nd"(port) );
}
uint32_t sysInLong(uint16_t port){
	uint32_t ret;
	__asm__ volatile ( "inl %1, %0"
			: "=a"(ret)
			: "Nd"(port) );
	return ret;
}
void checkAllBuses(void) {
	uint8_t bus;
	uint8_t device,fun;
	for(bus = 0; bus < 255; bus++) {
		for(device = 0; device < 32; device++) {
			for(fun=0;fun<7;fun++)	
			{checkDevice(bus, device, fun);}
		}
	}
}

void checkDevice(uint8_t bus, uint8_t device,uint8_t fun) {
//	uint8_t function = 0;

	uint16_t vendorID = getVendorID(bus, device, fun);
	if(vendorID == 0xFFFF) return;        // Device doesn't exist
	/* checkFunction(bus, device, function);
	   headerType = getHeaderType(bus, device, function);
	   if( (headerType & 0x80) != 0) {
	// It is a multi-function device, so check remaining functions 
	for(function = 1; function < 8; function++) {
	if(getVendorID(bus, device, function) != 0xFFFF) {
	checkFunction(bus, device, function);
	}
	}
	}*/
/*	else{
	 kprintf("VendorId:%d ",vendorID);
	}*/
}

uint16_t pciConfigReadWord (uint8_t bus, uint8_t slot,
		uint8_t func, uint8_t offset){
	uint32_t address;
	uint32_t lbus  = (uint32_t)bus;
	uint32_t lslot = (uint32_t)slot;
	uint32_t lfunc = (uint32_t)func;
	uint16_t tmp = 0;

	/* create configuration address as per Figure 1 */
	address = (uint32_t)((lbus << 16) | (lslot << 11) |
			(lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));

	/* write out the address */
	sysOutLong (0xCF8, address);
	/* read in the data */
	/* (offset & 2) * 8) = 0 will choose the first word of the 32 bits register */
	tmp = (uint16_t)((sysInLong (0xCFC) >> ((offset & 2) * 8)) & 0xffff);
	return (tmp);
}

uint32_t pciConfigReadLong (uint8_t bus, uint8_t slot,
                uint8_t func, uint8_t offset)
{
        uint32_t address;
        uint32_t lbus  = (uint32_t)bus;
        uint32_t lslot = (uint32_t)slot;
        uint32_t lfunc = (uint32_t)func;
        uint32_t tmp = 0;

        /* create configuration address as per Figure 1 */
        address = (uint32_t)((lbus << 16) | (lslot << 11) |
                        (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));

        /* write out the address */
        sysOutLong (0xCF8, address);
        /* read in the data */
        /* (offset & 2) * 8) = 0 will choose the first word of the 32 bits register */
        tmp = ((sysInLong (0xCFC)));
        return (tmp);
}

void pciConfigWrite(uint8_t bus, uint8_t slot,uint8_t func, uint8_t offset,uint32_t writeVal){
	uint32_t address;
        uint32_t lbus  = (uint32_t)bus;
        uint32_t lslot = (uint32_t)slot;
        uint32_t lfunc = (uint32_t)func;

        address = (uint32_t)((lbus << 16) | (lslot << 11) |
                        (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));

	
	sysOutLong (0xCF8,address);	
//	uint32_t tmp = ((sysInLong (0xCFC)));
//	sysOutLong (0xCF8, tmp); 
	sysOutLong (0xCFC, writeVal);
	return;
}
uint16_t getVendorID(uint8_t bus, uint8_t slot,uint8_t fun){
	uint16_t vendor, device;
	/* try and read the first configuration register. Since there are no */
	/* vendors that == 0xFFFF, it must be a non-existent device. */
	if ((vendor = pciConfigReadWord(bus,slot,fun,0)) != 0xFFFF) {
		device = pciConfigReadWord(bus,slot,fun,2);
		kprintf("DeviceId:%d, VendorId:%d",device,vendor);
		if(( (pciConfigReadWord(bus,slot,fun,10) & 0x00FF)  == 0x06) && ( (pciConfigReadWord(bus,slot,fun,10) & 0xFF00 ) >> 8 == 0x01)){
			kprintf(" Type:AHCI \n");
			uint32_t x = (uint32_t)0x3ffff000;
//			pciConfigWrite(bus,slot,fun,0x24,x);
			x = /*0xffffffff80000000+*/pciConfigReadLong (bus,slot,fun,0x24); 
			probe_port((hba_mem_t *)(uint64_t)(x));
		}
	kprintf("\n");
	} return (vendor);
}

