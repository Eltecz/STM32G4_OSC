#include "usbd_mult.h"


#define USB_MC_CONFIG_DESC_SIZ 106


extern USBD_HandleTypeDef hUsbDeviceFS;


extern uint8_t USBD_MSC_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
extern uint8_t USBD_MSC_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
extern uint8_t  USBD_MSC_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
extern uint8_t USBD_MSC_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
extern uint8_t USBD_MSC_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);


extern uint8_t  USBD_CDC_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
extern uint8_t  USBD_CDC_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
extern uint8_t  USBD_CDC_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);
extern uint8_t  USBD_CDC_EP0_RxReady(USBD_HandleTypeDef *pdev);


static uint8_t  USBD_Composite_Init (USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t  USBD_Composite_DeInit (USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t  USBD_Composite_Setup (USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t  USBD_Composite_DataIn (USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t  USBD_Composite_DataOut (USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t  *USBD_Composite_GetHSCfgDesc (uint16_t *length);
static uint8_t  *USBD_Composite_GetFSCfgDesc (uint16_t *length);
static uint8_t  *USBD_Composite_GetOtherSpeedCfgDesc (uint16_t *length);
static uint8_t  *USBD_Composite_GetDeviceQualifierDescriptor (uint16_t *length);
static uint8_t USBD_Composite_RxReady (USBD_HandleTypeDef *pdev);
static void USBD_Composite_Switch_MSC(USBD_HandleTypeDef *pdev);
static void USBD_Composite_Switch_CDC(USBD_HandleTypeDef *pdev);


USBD_ClassTypeDef  USBD_Mult =
{
  USBD_Composite_Init,
  USBD_Composite_DeInit,
  USBD_Composite_Setup,
  NULL, /*EP0_TxSent*/
  USBD_Composite_RxReady, /*EP0_RxReady*/
  USBD_Composite_DataIn,
  USBD_Composite_DataOut,
  NULL, /*SOF */
  NULL,
  NULL,
  USBD_Composite_GetHSCfgDesc,
  USBD_Composite_GetFSCfgDesc,
  USBD_Composite_GetOtherSpeedCfgDesc,
  USBD_Composite_GetDeviceQualifierDescriptor,
};


/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
__ALIGN_BEGIN uint8_t USBD_Composite_CfgDesc[USB_MC_CONFIG_DESC_SIZ] __ALIGN_END =
{
  /*Configuration Descriptor*/
  0x09,   /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION,      /* bDescriptorType: Configuration */
  USB_MC_CONFIG_DESC_SIZ,           /* wTotalLength:no of returned bytes */
  0x00,
  0x03,   /* bNumInterfaces: 3 interface */ /* +++lakun��CDC���������ӿڣ�MSC����һ���ӿڣ�������3 */
  0x01,   /* bConfigurationValue: Configuration value */
  0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
  0xC0,   /* bmAttributes: self powered */
  0x32,   /* MaxPower 0 mA */

  /*---------------------------------------------------------------------------*/

  //
  // +++lakun: IAD(Interface Association Descriptor)������ָʾCDC
  //
  0X08,  // bLength: Interface Descriptor size���̶�ֵ
  0X0B,  // bDescriptorType: IAD���̶�ֵ
  0X00,  // bFirstInterface����һ���ӿڵ���ʼ��ţ���0��ʼ
  0X02,  // bInterfaceCount����IAD�µĽӿ�����
  0X02,  // bFunctionClass: CDC��������IAD��һ��CDC���͵��豸
  0X02,  // bFunctionSubClass�������ͣ�Ĭ�ϼ���
  0X01,  // bFunctionProtocol������Э�飬Ĭ�ϼ���
  0X00,  // iFunction

  /*Interface Descriptor */
  0x09,   /* bLength: Interface Descriptor size */
  USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x00,   /* bInterfaceNumber: Number of Interface */                   /* +++lakun:�ӿڱ��  */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x01,   /* bNumEndpoints: One endpoints used */
  0x02,   /* bInterfaceClass: Communication Interface Class */          /* +++lakun:��������һ��ͨ�Žӿ� */
  0x02,   /* bInterfaceSubClass: Abstract Control Model */
  0x01,   /* bInterfaceProtocol: Common AT commands */
  0x00,   /* iInterface: */

  /*Header Functional Descriptor*/
  0x05,   /* bLength: Endpoint Descriptor size */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x00,   /* bDescriptorSubtype: Header Func Desc */
  0x10,   /* bcdCDC: spec release number */
  0x01,

  /*Call Management Functional Descriptor*/
  0x05,   /* bFunctionLength */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x01,   /* bDescriptorSubtype: Call Management Func Desc */
  0x00,   /* bmCapabilities: D0+D1 */
  0x01,   /* bDataInterface: 1 */

  /*ACM Functional Descriptor*/
  0x04,   /* bFunctionLength */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
  0x02,   /* bmCapabilities */

  /*Union Functional Descriptor*/
  0x05,   /* bFunctionLength */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x06,   /* bDescriptorSubtype: Union func desc */
  0x00,   /* bMasterInterface: Communication class interface */           /* +++lakun:����������ָʾCDCͨ�Žӿڵı�ŵ� */
  0x01,   /* bSlaveInterface0: Data Class Interface */                    /* +++lakun:����������ָʾCDC���ݽӿڵı�ŵ� */

  /*Endpoint 2 Descriptor*/
  0x07,                           /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
  CDC_CMD_EP,                     /* bEndpointAddress */
  0x03,                           /* bmAttributes: Interrupt */
  LOBYTE(CDC_CMD_PACKET_SIZE),     /* wMaxPacketSize: */
  HIBYTE(CDC_CMD_PACKET_SIZE),
  CDC_FS_BINTERVAL,                           /* bInterval: */
  /*---------------------------------------------------------------------------*/

  /*Data class interface descriptor*/
  0x09,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: */
  0x01,   /* bInterfaceNumber: Number of Interface */                     /* +++lakun:CDC���ݽӿڵı��Ϊ1 */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x02,   /* bNumEndpoints: Two endpoints used */
  0x0A,   /* bInterfaceClass: CDC */
  0x00,   /* bInterfaceSubClass: */
  0x00,   /* bInterfaceProtocol: */
  0x00,   /* iInterface: */

  /*Endpoint OUT Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  CDC_OUT_EP,                        /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
  HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
  0x00,                              /* bInterval: ignore for Bulk transfer */

  /*Endpoint IN Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  CDC_IN_EP,                         /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
  HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
  0x00,                               /* bInterval: ignore for Bulk transfer */

  //
  // +++lakun: IAD(Interface Association Descriptor)
  //
  0X08,  // bLength: Interface Descriptor size���̶�ֵ
  0X0B,  // bDescriptorType: IAD���̶�ֵ
  0X02,  // bFirstInterface���ӿڵ���ʼ��ţ���0��1��ŵĽӿ�����CDC1�����Դ�2��ʼ��
  0X01,  // bInterfaceCount����IAD�µĽӿ�����
  0X08,  // bFunctionClass: MSC��������IAD��һ��MSC���͵��豸
  0X06,  // bFunctionSubClass�������ͣ�Ĭ�ϼ���
  0X50,  // bFunctionProtocol������Э�飬Ĭ�ϼ���
  0X05,  // iFunction

  /********************  Mass Storage interface ********************/
  0x09,   /* bLength: Interface Descriptor size */
  0x04,   /* bDescriptorType: */
  0x02,   /* bInterfaceNumber: Number of Interface */  /* +++lakun:��0��1��ŵ��ø���CDC������MSC�ӿڵı�Ŵ�2��ʼ */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x02,   /* bNumEndpoints*/
  0x08,   /* bInterfaceClass: MSC Class */
  0x06,   /* bInterfaceSubClass : SCSI transparent*/
  0x50,   /* nInterfaceProtocol */
  0x05,          /* iInterface: */
  /********************  Mass Storage Endpoints ********************/
  0x07,   /*Endpoint descriptor length = 7*/
  0x05,   /*Endpoint descriptor type */
  MSC_EPIN_ADDR,   /*Endpoint address (IN, address 1) */
  0x02,   /*Bulk endpoint type */
  LOBYTE(MSC_MAX_FS_PACKET),
  HIBYTE(MSC_MAX_FS_PACKET),
  0x00,   /*Polling interval in milliseconds */

  0x07,   /*Endpoint descriptor length = 7 */
  0x05,   /*Endpoint descriptor type */
  MSC_EPOUT_ADDR,   /*Endpoint address (OUT, address 1) */
  0x02,   /*Bulk endpoint type */
  LOBYTE(MSC_MAX_FS_PACKET),
  HIBYTE(MSC_MAX_FS_PACKET),
  0x00     /*Polling interval in milliseconds*/
} ;


/* USB Standard Device Descriptor */
uint8_t USBD_Composite_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0X40,
  0x01,
  0x00,
};


// ����������޸ĵ�USBD_MSC_Init
static void USBD_Composite_MSC_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
	USBD_Composite_Switch_MSC(pdev);

	/* Open EP OUT */
	USBD_LL_OpenEP(pdev, MSC_EPOUT_ADDR, USBD_EP_TYPE_BULK, MSC_MAX_FS_PACKET);
	pdev->ep_out[MSC_EPOUT_ADDR & 0xFU].is_used = 1U;

	/* Open EP IN */
	USBD_LL_OpenEP(pdev, MSC_EPIN_ADDR, USBD_EP_TYPE_BULK, MSC_MAX_FS_PACKET);
	pdev->ep_in[MSC_EPIN_ADDR & 0xFU].is_used = 1U;

	/* Init the BOT  layer */
	MSC_BOT_Init(pdev);
}

// ����������޸ĵ�USBD_CDC_Init
static void USBD_Composite_CDC_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
	USBD_CDC_HandleTypeDef   *hcdc = NULL;

	USBD_Composite_Switch_CDC(pdev);

	/* Open EP IN */
	USBD_LL_OpenEP(pdev, CDC_IN_EP, USBD_EP_TYPE_BULK, CDC_DATA_FS_IN_PACKET_SIZE);
	pdev->ep_in[CDC_IN_EP & 0xFU].is_used = 1U;

	/* Open EP OUT */
	USBD_LL_OpenEP(pdev, CDC_OUT_EP, USBD_EP_TYPE_BULK,CDC_DATA_FS_OUT_PACKET_SIZE);
	pdev->ep_out[CDC_OUT_EP & 0xFU].is_used = 1U;

	/* Open Command IN EP */
	USBD_LL_OpenEP(pdev, CDC_CMD_EP, USBD_EP_TYPE_INTR, CDC_CMD_PACKET_SIZE);
	pdev->ep_in[CDC_CMD_EP & 0xFU].is_used = 1U;

	hcdc = (USBD_CDC_HandleTypeDef *) pdev->pClassData;

	/* Init  physical Interface components */
	((USBD_CDC_ItfTypeDef *)pdev->pUserData)->Init();

	/* Init Xfer states */
	hcdc->TxState = 0U;
	hcdc->RxState = 0U;

	/* Prepare Out endpoint to receive next packet */
	USBD_LL_PrepareReceive(pdev, CDC_OUT_EP, hcdc->RxBuffer, CDC_DATA_FS_OUT_PACKET_SIZE);
}

uint8_t USBD_Composite_Init (USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
	USBD_Composite_MSC_Init(pdev, cfgidx);  // ��ʼ��MSC
	USBD_Composite_CDC_Init(pdev, cfgidx);  // ��ʼ��CDC
	return USBD_OK;
}

uint8_t  USBD_Composite_DeInit (USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
	return USBD_OK;
}

uint8_t  USBD_Composite_Setup (USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
	switch(req->wIndex)  // wIndex�ǽӿڱ�ţ��������������������ǵ�һ��IAD��CDCʹ�õ��Ǳ��0��1�����ӿڣ��ڶ���IAD��MSCʹ�õ��Ǳ��2�Ľӿ�
	{

	//
	// ��һ��IAD
	//
	case 0:  // CDC������ӿڱ��Ϊ0
	case 1:  // CDC�����ݽӿڱ��Ϊ1
		USBD_Composite_Switch_CDC(pdev);
		USBD_CDC_Setup(pdev, req);
		break;

	//
	// �ڶ���IAD
	//
	case 2:  // MSCֻ��һ���ӿ�,���Ϊ2
		USBD_Composite_Switch_MSC(pdev);
		USBD_MSC_Setup(pdev, req);
		break;

	//
	// ������IAD������У��������ʼ����
	//
	case 3:
		break;
	default:break;
	}
	return USBD_OK;
}

uint8_t  USBD_Composite_DataIn (USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	switch(epnum | 0X80)
	{
	case MSC_EPIN_ADDR:
	    USBD_Composite_Switch_MSC(pdev);
	    USBD_MSC_DataIn(pdev, epnum);
		break;
	case CDC_IN_EP:
	    USBD_Composite_Switch_CDC(pdev);
	    USBD_CDC_DataIn(pdev, epnum);
		break;
	default:break;
	}

	return USBD_OK;
}

uint8_t  USBD_Composite_DataOut (USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	switch(epnum)
	{
	case MSC_EPOUT_ADDR:
		USBD_Composite_Switch_MSC(pdev);
		USBD_MSC_DataOut(pdev, epnum);
		break;
	case CDC_OUT_EP:
		USBD_Composite_Switch_CDC(pdev);
		USBD_CDC_DataOut(pdev, epnum);
		break;
	default:break;
	}
	return USBD_OK;
}

uint8_t  *USBD_Composite_GetHSCfgDesc (uint16_t *length)
{
  *length = sizeof (USBD_Composite_CfgDesc);
  return USBD_Composite_CfgDesc;
}

uint8_t  *USBD_Composite_GetFSCfgDesc (uint16_t *length)
{
  *length = sizeof (USBD_Composite_CfgDesc);
  return USBD_Composite_CfgDesc;
}

uint8_t  *USBD_Composite_GetOtherSpeedCfgDesc (uint16_t *length)
{
  *length = sizeof (USBD_Composite_CfgDesc);
  return USBD_Composite_CfgDesc;
}

uint8_t  *USBD_Composite_GetDeviceQualifierDescriptor (uint16_t *length)
{
  *length = sizeof (USBD_Composite_DeviceQualifierDesc);
  return USBD_Composite_DeviceQualifierDesc;
}

static uint8_t  USBD_Composite_RxReady (USBD_HandleTypeDef *pdev)
{
	uint8_t ret = 0;
	switch(pdev->request.wIndex)  // wIndex�ǽӿڱ�ţ���������ͨ���ӿڱ��ȷ��CDC���������豸��EP0����
	{
		case 0:  // CDC������ӿڱ����0
		case 1:  // CDC�����ݽӿڱ����1
			USBD_Composite_Switch_CDC(pdev);
			ret = USBD_CDC_EP0_RxReady(pdev);
			break;
		// ����������豸���õ���EP0���գ����������
		case 2:
			break;
		default:break;
	}
    return ret;
}

static void USBD_Composite_Switch_MSC(USBD_HandleTypeDef *pdev)
{
	static USBD_MSC_BOT_HandleTypeDef USBD_MSC_Handle;
	USBD_MSC_RegisterStorage(pdev, &USBD_DISK_fops);
	pdev->pClassData = (void *)&USBD_MSC_Handle;
}

static void USBD_Composite_Switch_CDC(USBD_HandleTypeDef *pdev)
{
	static USBD_CDC_HandleTypeDef USBD_CDC_Handle;
	USBD_CDC_RegisterInterface(pdev, &USBD_CDC_fops);
	pdev->pClassData = (void *)&USBD_CDC_Handle;
}

