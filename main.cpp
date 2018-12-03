#include "mbed.h"
#include "https_request.h"

DigitalOut myled(LED1);
WiFiInterface *wifi;

const char SSL_CA_PEM[] = "-----BEGIN CERTIFICATE-----\n"
                        "MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n"
                        "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
                        "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n"
                        "QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n"
                        "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
                        "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n"
                        "9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n"
                        "CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n"
                        "nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n"
                        "43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n"
                        "T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n"
                        "gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n"
                        "BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n"
                        "TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n"
                        "DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n"
                        "hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n"
                        "06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n"
                        "PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n"
                        "YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n"
                        "CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n"
                        "-----END CERTIFICATE-----\n";

void Blink_LED(){
    while(1) {
        myled = 1; // LED is ON
        wait(0.2); // 200 ms
        myled = 0; // LED is OFF
        wait(1.0); // 1 sec
    } 
}

int Connect_to_Wifi(WiFiInterface *wifi){
    printf("\nConnecting to %s...\n", MBED_CONF_APP_WIFI_SSID);
    int ret = wifi->connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    if (ret != 0) {
        printf("\nConnection error: %d\n", ret);
        return 0;
    }

    printf("Success\n\n");
    printf("MAC: %s\n", wifi->get_mac_address());
    printf("IP: %s\n", wifi->get_ip_address());
    printf("Netmask: %s\n", wifi->get_netmask());
    printf("Gateway: %s\n", wifi->get_gateway());
    printf("RSSI: %d\n\n", wifi->get_rssi());
    
    return 1;
}

void Twilio_https_Request(NetworkInterface *network){
    printf("\n\nSending Twilio a https request!\n\n");
    
    HttpsRequest* request = new HttpsRequest(network, SSL_CA_PEM, HTTP_POST, MBED_CONF_APP_TWILIO_API_URL);
    request->set_header("Content-Type", "application/x-www-form-urlencoded");
    request->set_header("Authorization", MBED_CONF_APP_TWILIO_BASIC_AUTH);
    //const char body[] = "To=%2B16199475312&From=%2B18582953031&MediaUrl=https%3A%2F%2Fdemo.twilio.com%2Fowl.png&Body=Hello+From+Nucleo+Board!";
    HttpResponse* response = request->send(MBED_CONF_APP_TWILIO_REQUEST_BODY, strlen(MBED_CONF_APP_TWILIO_REQUEST_BODY));
    
    //Print Http Response
    printf("Status: %d - %s\n", response->get_status_code(), response->get_status_message().c_str());
    printf("Headers:\n");
    for (size_t ix = 0; ix < response->get_headers_length(); ix++) {
        printf("\t%s: %s\n", response->get_headers_fields()[ix]->c_str(), response->get_headers_values()[ix]->c_str());
    }
    printf("\nBody (%lu bytes):\n\n%s\n", response->get_body_length(), response->get_body_as_string().c_str());
    
    delete request;
}

int main() {
    printf("\fSmart Security Cam V1.0\n");

#ifdef MBED_MAJOR_VERSION
    printf("Mbed OS version %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
#endif
    
    //Intialize wifi interface
    wifi = WiFiInterface::get_default_instance();
    if (!wifi) {
        printf("ERROR: No WiFiInterface found.\n");
        //return -1;
    }
    
    //Connect to wifi access point specified in mbed_app.json
    if(Connect_to_Wifi(wifi) == 1){
        printf("\nConnection to Wifi was SUCCESSFUL!\n");    
    } else{
        printf("\nConnection to wifi FAILED...\n");        
    }
    
    //Send a http request to the Twilio REST API
    Twilio_https_Request(wifi);
    
    //Disconnect from wifi access point
    wifi->disconnect();
    printf("\nDone: Wifi has successfully disconnected.\n");
    
    Blink_LED();
}