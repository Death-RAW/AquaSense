from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient
import MQTTSNClient
import json
import time

MQTTClient = AWSIoTMQTTClient("MQTTSNBridge")
MQTTSNClient = MQTTSNClient.Client("bridge", port=1885)

class Callback:
  # function that replies a message from the MQTTSN broker to the MQTT one
  # and inserts into the database the message just arrived
  def messageArrived(self, topicName, payload, qos, retained, msgid):
      message = payload.decode("utf-8")
      print(topicName, message)
      MQTTClient.publish(topicName, message, qos)
      return True


# path to certificates
path = "/home/root/MQTT/certs/"

# Access Configuration
MQTTClient.configureEndpoint("a1xyjl41ufshwl-ats.iot.us-east-1.amazonaws.com", 8883)
MQTTClient.configureCredentials(path+"AmazonRootCA1.pem",
                                path+"fbb18bb1ec80a8186de8ebaaeb1b0ebcb6cd6d078741bb5f69828223417c95a6-private.pem.key",
                                path+"fbb18bb1ec80a8186de8ebaaeb1b0ebcb6cd6d078741bb5f69828223417c95a6-certificate.pem.crt")

# MQTT broker configuration
MQTTClient.configureOfflinePublishQueueing(-1) 
MQTTClient.configureDrainingFrequency(2)  
MQTTClient.configureConnectDisconnectTimeout(10)  
MQTTClient.configureMQTTOperationTimeout(5)  

# register the callback
MQTTSNClient.registerCallback(Callback())

# make connections to the clients
MQTTClient.connect()
MQTTSNClient.connect()


while True:

    MQTTSNClient.subscribe("sensor/aquarium1")

# disconnect from the clients
MQTTSNClient.disconnect()
MQTTClient.disconnect()
