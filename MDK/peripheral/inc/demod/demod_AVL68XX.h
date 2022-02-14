
#ifndef _DEMOD_AVL68XX_
#define _DEMOD_AVL68XX_
    
#ifdef __cplusplus
    extern "C" {
#endif

    typedef void* Pavl68xx_handle;
    vatek_result avl68xx_create(Pvatek_i2c vi2c, Pavl68xx_handle *havl68xx);
    vatek_result avl68xx_destroy(Pavl68xx_handle havl68xx);
    vatek_result avl68xx_i2cbypass(Pavl68xx_handle havl68xx, uint8_t bypasson);
    vatek_result avl68xx_lock(Pavl68xx_handle havl68xx, Pdemod_lockparm parm);
    vatek_result avl68xx_getstatus(Pavl68xx_handle havl68xx, demod_status *status);
    vatek_result avl68xx_getSQI(Pavl68xx_handle havl68xx, uint32_t *SQI);
    vatek_result avl68xx_getSNR(Pavl68xx_handle havl68xx, uint32_t *SNR);
    vatek_result avl68xx_getPER(Pavl68xx_handle havl68xx, uint32_t *PER);
    
#ifdef __cplusplus
    }
#endif

#endif

