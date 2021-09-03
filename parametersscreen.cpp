#include "parametersscreen.h"
#include "ui_parametersscreen.h"

ParametersScreen::ParametersScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParametersScreen)
{
    ui->setupUi(this);
    connect(this->ui->refreshButton, &QPushButton::clicked, this, &ParametersScreen::onRefreshButtonClicked);
    connect(this->ui->closeButton, &QPushButton::clicked, this, &ParametersScreen::onCloseButtonClicked);
    connect(this->ui->unCheckAllPushButton, &QPushButton::clicked, this, &ParametersScreen::onUnCheckAllButtonClicked);
    connect(this->ui->checkAllPushButton, &QPushButton::clicked, this, &ParametersScreen::onCheckAllButtonClicked);
    connect(this->ui->runButton, &QPushButton::clicked, this, &ParametersScreen::onRunSelectedFilesButtonClicked);
    connect(this->ui->deleteSelectedFilesButton, &QPushButton::clicked, this , &ParametersScreen::onDeleteSelectedFilesButtonClicked);
}

ParametersScreen::~ParametersScreen() {
    delete ui;
}

void ParametersScreen::setParameterList(const QList<Parameter> &value) {
    clearParameterList();
    this->addParameters(value);
}

void ParametersScreen::addCheckBox(const Parameter &param) {

    if(this->ui->widget->layout() == nullptr)
        return;

    QString fileName = param.fileName;
    fileName.remove(QApplication::applicationDirPath() + "/params/");
    QCheckBox *checkBox = new QCheckBox(fileName, this);

    this->ui->widget->layout()->addWidget(checkBox);
    checkBoxParametersMap.insert(fileName, checkBox);
}

void ParametersScreen::clearParameterList() {

    if(checkBoxParametersMap.size() == 0)
        return;

    QMapIterator<QString, QCheckBox *> it(checkBoxParametersMap);

    while (it.hasNext()) {
        it.next();
        QCheckBox *checkBox= it.value();
        checkBoxParametersMap.remove(it.key());

        if(checkBox != nullptr) {
            this->ui->widget->layout()->removeWidget(checkBox);
            delete checkBox;
            checkBox = nullptr;
        }

    }

    this->parameterList.clear();
}

void ParametersScreen::addCheckBoxs(const QList<Parameter> &list) {
    for (int i=0; i<list.size(); ++i ) {
        addCheckBox(list.at(i));
    }
}

void ParametersScreen::addParameters(const QList<Parameter> &value) {
    for (int i=0; i<value.size(); ++i ) {
        addCheckBox(value.at(i));
        this->parameterList.append(value.at(i));
    }
}

void ParametersScreen::addParameter(const Parameter &value) {
    this->parameterList.append(value);
    addCheckBox(value);
}

void ParametersScreen::onCloseButtonClicked() {
    this->hide();
}

void ParametersScreen::onUnCheckAllButtonClicked() {
    QMapIterator<QString, QCheckBox *> it(checkBoxParametersMap);

    while (it.hasNext()) {
        it.next();
        QCheckBox *checkBox= it.value();

        if(checkBox != nullptr) {
            checkBox->setChecked(false);
        }

    }
}

void ParametersScreen::onCheckAllButtonClicked() {
    QMapIterator<QString, QCheckBox *> it(checkBoxParametersMap);

    while (it.hasNext()) {
        it.next();
        QCheckBox *checkBox= it.value();

        if(checkBox != nullptr) {
            checkBox->setChecked(true);
        }

    }
}

void ParametersScreen::onRunSelectedFilesButtonClicked() {
    this->ui->progressBar->setValue(0);
    this->ui->currentFileLabel->setText("");
    this->ui->currentFileLabel->repaint();

    QList<Parameter> checkedParameters;
    QMapIterator<QString, QCheckBox *> it(checkBoxParametersMap);

    while (it.hasNext()) {
        it.next();
        QCheckBox *checkBox= it.value();

        if(checkBox != nullptr) {
            if(checkBox->isChecked()) {
                Parameter param;
                param.fileName = it.key();
                param.isEnabled = true;
                checkedParameters.append(param);
            }
        }

    }

    emit this->runCheckedParameters(checkedParameters);
}

void ParametersScreen::onRunPercentageSend(int percentage) {
    this->ui->progressBar->setValue(percentage);
}

void ParametersScreen::onDeleteSelectedFilesButtonClicked() {
    QList<Parameter> checkedParameters;
    QMapIterator<QString, QCheckBox *> it(checkBoxParametersMap);

    while (it.hasNext()) {
        it.next();
        QCheckBox *checkBox= it.value();

        if(checkBox != nullptr) {
            if(checkBox->isChecked()) {
                QString fileName = it.key();
                QFile::remove(QApplication::applicationDirPath() + "/params/" + fileName);
            }
        }

    }

    emit this->refresh();
}

void ParametersScreen::onCurrentFileChanged(const QString &currentFile) {
    this->ui->currentFileLabel->setText(currentFile);
    this->ui->currentFileLabel->repaint();
}

void ParametersScreen::onRefreshButtonClicked() {
    this->ui->progressBar->setValue(0);
    this->ui->currentFileLabel->setText("");
    this->ui->currentFileLabel->repaint();
    emit this->refresh();
}
