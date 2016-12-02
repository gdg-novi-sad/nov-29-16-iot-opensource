## Meetup info:
        Meetup title: IoT and open-source in commercial solutions 
        Date: 29.11.2016.
        Presentation:
        https://drive.google.com/open?id=1bmSv2XgTnSY9rWnUyZLAMAFsHmOHjWUqJLEd1rTgyw0

/*Readme under constructions*/

## WolkSense cloud:
        https://wolksense.com/
        There you can register to WolkSense and find all the relevant information

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

    Build the project:
        Navigate to Rpi and configure the project by running configure script:
           ./configure
        Navigate to Rpi/build . Build the project with use of make command

## Run Rpi part:
        Runing IotClient And LoRa module:
                Navigate to Rpi/out
                Configuration folder holds ssl certificate for IoT client
                bin folder holds IoT client adn LoRa module executable
                include folder holds headers from libraies that IoT client uses
                lib folder holds libraries that IoT client uses
                First you register device that you want to connect to wolksense:
                - run IoTclient with following command arguments: ./IoTclient device_name wolksense_username wolksense_password
                Go to Rpi/IoTclient/core/src/Ipc.cpp and edit DEV_SERIAL and DEV_PASSWORD with informations from sensor registration
                Go to bin folder and start IoT client (./IoTclient)
                Go to bin folder and start LoRa module (./LoRa)





	

		



