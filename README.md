# SENN - Qt application

![image](https://user-images.githubusercontent.com/50624481/132002849-ad566890-1fbb-4d71-aeee-f8f2c7fc3972.png) 

## 📥 Download installation setup for Windows [here](https://drive.google.com/uc?export=download&id=1pJxE7oBTfhh4YipyvsrfbqhvHwmcAkdE)⏬

## 	♦️ Description

Spatially extended nonlinear node (SENN) is a program made in Fortran that provides a model for estimating electrical dose at the site of nerve or muscle excitation. Theory and software were devised by J. Patrick Reilly and Alan Diamant, two OSEL research fellows, more [here](https://www.fda.gov/about-fda/cdrh-offices/senn-nerve-stimulation).

With this project, an improved application was made using the  **Qt Widget based application** incorporating the SENN model and adding new functionalities. Now with easier use, due to a simple and intuitive user interface and a range of functionalities that were not possible before. From multiple relaunching with different parameters to the ability to convert automatically from text files to excel files and filter important data. The idea of this project is ultimately to be further upgraded and to be in wider use due to its simplicity.


### 📑 Installation guide 

Once you download the installation setup you will see the icon 

![setup-icon](https://user-images.githubusercontent.com/50624481/131310377-0945302d-b215-483d-a284-ca0786b5bf21.png)

If you are faced with a warning of antivirus or windows defender, it is because in this setup there is no publisher license (which has to be bought additionally), so antivirus or windows defender will show you the message “Windows protected your PC” and also , “Running this app might put your PC at risk” just click More info option, and chose option Run anyway.

![windows-defender](https://user-images.githubusercontent.com/50624481/131796403-067b3f76-85e7-4bfc-bf1d-a999959bb207.png)

You will be asked additionally "do you want to allow this app from an unknown publisher to make changes to your device", you can just skip this part by clicking Yes.

![windows-defender](https://user-images.githubusercontent.com/50624481/131369996-de3ca4f3-bb6f-417b-90b1-fcc39752ce9e.png)

[Software-publisher-certificate](https://comodosslstore.com/resources/what-is-a-software-publisher-certificate-how-do-i-get-one/) more info.

When you run setup, accept the terms and conditions 

![image](https://user-images.githubusercontent.com/50624481/131310703-b8737d1b-5812-4313-9ce4-f04923244e5e.png)

Choose a destination where the app will be installed

![destination-install](https://user-images.githubusercontent.com/50624481/131311051-43cf21b2-60dc-4ee0-82e1-88048c1e9c09.png)

For fast start and simplicity, you can select the Desktop shortcut option

![desktop-shortcut](https://user-images.githubusercontent.com/50624481/131312177-b0c87d34-68d1-4d3d-9c10-5d1554573852.png)

### 	🖥️ Application user guide 

After installation when you run the app you will, first of all, see **parameters** fields divided into three sections which are FIBER, STIMULUS, and CONTROL

![program-ui](https://user-images.githubusercontent.com/50624481/131376439-6a0e240e-0dc4-41ad-b182-f639f9190b7c.png)

You can add default values for parameters for the first run, also this option helps you because some parameters will not be changed a lot. So option **Set default values** can be of great help when you start using this application. 

More in-depth about parameters in the book [Electrostimulation: Theory, Applications, and Computational Model,  J. Patrick Reilly ](https://www.amazon.com/Electrostimulation-Theory-Applications-Computational-Model-ebook/dp/B008OWVSSE) chapter 12.

After you added your parameters in the fields, then you can proceed to save parameters with the option Add parameters.
Then you will be prompted with a box to add a name to the text file where your parameters will be saved

![Add-parameters-button](https://user-images.githubusercontent.com/50624481/131376683-9debb993-b38d-45fd-8187-19da48715436.png)

If successful you will be prompted with a message 

![Add-parameters-button-success](https://user-images.githubusercontent.com/50624481/131376833-176e73a8-b713-4db8-87ce-806304fd90ca.png)

When you have added your parameters you can go ahead and click **Select Parameters and Start** button
then you will see the parameters screen box

![parameters-screen-box](https://user-images.githubusercontent.com/50624481/131377282-172f9ea4-1c67-4454-985b-f557101ba380.png)

You can now see the values of your parameters added with the file name you have chosen, in my case, I used the name "demonstracija".
As you can see, you have here many options: 
- **Chech All** button will check all files which you have added. This option is helpful when we add many parameters 
files, then simply clicking this button you have all files selected ready to be run. 
- **UnChechAll** as the name says it opposite of Chech all button.
- **Delete Selected files** will delete parameters files that we have been added.
- **Progress bar** will show you which file is currently being processed, and which files have been handled 
- **Refresh** button will refresh the current state
- **Stop** will stop the process
- **Close** button will close Parameters window 
- **Run For Selected Files** button runs through SENN mathematical equations, with parameters which we added, either one or multiple parameters ( for multiple output files), here we have added functionality that converts the output of SENN data.out text file, converts it into an excel file, and sorts from all unnecessary information, specific information that we need about our simulation.
More about this in the developer section.

After we run button Run For Selected Files we will be asked to select folder where we will save our excel, then we after 

![image](https://user-images.githubusercontent.com/50624481/131337090-52ff33e2-715c-4501-a9f2-19b0699e5a67.png)





## ⌨️ For Developers

### 📥 Download repo [here](https://github.com/milic12/SENN-application/archive/refs/heads/master.zip) ⏬

Once you download, fork or clone source code you will see this structure of the app:

```js
 app name
    └── app-name.pro
    └──	Qlibxlsxwriter
    └── Headers
    └── Sources
              └── main.cpp
                 ├── mainwindow.cpp
                 └── parametersscreen.cpp

    └── Forms
            └── mainwindow.ui
            └── parametersscreen.ui
```

In **Source** files, we have mainwindow.cpp which contains functions and logic of MainWindow screen, it is connected with parametersscreen.cpp which contains functions and logic of Parameters window. UML diagram shows functions used and parent-child relation between main window and parameters screen

![Uml-diagram](https://user-images.githubusercontent.com/50624481/131505086-85b0ee71-7ab9-419d-a0bb-2d50d0468ce6.png)+

**Form** files contain a designer view of the main window and also the parameters window.
A designer view is a helpful tool because it allows you to place elements and immediately see how it looks rather than programming it from scratch and adding CSS, after you add some elements you can then go directly in mainwindow.cpp or parameterswindow.cpp and add code functionalities and logic.

To menition some of classes you my want to change, like saveAsExcel class 

```c++
 saveAsExcel(const QList<QStringList> &contents,  QString &error, QString excelFile) {

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
```

You can see here if statements, which have a role to detect specific words from where you want to start filtering data.out file and have it at the end converted to an excel file. Now here you can add changes if you need other values from data.out files.

### 🚀  Deployment 

After we finished app production and developement now we can deploy our app to siplify usage for non-developers, Qt docs for [Windows](https://doc.qt.io/qt-5/windows-deployment.html) also for [macOS](https://doc.qt.io/qt-5/macos-deployment.html) and [Linux](https://doc.qt.io/qt-5/linux-deployment.html).

The thing to bear in mind is that this app was planed and design to be deployed, so when you download source code and build a project, functionalities of program won’t work immediately, because the path to specific files is set to locations in the deployed folder. 

As said above having deployed application is a great way for people whose primary focus is not software development to have a fast and simple approach to the usage of this application.



## License

MIT License

Copyright (c) 2021 Manojlo Ilic

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
