#include "Roster.h"
#include "../../MainWindow.h"

CRoster::CRoster(QObject *parent) :
    QObject(parent)
{
    Init((MainWindow*)parent);
}

CRoster::CRoster(QString jid, QSet<QString> groups, MainWindow *parent) : QObject((QObject*)parent)
{
    Init(parent);
    SetJid(jid);
    SetGroups(groups);
}

CRoster::~CRoster()
{
    qDebug("CRoster::~CRoster");
    std::list<QStandardItem*>::iterator it;
    for(it = m_lstUserListItem.begin(); it != m_lstUserListItem.end(); it++)
    {
        QStandardItem* p = *it;
        if(p->parent())
        {
            p->parent()->removeRow(p->row()); //控件会自己释放 QStandardItem 内存
        }
    }
}

int CRoster::Init(MainWindow *parent)
{
    m_nNewMessageNumber = 0;
    m_pMainWindow = parent;
    m_Message.SetRoster(this, m_pMainWindow);
    //TODO:
    m_Video.SetClient(m_pMainWindow->m_pClient);
    return 0;
}

QString CRoster::Name()
{
    return QXmppUtils::jidToUser(m_szJid);
}

QString CRoster::BareJid()
{
    return QXmppUtils::jidToBareJid(m_szJid);
}

QString CRoster::Domain()
{
    return QXmppUtils::jidToDomain(m_szJid);
}

QString CRoster::Resouce()
{
    return QXmppUtils::jidToResource(m_szJid);
}

int CRoster::SetJid(QString jid)
{
    m_szJid = jid;
    return 0;
}

QSet<QString> CRoster::Groups()
{
    return m_Groups;
}

int CRoster::SetGroups(const QSet<QString> &groups)
{
    m_Groups = groups;
}

QList<QStandardItem*> CRoster::GetItem()
{
    //呢称条目
    QStandardItem* pItem = new QStandardItem(Name());
    QVariant v;
    v.setValue(this);
    pItem->setData(v);
    pItem->setEditable(true);//允许双击编辑
    m_lstUserListItem.push_back(pItem);

    //消息条目
    QStandardItem* pMessageCountItem = new QStandardItem("0");
    v.setValue(this);
    pMessageCountItem->setData(v);
    pMessageCountItem->setEditable(false);//禁止双击编辑

    //TODO:未读新消息数目树形控件中未显示
    QList<QStandardItem *> lstItems;
    lstItems.push_back(pItem);
    lstItems.push_back(pMessageCountItem);

    return lstItems;
}

QString CRoster::GetStatusText(QXmppPresence::Status status)
{
    if(QXmppPresence::Status::Online == status.type())
        return tr("OnLine");
    else if(QXmppPresence::Status::Away == status.type())
        return tr("Temporarily away");
    else if(QXmppPresence::Status::Chat == status.type())
        return tr("Chat");
    else if(QXmppPresence::Status::DND == status.type())
        return tr("Do not disturb");
    else if(QXmppPresence::Status::Invisible == status.type())
        return tr("Invisible");
    else if(QXmppPresence::Status::XA == status.type())
        return tr("Away for an extended period");
}

QColor CRoster::GetStatusColor(QXmppPresence::Status status)
{
    if(QXmppPresence::Status::Online == status.type())
        return QColor(0, 255, 0);
    else if(QXmppPresence::Status::Away == status.type())
        return QColor(255, 0, 255);
    else if(QXmppPresence::Status::Chat == status.type())
        return QColor(0, 255, 0);
    else if(QXmppPresence::Status::DND == status.type())
        return QColor(255, 0, 0);
    else if(QXmppPresence::Status::Invisible == status.type())
        return QColor(255, 255, 255);
    else if(QXmppPresence::Status::XA == status.type())
        return QColor(255, 0, 255);
}

//TODO:修改成图片表示
int CRoster::ChangedPresence(QXmppPresence::Status::Type status)
{
    std::list<QStandardItem*>::iterator it;
    for(it = m_lstUserListItem.begin(); it != m_lstUserListItem.end(); it++)
    {
        QStandardItem* p = *it;
        p->setData(GetStatusColor(status), Qt::BackgroundRole);
    }
    return 0;
}

int CRoster::ShowMessageDialog()
{
    int nRet = 0;

    m_Message.show();
    m_Message.activateWindow();//激活窗口，置顶

    return nRet;
}

int CRoster::AppendMessage(const QString &szMessage)
{
    m_nNewMessageNumber++;
    //TODO:设置控件计数

    return m_Message.AppendMessage(szMessage);
}

int CRoster::CleanNewMessageNumber()
{
    m_nNewMessageNumber = 0;
    //TODO:清除控件计数

    return 0;
}
