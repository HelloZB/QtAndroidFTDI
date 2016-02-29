#ifndef CUSTOMPROXY4QMLMODEL_H
#define CUSTOMPROXY4QMLMODEL_H

#include <QSortFilterProxyModel>
#include <QString>
#include "qmlitemmodel.h"

class CustomProxy4QmlModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CustomProxy4QmlModel(const QList<int> &sortCols, QObject *parent = 0);

signals:

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

public slots:
    void setNewFileterStr(QString str);

private:
    QList<int> sortCols;

};

#endif // CUSTOMPROXY4QMLMODEL_H
