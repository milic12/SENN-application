#ifndef PARAMETERSSCREEN_H
#define PARAMETERSSCREEN_H

#include <QWidget>
#include "structs.h"
#include <QCheckBox>
#include <QMap>
#include <QFile>

namespace Ui {
class ParametersScreen;
}

class ParametersScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ParametersScreen(QWidget *parent = nullptr);
    ~ParametersScreen();

    void setParameterList(const QList<Parameter> &value);
    void addCheckBoxs(const QList<Parameter> &list);
    void addCheckBox(const Parameter &param);
    void clearParameterList();
    void addParameters(const QList<Parameter> &value);
    void addParameter(const Parameter &value);
    void onCloseButtonClicked();
    void onUnCheckAllButtonClicked();
    void onCheckAllButtonClicked();
    void onRunSelectedFilesButtonClicked();
    void onRunPercentageSend(int percentage);
    void onDeleteSelectedFilesButtonClicked();
    void onCurrentFileChanged(const QString &currentFile);

protected slots:
    void onRefreshButtonClicked();

private:
    Ui::ParametersScreen *ui;
    QList<Parameter> parameterList;
    QMap<QString, QCheckBox *> checkBoxParametersMap;

signals:
    void refresh();
    void runCheckedParameters(const QList<Parameter> &checkedParameters);
};

#endif // PARAMETERSSCREEN_H
