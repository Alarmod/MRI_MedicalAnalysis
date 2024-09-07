1. Enable registry tweak that allows using the video card connected to the Your monitor for calculations. 
You need to run the driver_patch.reg file and import the changes into the registry.

2. Configure nvidiaProfileInspector
   2.1. Download and unpack nvidiaProfileInspector.zip to local folder nvidiaProfileInspector.
   2.2. Run nvidiaProfileInspector.exe as Admin
   2.3. Set settings as You see on .\nvidiaProfileInspector\settings_1.png and .\nvidiaProfileInspector\settings_2.png
   2.4. Save settings with button "Apply changes". These settings remain in effect until you decide to completely reinstall Your video card driver.

3. Configure rigel.exe
   3.1. Download and unpack rigel-X.XX.X-win.zip
   3.2. Add folder clocks_control_and_driver_patch with rigel.exe to Your antivirus exclusion folders
   3.3. Set rigel.exe settings as You see on rigel_security_settings.png

4. Then restart your computer.

5. Every time you want to use the video card to work with the library, run corresponding bat-file for Your GPU or create similar with optimal for You customizations.
These settings (from rigel.exe) remain in effect until you restart Your computer.
