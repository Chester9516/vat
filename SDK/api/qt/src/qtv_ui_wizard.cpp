#include <ui/base/qtv_ui_wizard.h>
#include <QAbstractButton>
#include <QShowEvent>
#include <QHideEvent>

/* qtvUIWizardPage */

qtvUIWizardPage::qtvUIWizardPage(qtvUIMain* main, uint32_t id, QWidget* parent) :
	m_main(main),
	m_tab(NULL),
	m_id(id),
	QWizardPage(parent)
{
	if (id == BCWIZARD_ID_LOCALE)m_tab = new qtvUITabDtv(main, this);
	else if (id == BCWIZARD_ID_SOURCE)m_tab = new qtvUITabSource(main, this);
	else if (id == BCWIZARD_ID_SOURCEVALID)m_tab = new qtvUITabEnum(main, this);
	else if (id == BCWIZARD_ID_CONTEXT)m_tab = new qtvUITabContext(main, this);
	else if (id == BCWIZARD_ID_MODULATION)m_tab = new qtvUITabProperties(main, qui_tab_properties_modulation, this);
	else if (id == BCWIZARD_ID_MUXPARAM)m_tab = new qtvUITabProperties(main, qui_tab_properties_muxparam, this);
	else if (id == BCWIZARD_ID_VIDEOCODEC)m_tab = new qtvUITabProperties(main, qui_tab_properties_videocodec, this);
	else if (id == BCWIZARD_ID_AUDIOCODEC)m_tab = new qtvUITabProperties(main, qui_tab_properties_audiocodec, this);
	else if (id == BCWIZARD_ID_NODEVICE)m_tab = new qtvUITabNoDevice(main, this);
	if (m_tab)setTab(m_tab);
}

qtvUIWizardPage::~qtvUIWizardPage()
{
	if (m_tab)
	{
		disconnect(m_tab, SIGNAL(notifyValidChanged(bool)), this, SLOT(recvValidChanged(bool)));
		delete m_tab;
	}
}

void qtvUIWizardPage::setTab(qtvUITabBase* tab)
{
	m_tab = tab;
	if (m_tab)
	{
		setTitle(m_tab->_name());
		setSubTitle(m_tab->_descriptor());
		setLayout(m_tab->layout());
		connect(m_tab, SIGNAL(notifyValidChanged(bool)), this, SLOT(recvValidChanged(bool)));
	}
}

void qtvUIWizardPage::setWizardButtonEnable(QWizard::WizardButton button, bool isenable)
{
	wizard()->button(button)->setEnabled(isenable);
}

void qtvUIWizardPage::initializePage()
{
	
}

void qtvUIWizardPage::cleanupPage()
{
	
}

bool qtvUIWizardPage::isComplete() const
{
	return m_tab->_valid();
}

bool qtvUIWizardPage::validatePage()
{
	bool bok = m_tab->_valid();
	if (bok)m_tab->tabFinish();
	return bok;
}

void qtvUIWizardPage::tabEnter()
{
	m_tab->tabEnter();
}

void qtvUIWizardPage::tabLeave()
{
	m_tab->tabLeave();
}

void qtvUIWizardPage::recvValidChanged(bool bvalid)
{
	emit completeChanged();
}

/* qtvUIWizardBase */

qtvUIWizardBase::qtvUIWizardBase(qtvUIMain* main, QWidget* parent) :
	m_result(vatek_badstatus),
	m_currentpage(NULL),
	m_isdone(0),
	m_main(main),
	QWizard(parent)
{
	setWizardStyle(QWizard::ClassicStyle);
	setFixedSize(600, 400);
	setWindowFlags((windowFlags() & ~Qt::WindowContextHelpButtonHint));
	setWindowFlags(windowFlags() | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
	setOption(WizardOption::IndependentPages, false);
	connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(recvPageChanged(int)));
}

qtvUIWizardBase::~qtvUIWizardBase()
{
	if (m_currentpage)m_currentpage->tabLeave();
	m_currentpage = NULL;
	disconnect(this, SIGNAL(currentIdChanged(int)), this, SLOT(recvPageChanged(int)));
}

void qtvUIWizardBase::done(int result)
{
	QWizard::done(result);
	if (result == QDialog::Accepted)
		m_result = vatek_success;
	else if (result == QDialog::Rejected)
		m_result = vatek_badparam;
	else m_result = vatek_unsupport;
	m_isdone = 1;
}

void qtvUIWizardBase::pushPage(uint32_t id)
{
	qtvUIWizardPage* page = new qtvUIWizardPage(m_main,id,this);
	m_pages.append(page);
	setPage(id, page);
}

qtvUIWizardPage* qtvUIWizardBase::getPage(uint32_t id)
{
	for (int32_t i = 0; i < m_pages.count(); i++)
	{
		if (m_pages.at(i)->_id() == id)
			return m_pages.at(i);
	}
	return NULL;
}

void qtvUIWizardBase::recvPageChanged(int id)
{
	if (m_currentpage)m_currentpage->tabLeave();
	m_currentpage = getPage(id);
	if (m_currentpage)m_currentpage->tabEnter();
}
