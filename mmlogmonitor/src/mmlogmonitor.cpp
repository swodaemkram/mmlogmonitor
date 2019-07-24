//============================================================================
// Name        : mmlogmonitor.cpp
// Author      : Mark Meadows
// Version     : 0.5.0
// Copyright   : Copyright (c) 2019 Mark MEadows
// Description : mmlogmonitor in C++, Gangnam-style
//============================================================================
#include <iostream>
#include <string>
#include <stdio.h>
#include <curl/curl.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

char LogLocation[250] = {};   //A place to store the Log location
char WebHookURL[250] = {};	 //A Place to put the WebHookURL
char Filter[250] = {};       //A Place for the Filter
char NewLogMessage[250] = {};
char OldLogMessage[250] = {};
char SendToWebHook[500] = {};
char SentFromWhom[250] = {};

string logmessage = "";
string Version = "mmlogmonitor Ver. 0.5.0";
string ByWho = "By Mark Meadows";

void send_data_to_mattermost(void);
void read_config(void);
void log_function(string log_message);
void get_log_data(void);

struct curl_slist *headers = NULL;

int main() {

	read_config();//(completed)
	logmessage = "mmlogmonitor has started";
	log_function(logmessage);
	logmessage ="";


	while(1)							//This is going to be a service so forever loop
		{
			get_log_data();

			printf("New Message = %s\n",NewLogMessage);
			printf("Old Message = %s\n",OldLogMessage);

			if (strcmp(NewLogMessage,OldLogMessage) != 0)

			    {
				char * isinthere = {};
				isinthere = strstr(NewLogMessage,Filter); //The filter is applied here
				if (isinthere != NULL || strcmp(Filter,"none") == 0 || strcmp(Filter,"all") == 0) // and this allows for filter type none or all to send everything
				{
					log_function(NewLogMessage);
					send_data_to_mattermost();  //Send the gleaned data to MatterMost Server Via Web Hook(completed)
				}
			}

			strcpy(OldLogMessage,NewLogMessage);
			sleep(5);					//Speed of checking Log file set for every 5 seconds
		}


	return 0;
}
/*
===============================================================================================
Lets Send Data To MatterMost
===============================================================================================
*/
void send_data_to_mattermost(void)
{

		   CURL *curl;
		   CURLcode res;
		   curl_global_init(CURL_GLOBAL_DEFAULT);
		   curl = curl_easy_init();

		   // Need to build the MatterMost URL to send the Data to the WebHook
		   //Sample Curl Command to post to mattermost
		   //curl -i -X POST -H 'Content-Type: applicati/json' -d '{"text": "This is a test of the Matermost web hook system "}' http://talk.kyin.net/hooks/6c78zsda4fy

           sprintf(SendToWebHook,"{\"text\": \"%s  %s\"}",NewLogMessage,SentFromWhom); //We have to escape all the JSON crap!

		   curl_easy_setopt(curl, CURLOPT_URL,WebHookURL);
		   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); //Dont Check SSL Cert.
		   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); //Dont Check SSL Cert.
		   //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, R"anydelim( {"text": "You removed public link for DFocuments "} )anydelim");
		   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, SendToWebHook);
		   headers = curl_slist_append(headers, "Expect:");    //Set Header Types For JSON
		   headers = curl_slist_append(headers, "Content-Type: application/json"); //Set Header Type For JSON
		   curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		   res = curl_easy_perform(curl);

		   if(res != CURLE_OK)
		   {
			logmessage = "Unable to Connect to Mattermost Server";
			log_function(logmessage);
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
		    curl_easy_strerror(res));
	        curl_easy_cleanup(curl);
	       }

		   curl_global_cleanup();
		   strncpy(OldLogMessage,NewLogMessage,strlen(NewLogMessage));
		   return;
}
/*
============================================================================================
End of Sending Data To MatterMost
============================================================================================
Lets read the Config File and Load it
============================================================================================
 */
void read_config(void)
{
	//With only a few lines in the config file we will just make a conf "text" file
	//We will put it in the normal place /etc and we will call it ncrssmmd.conf
	FILE *Config_File = NULL;                        // declare config file Pointer

		 		Config_File = fopen("/etc/mmlogmonitor.conf", "r");  	// Open config file
		 		if (Config_File == NULL){
		 			logmessage = "Could not open configuration file";
		 			log_function(logmessage);
		 			printf("Could not open Config File\n");
		 			exit(1);
		 		}

		 		fscanf(Config_File,"%[^\n]\n", LogLocation);      //This will Read to the end of each line until a carriage return
		 		fscanf(Config_File,"%[^\n]\n", WebHookURL);	 //This will Read to the end of each line until a carriage return
		 		fscanf(Config_File,"%[^\n]\n", Filter);		 //This will Read to the end of each line until a carriage return
		 		fscanf(Config_File,"%[^\n]\n",SentFromWhom); //This will Read to the end of each line until a carriage return

		 		fclose(Config_File);
		 		std::string logmessage1 = "=============================================================";
		 		log_function(logmessage1);
		 		log_function(Version);
		 		log_function(ByWho);
		 		log_function(logmessage1);
		 		logmessage1 = "";
		 		logmessage = "";
		 		logmessage1 = "The Config File Say the Log File Location = ";
		 		std::string logmessage2 = LogLocation;
		 		std::string logmessage = logmessage1 + logmessage2;
		 		log_function(logmessage);
		 		logmessage1 = "";
		 		logmessage2 = "";
		 		logmessage = "";
		 		logmessage1 = "The Config File Say the WebHookURL = ";
		 		logmessage2 = WebHookURL;
		 		logmessage = logmessage1 + logmessage2;
		 		log_function(logmessage);
		 		logmessage1 = "";
		 		logmessage2 = "";
		 		logmessage = "";
		 		logmessage1 = "The Config File Say the Filter = ";
		 		logmessage2 = Filter;
		 		logmessage = logmessage1 + logmessage2;
		 		log_function(logmessage);
		 		logmessage1 = "";
		 		logmessage2 = "";
		 		logmessage = "";
		 		logmessage1 = "The Config File Say the SentFromWhom = ";
		 		logmessage2 = SentFromWhom;
		 		logmessage = logmessage1 + logmessage2;
		 		log_function(logmessage);
		 		logmessage1 = "";
		 		logmessage2 = "";
		 		logmessage = "";
		 		logmessage = "Config File Loaded ...";
		 		log_function(logmessage);
		 		logmessage = "";

	return;
}
/*
============================================================================================
Config File Loaded
============================================================================================
Log Function
============================================================================================
*/

void log_function(string log_message)
{

		struct timespec ts;
	    timespec_get(&ts, TIME_UTC);
	    char buff[100];
	    strftime(buff, sizeof buff, "%D %T", gmtime(&ts.tv_sec));

		 ofstream file;
		 file.open ("/var/log/mmlogmonitor.log",std::ios_base::app);

		char MyTime[27];
		sprintf(MyTime," %s.%09ld " , buff,ts.tv_nsec); //Format and apply data

		file<<MyTime;
		file<<log_message;
		file<<"\n";
		file.close();

		return;

}
/*
================================================================================================
End of the Log Function
================================================================================================
 Get Data from Log File
================================================================================================
 */
void get_log_data(void)
{

					FILE *Log_File = NULL;                          // declare config file Pointer

			 		Log_File = fopen("/var/log/auth.log", "r");  	// Open config file

			 		if (Log_File == NULL)
			 		{
			 			logmessage = "Could not open the requested log file";
			 			log_function(logmessage);
			 			printf("Could not open the requested log file\n");
			 			exit(1);
			 		}


			 		while(!feof(Log_File))

			 		{

			 			fscanf(Log_File,"%[^\n]\n", NewLogMessage);      //Read a line from the log

			 		}

			 		fclose(Log_File);

		return;

}
/*
================================================================================================
End of get data from log file
================================================================================================
 */
