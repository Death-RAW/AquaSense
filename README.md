
# Aqua Sense 

Aqua Sense is a IOT soulution to monitor the consditions of aquariums to ensure the required essential attributes are met. This project was developed as a partial requirement for the Internet of Things module Mini project 1 of University if Oulu. 

Following resources has been used for this porject: 





## 🔗 Resources 
[![Static Badge](https://img.shields.io/badge/RIOT-https%3A%2F%2Fgithub.com%2FRIOT-OS%2FRIO?link=https%3A%2F%2Fgithub.com%2FRIOT-OS%2FRIOT)](https://github.com/RIOT-OS/RIOT) 

[![Website](https://img.shields.io/website?url=https%3A%2F%2Fwww.iot-lab.info%2F&up_message=FIT-IoT-LAB)](https://www.iot-lab.info/)

[![Website](https://img.shields.io/website?url=https%3A%2F%2Fen.wikipedia.org%2Fwiki%2FConstrained_Application_Protocol&up_message=COAP)](https://en.wikipedia.org/wiki/Constrained_Application_Protocol)

[![Website](https://img.shields.io/website?url=https%3A%2F%2Fmqtt.org%2F&up_message=MQTT&up_color=4594cc&down_message=MQTT)](https://mqtt.org/)

[![Website](https://img.shields.io/website?url=https%3A%2F%2Faws.amazon.com%2Ffree%2F%3Fgclid%3DCj0KCQiA2eKtBhDcARIsAEGTG43PtT4JrzgiotUlY8QZD4F46gDoQlluN56k1YAsYFJeofW8eu6SKRwaAontEALw_wcB%26trk%3D8b3cfe80-3b72-45e5-af97-ee5567921658%26sc_channel%3Dps%26ef_id%3DCj0KCQiA2eKtBhDcARIsAEGTG43PtT4JrzgiotUlY8QZD4F46gDoQlluN56k1YAsYFJeofW8eu6SKRwaAontEALw_wcB%3AG%3As%26s_kwcid%3DAL!4422!3!444204369320!e!!g!!amazon%2520aws!10287741720!106541669430%26trk%3D8b3cfe80-3b72-45e5-af97-ee5567921658%26sc_channel%3Dps%26targetid%3Dkwd-2796001743%26all-free-tier.sort-by%3Ditem.additionalFields.SortRank%26all-free-tier.sort-order%3Dasc%26awsf.Free%2520Tier%2520Types%3D*all%26awsf.Free%2520Tier%2520Categories%3D*all&up_message=AWS&up_color=%23FFA500&down_message=AWS)](https://aws.amazon.com/free/)


## Authors

- [@octokatherine](https://www.github.com/octokatherine)
- [@octokatherine](https://www.github.com/octokatherine)
- [@octokatherine](https://www.github.com/octokatherine)


## High Level Architecture Diagram
## Documentation

[Documentation](https://linktodocumentation)


## Installation : Preparation
- [x] Step 1 : Create a FIT-IoT lab account for accessing the testbed from [FIT Iot Testbed](https://www.iot-lab.info/testbed/signup)

- [x] Step 2 : SSH in to the FIT-IoT lab:  [Instructions here](https://www.iot-lab.info/docs/getting-started/ssh-access/)

- [x] Step 3 : clone https://github.com/RIOT-OS/RIOT.git the RIOT OS files with examples
```bash
   username@ghost:~$ git clone https://github.com/RIOT-OS/RIOT.git
```
- [x] Step 4 : Downlaod the sensor node firmware from git. (its ideal to clone the repo to the RIOT/examples/) if you clone the firmware elsewhere, change the RIOTBASE ?= $(CURDIR)/../.. to mach the folder hierarchi. 

```bash
   username@gsite:~/RIOT/examples$ git clone https://github.com/Death-RAW/AquaSense.git 
```
- [x] Step 5 : For the project to work, a minimim of 3 boards are required. 

* M3 Board 1 : for runnig the boarder router 
* M3 Board 2 : for reading the sensor data
* A8 Board 1 : for runnig the MQTT broker and python 

```bash
   username@site:~/RIOT$ iotlab-experiment submit -n riot_mqtt -d 180 -l 2,archi=m3:at86rf231+site=grenoble -l 1,archi=a8:at86rf231+site=grenoble
```
- [x] Step 6 : Note the assinged nodes for future reference buy issuing the follwoing command. 
*for more detailed information on submitting an experiment with FIT-IoT Lab, refer to the [documentation here.](https://www.iot-lab.info/legacy/tutorials/iotlab-experimenttools-client/index.html#Submit_an_experiment) 

```bash
   username@site:~/RIOT$ iotlab-experiment get -i <id> -r 
```


## Installation : Flashing and Running the Firmware
We recommend that you use 3 separate terminals to make and flash firmware. note that the commands below is assuming that the following boards has been assinged in the experiment. change the id to mach your assinged nodes before exicuting.

* M3-100 : will be used for boarder router
* M3-101 : will be used for sensor board
* A8-102 : wil lbe used for MQTT broker

**M3-100 : Boarder Router Setup : terminal 1**

We will use the generic boarder router example within your RIOT folder for this task. for additional information on the boarder router functionality, refer to [this guide](https://www.iot-lab.info/learn/tutorials/riot/riot-public-ipv6-m3/). 
- [x] Set the RIOT source in ther terminal. 
```bash
username@site:~$source /opt/riot.source
```
- [x] Make the firmware. Use a DEFAULT_CHANNEL of your choosing. default is 26, allowed values goes from 11 to 26. Build this firmware with a baudrate of 500000. This is mandatory for ethos_uhcpd.py script to work effectively since the UART baudrate of the M3 is 500000.
```bash
username@site:~/RIOT$make ETHOS_BAUDRATE=500000 DEFAULT_CHANNEL=15 BOARD=iotlab-m3 -C examples/gnrc_border_router clean all 
```
- [x] flash the firmware.
```bash
username@site:~/RIOT$iotlab-node --flash examples/gnrc_border_router/bin/iotlab-m3/gnrc_border_router.elf -l grenoble,m3,100
```
- [x] We need to find an IP and tap currently unoccupied. use a tap and an IP not listed.
```bash
username@site:~/RIOT$ip -6 route
```
- [x] Now you can configure the network of the border router on m3-100 and propagate an IPv6 prefix with ethos_uhcpd.py
```bash
username@site:~/RIOT$sudo ethos_uhcpd.py m3-100 tap0 2001:660:5309:3105::1/64
```
- [x] if you are getting the following output, everything seems to be working. 
```bash
net.ipv6.conf.tap0.forwarding = 1
net.ipv6.conf.tap0.accept_ra = 0
----> ethos: sending hello.
----> ethos: activating serial pass through.
----> ethos: hello reply received
```
* *Note 1: leave the terminal open (you don’t want to kill ethos_uhcpd.py, it bridges the Boarder router to the front-end network)*
* *Note 2: If you have an error “Invalid prefix – Network overlapping with routes”, it’s because another experiment is using the same ipv6 prefix (e.g. 2001:660:5309:3100::1/64).*

**M3-101 : Sensor Router Setup Terminal 2**

Use the cloned AquaSense files to make and flash the remaining m3 node. navigate to the coloned AquaSense directory.

- [x] Set the RIOT source in ther terminal. 
```bash
username@site:~/RIOT/$source /opt/riot.source
```

- [x] Make the firmware. 
```bash
username@site:~/RIOT$make ETHOS_BAUDRATE=500000 DEFAULT_CHANNEL=15 BOARD=iotlab-m3 -C examples/AquaSense clean all
```
- [x] Flash the firmware to the board
```bash
username@site:~/RIOT$iotlab-node --flash examples/AquaSense/bin/iotlab-m3/Aqua_Sense.elf -l grenoble,m3,101
```

- [x] Check if the boarder router has assinged an ipv6 address to the board. to do this, login to the shell of m3-101 and issue the *ifconfig* command.
```bash
username@site:~/RIOT$nc m3-101 20000
```
- [x] You shoud see the ipv6 address assigned in the previous step under inet6 addr. example shell output is given below for yuor reference. 
```bash
username@site:~/RIOT/examples/AquaSense/$nc m3-101 20000
```

```bash
> ifconfig
ifconfig
Iface  5  HWaddr: 1F:60  Channel: 11  Page: 0  NID: 0x23  PHY: O-QPSK 
          
          Long HWaddr: A6:DA:E9:93:B3:CC:9F:60 
           TX-Power: 0dBm  State: IDLE  max. Retrans.: 3  CSMA Retries: 4 
          AUTOACK  ACK_REQ  CSMA  L2-PDU:102  MTU:1280  HL:64  6LO  
          IPHC  
          Source address length: 8
          Link type: wireless
          inet6 addr: fe80::a4da:e993:b3cc:9f60  scope: link  VAL
          inet6 addr: 2001:660:5307:3102:a4da:e993:b3cc:9f60  scope: global  TNT[3]
          inet6 group: ff02::1
          
> 
```
**A8-102 : MQTT RSMB Broker Setup : terminal 1**

[A8-M3 board](https://iot-lab.github.io/docs/boards/iot-lab-a8-m3/) is capable of running applications used in advanced devices such as set-top box or smartphone/tablet in order to concentrate sensors information. 

RSMB: Really Small Message Broker
The Really Small Message Broker is a server implementation of the MQTT and MQTT-SN protocols. Any client that implements this protocol properly can use this server for sending and receiving messages. The main reason for using RSMB over the main Mosquitto codebase is that Mosquitto doesn't currently have support for the MQTT-SN protocol. RSMB should be pre installed on the A8 node already. see here for more details on [mosquitto.rsmb](https://github.com/eclipse/mosquitto.rsmb)

- [x]  Step 1: login to the A8 shell
```bash
username@site:~ssh root@node-a8-102
```
- [x]  Step 2: Get AWS IoT Device SDK for Python
The AWS IoT Device SDK for Python allows developers to write Python script to use their devices to access the AWS IoT platform through MQTT or MQTT over the WebSocket protocol. By connecting their devices to AWS IoT, users can securely work with the message broker, rules, and the device shadow (sometimes referred to as a thing shadow) provided by AWS IoT and with other AWS services like AWS Lambda, Amazon Kinesis, Amazon S3, and more. Get the AWS IoT Device SDK for Python using git repository.
```bash
root@node-a8-102:~#git clone https://github.com/aws/aws-iot-device-sdk-python.git
```
- [x]  Step 3: Install the AWS IoT Device SDK for Python
```bash
root@node-a8-102:~# cd aws-iot-device-sdk-python
root@node-a8-102:~aws-iot-device-sdk-python# python setup.py install
```
- [x]  Step 4: Now that we have the AWS SDK, lets configure the MQTTSN client. download the required firmware and python files from our git repository on to the A8. 
```bash
root@node-a8-102:~aws-iot-device-sdk-python#cd ..
root@node-a8-102:~#git clone https://github.com/Death-RAW/AquaSense.git
```
- [x]  Step 5: We need to have a **config.conf** file in the root of the A8-102 host. You can find a sample file inside the MQTT folder you just cloned. copy the file to the root using the commands below.
```bash
root@node-a8-102:~#cp ./AquaSense/config.conf .
```
- [x]  Step 6: You can open the config.conf file to do your own configurations by using vim. sample file looks like below.
```bash
root@node-a8-102:~#vim config.conf
```
```bash
# add some debug output
#trace_output protocol
allow_anonymous true
   
# listen for MQTT-SN traffic on UDP port 1885
listener 1885 INADDR_ANY mqtts
ipv6 true
   
# listen to MQTT connections on tcp port 1886
listener 1886 INADDR_ANY
ipv6 true
```
- [x]  Step 7: Lets check if the A8 has been configured with an ipv6 address. we will use this ip address to connect MQTT broker. in this instance, ipv6 address is 2001:660:5307:3000::67
```bash
root@node-a8-102:~#ip -6 -o addr show eth0
```
```bash
2: eth0    inet6 2001:660:5307:3000::67/64 scope global \       valid_lft forever preferred_lft forever
2: eth0    inet6 fe80::fadc:7aff:fe01:95f8/64 scope link \       valid_lft forever preferred_lft forever
```

- [x]  Step 8: Lets start the MQTT broker with our configuration.
```bash
root@node-a8-102:~#broker_mqtts config.conf
```
- [x]  Following output will be displayed to indicate that your broker is up and running.
```bash
20240131 032058.858 CWNAN9999I Really Small Message Broker
20240131 032058.861 CWNAN9998I Part of Project Mosquitto in Eclipse
(http://projects.eclipse.org/projects/technology.mosquitto)
20240131 032058.863 CWNAN0049I Configuration file name is config.conf
20240131 032058.867 CWNAN0053I Version 1.3.0.2, Dec 20 2020 23:34:33
20240131 032058.868 CWNAN0054I Features included: bridge MQTTS 
20240131 032058.869 CWNAN9993I Authors: Ian Craggs (icraggs@uk.ibm.com), Nicholas O'Leary
20240131 032058.872 CWNAN0300I MQTT-S protocol starting, listening on port 1885
20240131 032058.874 CWNAN0014I MQTT protocol starting, listening on port 1886
20240131 032059.849 CWNAN0000I Client connected to udp port 1885 from Aqua_Sense (2001:660:5307:3101:a4da:e993:b3cc:9f60:1884)
```
- [x]  Step 8: Now that our MQTT broker is working, we can use it to communicate with the AWS IOT Thigs. For this, we need the cerificates obtained from the AWS IOT things configuration. Lets jump in to the AWS clould server configuration before running the application. 
