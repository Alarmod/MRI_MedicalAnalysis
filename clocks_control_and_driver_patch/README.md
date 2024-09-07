# Manual for GPU configuration

1. Enable registry tweak that allows using the video card connected to the Your monitor for calculations. 
You need to run the _driver_patch.reg_ file and import the changes into the registry

2. Configure _nvidiaProfileInspector_

   2.1. Download and unpack _nvidiaProfileInspector.zip_ to local folder _nvidiaProfileInspector_
   
   2.2. Run _nvidiaProfileInspector.exe_ as Admin
   
   2.3. Set settings as You see on _.\nvidiaProfileInspector\settings_1.png_ and _.\nvidiaProfileInspector\settings_2.png_
   
   2.4. Save settings with button "Apply changes". These settings remain in effect until You decide to completely reinstall Your video card driver (clean installation)

3. Configure rigel.exe

   3.1. Add the _clocks_control_and_driver_patch_ folder, where _rigel.exe_ will be stored, to the exclusion folders of Your antivirus

   3.2. Download and unpack _rigel-X.XX.X-win.zip_

   3.3. Set _rigel.exe_ settings as You see on _rigel_security_settings.png_

4. Restart Your computer

5. Every time if You want to use the video card to work with the library, run corresponding bat-file for Your GPU or pre-created similar bat-file with optimal for You customizations. These settings (locked by _rigel.exe_) remain in effect until you restart Your computer

# ///////////////////////////////////////////////

# Руководство по настройке GPU

1. Активируйте твик реестра, который позволяет использовать для расчетов видеокарту, подключенную к Вашему монитору.
Вам необходимо запустить файл _driver_patch.reg_ и импортировать изменения в реестр

2. Настройте _nvidiaProfileInspector_

   2.1. Загрузите и распакуйте _nvidiaProfileInspector.zip_ в локальную папку _nvidiaProfileInspector_
   
   2.2. Запустите _nvidiaProfileInspector.exe_ от имени администратора
   
   2.3. Установите настройки, как Вы видите на _.\nvidiaProfileInspector\settings_1.png_ и _.\nvidiaProfileInspector\settings_2.png_
   
   2.4. Сохраните настройки кнопкой "Apply changes". Эти настройки остаются в силе до тех пор, пока Вы не решите полностью переустановить драйвер Вашей видеокарты (чистая установка)

3. Настройте rigel.exe
   
   3.1. Добавьте папку _clocks_control_and_driver_patch_, в которой будет храниться с _rigel.exe_, в папки исключения Вашего антивируса
   
   3.2. Скачайте и распакуйте _rigel-X.XX.X-win.zip_
   
   3.3. Установите настройки _rigel.exe_, как Вы видите на _rigel_security_settings.png_

4. Перезагрузите Ваш компьютер

5. Каждый раз, если Вы хотите использовать видеокарту для работы с библиотекой, запускайте соответствующий bat-файл для Вашего GPU или предварительно созданный аналогичный bat-файл с оптимальными для Вас настройками. Эти настройки (заблокированные _rigel.exe_) остаются в силе до перезагрузки Вашего компьютера
