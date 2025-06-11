# pico_pi_hdmi_platformer
Small platformer game on raspberry pico pi using Spotpear Hhhhdmi adapter <br>

Simply install latest Arduino IDE on Linux (reccomended because its faster). <br>
Then add in preferences Additional Boards Manager URLs: <br>
https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json <br>
Install the Pico Pi board. <br>
Then Install PicoDVI library to Arduino IDE trough Library manager. <br>
Connect all wires(USB-C from pico to Computer and HDMI from pico to TV).
Select Right Port of Pico Pi. <br>

Upload. <br>

Controls are from buttons in baudrate 115200: <br>
asdw <br>
And hit enter after each button pressed to move the player. <br>
There are TileEditor.html and map_editor.html included. If you need to edit game visually.<br>
You can run them by simply opening html on your browser.
![Demo](hdmi_pico_platformer.gif)
<br>

![Demo](for_readme/1749552050325.jpg)
![Demo](for_readme/1749552050377.jpg)
![Demo](for_readme/1749552050248.jpg)
