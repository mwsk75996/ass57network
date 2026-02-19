/* 
A C++ MQTT Publisher that publishes 
to the test/temperature topic 
*/

#include <mosquittopp.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

class MQTTPublisher : public mosqpp::mosquittopp
{
public:
    bool active = false;
    MQTTPublisher(const char *id, const char *host, int port) : mosquittopp(id)
    {
        int keepalive = 60;
        connect(host, port, keepalive);
    }

    void on_connect(int rc)
    {
        if (rc == 0) {
            active = true;
            std::cout << "Connected to MQTT broker." << std::endl;
        } else {
            std::cout << "Failed to connect. RC: " << rc << std::endl;
        }
    }

    void on_publish(int mid)
    {
        if (active){
            std::cout << "Message published successfully." << std::endl;
        }
    }
};

int main()
{
    const char *host = "192.168.1.12";
    int port = 1883;
    const char *topic = "kitchen/sensor/temperature ";

    mosqpp::lib_init();

    MQTTPublisher publisher("publisher", host, port);

    while (true) {
        std::string message = "Hello from Publisher Xubuntu: " + std::to_string(time(0));
        publisher.publish(NULL, topic, message.length(), message.c_str());
        publisher.loop();
        sleep(5);
    }

    mosqpp::lib_cleanup();

    return 0;
}
