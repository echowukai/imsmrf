#include "unit_tests.h"
// Test For beep = "yes"
//Unit Test State Machine States(UTSM_States)
enum e_BeepTest1States
{
	Beep1_W4Start_Play,
	Beep1_W4Start_Recording,
	Beep1_W4Mscml_Response,
};
enum e_BeepTest1States eRT1CurrentState;
char RT1_XFile[] = "<?xml version=\"1.0\"?>\
        <MediaServerControl version=\"1.0\">\
        <request>\
        <playrecord id=\"5556123\"\
        recurl=\"file:////nfs.example.com/rec/name.wav\"\
        recencoding=\"msgsm\"\
        initsilence=\"500\" endsilence=\"300\" duration=\"3000\"\
        barge=\"yes\"\
        beep=\"yes\"\
        mode=\"overwrite\"\
        cleardigits=\"no\"\
        escapekey=\"*\"\
        recstopmask=\"0123456789#*\">\
        </playrecord>\
        </request>\
        </MediaServerControl>"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             ;

/******************************************************************************
** Function Name:
**          CalculateStringLength
**
** Parameters:
**        char* pcMyString_i
**           type: input
**           detail : this is a C style NULL terminated char array whose
**                  length is to be calculated
**
**        BOOL  bLogEnable_i
**            type: input
**            detail: This is a flag indicating whether logging should
**                  be done inside this function or not
**
**
** Return:
**      An integer specifying the size in characters of pcMyString_i
**
**
** Additional Details:
**      Any additional details about the function may be added here
**      if Required
**
*******************************************************************************
** Created By:      : Suliman Shah
*******************************************************************************
*******************************************************************************
*/
int execute_test( void* pvMSContext_i )
{
	xmlDocPtr docPtr;

	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n" );
	docPtr = xmlParseMemory( RT1_XFile, strlen( RT1_XFile ) );
	if( docPtr == NULL )
	{
		fprintf( stderr, "xmlParseMemory Cannot Parse the XML File\n" );
		return (0);
	}
	t_MsEvent* poPSMInputEvent = (t_PlayRecordSMInputEvents*)pvMSContext_i;
	t_MsEvent oPSMInputEvent;
	oPSMInputEvent.poMscmlPlayRecordContext = poPSMInputEvent->poMscmlPlayRecordContext;
	oPSMInputEvent.eType = MSCML_REQUEST;
	void* pvMSCMLContext;
	MOP_CreateSM( &pvMSCMLContext, g_nMscmlPlayTraceID, "", g_pvUTContext );
	oPSMInputEvent.poMscmlPlayRecordContext = pvMSCMLContext;
	oPSMInputEvent.uEventParams.oMSCMLRequestParams.pxdMSCMLPlayRequestFile = docPtr;
	eRT1CurrentState = Beep1_W4Start_Play;
	if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( g_pvUTContext, MSCML_OP_EVENT, &oPSMInputEvent, sizeof (t_MsEvent)))
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
		return (0);
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Leaving.\n" );
	return (1);
}

/******************************************************************************
** Function Name:
**          UT_Test_Mscml_Play_Events_Registerar
**
** Parameters:
**        char* pcMyString_i
**           type: input
**           detail : this is a C style NULL terminated char array whose
**                  length is to be calculated
**
**        BOOL  bLogEnable_i
**            type: input
**            detail: This is a flag indicating whether logging should
**                  be done inside this function or not
**
**
** Return:
**      An integer specifying the size in characters of pcMyString_i
**
**
** Additional Details:
**      Any additional details about the function may be added here
**      if Required
**
*******************************************************************************
** Created By:      : Suliman Shah
*******************************************************************************
*******************************************************************************
*/
// MSCML_PLAY_RES
void UT_Test_Mscml_Play_MSCML_PLAY_RES_Handler( void* pvMSContext_i, void* pvParseXMLPContext_i, void* pvEventHandle_i, void* pvEventPayload_i )
{
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n\n\n" );

	//have changed this from t_MsEvent to t_MsEvent
	t_MsEvent* poPSMOutputEvent;
	poPSMOutputEvent = (t_MsEvent*) pvEventPayload_i;
	if( pvMSContext_i == NULL || poPSMOutputEvent->uEventParams.oMSCMLResponseParams.xdpMSCMLResponse == NULL )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Failed: NULL Valued Argument.\n" );
		return;
	}
	if( eRT1CurrentState == Beep1_W4Mscml_Response && poPSMOutputEvent->eType == MSCML_RESPONSE )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "SUCCESS: UNIT TEST(beep = yes) IS SUCCESSFUL\n:" );
	}
	else
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "UNIT TEST(beep = yes) FAILED\n:" );
	}
	xmlDocDump( stdout, poPSMOutputEvent->uEventParams.oMSCMLResponseParams.xdpMSCMLResponse );
	printf( "\n\n\n" );

	switch( poPSMOutputEvent->eType )
	{
		case MSCML_RESPONSE:
			//Memory CleanUp
			MOP_DestroySM( poPSMOutputEvent );

			break;

		case STOP_PLAY:
			break;

		case START_PLAY:
			break;

		case STOP_RECORDING:
			break;

		case START_RECORDING:
			break;

			/*case SM_STOPPED:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "PLAY_ENDED\n:" );
			break;

			case NONE:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, " NONE\n:" );
			break;

			case START_TIMER:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "START_TIMER\n:" );
			break;
			*/
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Leaving.\n" );
}

/******************************************************************************
** Function Name:
**          UT_TesSTART_PLAYSTART_PLAYSTART_PLAYSTART_PLAYSTART_PLAYSTART_PLAYt_Mscml_Play_Events_Registerar
**
** Parameters:
**        char* pcMyString_i
**           type: input
**           detail : this is a C style NULL terminated char array whose
**                  length is to be calculated
**
**        BOOL  bLogEnable_i
**            type: input
**            detail: This is a flag indicating whether logging should
**                  be done inside this function or not
**
**
** Return:
**      An integer specifying the size in characters of pcMyString_i
**
**
** Additional Details:
**      Any additional details about the function may be added here
**      if Required
**
*******************************************************************************
** Created By:      : Suliman Shah
*******************************************************************************
*******************************************************************************
*/
//PLAYER_REQ
void UT_Player_PLAYER_REQ_Handler( void* pvMSContext_i, void* pvParseXMLContext_i, void* pvEventHandle_i, void* pvEventPayload_i )
{
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n" );

	t_MsEvent* poPSMInputEvent;
	poPSMInputEvent = (t_MsEvent*) pvEventPayload_i;
	if( pvMSContext_i == NULL || poPSMInputEvent == NULL )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Failed: NULL Valued Argument.\n" );
		return;
	}
	//t_PlayerInputEvent structure for the Player module
	//have changed this from t_MsEvent to t_MsEvent

	t_MsEvent* poPlayerInputEvent;
	poPlayerInputEvent = (t_MsEvent*) pvEventPayload_i;
	if( eRT1CurrentState == Beep1_W4Start_Play && poPlayerInputEvent->eType == START_PLAY )
	{
		eRT1CurrentState = Beep1_W4Start_Recording;
	}

	switch( poPlayerInputEvent->eType )
	{
		case START_PLAY:
			//it should not be of the type t_PlayerInputEvent
			if( MS_ERR_NONE != Generate_PLAY_ENDED_For_Mscml_Play( pvMSContext_i, poPSMInputEvent ))   //, PLAY_ENDED))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "All_Events_Registerar failed.\n" );
				return;
			}
			break;

		case STOP_PLAY:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "TEST FAILED.\n" );
			//check the second arguemnt, its of t_MsEvent type
			//it should not be of the type t_PlayerInputEvent
			if( MS_ERR_NONE != Generate_PLAY_ENDED_For_Mscml_Play( pvMSContext_i, poPSMInputEvent ))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "All_Events_Registerar failed.\n" );
				return;
			}
			break;

		case MSCML_RESPONSE:

			break;

		case STOP_RECORDING:
			if( eRT1CurrentState == Beep1_W4Start_Recording )
			{
				eRT1CurrentState = Beep1_W4Mscml_Response;
				if( MS_ERR_NONE != Generate_RECORDING_STOPPED_For_Mscml_Play( pvMSContext_i, poPSMInputEvent ))
				{
					MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Generate_RECORDING_STOPPED_For_Mscml_Play failed.\n" );
					return;
				}
			}
			else
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "TEST FAILED.\n" );
				exit( 0 );
			}
			break;

		case START_RECORDING:
			break;
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Leaving.\n" );
}

