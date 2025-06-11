# pico_pi_hdmi_platformer
Small platformer game on raspberry pico pi using Spotpear HDMI adapter <br>

Simply install latest Arduino IDE on Linux (recomended because its faster). <br>
Then add in preferences Additional Boards Manager URLs: <br>
https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json <br>
Install the Pico Pi board. <br>
Then Install PicoDVI library to Arduino IDE trough Library manager. <br>
Connect all wires(USB-C from Pico Pi to Computer and HDMI from pico to TV).
Select Right Port of Pico Pi. <br>

Upload. <br>

Controls are from buttons in baudrate 115200: <br>
left = a, right = d, w = jump <br>
And hit enter in Serial Monitor after each button is pressed to move the player. <br>
You can also use PuTTY for non using enter all time after key is pressed to play it more easily.<br>

There are TileEditor.html and map_editor.html included. If you need to edit game visually.<br>
You can run them by simply opening html on your browser.
You can also edit tiles in jpg format in some painting app 10x10 pixels and then open them one by one in TileEditor.html.<br>
![Demo](hdmi_pico_platformer.gif)
<br>
<img src="for_readme/1749552050325.jpg" alt="Demo" width="400" style="transform: rotate(-90deg);"/>
<img src="for_readme/1749552050377.jpg" alt="Demo" width="400"/>
<br>
<img src="for_readme/1749552050248.jpg" alt="Demo" width="400"/>
