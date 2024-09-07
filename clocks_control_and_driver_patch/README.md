1. Enable registry tweak that allows using the video card connected to the Your monitor for calculations. 
You need to run the _driver_patch.reg_ file and import the changes into the registry

2. Configure _nvidiaProfileInspector_
   2.1. Download and unpack _nvidiaProfileInspector.zip_ to local folder _nvidiaProfileInspector_
   2.2. Run _nvidiaProfileInspector.exe_ as Admin
   2.3. Set settings as You see on _.\nvidiaProfileInspector\settings_1.png_ and _.\nvidiaProfileInspector\settings_2.png_
   2.4. Save settings with button "Apply changes". These settings remain in effect until You decide to completely reinstall Your video card driver

3. Configure rigel.exe
   3.1. Add folder _clocks_control_and_driver_patch_ with _rigel.exe_ to Your antivirus exclusion folders
   3.2. Download and unpack _rigel-X.XX.X-win.zip_
   3.3. Set _rigel.exe_ settings as You see on _rigel_security_settings.png_

4. Restart Your computer

5. Every time if You want to use the video card to work with the library, run corresponding bat-file for Your GPU or create similar bat-file with optimal for You customizations. These settings (locked by _rigel.exe_) remain in effect until you restart Your computer.
