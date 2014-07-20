/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "stm32_eval_sdio_sd.h"
#include "misc.h"

/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */

/*
#define ATA		0
#define MMC		1
#define USB		2
 */

//static volatile DSTATUS Stat = STA_NOINIT;	/* Disk status */

extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
extern USBH_HOST                     USB_Host;


/*-----------------------------------------------------------------------*/
/* Inicializes a Drive                                                    */

DSTATUS disk_initialize (BYTE drv)    /* Physical drive nmuber (0..) */
{
  DSTATUS stat = STA_NOINIT;

	switch(drv) {
	case MMC:
		stat = SD_disk_initialize(MMC);
		break;

	case USB:
		stat = USB_disk_initialize(MMC);
	  break;
  }
  
  return stat;
  
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;

	switch (drv) {
	case ATA :
//		stat = ATA_disk_status();
		// translate the reslut code here
  		
//		return stat;

	case MMC :
		stat = SD_disk_status(MMC);
		// translate the reslut code here


	case USB :
		stat = USB_disk_status(USB);
		// translate the reslut code here

	}

	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{
	DRESULT res;

	switch (drv) {
	case ATA :
//		result = ATA_disk_read(buff, sector, count);
		// translate the reslut code here

		return res;

	case MMC :
		res = SD_disk_read(MMC,buff, sector, count);
		// translate the reslut code here

		return res;

	case USB :
		res = USB_disk_read(USB, buff, sector, count);
		// translate the reslut code here

		return res;
	}
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
	DRESULT res;

	switch (drv) {
	case ATA :
//		result = ATA_disk_write(buff, sector, count);
		// translate the reslut code here

		return res;

	case MMC :
		res = SD_disk_write(MMC, buff, sector, count);
		// translate the reslut code here

		return res;

	case USB :
		res = USB_disk_write(USB, buff, sector, count);
		// translate the reslut code here

		return res;
	}
	return RES_PARERR;
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	switch (drv) {
	case ATA :
		// pre-process here

//		result = ATA_disk_ioctl(ctrl, buff);
		// post-process here

		return res;

	case MMC :
		// pre-process here

		res = SD_disk_ioctl(MMC, ctrl, buff);
		// post-process here

		return res;

	case USB :
		// pre-process here

		res = USB_disk_ioctl(USB, ctrl, buff);
		// post-process here

		return res;
	}
	return RES_PARERR;
}


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS SD_disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  SD_Error res = SD_OK;

  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 

  res =  SD_Init(); 

  if(res == SD_OK)
  {
    res = (SD_Error)0x0;
  }
  return ((DSTATUS)res);
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS SD_disk_status (
	BYTE drv		/* Physical drive nmuber (0) */
)
{
	if (drv) return STA_NOINIT;		/* Supports only single drive */
	return 0;
}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT SD_disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	        /* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{
        SD_Error status = SD_OK;

        SD_ReadMultiBlocks(buff, sector << 9, 512, 1);

        /* Check if the Transfer is finished */
        status =  SD_WaitReadOperation();
        while(SD_GetStatus() != SD_TRANSFER_OK);

        if (status == SD_OK)
        {
          return RES_OK;
        }
        else
        {
          return RES_ERROR;
        }
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
/* The FatFs module will issue multiple sector transfer request
/  (count > 1) to the disk I/O layer. The disk function should process
/  the multiple sector transfer properly Do. not translate it into
/  multiple single sector transfers to the media, or the data read/write
/  performance may be drasticaly decreased. */

#if _READONLY == 0
DRESULT SD_disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
        SD_Error status = SD_OK;

        SD_WriteMultiBlocks((BYTE *)buff, sector << 9, 512, 1);

        /* Check if the Transfer is finished */
        status = SD_WaitWriteOperation();
        while(SD_GetStatus() != SD_TRANSFER_OK);     

        if (status == SD_OK)
        {
          return RES_OK;
        }
        else
        {
          return RES_ERROR;
        }
}
#endif /* _READONLY */

/*-----------------------------------------------------------------------*/
/* Get current time                                                      */
/*-----------------------------------------------------------------------*/
/*
DWORD get_fattime ()
{
	return	((2006UL-1980) << 25)	      // Year = 2006
			| (2UL << 21)	      // Month = Feb
			| (9UL << 16)	      // Day = 9
			| (22U << 11)	      // Hour = 22
			| (30U << 5)	      // Min = 30
			| (0U >> 1)	      // Sec = 0
			;
}
*/

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT SD_disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
		DRESULT res = RES_OK;
		switch (ctrl) {
		

		case GET_SECTOR_COUNT :	  // Get number of sectors on the disk (DWORD)
			*(DWORD*)buff = 131072;	// 4*1024*32 = 131072
			res = RES_OK;
			break;

		case GET_SECTOR_SIZE :	  // Get R/W sector size (WORD) 
			*(WORD*)buff = 512;
			res = RES_OK;
			break;

		case GET_BLOCK_SIZE :	    // Get erase block size in unit of sector (DWORD)
			*(DWORD*)buff = 32;
			res = RES_OK;
		  }
	  
	return res;
}
						  
