#include "./internal/qinternal_core.h"
#include <vatek_sdk_device.h>
#include <core/ui/ui_props/ui_props_chip.h>

qi_bridge::qi_bridge(hvatek_bridge hbridge) :
	m_hbridge(hbridge),
	m_binfo(vatek_bridge_get_info(hbridge))
{
	int32_t i = 0;
	vatek_result nres = vatek_success;
	for (i = 0; i < m_binfo->source_nums; i++)
	{
		Pbridge_source newsource = (Pbridge_source)malloc(sizeof(bridge_source));
		nres = vatek_bridge_get_av_source(hbridge, i, newsource);
		if (is_vatek_success(nres))m_sources.append(newsource);
		else free(newsource);

		if (!is_vatek_success(nres))break;
	}

	if (!is_vatek_success(nres))VWAR("bridge enum source fail : %d", nres);
}

vatek_result qi_bridge::checkSource(Pbridge_source psource)
{
	return vatek_bridge_get_av_source_status(m_hbridge, psource);
}

vatek_result qi_bridge::startSource(Pbridge_source psource)
{
	return vatek_bridge_start_av_source(m_hbridge, psource);
}

vatek_result qi_bridge::stopSource(Pbridge_source psource)
{
	return vatek_bridge_stop_av_source(m_hbridge);
}

vatek_result qi_bridge::updateSource(int32_t idx, Pbridge_source psource)
{
	return vatek_bridge_get_av_source(m_hbridge, idx, psource);
}

qi_bridge::~qi_bridge()
{
	vatek_bridge_close(m_hbridge);

	while (m_sources.count())
	{
		free(m_sources.first());
		m_sources.removeFirst();
	}
}

/* qi_device */

qi_device::qi_device(hvatek_chip hchip, qi_bridge* bridge) :
	m_hchip(hchip),
	m_bridge(bridge),
	m_name(vatek_device_get_name(hchip))
{

}

qi_device::~qi_device()
{
	if (m_bridge)delete m_bridge;
	if (m_hchip)vatek_device_close(m_hchip);
}

vatek_result qi_device::createDevice(hvatek_chip hchip, qi_device** device)
{
	vatek_result nres = vatek_success;
	if (vatek_device_get_bus(hchip) == DEVICE_BUS_BRIDGE)
	{
		hvatek_bridge hbridge = NULL;
		nres = vatek_bridge_open(hchip, &hbridge);
		if (is_vatek_success(nres))
			*device = new qi_device(hchip, new qi_bridge(hbridge));
	}
	else *device = new qi_device(hchip, NULL);
	return nres;
}

vatek_result qi_device::rebootDevice(qi_device* pdevice)
{
	vatek_result nres = vatek_device_close_reboot(pdevice->m_hchip);
	pdevice->m_hchip = NULL;
	delete pdevice;
	return nres;
}

/* qi_devicemanage */

qi_devicemanage& qi_devicemanage::getInstance()
{
	static qi_devicemanage oneInstance;
	return oneInstance;
}

qi_devicemanage::qi_devicemanage() :
	m_hdevices(NULL)
{

}

qi_devicemanage::~qi_devicemanage()
{
	resetDevices();
}

void qi_devicemanage::resetDevices()
{
	if (m_hdevices)
	{
		while (m_devices.count())
		{
			delete m_devices.first();
			m_devices.removeFirst();
		}
		vatek_device_list_free(m_hdevices);
	}
	m_hdevices = NULL;
}

vatek_result qi_devicemanage::enumDevices(uint32_t bus, hal_service_mode mode)
{
	vatek_result nres = vatek_success;
	resetDevices();
	nres = vatek_device_list_enum(bus, mode, &m_hdevices);
	if (nres > vatek_success)
	{
		int32_t i = 0;
		int32_t nums = nres;
		for (i = 0; i < nums; i++)
		{
			hvatek_chip hchip = NULL;
			nres = vatek_device_open(m_hdevices, i, &hchip);
			if (is_vatek_success(nres))
			{
				qi_device* device = NULL;
				nres = qi_device::createDevice(hchip, &device);
				if (!is_vatek_success(nres))vatek_device_close(hchip);
				else m_devices.append(device);
			}
		}
		nres = (vatek_result)m_devices.count();
	}
	return nres;
}

vatek_result qi_devicemanage::openDevice(int32_t idx, qtvDevice** device)
{
	vatek_result nres = vatek_badparam;
	if (idx < m_devices.count())
	{
		qi_device* pdevice = static_cast<qi_device*>(m_devices.at(idx));
		*device = pdevice;
		nres = vatek_success;
	}
	return nres;
}

void qi_devicemanage::closeDevice(qtvDevice* device, int32_t isreboot)
{
	qi_device* pdevice = static_cast<qi_device*>(device);
	for (int32_t i = 0; i < m_devices.count(); i++)
	{
		if (m_devices.at(i) == device)
		{
			if (isreboot)qi_device::rebootDevice(pdevice);
			m_devices.removeAt(i);
			break;
		}
	}
}

/* qtvDevice */

const char* qtvDevice::getServiceName(qtvDevice* device)
{
	return ui_enum_get_str(hal_service_mode, device->_chip_info()->hal_service);
}

/* qtvDeviceMange */

vatek_result qtvDeviceManage::enumDevices(uint32_t bus, hal_service_mode mode)
{
	return qi_devicemanage::getInstance().enumDevices(bus, mode);
}

vatek_result qtvDeviceManage::openDevice(int32_t idx, qtvDevice** device)
{
	return qi_devicemanage::getInstance().openDevice(idx, device);
}

void qtvDeviceManage::closeDevice(qtvDevice* device, int32_t isreboot)
{
	qi_devicemanage::getInstance().closeDevice(device,isreboot);
}
