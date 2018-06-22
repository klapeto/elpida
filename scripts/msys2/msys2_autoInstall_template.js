function Controller()
{
    installer.setValue("TargetDir", "__MSYS_FOLDER__");
    installer.setDefaultPageVisible(QInstaller.Introduction, true);
    installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
    installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
    installer.setDefaultPageVisible(QInstaller.StartMenuSelection, false);
    installer.setDefaultPageVisible(QInstaller.LicenseCheck, false);
    installer.setDefaultPageVisible(QInstaller.ReadyForInstallation, false);
    installer.setDefaultPageVisible(QInstaller.PerformInstallation, false);
    installer.setDefaultPageVisible(QInstaller.InstallationFinished, false);
    installer.setDefaultPageVisible(QInstaller.FinishedPage, true);
    ComponentSelectionPage.selectAll();
    installer.setMessageBoxAutomaticAnswer("OverwriteTargetDirectory", QMessageBox.Yes);
    installer.setMessageBoxAutomaticAnswer("stopProcessesForUpdates", QMessageBox.Ignore);
   
}

Controller.prototype.IntroductionPageCallback = function()
{
    // this is not portable. If people at MSYS decide to change the internal archive naming scheme, this will not work and will have be modified
    installer.performOperation("Extract", ["installer://com.msys2.root.base/__MSYS_VERSION__msys__ARCH_BITS__.7z", "__MSYS_FOLDER__"]);
    gui.clickButton(buttons.CancelButton);
}

Controller.prototype.FinishedPageCallback = function()
{    
    gui.clickButton(buttons.FinishButton);
}