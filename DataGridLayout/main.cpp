#include "itemsmodel.h"

#include <QDateTime>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <Controls/datagrid.h>
#include <Controls/datagridcolumn.h>
#include <Controls/datagridheaderitempresenter.h>
#include <Controls/datagridheaderpresenter.h>
#include <Controls/datagriditemlayout.h>
#include <Controls/datagridrowitempresenter.h>
#include <Controls/datagridrowpresenter.h>
#include <Controls/observableobjecteventargs.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<DataGrid>("com.mapal.unibase", 1, 0, "DataGrid");
    qmlRegisterType<DataGridColumn>("com.mapal.unibase", 1, 0, "DataGridColumn");
    qmlRegisterType<DataGridHeaderItemPresenter>("com.mapal.unibase", 1, 0, "DataGridHeaderItemPresenter");
    qmlRegisterType<DataGridHeaderPresenter>("com.mapal.unibase", 1, 0, "DataGridHeaderPresenter");
    qmlRegisterType<DataGridItemLayout>("com.mapal.unibase", 1, 0, "DataGridItemLayout");
    qmlRegisterType<DataGridRowItemPresenter>("com.mapal.unibase", 1, 0, "DataGridRowItemPresenter");
    qmlRegisterType<DataGridRowPresenter>("com.mapal.unibase", 1, 0, "DataGridRowPresenter");
    qmlRegisterType<ObservableObjectEventArgs>("com.mapal.unibase", 1, 0, "ObservableObjectEventArgs");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QObject *root = engine.rootObjects().first();
    DataGrid *dataGrid = root->findChild<DataGrid*>("datagrid");

    if (dataGrid)
    {
        auto model = new ItemsModel();
        QList<QList<QVariant>> items;
        QList<QString> names({"Apple", "Apricot", "Avocado", "Banana", "Cranberry"});

        for (int i = 0; i < 100; i++)
        {
            QList<QVariant> item;

            item.push_back(names[i % 5]);
            item.push_back(i);
            item.push_back((double)(i % 5) * 2);

            items.push_back(item);
        }

        model->populate(QList<QString>({"Name", "Price", "Cost"}), items);

        dataGrid->setModel(model);
    }

    return app.exec();
}
