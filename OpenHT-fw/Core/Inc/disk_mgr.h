/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DISK_MGR_H
#define __DISK_MGR_H

#ifdef __cplusplus
extern "C" {
#endif


#include "stm32469i_discovery_sd.h"

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"


void test_fat(void);


#ifdef __cplusplus
}
#endif

#endif /* __DISK_MGR_H */
