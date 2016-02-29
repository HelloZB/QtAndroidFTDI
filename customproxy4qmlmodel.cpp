#include "customproxy4qmlmodel.h"
//========================================================================================
CustomProxy4QmlModel::CustomProxy4QmlModel(const QList<int> &sortCols, QObject *parent) :
    QSortFilterProxyModel(parent), sortCols(sortCols)
{

}

//========================================================================================
bool CustomProxy4QmlModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{

    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    bool retVal = false;
    if(sortCols.isEmpty()){
        retVal = true;
    }else{
        for( int i = 0, iMax = sortCols.size(); i < iMax && !retVal; i++)
            retVal = index.data(sortCols.at(i)).toString().contains(filterRegExp());

    }

    return retVal;
}
//========================================================================================
void CustomProxy4QmlModel::setNewFileterStr(QString str)
{
    if(str.contains("'")){
        str.replace("'", " ");
    }

    QRegExp regExp(str, Qt::CaseInsensitive, QRegExp::RegExp);
    this->setFilterRegExp(regExp);
}
//========================================================================================
