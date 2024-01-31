
# Aqua Sense 

Aqua Sense is a IOT soulution to monitor the consditions of aquariums to ensure the required essential attributes are met. This project was developed as a partial requirement for the Internet of Things module Mini project 1 of University if Oulu. 

Following resources has been used for this porject: 

## DEMO VIDEO
**[YOUTUBE Demo Video LINK ](https://youtu.be/fB81RPle3A0)**
## 🔗 Resources 
[![Static Badge](https://img.shields.io/badge/RIOT-https%3A%2F%2Fgithub.com%2FRIOT-OS%2FRIO?link=https%3A%2F%2Fgithub.com%2FRIOT-OS%2FRIOT)](https://github.com/RIOT-OS/RIOT) 

[![Website](https://img.shields.io/website?url=https%3A%2F%2Fwww.iot-lab.info%2F&up_message=FIT-IoT-LAB)](https://www.iot-lab.info/)


[![Website](https://img.shields.io/website?url=https%3A%2F%2Fmqtt.org%2F&up_message=MQTT&up_color=4594cc&down_message=MQTT)](https://mqtt.org/)

[![Website](https://img.shields.io/website?url=https%3A%2F%2Faws.amazon.com%2Ffree%2F%3Fgclid%3DCj0KCQiA2eKtBhDcARIsAEGTG43PtT4JrzgiotUlY8QZD4F46gDoQlluN56k1YAsYFJeofW8eu6SKRwaAontEALw_wcB%26trk%3D8b3cfe80-3b72-45e5-af97-ee5567921658%26sc_channel%3Dps%26ef_id%3DCj0KCQiA2eKtBhDcARIsAEGTG43PtT4JrzgiotUlY8QZD4F46gDoQlluN56k1YAsYFJeofW8eu6SKRwaAontEALw_wcB%3AG%3As%26s_kwcid%3DAL!4422!3!444204369320!e!!g!!amazon%2520aws!10287741720!106541669430%26trk%3D8b3cfe80-3b72-45e5-af97-ee5567921658%26sc_channel%3Dps%26targetid%3Dkwd-2796001743%26all-free-tier.sort-by%3Ditem.additionalFields.SortRank%26all-free-tier.sort-order%3Dasc%26awsf.Free%2520Tier%2520Types%3D*all%26awsf.Free%2520Tier%2520Categories%3D*all&up_message=AWS&up_color=%23FFA500&down_message=AWS)](https://aws.amazon.com/free/)


## Authors
- [@NirashaThennakoon](https://github.com/NirashaThennakoon) Nirasha Kaluarachchi Thennakoon Appuhamilage
- [@Shazam007](https://github.com/Shazam007) Kavindu Wijesinghe Arachchilage
- [@Death-RAW](https://github.com/Death-RAW) : Iresh Jayasundara Mudiyanselage
## High Level Architecture Diagram

![alt text](https://github.com/Death-RAW/AquaSense/assets/61182412/2a0b5ac4-9c61-433f-9d78-2dbc7b1a8e3d)
## Installation : Preparation



- [x]  Step 1 : Create a FIT-IoT lab account for accessing the testbed from [FIT Iot Testbed](https://www.iot-lab.info/testbed/signup)
- [x]  Step 2 : SSH in to the FIT-IoT lab:  [Instructions here](https://www.iot-lab.info/docs/getting-started/ssh-access/)
- [x]  Step 3 : clone https://github.com/RIOT-OS/RIOT.git the RIOT OS files with examples
```bash
   username@ghost:~$ git clone https://github.com/RIOT-OS/RIOT.git
```
- [x]  Step 4 : Downlaod the sensor node firmware from git. (its ideal to clone the repo to the RIOT/examples/) if you clone the firmware elsewhere, change the RIOTBASE ?= $(CURDIR)/../.. to mach the folder hierarchi. 

```bash
   username@gsite:~/RIOT/examples$ git clone https://github.com/Death-RAW/AquaSense.git 
```
- [x]  Step 5 : For the project to work, a minimim of 3 boards are required. 

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
- [x]  Go back to the sensor router and start reading the data. copy thr ipv6 addredd observed in step 7
```bash
username@site:~/RIOT$nc m3-101 2000
>
Run 2001:660:5307:3000::67 1885 1

```
- [x]  Step 8: Now that our MQTT broker is working, we can use it to communicate with the AWS IOT Thigs. For this, we need the cerificates obtained from the AWS IOT things configuration. Lets jump in to the AWS clould server configuration before running the application. 

## AWS Configuration
**Overview**
Data Flow

Initially, the sensor sends data packets through MQTT publish to the AWS IoT Thing. Subsequently, the data is forwarded to a subscribed service hosted on an EC2 instance in the same region. The received data is then parsed and transmitted to the Influx time series database. To ensure data persistence in case of failure or downtime in EC2 services, InfluxDB data storage locations are linked to external AWS EBS drives.

For effective service management and to minimize failures and downtimes, PM2 is employed as the service manager on the EC2 instance. Grafana serves as the visualization tool due to its native support for InfluxDB as a datasource and its robust features for time series data visualization. To replicate this entire process, follow the steps outlined below

AWS IOT thing setup

AWS IoT (Internet of Things) is a comprehensive platform by Amazon Web Services designed to connect and manage IoT devices securely. One key component is the AWS IoT Core, functioning as an MQTT (Message Queuing Telemetry Transport) broker, facilitating efficient communication between devices and the cloud. MQTT is a lightweight protocol suitable for resource-constrained devices. AWS IoT Core handles device registration, authentication, and message routing, ensuring secure and reliable data exchange.

- [x]  Create an AWS IOT thing : [Click here and follow the steps below](https://us-east-1.console.aws.amazon.com/iot/home?region=us-east-1#/device-management-overview )
```bash
Create a thing → Enter the thing name → Select “Auto-generate a new certificate (recommended)” → Create a policy → Add following policy 
```
Policy effect		Policy effect		Policy resource
Allow			        *			       *

| Policy effect     | Policy effect | Policy resource |
| ----------- | ----------- |----------- |
| Allow       | X           | X

```bash 
Choose the created policy → create thing → download the certificates
```
- [x]  Grab the thing endpoint for future usages from,
```bash
IOT core →  Settings → get the endpoint when necessary
```
Sample endpoint - *a1xyjl41ufshwl-ats.iot.us-east-1.amazonaws.com*

- [x]  You will have to add this endpoint and downloaded certificates to following two places, We suggest you to copy the certificate files to a private git repo at this point. you can use this git repo to clone the files in to the root of A8-102 host. folder name must be *certs*

* In the A8 console repository where the MQTT topic publishing action runs
* In the EC2 instance where the subscription to the topic happens

- [x]  EC2 instance setup and EBS mount for persistence storage

Create an instance using the Ubuntu 22 AMI. In the 'Configure Storage' section, add two separate volumes for future InfluxDB storage. This additional storage is essential to ensure that data in the database remains intact even in the event of a failure in the instance or database service.
When choosing the storage options, it's advisable to reserve the storage, taking the following into consideration:

```bash
EBS1 - /dev/sdf - high IOPs low storage size - influx quick cache (WAL)
EBS2 - /dev/sdg - low IOPs larger storage size - influx Data 
```
- [x] SSH into the instance and execute the following commands for EBS mounting

```sudo
sudo apt update
//Create partitions
sudo mkfs.ext4 /dev/xvdf
sudo mkfs.ext4 /dev/xvdg
//Create folders for upcoming influx database
mkdir -p /opt/influx/{wal,data,ssl}
//Mount those folders with created partitions
mount /dev/xvdf /opt/influx/wal/
mount /dev/xvdg /opt/influx/data/
//With following, the mount point will not get released with restart action
nano /etc/fstab
dev/xvdf   /opt/influx/wal/   ext4   defaults,nofail   0   2
dev/xvdg   /opt/influx/data/   ext4   defaults,nofail   0   2
```

**Influxdb and Grafana services**

Influxdb
InfluxDB is a high-performance and open-source time-series database designed for efficiently storing and querying timestamped data. It is particularly useful in IoT applications where large volumes of time-series data, such as sensor readings, need to be managed in real-time. InfluxDB's optimized architecture and query language make it well-suited for handling and analyzing time-stamped information, providing a reliable solution for IoT data storage and retrieval.

- [x] refer to the [Influxdb Installation guide - official documentation](https://docs.influxdata.com/influxdb/v2/install/?t=Linux).
- [x]  Execute following for required configurations

//enable influx db after installation
```sudo
sudo systemctl unmask influxdb.service
sudo systemctl enable influxdb
```

//Give earlier created folder access for influxdb user
```sudo
chown -R influxdb:influxdb /opt/influx/
```

//Backup the current influx config file
```sudo
cp /etc/influxdb/influxdb.conf{,-bak}
```

//Replace locations in config file in-place
```sudo
sed -i s./var/lib/influxdb/meta./opt/influx/data/meta. /etc/influxdb/influxdb.conf
sed -i s./var/lib/influxdb/data./opt/influx/data/data. /etc/influxdb/influxdb.conf
sed -i s./var/lib/influxdb/wal./opt/influx/wal. /etc/influxdb/influxdb.conf
```

//Start the service
```sudo
sudo systemctl start influxdb
```

//Access db and create the database and user in the influx console
```sudo
influx -precision rfc3339 (to see timestamp easily)
CREATE DATABASE iotDB
USE iotDB
```

//Create user using the API (can be also done using the cli)
```sudo
curl -XPOST "http://localhost:8086/query" --data-urlencode "q=CREATE USER iotuser WITH PASSWORD 'iotpass' WITH ALL PRIVILEGES"
```

//To log into the cli after user setup
```sudo
influx -username 'iotuser' -password 'iotpass' -precision rfc3339
```
**Grafana**

Grafana is an open-source analytics and monitoring platform that allows users to visualize and analyze data from various sources in real-time. It provides a flexible and customizable dashboard interface, enabling users to create dynamic, interactive visualizations and charts to gain insights into their data. With Grafana's native support for InfluxDB, users can seamlessly connect Grafana dashboards to InfluxDB instances. This integration provides a straightforward and efficient way to visualize time-series data stored in InfluxDB. 

[Influxdb Installation guide - official documentation](https://grafana.com/docs/grafana/latest/setup-grafana/installation/debian/)

- [x]  Access the ui with http://<public ip of ec2 instance>/:3000 and create initial credentials and log into the dashboard.
- [x]  Setup the dashboard by importing given dashboard template

**Subscriber setup on EC2**
- [x]  SSH into the instance 
```bash
ssh -i "<keypair for aws>" <host URL for the instance>
sudo apt update
sudo apt install nodejs npm
```

- [x]  Clone the repository into the current directory
```bash
https://github.com/Shazam007/aws-iot-ec2
```
- [x]  Install node packages
```bash
cd aws-iot-ec2
npm install
```
- [x]  copy the generated certificates of the IOT thing to this directory by cloning a repository including those certificates or secure copy with linux (scp to the host location)
- [x]  change the required data in thing-subscriber.js
```bash
keyPath
certPath
caPath
clientId
host
```

**InfluxDB credentials (created in the influxdb installation steps)**

- [x]  install pm2 as the service manager
```bash
npm install pm2@latest -g
```
- [x]  Start the server
```bash
pm2 start thing-subscriber.js
```

- [x]  get the running services under pm2
```bash
Pm2 status 
```
- [x]  get logs of the running service
```bash
pm2 logs
```
Now you should be able to see the incoming data objects from the publishing topic in the pm2 logs. Navigate to the Grafana dashboard to visualize the data.

## Running the system
Great! you made it so far. few more steps and we are ready to run the AquaSense system. make sure you have cloned the certificates in to the certs folder of the A8-102 root. 

- [x]  In A8-102 host, run the MQTTSNbridge.py

```bash
root@node-a8-102:~#python3 MQTTSNbridge.py
```

You will see the sensor data in your AWS dashboad at this point. 