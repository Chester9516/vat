
#ifndef _DEMOD_LGDT3305_
#define _DEMOD_LGDT3305_
    
#ifdef __cplusplus
    extern "C" {
#endif

    typedef void* Plgdt3305_handle;
    vatek_result lgdt3305_create(Pvatek_i2c vi2c, Plgdt3305_handle *hlgdt3305);
    vatek_result lgdt3305_destroy(Plgdt3305_handle hlgdt3305);
    vatek_result lgdt3305_i2cbypass(Plgdt3305_handle hlgdt3305, uint8_t bypasson);    
    vatek_result lgdt3305_lock(Plgdt3305_handle hlgdt3305, Pdemod_lockparm parm);
    vatek_result lgdt3305_getstatus(Plgdt3305_handle hlgdt3305, demod_status *status);
    vatek_result lgdt3305_getSNR(Plgdt3305_handle hlgdt3305, uint32_t *SNR);
    vatek_result lgdt3305_getPER(Plgdt3305_handle hlgdt3305, uint32_t *PER);

#ifdef __cplusplus
    }
#endif

#endif

