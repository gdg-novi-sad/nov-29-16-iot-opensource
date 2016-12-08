## Meetup info:
        Meetup title: IoT and open-source in commercial solutions 
        Date: 29.11.2016.
        Presentation:
        https://drive.google.com/open?id=0B9dHqnT9YLf3aEhlaF8xVTFodVU
	
## Introduction:
	Smart City showcase is based on LoRa protocol, but you should not limit yourself on just running the sample code.
	Project can be easily be modified so that it is used for different purposes. 
	You can for example just swap LoRa process with Python script that will handle BLE devices and send gathered data to MQTT broker. 
	If README file is faulty or you find something missing open an issue about it and I will give my best to fix it.
	Remember this is just a showcase, code is not perfect, but we can work on that together. Especially Android app is a little bit sketchy :) .
	It should run on different platforms. Showcase has been tried out on PC and Rpi.

## WolkSense cloud:
        https://wolksense.com/
        There you can register to WolkSense and find all the relevant information
	
## LoRa gateway (receiver) components:
	- Raspberry Pi 3
	- Pi 2 click shield ( http://www.mikroe.com/click/pi2-shield/ )
	- LoRa click ( http://www.mikroe.com/click/lora/ )
	

## Build Rpi project:

	Install dependencies:
		sudo apt-get update
		sudo apt-get install zlib1g-dev
		sudo apt-get install cmake

	Install mosquito:

		Instructions:

			http://www.instructables.com/id/Installing-MQTT-BrokerMosquitto-on-Raspberry-Pi/

		Instructions from the link above:

			wget http://repo.mosquitto.org/debian/mosquitto-repo.gpg.key

			sudo apt-key add mosquitto-repo.gpg.key

			cd /etc/apt/sources.list.d/

			sudo wget http://repo.mosquitto.org/debian/mosquitto-wheezy.list

			sudo apt-get update

			sudo apt-get install mosquitto


	Configure the project and install necessary libraries:
		Navigate to Rpi/lib folder and run make_install script:
		./make_install.sh 
		Notice: Be patient :) It will take some time for Poco to compile on Rpi.

	Build the project:
		Navigate to Rpi and configure the project by running configure script:
		./configure
		Navigate to Rpi/build . Build the project with use of make command

## Run Rpi part:

        Running IotClient And LoRa module:
                Navigate to Rpi/out
                Configuration folder holds ssl certificate for IoT client
                bin folder holds IoT client and LoRa module executable
                include folder holds headers from libraries that IoT client uses
                lib folder holds libraries that IoT client uses
                First you register device that you want to connect to wolksense:
                - run IoTclient with following command arguments: ./IoTclient device_name wolksense_username wolksense_password (sensor serial and password will be prompted)
                Go to Rpi/IoTclient/core/src/Ipc.cpp and edit DEV_SERIAL and DEV_PASSWORD with informations from sensor registration
                Go to bin folder and start IoT client (./IoTclient)
                Go to bin folder and start LoRa module (./LoRa)
		
## Arduino:
	
	Arduino folder content:
		- LoRasensor.ino code for Arduino board
		- MQ135-master.zip - library for Air quality sensor
		- cactus_io_BME280_I2C.zip - library for Temperature/Pressure/Humidity sensor
	
	LoRa sensor hardware:
		- Arduino Mega 2650
		- Arduino Mega Click Shield ( http://www.mikroe.com/click/arduino-mega-shield/ )
		- LoRa click ( http://www.mikroe.com/click/lora/ )
		- Weather click ( http://www.mikroe.com/click/weather/ )
		- Air Quality click ( http://www.mikroe.com/click/air-quality/ )
		
## Android:
	
	Run project in Android studio, edit MQTT broker address with the IP address of LoRa receviver device.
		





	

		



