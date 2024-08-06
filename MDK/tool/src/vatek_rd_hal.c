/*pull high pin E1 to use this function to read/write HAL register*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vatek_hms.h"
#include "vatek_api.h"
#include "vatek_broadcast_api.h"
#include "phy/phy_h1.h"

extern Ph1_handle vatek_get_h1_i2c(void);
extern vatek_result h1_write_reg(Ph1_handle hh1, uint32_t addr, uint8_t val);
extern vatek_result h1_read_reg(Ph1_handle hh1, uint32_t addr, uint8_t* val);

vatek_result vatek_all_rd_hal()
{
	vatek_result result = vatek_result_unknown;
	uint32_t reg_r_val = 0, reg_w_val = 0, reg_addr = 0;
	uint8_t h_r_val = 0, h_w_val = 0;
	char chip_series[1], rd_cmd[1];
	Phbroadcast bc_handle = vatek_get_main_i2c();
	Ph1_handle h1_handle = vatek_get_h1_i2c();
#if defined(VATEK_B2) || defined(VATEK_B3) || defined(VATEK_B3_PLUS) || defined(PHY_H1)
	printf("input chip series b/h\r\n");
	scanf("%c",chip_series);
	if((chip_series[0] == 'b') || (chip_series[0] == 'B')){
		printf("read or write? w/r\r\n");
		scanf("%c",rd_cmd);
		printf("%s\r\n",rd_cmd);
		if((rd_cmd[0] == 'r') || (rd_cmd[0] == 'R')){
			printf("input read hal reg address\r\n");
			scanf("%x",&reg_addr);
			printf("0x%x\r\n",reg_addr);
			result = vatek_broadcast_hal_read(bc_handle, reg_addr, &reg_r_val);
			if(result != vatek_result_success)
				printf("read hal fail, %d\r\n",result);
			else
				printf("read hal reg 0x%x = %d\r\n",reg_addr, reg_r_val);
		}
		if((rd_cmd[0] == 'w') || (rd_cmd[0] == 'W')){
			printf("input write hal reg address\r\n");
			scanf("%x",&reg_addr);
			printf("0x%x\r\n",reg_addr);
			printf("input value for write in hal reg\r\n");
			scanf("%d",&reg_w_val);
			result = vatek_broadcast_hal_write(bc_handle, reg_addr, reg_w_val);
			if(result == vatek_result_success)
				result = vatek_broadcast_hal_read(bc_handle, reg_addr, &reg_r_val);
			if(result != vatek_result_success)
				printf("write hal fail, %d\r\n",result);
			else
				printf("write hal reg 0x%x = %d\r\n",reg_addr, reg_r_val);
		}
	}
	else if((chip_series[0] == 'h') || (chip_series[0] == 'H')){
		printf("read or write? w/r\r\n");
		scanf("%c",rd_cmd);
		printf("%s\r\n",rd_cmd);
		if((rd_cmd[0] == 'r') || (rd_cmd[0] == 'R')){
			printf("input read hal reg address\r\n");
			scanf("%x",&reg_addr);
			printf("0x%x\r\n",reg_addr);
			result = h1_read_reg(h1_handle, reg_addr, &h_r_val);
			if(result != vatek_result_success)
				printf("read hal fail, %d\r\n",result);
			else
				printf("read h1 hal reg 0x%x = %d\r\n",reg_addr, h_r_val);
		}
		if((rd_cmd[0] == 'w') || (rd_cmd[0] == 'W')){
			printf("input write hal reg address\r\n");
			scanf("%x",&reg_addr);
			printf("0x%x\r\n",reg_addr);
			printf("input value for write in hal reg\r\n");
			scanf("%d",&reg_w_val);
			result = h1_write_reg(h1_handle, reg_addr, h_w_val);
			if(result == vatek_result_success)
				result = h1_read_reg(h1_handle, reg_addr, &h_r_val);
			if(result != vatek_result_success)
				printf("write h1 hal fail, %d\r\n",result);
			else
				printf("write h1 hal reg 0x%x = %d\r\n",reg_addr, h_r_val);
		}
	}
#elif defined(VATEK_A3)
	printf("read or write? w/r\r\n");
	scanf("%c",rd_cmd);
	printf("%s\r\n",rd_cmd);
	if((rd_cmd[0] == 'r') || (rd_cmd[0] == 'R')){
		printf("input read hal reg address\r\n");
		scanf("%x",&reg_addr);
		printf("0x%x\r\n",reg_addr);
		result = vatek_broadcast_hal_read(bc_handle, reg_addr, &reg_r_val);
		if(result != vatek_result_success)
			printf("read hal fail, %d\r\n",result);
		else
			printf("read hal reg 0x%x = %d\r\n",reg_addr, reg_r_val);
	}
	if((rd_cmd[0] == 'w') || (rd_cmd[0] == 'W')){
		printf("input write hal reg address\r\n");
		scanf("%x",&reg_addr);
		printf("0x%x\r\n",reg_addr);
		printf("input value for write in hal reg\r\n");
		scanf("%d",&reg_w_val);
		result = vatek_broadcast_hal_write(bc_handle, reg_addr, reg_w_val);
		if(result == vatek_result_success)
			result = vatek_broadcast_hal_read(bc_handle, reg_addr, &reg_r_val);
		if(result != vatek_result_success)
			printf("write hal fail, %d\r\n",result);
		else
			printf("write hal reg 0x%x = %d\r\n",reg_addr, reg_r_val);
	}
#elif defined(VATEK_V1)
	printf("read or write? w/r\r\n");
	scanf("%c",rd_cmd);
	printf("%s\r\n",rd_cmd);
	if((rd_cmd[0] == 'r') || (rd_cmd[0] == 'R')){
		printf("input read hal reg address\r\n");
		scanf("%x",&reg_addr);
		printf("0x%x\r\n",reg_addr);
		result = vatek_encoder_hal_read_v1(bc_handle, reg_addr, &reg_r_val);
		if(result != vatek_result_success)
			printf("read hal fail, %d\r\n",result);
		else
			printf("read hal reg 0x%x = %d\r\n",reg_addr, reg_r_val);
	}
	if((rd_cmd[0] == 'w') || (rd_cmd[0] == 'W')){
		printf("input write hal reg address\r\n");
		scanf("%x",&reg_addr);
		printf("0x%x\r\n",reg_addr);
		printf("input value for write in hal reg\r\n");
		scanf("%d",&reg_w_val);
		result = vatek_encoder_hal_write_v1(bc_handle, reg_addr, reg_w_val);
		if(result == vatek_result_success)
			result = vatek_encoder_hal_read_v1(bc_handle, reg_addr, &reg_r_val);
		if(result != vatek_result_success)
			printf("write hal fail, %d\r\n",result);
		else
			printf("write hal reg 0x%x = %d\r\n",reg_addr, reg_r_val);
	}
#endif	
	
}

