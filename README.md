# ESP32_Water_Sensor
A simple water sensor that will send a text message using and ESP32 and IFTTT.

[![ESP32 Water Sensor](https://cdn.sparkfun.com/r/500-500/assets/home_page_posts/2/2/9/7/ESP32_Water_Sensor-01.jpg)](https://cdn.sparkfun.com/assets/home_page_posts/2/2/9/7/ESP32_Water_Sensor-01.jpg) 

For more information, check out [this blog post](https://www.sparkfun.com/news/2297).

### Additional software

To to get the ESP32 to send a text message, we'll need to create an account on [IFTTT](https://ifttt.com/discover), and set up the [Maker](https://ifttt.com/maker) and [SMS](https://ifttt.com/sms) services. For maker, we'll use "ESP" as the trigger event name, and the message can have whatever information you'd like in the message, but to send the actual water level "{{Value1}}"  is used at the end of message. Example: "The water level is {{Value1}}", where value1 is a JSON object that we set to "HIGH" or "LOW".
