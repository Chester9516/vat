#ifndef QTV_SERVICE_USB_H
#define QTV_SERVICE_USB_H

#include <core/qtv_media.h>
#include <core/qtv_service.h>
#include <vatek_sdk_usbstream.h>
#include <vatek_sdk_usbmux.h>

typedef struct _qtv_usbmuxer_param
{
	uint16_t pcr_pid;
	uint16_t pcr_interval;
	uint16_t latency_ms;
	uint16_t recv;
}qtv_usbmuxer_param,*Pqtv_usbmuxer_param;

class QTV_DECL qtvServiceUSBStream : public qtvServiceBase
{
	Q_OBJECT
public:
	static vatek_result createServiceUSBStream(qtvDevice* device);
};

class qtvUSBMuxerProgram;
class qtvUSBMuxerChannel;

class QTV_DECL qtvUSBMuxerStream : public qtvMuxStream
{
	Q_OBJECT
public:
	virtual qtvCodec* _codec() = 0;
	virtual qtvUSBMuxerProgram* _program() = 0;
	virtual qtvPropertyCollection* _properties() = 0;
};

class QTV_DECL qtvUSBMuxerProgram : public qtvMuxProgram
{
	Q_OBJECT
public:
	virtual qtvUSBMuxerChannel* _channel() = 0;

	virtual void clear() = 0;
	virtual vatek_result createStream(uint16_t pid, qtvCodec* codec, qtvUSBMuxerStream** stream) = 0;
	virtual vatek_result getStream(uint16_t pid, qtvUSBMuxerStream** stream) = 0;
	virtual vatek_result commitStream(qtvUSBMuxerStream* stream) = 0;
};

class QTV_DECL qtvUSBMuxerChannel : public qtvMuxChannel
{
	Q_OBJECT
public:
	virtual void clear() = 0;
	virtual vatek_result createProgram(uint16_t pmtpid, qtvUSBMuxerProgram** program) = 0;
	virtual vatek_result getProgram(uint16_t pmtpid, qtvUSBMuxerProgram** program) = 0;
	virtual vatek_result commitProgram(qtvUSBMuxerProgram* program) = 0;
};

typedef void* hqtv_program;

class QTV_DECL qtvUSBMuxSource : public QObject
{
	Q_OBJECT
public:
	virtual Pusbmux_source getHandle() = 0;
	virtual vatek_result startProgram(qtvUSBMuxerProgram* program, Pusbmux_program uprogram, hqtv_program* hprogram) = 0;
	virtual vatek_result pushStream(hqtv_program hprogram, qtvUSBMuxerStream* stream, Pusbmux_stream ustream) = 0;
	virtual void stopProgram(hqtv_program hprogram) = 0;
};

class QTV_DECL qtvServiceUSBMuxer : public qtvServiceBase
{
	Q_OBJECT
public:
	static vatek_result createService(qtvDevice* device, qtvServiceUSBMuxer** service);
	~qtvServiceUSBMuxer() {}

	virtual Pqtv_usbmuxer_param _mux_param() = 0;
	virtual qtvPropertyCollection* _mux_properties() = 0;

	virtual void resetChannel() = 0;
	virtual vatek_result createChannel(mux_spec_mode spec, mux_country_code country, qtvUSBMuxerChannel** channel) = 0;
	virtual vatek_result getChannel(qtvUSBMuxerChannel** channel) = 0;
	virtual vatek_result commitChannel(qtvUSBMuxerChannel* channel) = 0;

	virtual void resetOutput(uint32_t freqkhz, uint32_t bw_sb, modulator_type type) = 0;

	virtual vatek_result startBroadcast(qtvUSBMuxSource* source) = 0;
	virtual vatek_result sendFrame(Pusbmux_stream hstream,Pmux_pes_frame pframe) = 0;
	virtual vatek_result stopBroadcast() = 0;

protected:
	explicit qtvServiceUSBMuxer(qtv_service_mode mode, qtvDevice* device) : qtvServiceBase(mode, device) {}
};

#endif
