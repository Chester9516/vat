#include <core/base/device_usb.h>
#include <core/tools/tool_bufstream.h>

const char usbbulk_raw_tag[USBBULK_TAG_LEN] = { 'V','A','T','E','K','-','2','\0', };

int32_t usbbulk_buffer_check(uint8_t* pbuf)
{
	return !strcmp((const char*)&pbuf[0], &usbbulk_raw_tag[0]);
}

vatek_result usbbulk_command_set(Pusbbulk_command pcmd, uint8_t* pbuf)
{
	vatek_result nres = vatek_success;
	if ((pcmd->mode & USBBULK_CMD_MASK) == USBBULK_CMD_BASE)
	{
		bufstream bs = { USBBULK_TAG_LEN, pbuf };
		memcpy(&pbuf[0], &usbbulk_raw_tag[0], USBBULK_TAG_LEN);
		putbuf_uint32_t(&bs, (uint32_t)pcmd->mode);
		if (pcmd->mode == usbbulk_flash)
		{
			putbuf_uint32_t(&bs, (uint32_t)pcmd->_h.flash.mode);
			putbuf_uint32_t(&bs, pcmd->_h.flash.section);
		}
		else if (pcmd->mode == usbbulk_aux)
		{
			putbuf_uint32_t(&bs, (uint32_t)pcmd->_h.aux.mode);
			putbuf_uint32_t(&bs, pcmd->_h.aux.length);
			putbuf_uint32_t(&bs, pcmd->_h.aux.bitrate);
		}
		else nres = vatek_badparam;
	}
	else nres = vatek_badparam;
	return nres;
}

vatek_result usbbulk_command_get(Pusbbulk_command pcmd, uint8_t* pbuf)
{
	vatek_result nres = vatek_format;
	if (usbbulk_buffer_check(pbuf))
	{
		bufstream bs = { USBBULK_TAG_LEN,pbuf };
		uint32_t mode = getbuf_uint32_t(&bs);
		if ((mode & USBBULK_CMD_MASK) == USBBULK_CMD_BASE)
		{
			nres = vatek_success;
			pcmd->mode = (usbbulk_mode)mode;
			if (pcmd->mode == usbbulk_flash)
			{
				pcmd->_h.flash.mode = (usbflash_mode)getbuf_uint32_t(&bs);
				pcmd->_h.flash.section = getbuf_uint32_t(&bs);
			}
			else if (pcmd->mode == usbbulk_aux)
			{
				pcmd->_h.aux.mode = (usbaux_mode)getbuf_uint32_t(&bs);
				pcmd->_h.aux.length = getbuf_uint32_t(&bs);
				pcmd->_h.aux.bitrate = getbuf_uint32_t(&bs);
			}
			else nres = vatek_badparam;
		}
		else nres = vatek_badparam;
	}
	return nres;
}

vatek_result usbbulk_result_set(Pusbbulk_result presult, uint8_t* pbuf)
{
	vatek_result nres = vatek_success;
	if ((presult->mode & USBBULK_CMD_MASK) == USBBULK_CMD_BASE)
	{
		bufstream bs = { USBBULK_TAG_LEN,pbuf };
		memcpy(&pbuf[0], &usbbulk_raw_tag[0], USBBULK_TAG_LEN);
		putbuf_uint32_t(&bs, (uint32_t)presult->mode);
		putbuf_uint32_t(&bs, (uint32_t)presult->result);
		putbuf_uint32_t(&bs, presult->len);
		putbuf_uint32_t(&bs, presult->position);
	}
	else nres = vatek_badparam;
	return nres;
}

vatek_result usbbulk_result_get(Pusbbulk_result presult, uint8_t* pbuf)
{
	vatek_result nres = vatek_format;
	if (usbbulk_buffer_check(pbuf))
	{
		bufstream bs = { USBBULK_TAG_LEN,pbuf };
		uint32_t mode = getbuf_uint32_t(&bs);
		if ((mode & USBBULK_CMD_MASK) == USBBULK_CMD_BASE)
		{
			presult->mode = (usbbulk_mode)mode;
			presult->result = (vatek_result)getbuf_uint32_t(&bs);
			presult->len = getbuf_uint32_t(&bs);
			presult->position = getbuf_uint32_t(&bs);
			nres = vatek_success;
		}
		else nres = vatek_badparam;
	}
	return nres;
}
