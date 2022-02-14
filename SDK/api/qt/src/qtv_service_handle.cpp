#include "internal/qinternal_core.h"

qi_servicehandle::qi_servicehandle(hvatek_chip hchip) :
	m_hchip(hchip),
	m_hthread(NULL),
	m_hlock(NULL),
	m_calibrationcount(0),
	m_updatetick(0),
	m_status(qstatus_idle),
	m_encalibration(0)
{
	Pchip_info pinfo = vatek_device_get_info(hchip);
	if (!is_vatek_success(cross_os_create_mutex(&m_hlock)))
		VERR("create mutex fail");

	if (pinfo->peripheral_en & PERIPHERAL_CALIBRATION)
	{
		m_encalibration |= QCALIBRATION_EN;
		if (pinfo->peripheral_en & PERIPHERAL_FINTEKR2)
			m_encalibration |= QCALIBRATION_R2;
	}
	memset(&m_calibration, 0, sizeof(calibration_param));
}

qi_servicehandle::~qi_servicehandle()
{
	stop();
	cross_os_free_mutex(m_hlock);
}

vatek_result qi_servicehandle::start()
{
	vatek_result nres = vatek_badstatus;
	if (m_status == qstatus_idle)
	{
		nres = vatek_device_calibration_load(m_hchip, &m_calibration);
		if(is_vatek_success(nres))
			nres = startService();
		if (is_vatek_success(nres))
		{
			nres = vatek_hwfail;
			m_status = qstatus_running;
			m_updatetick = 0;
			m_calibrationcount = 0;
			m_hthread = cross_os_create_thread(qi_servicehandle::service_handler, this);
			if (!m_hthread)m_status = qstatus_idle;
			else nres = vatek_success;
			if (!is_vatek_success(nres))stopService();
		}
	}
	return nres;
}

uint32_t qi_servicehandle::calibratiobCheck()
{
	return m_encalibration;
}

vatek_result qi_servicehandle::calibrationSet(Pcalibration_param pcalibration)
{
	vatek_result nres = vatek_badstatus;
	if (m_status == qstatus_running)
	{
		if (!m_encalibration)nres = vatek_unsupport;
		else
		{
			internalLock();
			m_calibrationcount++;
			memcpy(&m_calibration, pcalibration, sizeof(calibration_param));
			internalUnlock();
			nres = vatek_success;
		}
	}
	return nres;
}

vatek_result qi_servicehandle::calibrationGet(Pcalibration_param pcalibration)
{
	vatek_result nres = vatek_unsupport;
	if (m_encalibration)
	{
		memcpy(pcalibration, &m_calibration, sizeof(calibration_param));
		nres = vatek_success;
	}
	return nres;
}

vatek_result qi_servicehandle::calibrationSave()
{
	vatek_result nres = vatek_badstatus;
	if (!m_encalibration)nres = vatek_unsupport;
	else
	{
		if (m_calibrationcount)
		{
			internalLock();
			nres = vatek_device_calibration_save(m_hchip, &m_calibration);
			internalUnlock();
			m_calibrationcount = 0;
		}
		else nres = vatek_success;
	}
	return nres;
}

vatek_result qi_servicehandle::polling(Pbroadcast_info* pinfo)
{
	vatek_result nres = vatek_badstatus;
	if (m_status != qstatus_idle)
	{
		if (pinfo)*pinfo = getServiceInfo();
		nres = vatek_success;
	}
	return nres;
}

void qi_servicehandle::stop()
{
	if (m_status != qstatus_idle)
	{
		m_status = qstatus_stop;
		while (m_status == qstatus_stop)
			cross_os_sleep(1);
		stopService();
		cross_os_free_thread(m_hthread);
	}
	m_status = qstatus_idle;
}

void qi_servicehandle::internalLock()
{
	cross_os_lock_mutex(m_hlock);
}

void qi_servicehandle::internalUnlock()
{
	cross_os_release_mutex(m_hlock);
}

void qi_servicehandle::service_handler(Pcross_thread_param param)
{
	qi_servicehandle* core = (qi_servicehandle*)param->userparam;
	vatek_result nres = vatek_success;
	broadcast_status bcstatus = bcstatus_idle;
	Pbroadcast_info pinfo = core->getServiceInfo();
	uint32_t ncalibration = 0;

	while (core->m_status == qstatus_running)
	{
		if (is_vatek_success(nres))
		{
			nres = core->pollingService();
			if (is_vatek_success(nres))
			{
				uint32_t eclipse = cross_os_get_tick_ms() - core->m_updatetick;
				if (eclipse > 1000)
				{
					core->updateServiceTick();
					core->m_updatetick = cross_os_get_tick_ms();

					if (ncalibration != core->m_calibrationcount)
					{
						nres = vatek_device_calibration_apply(core->m_hchip, &core->m_calibration);
						if (is_vatek_success(nres))
							ncalibration = core->m_calibrationcount;
					}
				}
			}
			if (!is_vatek_success(nres))VWAR("service_handler fail : %d", nres);
		}
		else
		{
			if (pinfo->status != bcstatus_fail_unknown)
				pinfo->status = bcstatus_fail_unknown;
			cross_os_sleep(1);
		}
	}
	core->m_status = qstatus_idle;
	param->result = nres;
}

