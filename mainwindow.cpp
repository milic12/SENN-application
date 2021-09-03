#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QDir>
#include <QDesktopServices>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QDebug>
#include <QRegularExpression>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->parametersScreen.clearParameterList();
    readExistingParameters();
    this->parametersScreen.setParent(this);
    this->parametersScreen.hide();
    this->parametersScreen.setWindowFlags(Qt::Window);
    /*connect(this->ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartButtonClicked);*/
    connect(this->ui->closeButton, &QPushButton::clicked, this, &MainWindow::onCloseButtonClicked);
    connect(this->ui->addParametersButtton, &QPushButton::clicked, this, &MainWindow::onAddParametersButtonClicked);
    connect(this->ui->selectAndRunButton, &QPushButton::clicked, this, &MainWindow::onSelectAndRunParametersClicked);
    connect(&this->parametersScreen, &ParametersScreen::refresh, this, &MainWindow::onRefresh);
    connect(&this->parametersScreen, &ParametersScreen::runCheckedParameters, this, &MainWindow::onRunCheckedParameters);
    connect(this, &MainWindow::runPercentage, &this->parametersScreen, &ParametersScreen::onRunPercentageSend);
    connect(this->ui->setDefaultValuesCheckBox, &QCheckBox::stateChanged, this, &MainWindow::onSetDefaultValues);
    connect(this, &MainWindow::currentFileChanged, &this->parametersScreen, &ParametersScreen::onCurrentFileChanged);
}

MainWindow::~MainWindow() {
    delete ui;
}


/*void MainWindow::onStartButtonClicked() {
    qDebug() << "- start button ";
    QProcess process;

    QString program = QApplication::applicationDirPath() + "/SENN.exe";
    QStringList arguments;
    process.setWorkingDirectory(QApplication::applicationDirPath());

    process.start(program, arguments);
    std::vector<HWND> windows;
    this->getAllWindowsFromProcessID((DWORD)process.processId(), windows);

    for (std::vector<HWND>::iterator it=windows.begin(); it!= windows.end(); ++it ) {

           QWindow *container = QWindow::fromWinId((WId)(*it));
           QWidget *widget = QWidget::createWindowContainer(container);
           widget->setParent(this);
           widget->setAttribute(Qt::WA_DeleteOnClose);
           widget->show();
    }

    process.waitForFinished();
    QString stdError = process.readAllStandardError();   //Reads standard error
    qDebug() << "Error " << stdError;
}*/

void MainWindow::onCloseButtonClicked() {
    qApp->quit();
}

void MainWindow::saveAsExcelTheDataFile(QString excelFile) {
    QString selectedFile = QApplication::applicationDirPath() + "/data.out";
    QList<QStringList> fileContents;
    QString error;

    if(!parseFileContent(selectedFile, fileContents, error)) {
        QMessageBox::critical(this, tr("Error"), tr(error.toStdString().c_str()));
        return ;
    }

    if (excelFile == "") {
        error =  "Saved excel file name is empty";
        QMessageBox::critical(this, tr("Error"), tr(error.toStdString().c_str()));

        return ;
    }

    if(!saveAsExcel(fileContents, error, excelFile)) {
        QMessageBox::critical(this, tr("Error"), tr(error.toStdString().c_str()));
        return;
    }

}

void MainWindow::onAddParametersButtonClicked() {

    bool ok;
    QString fileName = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("File Name:"), QLineEdit::Normal,
                                         "untitled.txt", &ok);

    if(!ok) {
        return;
    }

    if(fileName.isEmpty()) {
        QMessageBox::critical(this, tr("Error"), "File name is null.");
        this->onAddParametersButtonClicked();
        return;
    }


    if(isParameterFileExits(fileName)) {
        QMessageBox msgBox;
        msgBox.setText("A file with this filename already exists.");
        msgBox.setInformativeText("Do you want to update the file?");
        msgBox.setStandardButtons( QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();

        if(ret == QMessageBox::Cancel){
            this->onAddParametersButtonClicked();
            return;
        }
    }
    fileName = QApplication::applicationDirPath() + "/params/" + fileName;

    QFile file(fileName);

    if(file.open( QIODevice::ReadWrite)) {

        if(writeParamsToFile(file)) {
            unsigned long long int id = getUniqueId();
            QMessageBox::information(0, "info", "Parameters are added");
            addFileToParameters(fileName, id);
        } else {
            QMessageBox::critical(this, tr("Error"), "Error : " + file.errorString());
            return;
        }

    } else {
        QMessageBox::critical(this, tr("Error"), "Error : " + file.errorString());
        return;
    }


}

void MainWindow::onSelectAndRunParametersClicked() {
    this->parametersScreen.show();
    this->parametersScreen.raise();
}

bool MainWindow::parseFileContent(const QString &fileName, QList<QStringList> &fileContents, QString &error) {
    QFile file(fileName);

    if (file.open(QIODevice::ReadWrite)) {

        QTextStream stream(&file);

        while (!stream.atEnd()) {
           fileContents.append(stream.readLine().split(QRegularExpression("\\s+"), Qt::SkipEmptyParts));
        }

        file.close();


    } else {
        QMessageBox::critical(this, tr("Error"), tr("%s file is not opened", fileName.toStdString().c_str()));
        error = "Error openning file";
        return false;
    }

    return true;
}

bool MainWindow::saveAsExcel(const QList<QStringList> &contents,  QString &error, QString excelFile) {

    lxw_workbook  *workbook  = workbook_new(excelFile.toStdString().c_str());
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    bool start = false;

    int worksheetRow = 0;
    for (int row = 0; row<contents.size(); ++row) {

        QStringList columns = contents.at(row);
        if(columns.size() < 6)
            continue;
        //ITER	VMAX	NXGT	UIO	TMAX	NODE
        if(columns.at(0).contains("ITER") && columns.at(1).contains("VMAX")  && columns.at(2).contains("NXGT")
           && columns.at(3).contains("UIO")  && columns.at(4).contains("TMAX") && columns.at(5).contains("NODE")) {
            start = true;
        }

        if(columns.at(1).contains("END")  && columns.at(2).contains("OF")  && columns.at(3).contains("RUN")) {
            start = false;
        }

        if(start) {
            for (int col=0; col<columns.size(); ++col ) {
                lxw_error err =  worksheet_write_string(worksheet, worksheetRow, col, columns.at(col).toStdString().c_str(), NULL);
                if(err != lxw_error::LXW_NO_ERROR) {
                    error = "Error when writing to excel. Error code " + QString::number(err);
                    qDebug() << error;
                    workbook_close(workbook);
                    return false;
                }
            }

            ++worksheetRow;
        }

    }

    return (workbook_close(workbook) == lxw_error::LXW_NO_ERROR) ? true: false;
}

void MainWindow::readExistingParameters() {
    if(!QDir(QApplication::applicationDirPath() + "/params").exists()) {
        QDir().mkdir(QApplication::applicationDirPath() + "/params");
        return;
    } else {
        QStringList list = QDir(QApplication::applicationDirPath() + "/params").entryList();

        for (int i=0; i<list.size(); ++i) {
            //check if filename is a numberİ
            QString file = list.at(i);

            if(file == "." || file == "..")
                continue;

            addFileToParameters(file, i+1);
            ++this->id;
        }
    }
}

void MainWindow::onRefresh() {
    this->parameterList.clear();
    this->id = 0;
    this->parametersScreen.clearParameterList();
    readExistingParameters();
}

void MainWindow::onRunCheckedParameters(const QList<Parameter> &checkedParameters) {
    int percentage = 0;

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                QDir::currentPath(),
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);

    QFile inputfile(QApplication::applicationDirPath() + "/inparam.txt");

    if(!inputfile.open(QIODevice::ReadWrite)) {
        QMessageBox::critical(this, tr("Error"), "Couldn't open the file " + inputfile.fileName());
        return;
    }

    for (int i=0; i<checkedParameters.size(); ++i) {
        this->runSENNAndSaveAs(checkedParameters.at(i).fileName, inputfile, dir, i);
        percentage = ((float)(i+1)/(float)checkedParameters.size())*100.00;
        emit this->runPercentage(percentage);
    }

    QMessageBox::information(this, tr("All files are processed successfully"), "Conversion is successful");

    inputfile.close();
}

void MainWindow::onSetDefaultValues() {
    if(this->ui->setDefaultValuesCheckBox->isChecked()) {
        setFiberValues();
        setStimulusValues();
        setControlValues();
    }
}


void MainWindow::setFiberValues() {
    QFile file(QApplication::applicationDirPath() + "/inparam.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList lineValues = line.split("=");
        if(lineValues.size() == 2) {
            QString valueName = lineValues.at(0);
            valueName = valueName.remove(" ").remove(",");
            QString value = lineValues.at(1);
            value.remove(" ").remove(",");

            QList<QDoubleSpinBox *> list = this->ui->fiberGroupBox->findChildren<QDoubleSpinBox *> ();

            for (int i=0; i<list.size(); ++i) {
                QString objectName = list.at(i)->objectName();
                if(objectName.remove("fiber").remove("SpinBox") == valueName) {
                    list.at(i)->setValue(value.toDouble());
                }
            }
        }

    }

    file.close();
}

void MainWindow::setStimulusValues() {

    QFile file(QApplication::applicationDirPath() + "/inparam.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList lineValues = line.split("=");
        if(lineValues.size() == 2) {
            QString valueName = lineValues.at(0);
            valueName = valueName.remove(" ").remove(",");
            QString value = lineValues.at(1);
            value.remove(" ").remove(",");

            QList<QDoubleSpinBox *> list = this->ui->stimulusGroupBox->findChildren<QDoubleSpinBox *> ();

            for (int i=0; i<list.size(); ++i) {
                QString objectName = list.at(i)->objectName();
                if(objectName.remove("stimulus").remove("SpinBox") == valueName) {
                    list.at(i)->setValue(value.toDouble());
                }
            }
        }

    }

    file.close();
}

void MainWindow::setControlValues() {
    QFile file(QApplication::applicationDirPath() + "/inparam.txt");

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList lineValues = line.split("=");
        if(lineValues.size() == 2) {
            QString valueName = lineValues.at(0);
            valueName = valueName.remove(" ").remove(",");
            QString value = lineValues.at(1);
            value.remove(" ").remove(",");

            QList<QDoubleSpinBox *> list = this->ui->controlGroupBox->findChildren<QDoubleSpinBox *> ();

            for (int i=0; i<list.size(); ++i) {
                QString objectName = list.at(i)->objectName();
                if(objectName.remove("control").remove("SpinBox") == valueName) {
                    list.at(i)->setValue(value.toDouble());
                }
            }
        }

    }

    file.close();
}

void MainWindow::runSENNAndSaveAs(const QString &parameterFile, QFile &inputFile, const QString &outputDir, int index) {
    QFile newFile(QApplication::applicationDirPath() + "/params/" + parameterFile);
    if(newFile.open(QIODevice::ReadWrite)) {
        emit this->currentFileChanged(parameterFile);
        inputFile.resize(0);
        QString all = newFile.readAll();
        inputFile.write(all.toUtf8());
        inputFile.flush();
        newFile.close();

        QProcess process;
        QString program = QApplication::applicationDirPath() + "/SENN.exe";
        QStringList arguments;
        process.setWorkingDirectory(QApplication::applicationDirPath());
        process.start(program, arguments);


        std::vector<HWND> windows;
        this->getAllWindowsFromProcessID((DWORD)process.processId(), windows);

        for (std::vector<HWND>::iterator it=windows.begin(); it!= windows.end(); ++it ) {

               QWindow *container = QWindow::fromWinId((WId)(*it));
               QWidget *widget = QWidget::createWindowContainer(container);
               widget->setParent(this);
               widget->setAttribute(Qt::WA_DeleteOnClose);
               widget->hide();
        }

        hideAllWindows(windows);
        process.waitForFinished();

        QString stdError = process.readAllStandardError();   //Reads standard error

        if(!stdError.isEmpty())
            qDebug() << "Error " << stdError;

        this->saveAsExcelTheDataFile(outputDir + "/" + parameterFile + "-out.xlsx");

    } else {
        QMessageBox::critical(this, tr("Error"), "Couldn't open the file " + newFile.fileName());
    }
}

void MainWindow::getAllWindowsFromProcessID(DWORD dwProcessID, std::vector<HWND> &vhWnds)
{
    HWND hCurWnd = nullptr;
    do
    {
        hCurWnd = FindWindowEx(nullptr, hCurWnd, nullptr, nullptr);
        DWORD checkProcessID = 0;
        GetWindowThreadProcessId(hCurWnd, &checkProcessID);
        if (checkProcessID == dwProcessID)
        {
            vhWnds.push_back(hCurWnd);  // add the found hCurWnd to the vector
            //wprintf(L"Found hWnd %d\n", hCurWnd);
        }
    }
    while (hCurWnd != nullptr);
}

void MainWindow::hideAllWindows(std::vector<HWND> &vhWnds) {
    for (HWND n : vhWnds) {
        ShowWindow( n, SW_HIDE);
    }
}


QStringList MainWindow::getFIBERParams() {
    QStringList list;
    list.append(" NNODES=" + QString::number(this->ui->fiberNNODESSpinBox->value(), 'f', this->ui->fiberNNODESSpinBox->decimals()) + ",\n");
    list.append(" NLIN1=" + QString::number(this->ui->fiberNLIN1SpinBox->value(), 'f', this->ui->fiberNLIN1SpinBox->decimals()) + ",\n");
    list.append(" NLIN2=" + QString::number(this->ui->fiberNLIN2SpinBox->value(), 'f', this->ui->fiberNLIN2SpinBox->decimals()) + ",\n");
    list.append(" NODE1=" + QString::number(this->ui->fiberNODE1SpinBox->value(), 'f', this->ui->fiberNODE1SpinBox->decimals()) + ",\n");
    list.append(" DIAM=" + QString::number(this->ui->fiberDIAMSpinBox->value(), 'f', this->ui->fiberDIAMSpinBox->decimals()) + ",\n");
    list.append(" GAP=" + QString::number(this->ui->fiberGAPSpinBox->value(), 'f', this->ui->fiberGAPSpinBox->decimals()) + ",\n");
    list.append(" CM=" + QString::number(this->ui->fiberCMSpinBox->value(), 'f', this->ui->fiberCMSpinBox->decimals()) + ",\n");
    list.append(" GM=" + QString::number(this->ui->fiberGMSpinBox->value(), 'f', this->ui->fiberGMSpinBox->decimals()) + ",\n");
    list.append(" RHOI=" + QString::number(this->ui->fiberRHOISpinBox->value(), 'f', this->ui->fiberRHOISpinBox->decimals()) + ",\n");
    list.append(" RHOE=" + QString::number(this->ui->fiberRHOESpinBox->value(), 'f', this->ui->fiberRHOESpinBox->decimals()) + ", \n");
    return list;
}

QStringList MainWindow::getSTIMULUSParams() {
    QStringList list;
    list.append(" XC=" + QString::number(this->ui->stimulusXCSpinBox->value(), 'f', this->ui->stimulusXCSpinBox->decimals()) + ",\n");
    list.append(" YC=" + QString::number(this->ui->stimulusYCSpinBox->value(), 'f', this->ui->stimulusYCSpinBox->decimals()) + ",\n");
    list.append(" XA=" + QString::number(this->ui->stimulusXASpinBox->value(), 'f', this->ui->stimulusXASpinBox->decimals()) + ",\n");
    list.append(" YA=" + QString::number(this->ui->stimulusYASpinBox->value(), 'f', this->ui->stimulusYASpinBox->decimals()) + ",\n");
    list.append(" WIREL=" + QString::number(this->ui->stimulusWIRELSpinBox->value(), 'f', this->ui->stimulusWIRELSpinBox->decimals()) + ",\n");
    list.append(" IWAVE=" + QString::number(this->ui->stimulusIWAVESpinBox->value(), 'f', this->ui->stimulusIWAVESpinBox->decimals()) + ",\n");
    list.append(" UIO=" + QString::number(this->ui->stimulusUIOSpinBox->value(), 'f', this->ui->stimulusUIOSpinBox->decimals()) + ",\n");
    list.append(" XPD=" + QString::number(this->ui->stimulusXPDSpinBox->value(), 'f', this->ui->stimulusXPDSpinBox->decimals()) + ",\n");
    list.append(" UIO2=" + QString::number(this->ui->stimulusUIO2SpinBox->value(), 'f', this->ui->stimulusUIO2SpinBox->decimals()) + ",\n");
    list.append(" XPD2=" + QString::number(this->ui->stimulusXPD2SpinBox->value(), 'f', this->ui->stimulusXPD2SpinBox->decimals()) + ",\n");
    list.append(" DELAY=" + QString::number(this->ui->stimulusDELAYSpinBox->value(), 'f', this->ui->stimulusDELAYSpinBox->decimals()) + ",\n");
    list.append(" FREQ=" + QString::number(this->ui->stimulusFREQSpinBox->value(), 'f', this->ui->stimulusFREQSpinBox->decimals()) + ",\n");
    list.append(" PHASE=" + QString::number(this->ui->stimulusPHASESpinBox->value(), 'f', this->ui->stimulusPHASESpinBox->decimals()) + ",\n");
    list.append(" FREQ2=" + QString::number(this->ui->stimulusFREQ2SpinBox->value(), 'f', this->ui->stimulusFREQ2SpinBox->decimals()) + ",\n");
    list.append(" PHASE2=" + QString::number(this->ui->stimulusPHASE2SpinBox->value(), 'f', this->ui->stimulusPHASE2SpinBox->decimals()) + ",\n");
    list.append(" AMP2=" + QString::number(this->ui->stimulusAMP2SpinBox->value(), 'f', this->ui->stimulusAMP2SpinBox->decimals()) + ",\n");
    list.append(" NSINES=" + QString::number(this->ui->stimulusNSINESSpinBox->value(), 'f', this->ui->stimulusNSINESSpinBox->decimals()) + ",\n");
    list.append(" DCOFF=" + QString::number(this->ui->stimulusDCOFFSpinBox->value(), 'f', this->ui->stimulusDCOFFSpinBox->decimals()) + ",\n");
    list.append(" TAUS=" + QString::number(this->ui->stimulusTAUSSpinBox->value(), 'f', this->ui->stimulusTAUSSpinBox->decimals()) + ",\n");
    list.append(" VREF=" + QString::number(this->ui->stimulusVREFSpinBox->value(), 'f', this->ui->stimulusVREFSpinBox->decimals()) + ",\n");
    list.append(" NP=" + QString::number(this->ui->stimulusNPSpinBox->value(), 'f', this->ui->stimulusNPSpinBox->decimals()) + ",\n");
    list.append(" FS=" + QString::number(this->ui->stimulusFSSpinBox->value(), 'f', this->ui->stimulusFSSpinBox->decimals()) + ",\n");
    list.append(" S=" + QString::number(this->ui->stimulusSSpinBox->value(), 'f', this->ui->stimulusSSpinBox->decimals()) + ",\n");
    list.append(" NTRP=" + QString::number(this->ui->stimulusNTRPSpinBox->value(), 'f', this->ui->stimulusNTRPSpinBox->decimals()) + ",\n");
    return list;
}

QStringList MainWindow::getCONTROLParams() {
    QStringList list;
    list.append(" ITHR=" + QString::number(this->ui->controlITHRSpinBox->value(), 'f', this->ui->controlITHRSpinBox->decimals()) + ",\n");
    list.append(" VTH=" + QString::number(this->ui->controlVTHSpinBox->value(), 'f', this->ui->controlVTHSpinBox->decimals()) + ",\n");
    list.append(" NTHNODE=" + QString::number(this->ui->controlNTHNODESpinBox->value(), 'f', this->ui->controlNTHNODESpinBox->decimals()) + ",\n");
    list.append(" DELT=" + QString::number(this->ui->controlDELTSpinBox->value(), 'f', this->ui->controlDELTSpinBox->decimals()) + ",\n");
    list.append(" DELT2M=" + QString::number(this->ui->controlDELT2MSpinBox->value(), 'f', this->ui->controlDELT2MSpinBox->decimals()) + ",\n");
    list.append(" FINAL=" + QString::number(this->ui->controlFINALSpinBox->value(), 'f', this->ui->controlFINALSpinBox->decimals()) + ",\n");
    list.append(" IPRNT=" + QString::number(this->ui->controlIPRNTSpinBox->value(), 'f', this->ui->controlIPRNTSpinBox->decimals()) + ",\n");
    return list;
}

bool MainWindow::writeParamsToFile(QFile &file) {
    file.write(" SINUSOID\n");
    file.write(" &FIBER\n");



    QStringList fiberParams = getFIBERParams();
    QStringList stimulusParams = getSTIMULUSParams();
    QStringList controlParams = getCONTROLParams();

    for (int i=0; i<fiberParams.size(); ++i) {
        file.write(fiberParams.at(i).toUtf8());
    }

    file.write(" &END\n");
    file.write(" &STIMULUS\n");

    for (int i=0; i<stimulusParams.size(); ++i) {
        file.write(stimulusParams.at(i).toUtf8());
    }

    file.write(" &END\n");
    file.write(" &CONTROL\n");

    for (int i=0; i<controlParams.size(); ++i) {
        file.write(controlParams.at(i).toUtf8());
    }

    file.write(" &END\n");


    for (int i=4; i<controlParams.size(); ++i) {
        file.write(controlParams.at(i).toUtf8());
    }
    file.write("\n");

    file.write(" &END\n");

    return true;
}

int MainWindow::getUniqueId() {
    return ++this->id;
}

void MainWindow::addFileToParameters(QString filename, int id) {
    Parameter parameter;
    parameter.fileName = filename;
    parameter.id = id;
    parameter.isEnabled = true;
    this->parameterList.append(parameter);
    this->parametersScreen.addParameter(parameter);
}

bool MainWindow::isParameterFileExits(const QString &fileName) {
    QStringList list = QDir(QApplication::applicationDirPath() + "/params").entryList();
    if(list.contains(fileName))
        return true;

    return false;
}

bool MainWindow::checkParmeterEquation() {
    double NLIN1 = this->ui->fiberNLIN1SpinBox->value();
    double NLIN2 = this->ui->fiberNLIN2SpinBox->value();
    double NNODES = this->ui->fiberNNODESSpinBox->value();
    double NODE1 = this->ui->fiberNODE1SpinBox->value();

    if((NLIN1>=1) && (NLIN1<=NNODES ) && (NLIN2 >=NLIN1 && NLIN2 <= NNODES) && (NODE1 >=1 && NODE1 <=NNODES))
        return true;
}

void MainWindow::changeAddParametersStatus() {
    if(this->checkParmeterEquation())
        this->ui->addParametersButtton->setEnabled(true);
    else
        this->ui->addParametersButtton->setEnabled(false);
}

void MainWindow::on_fiberNLIN1SpinBox_valueChanged(double arg1) {
    changeAddParametersStatus();
}

void MainWindow::on_fiberNNODESSpinBox_valueChanged(double arg1) {
    changeAddParametersStatus();
}

void MainWindow::on_fiberNODE1SpinBox_valueChanged(double arg1) {
    changeAddParametersStatus();
}

void MainWindow::on_fiberNLIN2SpinBox_valueChanged(double arg1) {
    changeAddParametersStatus();
}
