// To compile this simple example:
// $ gcc -o quick_start_example1 quick_start_example1.c -lnfc -Wall -lwiringPi

#include "includes.h"

/**
 * @brief	Constructor of the class 
 */
NFC::NFC ()
{
	Context = NULL;
	NFCDevice = NULL;
}

nfc_target* NFC::GetNFCTarget()
{
	return &NFCTarget;
}

/**
 * @brief	This function initializes the nfc class
 * 
 * @return	bool 	returns the status of the function
 */
bool NFC::Initialize ()
{
	// Initialize libnfc and set the nfc_context
	nfc_init(&Context);

	if (Context == NULL) {
		//Case: Unable to initialise the libnfc

		printf("Unable to init libnfc (malloc)\n");

		return false;
	}

	// Open, using the first available NFC device which can be in order of selection:
	//   - default device specified using environment variable or
	//   - first specified device in libnfc.conf (/etc/nfc) or
	//   - first specified device in device-configuration directory (/etc/nfc/devices.d) or
	//   - first auto-detected (if feature is not disabled in libnfc.conf) device
	NFCDevice = nfc_open(Context, NULL);

	if (NFCDevice == NULL) {
		//Case: Unable to open NFC device

		printf("ERROR: Unable to open NFC device.\n");

		// free the allocated Context
		free (Context);

		return false;
	}

	// Set opened NFC device to initiator mode
	if (nfc_initiator_init(NFCDevice) < 0) {
		//Case: Unable to set the device to initiator mode

		nfc_perror(NFCDevice, "nfc_initiator_init");

		// free the allocated NFC Context and Device
		free (Context);
		free (NFCDevice);

		return false;
	}

	return true;
}

void NFC::StartPolling()
{
	// Poll for a ISO14443A (MIFARE) tag
	const nfc_modulation nmMifare = {
	  .nmt = NMT_ISO14443A,
	  .nbr = NBR_106,
	};
	if (nfc_initiator_select_passive_target(NFCDevice, nmMifare, NULL, 0, &NFCTarget) > 0) {
		//Case: Got the signal from a NFC device

		//Display the information of the device
		DisplayInformation ();
	}
}

/**
 * @brief	This function finalizes the member variables
 * 
 */
void NFC::Finalize()
{
	// Close NFC device
	nfc_close(NFCDevice);

	// Release the context
	nfc_exit(Context);
}

void NFC::DisplayInformation()	
{
	printf("NFC reader: %s opened\n", nfc_device_get_name(NFCDevice));

	printf("The following (NFC) ISO14443A tag was found:\n");
    printf("    ATQA (SENS_RES): ");
    print_hex(NFCTarget.nti.nai.abtAtqa, 2);
    printf("       UID (NFCID%c): ", (NFCTarget.nti.nai.abtUid[0] == 0x08 ? '3' : '1'));
    print_hex(NFCTarget.nti.nai.abtUid, NFCTarget.nti.nai.szUidLen);
    printf("      SAK (SEL_RES): ");
    print_hex(&NFCTarget.nti.nai.btSak, 1);
    if (NFCTarget.nti.nai.szAtsLen) {
      printf("          ATS (ATR): ");
      print_hex(NFCTarget.nti.nai.abtAts, NFCTarget.nti.nai.szAtsLen);
    }
}


void NFC::print_hex(const uint8_t *pbtData, const size_t szBytes)
{
  size_t  szPos;

  for (szPos = 0; szPos < szBytes; szPos++) {
    printf("%02x  ", pbtData[szPos]);
  }
  printf("\n");
}

