/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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