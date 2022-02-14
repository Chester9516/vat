#ifndef QTV_PROPERTY_H
#define QTV_PROPERTY_H

#include <QList>
#include <QVariant>

#include <core/qtv_define.h>

class QTV_DECL qtvProperty : public QObject
{
	Q_OBJECT
public:
	virtual qtv_property_type _type() = 0;
	virtual const QVariant& _value() = 0;
	virtual QString _name() = 0;
	virtual QString _descriptor() = 0;
	virtual QString _display_value() = 0;

	virtual bool checkSelectValid(const Pui_prop_val val) = 0;

	virtual vatek_result setValue(const QVariant& value) = 0;
};

class QTV_DECL qtvPropertyHandler
{
public:
	virtual void raisePropertiesChanged() = 0;
	virtual void raisePropertyEdited(qtvProperty* prop) = 0;
};

class QTV_DECL qtvPropertyCollection : public QObject
{
	Q_OBJECT
public:
	explicit qtvPropertyCollection();
	~qtvPropertyCollection();

	virtual bool _valid() { return m_valid; }
	virtual bool _is_advance() { return m_isadvance; }

	virtual const QList<qtvProperty*> _properties() { return m_properties; }
	virtual void resetProperties() = 0;
	virtual vatek_result setAdvance(bool isenable) { return vatek_unsupport; }

	static qtvPropertyCollection* createChipInformation(Pchip_info pinfo);
	static qtvPropertyCollection* createProperties(const Pui_prop_item props, uint8_t* raw, const Pui_prop_item advprops = nullptr, uint8_t* advraw = nullptr);

protected:
	QList<qtvProperty*> m_properties;
	bool m_valid;
	bool m_isadvance;

	virtual void setValid(bool bvalid);
	virtual void clearProperties();
	virtual void insertProperty(qtvProperty* prop);
	virtual void insertProperty(const Pui_prop_item uiprop, uint8_t* raw);
	virtual void insertProperties(const Pui_prop_item uiprops, uint8_t* raw);

	virtual void refreshProperties() = 0;
	virtual bool checkPropertiesValid() { return m_valid; }

Q_SIGNALS:
	void notifyPropertiesChanged();
	void notifyValidChanged(bool bvalid);
};

#endif