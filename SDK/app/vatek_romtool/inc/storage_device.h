#ifndef ROM_DEVICE_H
#define ROM_DEVICE_H

#include <QObject>
#include <QThread>
#include <vatek_sdk_device.h>
#include <vatek_sdk_storage.h>

class storageDevice : public QObject
{
	Q_OBJECT
public:

	static storageDevice* openStorageImage(QString& szimg);
	static storageDevice* openStorageDevice(int32_t isenV0);
	static void closeStorageDevice(storageDevice* pdevice);

	vatek_result openStorage();
	vatek_result writeImage(Promfile_handle promfile);
	void closeStorage();
	QString& _information() { return m_szinfo; }
	int32_t _position() { return m_position; }
	hvatek_storage _hstorage() { return m_hstorage; }

Q_SIGNALS:
	void notifyStatusChanged();

protected:
	explicit storageDevice();
	~storageDevice();

	hvatek_storage m_hstorage;
	Pstorage_handle m_hshandle;
	int32_t m_position;
	QString m_szinfo;

	void updateStatus();

	static void storageHandler(rom_progress_msg msg, void* progressval, void* param);
};

#endif
