#ifndef VDSRCLIB_H_INCLUDED
#define VDSRCLIB_H_INCLUDED

#include <stddef.h>

#ifndef VDSRCLIB_API
#ifdef _MSC_VER
#define VDSRCLIB_API	__declspec(dllimport)
#else
#define VDSRCLIB_API	extern
#endif
#endif

/* Message Ids */
#define VDSRC_MSG_MAP										18
#define VDSRC_MSG_SPAT										19
#define VDSRC_MSG_BSM										20
#define VDSRC_MSG_CSR										21
#define VDSRC_MSG_EVA										22
#define VDSRC_MSG_IC										23
#define VDSRC_MSG_NMEA										24
#define VDSRC_MSG_PDM										25
#define VDSRC_MSG_PVD										26	
#define VDSRC_MSG_RSA										27
#define VDSRC_MSG_RTCM										28
#define VDSRC_MSG_SRM										29
#define VDSRC_MSG_SSM										30
#define VDSRC_MSG_TIM										31
#define VDSRC_MSG_PSM										32

/* Return Value */
#define VDSRC_OK											0
#define VDSRC_ERROR											-1
#define VDSRC_INVALID_PROBEID								-2
#define VDSRC_INVALID_TIME									-3
#define VDSRC_BUFFER_TOO_SMALL								-4
#define VDSRC_ARGUMENT_NULL									-5
#define VDSRC_NO_ADDED_POSITION								-6
#define VDSRC_UNSUPPORTED_MESSAGE							-7
#define VDSRC_DECODING_FAILED								-8
#define VDSRC_NO_DECODED_MESSAGE							-9

/* BasicVehicleClass */
#define VDSRC_BasicVehicleClass_unknownVehicleClass			0
#define VDSRC_BasicVehicleClass_passenger_Vehicle			10
#define VDSRC_BasicVehicleClass_lightTruck_Vehicle			20
#define VDSRC_BasicVehicleClass_truck_Vehicle				25
#define VDSRC_BasicVehicleClass_motorcycle					40
#define VDSRC_BasicVehicleClass_transit						50
#define VDSRC_BasicVehicleClass_emergency					60
#define VDSRC_BasicVehicleClass_otherTraveler				80
#define VDSRC_BasicVehicleClass_infrastructure				90

/* TransmissionState */
#define VDSRC_TransmissionState_neutral						0
#define VDSRC_TransmissionState_park						1
#define VDSRC_TransmissionState_forwardGears				2
#define VDSRC_TransmissionState_reverseGears				3
#define VDSRC_TransmissionState_unavailable					7

/* Null-Values */
#define VDSRC_Position_longitude_unavailable				1800000001
#define VDSRC_Position_latitude_unavailable					900000001
#define VDSRC_Position_elev_unknown							-4096
#define VDSRC_Position_heading_unavailable					28800
#define VDSRC_Position_speed_unavailable					8191
#define VDSRC_Object_direction_unavailable					28800

#ifdef __cplusplus
extern "C" {
#endif

VDSRCLIB_API int	VDSRCLib_Init(const char *probeId, int basicVehicleClass);
VDSRCLIB_API const char *VDSRCLib_GetLastErrorMessage();

VDSRCLIB_API void	VDSRCLib_SetTransmissionState(int transmissionState);
VDSRCLIB_API void	VDSRCLib_Encode_PVD_Start();
VDSRCLIB_API void	VDSRCLib_Encode_PVD_AddPosition(double utcTime, int lon, int lat, int elev, int heading, int speed);
VDSRCLIB_API int	VDSRCLib_Encode_PVD_SetObject(int obDist, int obDirect, double dateTime);
VDSRCLIB_API int	VDSRCLib_Encode_PVD_Finish(char *uperData, size_t *uperSize);

VDSRCLIB_API int	VDSRCLib_Decode(const char *uperData, size_t uperSize, int *messageId);
VDSRCLIB_API int	VDSRCLib_Get_RSA(int *msgCount, double *utcTime, int *typeEvent, int *headingSlice, int *extent, int *lon, int *lat, int *heading, int *speed);

VDSRCLIB_API int	VDSRCLib_FormatXML(const char *uperData, size_t uperSize, char *xmlData, size_t *xmlSize);

#ifdef __cplusplus
};
#endif

#endif	/* VDSRCLIB_H_INCLUDED */
