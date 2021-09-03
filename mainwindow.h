#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QStringList>
#include "xlsxwriter.h"
#include <QFile>
#include "parametersscreen.h"
#include "structs.h"
#include <iostream>
#include "windows.h"
#include "tlhelp32.h"
#include "tchar.h"
#include "vector"
#include "string"
#include <QWindow>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartButtonClicked();
    void onCloseButtonClicked();
    void onSetParametersButtonClicked();
    void onSaveButtonClicked();
    void onSaveAsExcelFileButtonClicked();
    void saveAsExcelTheDataFile(QString excelFile);
    void onAddParametersButtonClicked();
    void onSelectAndRunParametersClicked();
    bool parseFileContent(const QString &fileName, QList<QStringList> &fileContents, QString &error);
    bool saveAsExcel(const QList<QStringList> &contents, QString &error, QString excelFile);
    void readExistingParameters();
    void onRefresh();
    void onRunCheckedParameters(const QList<Parameter> &checkedParameters);
    void onSetDefaultValues();

    void setFiberValues();
    void setStimulusValues();
    void setControlValues();
    void runSENNAndSaveAs(const QString &parameterFile, QFile &inputFile, const QString &outputDir, int index);
    void getAllWindowsFromProcessID(DWORD dwProcessID, std::vector <HWND> &vhWnds);
    void hideAllWindows(std::vector<HWND> &vhWnds);

    void on_fiberNLIN1SpinBox_valueChanged(double arg1);

    void on_fiberNNODESSpinBox_valueChanged(double arg1);

    void on_fiberNODE1SpinBox_valueChanged(double arg1);

    void on_fiberNLIN2SpinBox_valueChanged(double arg1);

protected:
    QStringList getFIBERParams();
    QStringList getSTIMULUSParams();
    QStringList getCONTROLParams();
    bool writeParamsToFile(QFile &file);
    int getUniqueId();
    void addFileToParameters(QString filename, int id);
    bool isParameterFileExits(const QString &fileName);
    unsigned long long int id=0;
    QList<Parameter> parameterList;
    ParametersScreen parametersScreen;
    bool checkParmeterEquation();
    void changeAddParametersStatus();

//    void on_lineEdit_cursorPositionChanged(int arg1, int arg2);

//    void on_spinBox_5_textChanged(const QString &arg1);

//    void on_checkBox_stateChanged(int arg1);

signals:
    void runPercentage(int percentage);
    void currentFileChanged(const QString &currentFile);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
