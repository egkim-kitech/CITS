#include "ros/ros.h"
#include <sys/types.h>
#include <std_msgs/String.h>

#include "test/VDSRC.h"
#include "test/tinyxml2.h"
#include "test/tinyxml2.cpp"
#include <signal.h>
#include <test/LDM.h>
#include <test/RSA.h>
//#include "test/asn_application.h"
//#include "test/der_encode_primitive.h"
//#include "test/MapData.c"

#include <string.h>
#include <iostream>

#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>

//UPER 인코딩/디코딩에 사용할 버퍼
#define INIT_BUF_SIZE		4
char INITData[INIT_BUF_SIZE], Istring[INIT_BUF_SIZE];
size_t initSize = INIT_BUF_SIZE;

//UPER 인코딩/디코딩에 사용할 버퍼
#define UPER_BUF_SIZE		65535
char uperData[UPER_BUF_SIZE], utring[UPER_BUF_SIZE];
size_t uperSize = UPER_BUF_SIZE;

#define REST_BUF_SIZE		65535
char restData[REST_BUF_SIZE], rtring[REST_BUF_SIZE];
size_t restSize = REST_BUF_SIZE;

// xml doc 선언
tinyxml2::XMLDocument doc;

char MsgType[4];
int Ref_lon[4];
int Ref_lat[4];
uint size[4];
uint diff;
//XER(XML) 디코딩에 사용할 버퍼
#define XML_BUF_SIZE		65535
char xmlData[XML_BUF_SIZE];
size_t xmlSize = XML_BUF_SIZE;


int main(int argc, char **argv)
{
  ros::init(argc,argv,"LDM_receiver");
  ros::NodeHandle n;
  //ros::Publisher LDM_pub = n.advertise<std_msgs::String>("LDM_reader",1000);
  ros::Rate loop_rate(0.5);
  ros::Publisher RSA_pub = n.advertise<test::RSA>("RSA_data",1000);
  test::RSA msg;
  int client_sock;
  struct sockaddr_in serv_addr;
  client_sock = socket(PF_INET, SOCK_STREAM, 0);
  if(client_sock == -1)
		printf("Error socket");

  memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("192.168.41.32");
	serv_addr.sin_port=htons(atoi("9100"));

  if(connect(client_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	printf("connet fail");
  else
    printf("----------connet success-------------\n");

  printf("----------starting loop-------------\n");

  while(ros::ok())
  {
        //system("clear");
        printf("\n====in the loop=======");
        int count =0;
	      xmlSize = XML_BUF_SIZE;
          if(read(client_sock, MsgType, 4)!=0){    
            // printf("\n read : %s" ,MsgType);
            // printf("\nMsgdata:%s",MsgType);
	    
            if (strcmp(MsgType,"RSA ")==0){
                printf("RSA in");
                read(client_sock, Ref_lon, 4);
                read(client_sock, Ref_lat, 4);
                read(client_sock, size, 4);

                printf("\nref_lon:%i",Ref_lon[0]);
                printf("\nref_lat:%i",Ref_lat[0]);
                printf("\nsize:%u, %u,%u,%u,",size[0],size[1],size[2],size[3]);
                read(client_sock, uperData, size[0]);
                printf("\npayload:%02X",uperData);
                for (int i = 0; i < size[0]; i++)
                {
                    if ((i % 16) == 0)
                    printf("\n");
                    printf(" %02X", (unsigned char)uperData[i]);
                }
	            	memset(xmlData,0,xmlSize);
                VDSRCLib_FormatXML(uperData, size[0], xmlData, &xmlSize);
                printf("XML representation:\n%s\n", xmlData);
                doc.Parse(xmlData);
                int latitude,longditute,elevation;

              	tinyxml2::XMLElement* latelement = 
                doc.FirstChildElement()->
                FirstChildElement("value")->
                FirstChildElement()->
                FirstChildElement("position")->
                FirstChildElement("lat");
                latelement->QueryIntText(&latitude);  
                printf("\nlat:%i",latitude);

                tinyxml2::XMLElement* longelement = 
                doc.FirstChildElement()->
                FirstChildElement("value")->
                FirstChildElement()->
                FirstChildElement("position")->
                FirstChildElement("long");
                longelement->QueryIntText(&longditute);  
                printf("\nlong:%i",longditute);

                tinyxml2::XMLElement* eleelement = 
                doc.FirstChildElement()->
                FirstChildElement("value")->
                FirstChildElement()->
                FirstChildElement("position")->
                FirstChildElement("elevation");
                eleelement->QueryIntText(&elevation);  
                printf("\nele:%i",elevation);
                msg.latitude = latitude;
                msg.longditute = longditute;
                msg.elevation = elevation;
                RSA_pub.publish(msg);
            }
          }
        //ros::spinOnce();
        //loop_rate.sleep();

  }
  return 0;
}
