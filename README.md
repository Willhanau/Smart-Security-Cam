# Smart-Security-Cam

## Components Used
Board: NUCLEO-F401RE
Wifi Module: X-NUCLEO-IDW01M1 (FirmWare: 3.5.3)
Camera Module: OV7670 without FIFO buffer

## How to compile and run code
1. Login to https://console.mbed.com/
2. Upper right hand corner, to the right of the "Compiler" button, hover over the person icon
3. In the drop down menu, click on "My code"
4. In the center of the screen click on "Add GitHub repository"

***Note: You will have to fork this GitHub repo if you are not a contributor, before adding it.***

5. Enter the required info on the screen.
6. The repo should now be in your list of repositories in your mbed console.
7. Click on the added GitHub repository.
8. On the right hand side, under "Repository toolbox" select "Impoort to Compiler".
9. You will now be redirected to the mbed os online compiler where you can view, edit, and compile the project.
10. Right click the added repo and add a new file called "mbed_app.json".
11. Select "mbed_app_example.json" and copy the contents to "mbed_app.json" we just created.

12. Under the config section replace SSID_HERE, PASS_WORD_HERE, TWILIO_API_URL_HERE, BASIC_AUTH_STRING_HERE, and REQUEST_BODY_HERE with your information.
13. SSID_HERE is the name of your Wifi access point you wish to connect to.
14. PASS_WORD_HERE is the password for the Wifi access point you wish to conncet to.
15. TWILIO_API_URL_HERE is the twilio api url with your accound ID.

***For more information refer to: https://support.twilio.com/hc/en-us/articles/223133907-Simple-Example-for-Sending-Programmable-SMS-Text-or-Picture-Messages***

16. BASIC_AUTH_STRING_HERE is your basic authorization Account SID and Twilio api key encoded string.

***Note: The string starts with Basic.***

***Example: Basic qweqjbnwejkqbjkqweqkwjeqklwj***

***If the above does not make sense to you please use Postman(https://www.getpostman.com/) and practice making API requests to twilio. Once you have a working API request using Postman be sure to look at the code by clicking the "code" button in Postman to get the Basic auhtorization string to use.***

17. REQUEST_BODY_HERE is the x-www-form-urlencoded request body.
***Example: To=%2B13215699874&From=%2B13215699875&MediaUrl=https%3A%2F%2Fdemo.twilio.com%2Fowl.png&Body=Hello+From+Nucleo+Board!***

***You can use Postman to get this x-www-form-url encoded body once you successfully get an API request by clicking "Code" in Postman.***

18. Once all the above are set you may now compile. Be sure to select the NUCLEO-F401RE board in the upper right hand corner.

***Note: Other board type may work but testing was done using the above mentioned board.***

19. Once the binary is downloaded, drag and drop it onto your board.
20. Open a serial terminal such as putty or teraterm and set the baud rate to 9600 (AKA use default serial settings).
21. View the output. If you encounter runtime errors check that your info set in the "mbed_app.json" is correct.

### Using other Wifi modules
This program uses the X-NUCLEO-IDW01M1 wifi module, however other modules may be used by changing the "mbed_app.json" settings and adding your wifi driver to the project. For more information on how to do this please refer to: https://github.com/ARMmbed/mbed-os-example-wifi

# Reference and Guides used
Example Wifi Program by ARMmbed: https://github.com/ARMmbed/mbed-os-example-wifi

Wifi Driver: https://github.com/ARMmbed/wifi-x-nucleo-idw01m1

Wifi module setup: https://www.st.com/en/embedded-software/x-cube-wifi1.html

Wifi module firmware: https://www.st.com/content/st_com/en/products/embedded-software/wireless-connectivity-software/stsw-wifi001.html

Wifi Training by ARM: https://www.st.com/content/ccc/resource/sales_and_marketing/presentation/product_presentation/group0/d9/23/1a/c2/0c/55/46/2c/STSW-IDW002%20Wifi%20hands-on%20training/files/stsw-idw002_wifi_training.pdf/jcr:content/translations/en.stsw-idw002_wifi_training.pdf

Library used to make HTTP requests in mbed os: https://os.mbed.com/teams/sandbox/code/mbed-http/

Twilio Docs: https://www.twilio.com/docs/

Postman App: https://www.getpostman.com/
