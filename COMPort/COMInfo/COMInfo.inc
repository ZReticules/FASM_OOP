define DIGCF_DEFAULT           				0x00000001
define DIGCF_PRESENT           				0x00000002
define DIGCF_ALLCLASSES        				0x00000004
define DIGCF_PROFILE           				0x00000008
define DIGCF_DEVICEINTERFACE   				0x00000010

define DICS_FLAG_GLOBAL         			0x00000001
define DICS_FLAG_CONFIGSPECIFIC 			0x00000002
define DICS_FLAG_CONFIGGENERAL  			0x00000004 

define DIREG_DEV       						0x00000001
define DIREG_DRV       						0x00000002
define DIREG_BOTH      						0x00000004

define KEY_QUERY_VALUE         				0x0001
define KEY_SET_VALUE           				0x0002
define KEY_CREATE_SUB_KEY      				0x0004
define KEY_ENUMERATE_SUB_KEYS  				0x0008
define KEY_NOTIFY              				0x0010
define KEY_CREATE_LINK         				0x0020
define KEY_WOW64_32KEY         				0x0200
define KEY_WOW64_64KEY         				0x0100
define KEY_WOW64_RES           				0x0300

define SPDRP_DEVICEDESC                  	0x00000000;  // DeviceDesc (R/W)
define SPDRP_HARDWAREID                  	0x00000001;  // HardwareID (R/W)
define SPDRP_COMPATIBLEIDS               	0x00000002;  // CompatibleIDs (R/W)
define SPDRP_UNUSED0                     	0x00000003;  // unused
define SPDRP_SERVICE                     	0x00000004;  // Service (R/W)
define SPDRP_UNUSED1                     	0x00000005;  // unused
define SPDRP_UNUSED2                     	0x00000006;  // unused
define SPDRP_CLASS                       	0x00000007;  // Class (R--tied to ClassGUID)
define SPDRP_CLASSGUID                   	0x00000008;  // ClassGUID (R/W)
define SPDRP_DRIVER                      	0x00000009;  // Driver (R/W)
define SPDRP_CONFIGFLAGS                 	0x0000000A;  // ConfigFlags (R/W)
define SPDRP_MFG                         	0x0000000B;  // Mfg (R/W)
define SPDRP_FRIENDLYNAME                	0x0000000C;  // FriendlyName (R/W)
define SPDRP_LOCATION_INFORMATION        	0x0000000D;  // LocationInformation (R/W)
define SPDRP_PHYSICAL_DEVICE_OBJECT_NAME 	0x0000000E;  // PhysicalDeviceObjectName (R)
define SPDRP_CAPABILITIES                	0x0000000F;  // Capabilities (R)
define SPDRP_UI_NUMBER                   	0x00000010;  // UiNumber (R)
define SPDRP_UPPERFILTERS                	0x00000011;  // UpperFilters (R/W)
define SPDRP_LOWERFILTERS                	0x00000012;  // LowerFilters (R/W)
define SPDRP_BUSTYPEGUID                 	0x00000013;  // BusTypeGUID (R)
define SPDRP_LEGACYBUSTYPE               	0x00000014;  // LegacyBusType (R)
define SPDRP_BUSNUMBER                   	0x00000015;  // BusNumber (R)
define SPDRP_ENUMERATOR_NAME             	0x00000016;  // Enumerator Name (R)
define SPDRP_SECURITY                    	0x00000017;  // Security (R/W, binary form)
define SPDRP_SECURITY_SDS                	0x00000018;  // Security (W, SDS form)
define SPDRP_DEVTYPE                     	0x00000019;  // Device Type (R/W)
define SPDRP_EXCLUSIVE                   	0x0000001A;  // Device is exclusive-access (R/W)
define SPDRP_CHARACTERISTICS             	0x0000001B;  // Device Characteristics (R/W)
define SPDRP_ADDRESS                     	0x0000001C;  // Device Address (R)
define SPDRP_UI_NUMBER_DESC_FORMAT       	0X0000001D;  // UiNumberDescFormat (R/W)
define SPDRP_DEVICE_POWER_DATA           	0x0000001E;  // Device Power Data (R)
define SPDRP_REMOVAL_POLICY              	0x0000001F;  // Removal Policy (R)
define SPDRP_REMOVAL_POLICY_HW_DEFAULT   	0x00000020;  // Hardware Removal Policy (R)
define SPDRP_REMOVAL_POLICY_OVERRIDE     	0x00000021;  // Removal Policy Override (RW)
define SPDRP_INSTALL_STATE               	0x00000022;  // Device Install State (R)
define SPDRP_LOCATION_PATHS              	0x00000023;  // Device Location Paths (R)
define SPDRP_BASE_CONTAINERID            	0x00000024;  // Base ContainerID (R)

define SPDRP_MAXIMUM_PROPERTY            	0x00000025;  // Upper bound on ordinals

struct GUID
	Data1 dd ?
	Data2 dw ?
	Data3 dw ?
	Data4 db 8 dup (?);
ends

struct SP_DEVINFO_DATA 
    cbSize dd sizeof.SP_DEVINFO_DATA;
    ClassGuid GUID
    DevInst dd ?;    // DEVINST handle
    Reserved dq ?
ends

struct SP_DEVICE_INTERFACE_DATA 
  cbSize				dd sizeof.SP_DEVINFO_DATA
  InterfaceClassGuid	GUID  
  Flags					dd ?
  Reserved 				dq ?
ends

struct COMInfo
	hDevInfoSet 	dq ?
	devInfo 		SP_DEVINFO_DATA
	; devIface 		SP_DEVICE_INTERFACE_DATA
	countPorts 		dw ?
	init 			dm this
	getPortName		dm this
	getPortDescLen	dm this
	getPortInfo		dm this
	choseId			dm this
	const desc 		= SPDRP_DEVICEDESC
	const MFG 		= SPDRP_MFG
	const physName 	= SPDRP_PHYSICAL_DEVICE_OBJECT_NAME
	const service 	= SPDRP_SERVICE
ends

include "COMInfo.asm"