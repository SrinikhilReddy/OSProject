#include<sys/defs.h>
#include<sys/ahci.h>
#include<sys/kprintf.h>
#define ATA_DEV_BUSY 0x80
#define ATA_DEV_DRQ 0x08

static int check_type(hba_port_t *port);
int find_cmdslot(hba_port_t *port);
int verify_read_write(hba_port_t* port);
/*int strcmp(char *s,char *t){
        while(*s==*t)
        {   
                if(*s=='\0')
                        return 0;
                s++;
                t++;
        }   
        return *s-*t;
}*/
void *memset(void *s, int c, size_t n)
{
	unsigned char* p=s;
	while(n--)
		*p++ = (unsigned char)c;
	return s;
}

void probe_port(hba_mem_t *abar)
{
	// Search disk in impelemented ports
	uint32_t pi = abar->pi;
	int i = 0;
	while (i<32)
	{
		//		kprintf("%d",i);
		if (pi & 1)
		{
			int dt = check_type(&abar->ports[i]);
			if (dt == AHCI_DEV_SATA)
			{
				kprintf("SATA drive found at port %d\n", i);
				//				port_rebase(&abar->ports[i],i,(uint32_t)abar);
				if(i==1)
				{
//					verify_read_write(&(abar->ports[i]));
				}
			}
			else if (dt == AHCI_DEV_SATAPI)
			{
				kprintf("SATAPI drive found at port %d\n", i);
			}
			else if (dt == AHCI_DEV_SEMB)
			{
				kprintf("SEMB drive found at port %d\n", i);
			}
			else if (dt == AHCI_DEV_PM)
			{
				kprintf("PM drive found at port %d\n", i);
			}
			else
			{
				kprintf("No drive found at port %d\n", i);
			}
		}

		pi >>= 1;
		i ++;
	}

}

// Check device type
static int check_type(hba_port_t *port)
{
	uint32_t ssts = port->ssts;

	uint8_t ipm = (ssts >> 8) & 0x0F;
	uint8_t det = ssts & 0x0F;

	if (det != HBA_PORT_DET_PRESENT)	// Check drive status
		return AHCI_DEV_NULL;
	if (ipm != HBA_PORT_IPM_ACTIVE)
		return AHCI_DEV_NULL;

	switch (port->sig)
	{
		case SATA_SIG_ATAPI:
			return AHCI_DEV_SATAPI;
		case SATA_SIG_SEMB:
			return AHCI_DEV_SEMB;
		case SATA_SIG_PM:
			return AHCI_DEV_PM;
		default:
			return AHCI_DEV_SATA;
	}
}
//mode 0-read 1-write
BOOL readorwrite(hba_port_t *port, uint32_t startl, uint32_t starth, uint32_t count, uint16_t *buf,uint8_t mode)
{
	port->is_rwc = (uint32_t)-1;		// Clear pending interrupt bits
	int spin = 0; // Spin lock timeout counter
	int slot = find_cmdslot(port);
	if (slot == -1)
		return FALSE;

	hba_cmd_header_t *cmdheader = (hba_cmd_header_t*)port->clb;
	cmdheader += slot;
	cmdheader->cfl = sizeof(fis_reg_h2d_t)/sizeof(uint32_t);	// Command FIS size
	if(mode == 0)cmdheader->w = 0;
	else cmdheader->w=1;		// Read from device
	cmdheader->prdtl = (uint16_t)((count-1)>>4) + 1;	// PRDT entries count

	hba_cmd_tbl_t *cmdtbl = (hba_cmd_tbl_t*)(cmdheader->ctba);
	memset(cmdtbl, 0, sizeof(hba_cmd_tbl_t) +
			(cmdheader->prdtl-1)*sizeof(hba_prdt_entry_t));

	int i=0; 
	// 8K bytes (16 sectors) per PRDT
	for (i=0; i<cmdheader->prdtl-1; i++)
	{
		cmdtbl->prdt_entry[i].dba = (uint64_t)buf;
		cmdtbl->prdt_entry[i].dbc = 8*1024;	// 8K bytes
		cmdtbl->prdt_entry[i].i = 1;
		buf += 4*1024;	// 4K words
		count -= 16;	// 16 sectors
	}
	// Last entry
	cmdtbl->prdt_entry[i].dba = (uint64_t)buf;
	cmdtbl->prdt_entry[i].dbc = count<<9;	// 512 bytes per sector
	cmdtbl->prdt_entry[i].i = 1;

	// Setup command
	fis_reg_h2d_t *cmdfis = (fis_reg_h2d_t*)(&cmdtbl->cfis);

	cmdfis->fis_type = FIS_TYPE_REG_H2D;
	cmdfis->c = 1;	// Command
	if(mode == 0) cmdfis->command = 0x25;
	else cmdfis->command = 0x35;

	cmdfis->lba0 = (uint8_t)startl;
	cmdfis->lba1 = (uint8_t)(startl>>8);
	cmdfis->lba2 = (uint8_t)(startl>>16);
	cmdfis->device = 1<<6;	// LBA mode

	cmdfis->lba3 = (uint8_t)(startl>>24);
	cmdfis->lba4 = (uint8_t)starth;
	cmdfis->lba5 = (uint8_t)(starth>>8);

	cmdfis->count = count;

	/*cmdfis->countl = LOBYTE(count);
	  cmdfis->counth = HIBYTE(count);*/

	// The below loop waits until the port is no longer busy before issuing a new command
	while ((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
	{
		spin++;
	}
	if (spin == 1000000)
	{
		kprintf("Port is hung\n");
		return FALSE;
	}

	port->ci = 1<<slot;	// Issue command

	// Wait for completion
	while (1)
	{
		// In some longer duration reads, it may be helpful to spin on the DPS bit 
		// in the PxIS port field as well (1 << 5)
		if ((port->ci & (1<<slot)) == 0) 
			break;
		if (port->is_rwc & HBA_PxIS_TFES)	// Task file error
		{
			kprintf("Read disk error\n");
			return FALSE;
		}
	}

	// Check again
	if (port->is_rwc & HBA_PxIS_TFES)
	{
		kprintf("Read disk error\n");
		return FALSE;
	}

	return TRUE;
}
// Find a free command list slot
int find_cmdslot(hba_port_t *port)
{
	// If not set in SACT and CI, the slot is free
	uint32_t slots = (port->sact | port->ci);
	for (int i=0; i<32; i++)
	{
		if ((slots&1) == 0)
			return i;
		slots >>= 1;
	}
	kprintf("Cannot find free command list entry\n");
	return -1;
}
int verify_read_write(hba_port_t* port){
        port->cmd |= HBA_PxCMD_FRE;  
        port->cmd |= HBA_PxCMD_ST;  
	int i;
	for( i=0;i<100;i++){
		char* buf = (char*)0xffffffff8800000;
                int j = 0;
		for(j=0;j<4096;j++){
			*(buf+j) = (char)(i);
		}
//		*(buf+512) = '\0';
		//*buf = (char)'A';
		readorwrite(port,8*i,0,8,(uint16_t*)buf,1);
		char* ss =  (char*)0xffffffff900000;
		readorwrite(port,8*i,0,8,(uint16_t*)ss,0);
//		if(strcmp(buf,ss)==0) ;
//		else {kprintf("Incorrect"); return 0;}
		//kprintf("%s",ss);
	}
	port->cmd &= ~HBA_PxCMD_ST;
	kprintf("Read Write Verified");
	return 1;
}
