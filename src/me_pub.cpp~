#include "ros/ros.h"
#include "std_msgs/String.h"
#include "test/VDSRC.h"
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float64.h>

//UPER 인코딩/디코딩에 사용할 버퍼
#define UPER_BUF_SIZE		1024
char uperData[UPER_BUF_SIZE], string[UPER_BUF_SIZE];
size_t uperSize = UPER_BUF_SIZE;

//XER(XML) 디코딩에 사용할 버퍼
#define XML_BUF_SIZE		32768
char xmlData[XML_BUF_SIZE];
size_t xmlSize = XML_BUF_SIZE;

//CHAR message 사용에 사용할 버퍼
//char MsgType[4] = {0x50, 0x56, 0x44, 0x00};
char MsgType[4] = "PVD";
int Ref_lon[4];
int Ref_lat[4];
uint size[4];
//int lat = (int)(atof("35.2249915") * 1e7);
int lat;
int lon;
//int speed = atoi("50");
int speed;
double utcTime;
int heading = atoi("50");

char* format_timeStamp(double timeStamp)
{
	memset(string, 0, sizeof(string));
	time_t _tt = (time_t)timeStamp;
	tm *_tm = localtime(&_tt);
	strftime(string, sizeof(string), "%Y-%m-%d %H:%M:%S", _tm);
	return string;
}
void LATMessageCallback(const std_msgs::Int32 &msg)
{
	lat = msg.data*10;
}
void LONMessageCallback(const std_msgs::Int32 &msg)
{
	lon = msg.data*10;
}
void VELMessageCallback(const std_msgs::Int32 &msg)
{
	speed = msg.data;
}
void TIMMessageCallback(const std_msgs::Float64 &msg)
{
	utcTime = msg.data;
}
int main(int argc, char **argv)
{

  // 여기부터 LDM 소켓 및 ROS 초기 인자 설정
  ros::init(argc,argv,"LDM_tester");
  ros::NodeHandle n;
  ros::Subscriber latmessage_sub = n.subscribe("LAT",1000,LATMessageCallback);
  ros::Subscriber lonmessage_sub = n.subscribe("LON",1000,LONMessageCallback);
  ros::Subscriber velmessage_sub = n.subscribe("VEL",1000,VELMessageCallback);
  ros::Subscriber timmessage_sub = n.subscribe("TIME",1000,TIMMessageCallback);
  ros::Publisher LDM_pub = n.advertise<std_msgs::String>("LDM_publisher",1000);
  ros::Rate loop_rate(1);

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
		
			
	printf("----------connet success-------------\n");

  // main문
  while(ros::ok())
  {
		if(system("CLS"))
		    system("clear");
        //명령행 인자 입력: 차량ID, 경도(deg), 위도(deg), 방향(deg), 속도(km/h)
		char *vehicleId = "F002";
		//int lon = (int)(atof("126.8471154") * 1e7);
    	
		
    	

		//라이브러리 초기화: 차량ID와 차량종류 설정
		int retVal;
		retVal = VDSRCLib_Init(vehicleId, VDSRC_BasicVehicleClass_passenger_Vehicle);
		if (retVal != VDSRC_OK)
		{
			fprintf(stderr, "fail to initialize vdsrc library: %d %s\n", retVal, VDSRCLib_GetLastErrorMessage());
			exit(-1);
		}

		//현재 시간을 UTC값으로 저장
		//timeval tv;
		//if (gettimeofday(&tv, 0))
		//{
		//	fprintf(stderr, "fail to get timeofday\n");
		//	exit(-1);
		//}
		//double utcTime = (double)tv.tv_sec + (tv.tv_usec * 1e-6);

		//인코딩 파라미터 출력
		printf("\nEncoding ProbeVehicleData Message with\n");
		printf("\tVehicleId %s\n", vehicleId);
		printf("\tUTCTime %.3f %s\n", utcTime, format_timeStamp(utcTime));
		printf("\tPosition %d %d\n", lon, lat);
		printf("\tHeading %d Speed %d\n", heading, speed);

		//PVD 인코딩 시작
		VDSRCLib_Encode_PVD_Start();
		//시각 및 GPS 정보 입력
		VDSRCLib_Encode_PVD_AddPosition(utcTime, lon, lat, VDSRC_Position_elev_unknown, heading, speed);
		//객체 정보 입력
			int ob_dist = atoi("*"), ob_direct = atoi("*");
			VDSRCLib_Encode_PVD_SetObject(ob_dist, ob_direct, utcTime);

		//PVD 인코딩 완료 및 UPER 데이터 수신
		retVal = VDSRCLib_Encode_PVD_Finish(uperData, &uperSize);
		if (retVal != VDSRC_OK)
		{
			fprintf(stderr, "fail to encode pvd message: %d %s\n", retVal, VDSRCLib_GetLastErrorMessage());
			exit(-1);
		}

		//UPER 데이터의 크기와 16진수 덤프 출력
		printf("UPER encoded message size = %u", (unsigned)uperSize);
		for (size_t i = 0; i < uperSize; i++)
		{
			if ((i % 16) == 0)
				printf("\n");
			printf(" %02X", (unsigned char)uperData[i]);
		}
		printf("\n");

		//XML 형태로 변환 및 출력
		//retVal = VDSRCLib_FormatXML(uperData, uperSize, xmlData, &xmlSize);
		//if (retVal != VDSRC_OK)
		//{
		//	fprintf(stderr, "fail to format XML: %d %s\n", retVal, VDSRCLib_GetLastErrorMessage());
		//	exit(-1);
		//}
		//printf("XML representation:\n%s\n", xmlData);

		//여기서 모든 데이터를 append
		int total_size = uperSize + 16;
		char Merged_PVD_data[total_size-1];
			
		// printf("----This is test---- total %d bytes\n",total_size);
		 Merged_PVD_data[0] = (MsgType[0])&0xFF;
		 Merged_PVD_data[1] = (MsgType[1])&0xFF;
		 Merged_PVD_data[2] = (MsgType[2])&0xFF;
		 Merged_PVD_data[3] = (MsgType[3])&0xFF;
		// printf("MsgType0 =%i\n",Merged_PVD_data[0]);
		// printf("MsgType1 =%i\n",Merged_PVD_data[1]);
		// printf("MsgType2 =%i\n",Merged_PVD_data[2]);
		// printf("MsgType3 =%i\n",Merged_PVD_data[3]);
		 Merged_PVD_data[4] = (lon>>0)&0xFF;
		 Merged_PVD_data[5] = (lon>>8)&0xFF;
		 Merged_PVD_data[6] = (lon>>16)&0xFF;
		 Merged_PVD_data[7] = (lon>>24)&0xFF;
		// printf("MsgType4 =%i\n",Merged_PVD_data[4]);
		// printf("MsgType5 =%i\n",Merged_PVD_data[5]);
		// printf("MsgType6 =%i\n",Merged_PVD_data[6]);
		// printf("MsgType7 =%i\n",Merged_PVD_data[7]);
		
		 Merged_PVD_data[8] = (lat>>0)&0xFF;
		 Merged_PVD_data[9] = (lat>>8)&0xFF;
		 Merged_PVD_data[10] = (lat>>16)&0xFF;
		 Merged_PVD_data[11] = (lat>>24)&0xFF;
		// printf("MsgType8 =%i\n",Merged_PVD_data[8]);
		// printf("MsgType9 =%i\n",Merged_PVD_data[9]);
		// printf("MsgType10 =%i\n",Merged_PVD_data[10]);
		// printf("MsgType11 =%i\n",Merged_PVD_data[11]);

		 Merged_PVD_data[12] = (uperSize>>0)&0xFF;
		 Merged_PVD_data[13] = (uperSize>>8)&0xFF;
		 Merged_PVD_data[14] = (uperSize>>16)&0xFF;
		 Merged_PVD_data[15] = (uperSize>>24)&0xFF;
		// printf("MsgType12 =%i\n",Merged_PVD_data[12]);
		// printf("MsgType13 =%i\n",Merged_PVD_data[13]);
		// printf("MsgType14 =%i\n",Merged_PVD_data[14]);
		// printf("MsgType15 =%i\n",Merged_PVD_data[15]);

		for (int i=16; i < total_size; i++)
		 {
		 	Merged_PVD_data[i] = uperData[i-16];
		 	//printf("MsgType%i =%i\n",i,Merged_PVD_data[i]);
		 }

		for (int i = 0; i < sizeof(Merged_PVD_data); i++)
		{
			if ((i % 16) == 0)
				printf("\n");
			printf(" %02X", (unsigned char)Merged_PVD_data[i]);
		}

		if(send(client_sock,Merged_PVD_data,total_size,0) ==-1)
			printf("\n------------- Send failed--------------\n");
        else
		    printf("\n------------- Send Success--------------\n"); 
		ros::spinOnce();
		loop_rate.sleep();

  }
  return 0;
}
