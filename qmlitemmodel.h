#ifndef QMLITEMMODEL_H
#define QMLITEMMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QtCore>

class CustomDevice
{
public:
    CustomDevice(const int &firstRole, const int &maxCount, const QList<QVariant> &list);

    QVariant roleData(int role) const;

    QHash<int,QVariant> rolesData() const;

private:    
    QHash<int,QVariant> hashIntVar;
    int firtsRole;
};

class QmlItemModel : public QAbstractListModel
{
    Q_OBJECT
public:


    QmlItemModel(QStringList listRoleName, int firstRole = Qt::UserRole + 1, QObject *parent = 0);

    void addCustomDevice(const QList<QVariant> &list);

    void prependCustomDevice(const QList<QVariant> &list);


    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant dataRow(const int &row, int role = Qt::DisplayRole) const;

    void removeThisRow(const int &row);

    void clearAllData();

    void replaceData(const int &row, const int &role , const QVariant &dataVar);

    QList<QVariant> dataRowInList(const int &row);


protected:
    QHash<int,QByteArray> roleNames() const;

private:
    QList<CustomDevice> m_device;
    QHash<int,QByteArray> m_roleNames;
    int firstRole;
    int maxCount;

};

#endif // QMLITEMMODEL_H
