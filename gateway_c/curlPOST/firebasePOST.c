#include <stdio.h>
#include <curl/curl.h>
#define DEST_URL "https://iot-project-1e8b6.firebaseio.com/capteur.json"
int main(int argc, char *argv[])
{
		CURL *curl;
		CURLCODE res;
		curl_global_init(CURL_GLOBAL_DEFAULT);
		curl = curl_easy_init();
		if(curl){
				curl_easy_setopt(curl, CURLOPT_URL, DEST_URL );
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "%s", json_to_send);
		}
		curl_global_cleanup(curl);
		return 0;
}
