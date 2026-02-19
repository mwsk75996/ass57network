
/* 
A C++ MQTT Subscriber that subscribes 
to the test/temperature topic 
*/

#include <mosquittopp.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <unistd.h>

class MQTTSubscriber : public mosqpp::mosquittopp
{
public:
    MQTTSubscriber(const char *id, const char *host, int port) : mosquittopp(id)
    {
        connect(host, port, 60);
    }

    void on_connect(int rc) override
    {
        if (rc == 0) {
            std::cout << "Connected to MQTT broker." << std::endl;
            subscribe(NULL, "kitchen/sensor/temperature ");
        } else {
            std::cout << "Failed to connect. RC: " << rc << std::endl;
        }
    }

    void on_message(const struct mosquitto_message *message) override
    {
        if (message->payload && message->payloadlen > 0) {
            std::string payload(
                static_cast<const char *>(message->payload),
                static_cast<std::size_t>(message->payloadlen)
            );
            std::cout << "Received message on topic " << message->topic 
                      << ": " << payload << std::endl;
        } else {
            std::cout << "Received empty message on topic " << message->topic << std::endl;
        }
    }
};

int main()
{
    const char *host = "192.168.1.12";
    int port = 1883;

    mosqpp::lib_init();

    MQTTSubscriber subscriber("subscriber", host, port);

    while(true) {
        subscriber.loop();
        sleep(1);
    }

    mosqpp::lib_cleanup();

    return 0;
}
