#include "qmlitemmodel.h"

//=============================================================================
CustomDevice::CustomDevice(const int &firstRole, const int &maxCount, const QList<QVariant> &list)
{
    this->firtsRole = firstRole;
    for(int i = firstRole, j = 0, jMax = list.size(); i < 0xffff && j < maxCount && j < jMax; i++, j++){
            hashIntVar.insert(i,list.at(j));
    }
}

//=============================================================================
QVariant CustomDevice::roleData(int role) const
{
    if(role < firtsRole)
        return QVariant();
    return hashIntVar.value(role, QVariant());
}
//=============================================================================
QHash<int, QVariant> CustomDevice::rolesData() const
{
    return hashIntVar;
}
//=============================================================================

QmlItemModel::QmlItemModel(QStringList listRoleName, int firstRole, QObject *parent) :
    QAbstractListModel(parent), firstRole(firstRole)
{

    maxCount = listRoleName.size();
    for(int j = firstRole, i = 0; i < maxCount; i++ )
        m_roleNames.insert(j++,listRoleName.at(i).toLocal8Bit());
}
//=============================================================================
void QmlItemModel::addCustomDevice(const QList<QVariant> &list)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_device.append(CustomDevice(firstRole,maxCount, list));
    endInsertRows();
}
//=============================================================================
void QmlItemModel::prependCustomDevice(const QList<QVariant> &list)
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_device.prepend(CustomDevice(firstRole,maxCount, list));
    endInsertRows();
}
//=============================================================================
int QmlItemModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_device.count();
}
//=============================================================================
QVariant QmlItemModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= m_device.count())
        return QVariant();

    const CustomDevice dev = m_device.at(index.row());
    return dev.roleData(role);
}
//=============================================================================
QVariant QmlItemModel::dataRow(const int &row, int role) const
{
    if(row < 0 || row >= m_device.count())
        return QVariant();

    const CustomDevice dev = m_device.at(row);
    return dev.roleData(role);

//    const VideoDevice &videoDev = m_device.at(row);

//    switch(role){
//    case IpRole: return videoDev.ipStr();
//    case TcpPortRole: return videoDev.tcpPort();
//    case UsrNameRole: return videoDev.usrName();
//    case PasswdRole: return videoDev.passwd();
//    case DeviceFirmwareRole: return videoDev.deviceFirmware();
//    }
//    return QVariant();

}
//=============================================================================
void QmlItemModel::removeThisRow(const int &row)
{
    if(row < 0 || row >= m_device.count())
        return;
    beginRemoveRows(QModelIndex(),row,row);
    m_device.removeAt(row);
    endRemoveRows();
}
//=============================================================================
void QmlItemModel::clearAllData()
{
//    qDebug() << "m_device size = " << m_device.size();
    beginRemoveRows(QModelIndex(), 0, rowCount());
    m_device.clear();
    endRemoveRows();
}
//=============================================================================
void QmlItemModel::replaceData(const int &row, const int &role, const QVariant &dataVar)
{
    if(row < 0 || row >= m_device.count())
        return;

    beginRemoveRows(QModelIndex(),row,row);
    CustomDevice dev = m_device.takeAt(row);
    endRemoveRows();

    QHash<int,QVariant> devHash = dev.rolesData();

    if(devHash.contains(role))
        devHash.insert(role,dataVar);

    QList<int> listKeys = devHash.keys();
    qSort(listKeys);

    QList<QVariant> listVar;

    while(!listKeys.isEmpty())
        listVar.append(devHash.value(listKeys.takeFirst()));

    beginInsertRows(QModelIndex(), row,row);
    m_device.insert(row, CustomDevice(firstRole,maxCount, listVar));
    endInsertRows();

}
//=============================================================================
QList<QVariant> QmlItemModel::dataRowInList(const int &row)
{
    QList<QVariant> retList;
    if(row < 0 || row >= m_device.count())
        return retList;

    for(int role = firstRole, i = 0; i < maxCount ; i++, role++){
        const CustomDevice dev = m_device.at(row);
        retList.append(dev.roleData(role));
    }

    return retList;
}
//=============================================================================
QHash<int, QByteArray> QmlItemModel::roleNames() const
{
//    QHash<int, QByteArray> roles;
//    roles[IpRole] = "deviceIp";
//    roles[TcpPortRole] = "devPort";
//    roles[UsrNameRole] = "usrName";
//    roles[PasswdRole] = "psswrd";
//    roles[DeviceFirmwareRole] = "deviceFirmware";
//    return roles;
    return m_roleNames;
}
//=============================================================================







