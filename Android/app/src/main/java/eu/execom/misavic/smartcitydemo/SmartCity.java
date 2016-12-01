package eu.execom.misavic.smartcitydemo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;
import android.widget.Toast;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.json.JSONObject;

import java.math.BigInteger;
import java.security.SecureRandom;

public class SmartCity extends AppCompatActivity {

    MqttAndroidClient mqttAndroidClient;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_smart_city);



        //System.out.println("Mark 1 :) and date is " + data );
        SecureRandom random = new SecureRandom();
        String rndCliId = new BigInteger(130, random).toString(32);
        System.out.println("ClientID is " + rndCliId);
        //mqttAndroidClient = new MqttAndroidClient(this.getApplicationContext(), "tcp://10.0.5.193:1883", new BigInteger(130, random).toString(32));
        mqttAndroidClient = new MqttAndroidClient(SmartCity.this, "tcp://10.0.5.93:1883", rndCliId);
        //mqttAndroidClient = new MqttAndroidClient(this.getApplicationContext(), "tcp://10.0.2.157:1883", "datas");
        mqttAndroidClient.setCallback(new MqttCallback() {
            @Override
            public void connectionLost(Throwable cause) {
                System.out.println("Connection was lost!");

            }

            @Override
            public void messageArrived(String topic, MqttMessage message) throws Exception {
                System.out.println("Message Arrived!: " + topic + ": " + new String(message.getPayload()));
                String result = new String(message.getPayload());
                //{"Address":"LoRa","Barometer":"1015.31","Humidity":"29.01","Pollution":"5.62","Temp":"21.84"}
                JSONObject jObject = new JSONObject(result);
                String barometer = jObject.getString("Barometer");
                String humidity = jObject.getString("Humidity");
                String pollution = jObject.getString("Pollution");
                String temperature = jObject.getString("Temp");

                final TextView tempValue = (TextView) findViewById(R.id.TemperatureValue);
                tempValue.setText(temperature);

                final TextView pressValue = (TextView) findViewById(R.id.PressureValue);
                pressValue.setText(barometer);

                final TextView humidValue = (TextView) findViewById(R.id.HumidityValue);
                humidValue.setText(humidity);

                final TextView qualityValue = (TextView) findViewById(R.id.QualityValue);
                qualityValue.setText(pollution);


            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {
                System.out.println("Delivery Complete!");
            }
        });

        //connectMQTT();

        try {
            MqttConnectOptions options = new MqttConnectOptions();
            options.setCleanSession(true);
            options.setAutomaticReconnect(true);
            //options.setKeepAliveInterval(5);
            //options.setConnectionTimeout(5);

            final IMqttActionListener listener = new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    Toast.makeText(getApplicationContext(),
                            "MQTT Connection Success!" , Toast.LENGTH_LONG)
                            .show();
                    System.out.println("Connection Success!");
                    try {
                        //System.out.println("Subscribing to sensors/#");
                        String topic = "ipc/LoRa";
                        mqttAndroidClient.subscribe(topic, 1);
                        System.out.println("Subscribed to " + topic);
                        //System.out.println("Publishing message..");
                        //mqttAndroidClient.publish("/test", new MqttMessage("Hello world!".getBytes()));
                    } catch (MqttException ex) {

                    }

                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    System.out.println("Connection Failure!");
                    Toast.makeText(getApplicationContext(),
                            "MQTT Connection Failure!" , Toast.LENGTH_LONG)
                            .show();
                }
            };

            mqttAndroidClient.connect(options, null, listener);

        } catch (MqttException ex) {

        }




    }
}
