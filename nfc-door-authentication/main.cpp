/**
 * @file	main.c 
 *  
 * @brief	This file contains the main function. 
 *  
 * @author Amit Bose and Arpit Jindal 
 */

#include "includes.h"
#define FILENAME "nfclist"

void InsertData (NFC &nfcObject)
{
	ofstream file;
	nfc_target* target;
	
	//Opening the file
	file.open(FILENAME, ios_base::app);

	if (file.is_open ()) {
		//Case: File is opened

		//Get the reference of the target device
		target = nfcObject.GetNFCTarget();

		//Check target device
		if (target == NULL) {
			//Case: No target device
			printf ("Error: Provide target device\n");

			return;
		}

		//Insert the data into the file
		file<<(const char *)target->nti.nai.abtUid;

		//insert newline after each data
		file<<"\n";

		//Close the file
		file.close ();

		return;
	}

	printf("Error in Inserting data\n");
}

void SendSignal ()
{
	//Setting up WiringPi
	wiringPiSetup () ;

	//setting pin 0 to output mode
	pinMode (0, OUTPUT) ;

	//Sending High signal and than Low signal
	digitalWrite (0, HIGH) ; delay (500) ;
	digitalWrite (0,  LOW) ; delay (500) ;
}

void AuthenticateData (NFC &nfcObject)
{
	ifstream file;
	nfc_target* target;
	string data;
	bool flag;
	
	//Opening the file
	file.open(FILENAME, ios::in);

	//Allocating memory
	data = (char *)malloc (10 * sizeof(char));

	if (!file.is_open ()) {
		//Case: file unable to open

		printf("Error in Authenticating data\n");

		return;
	}

	//Get the reference of the target device
	target = nfcObject.GetNFCTarget();

	//Reading the data from the file
	while (getline (file, data)) {

		//if (!data.compare (0, 10, (const char *) target->nti.nai.abtUid))
		if (!memcmp (target->nti.nai.abtUid, data.data(), 4))
			flag = true;
	}

	file.close ();

	if (!flag) {
		//Case: Data not found

		printf("\nUnauthorised Access\n");

		return;
	}

	printf ("Access Granted\n");

	SendSignal ();
}

int main(int argc, const char *argv[])
{
		int choice;
		NFC nfcObject;

	//Initialising the nfc module
	if (nfcObject.Initialize() == false) {
		//Case: NFC failed to initialise

		printf("\nNFC initialization failed...\n");
		return 0;
	}

	do {
		printf("\nMenu:\n1. Insert NFC Data\n2. Authenticate NFC Data\n0. Exit\nEnter your choice: ");

		//User choice
		scanf ("%d", &choice);

		
		switch (choice) {
		case 1:
			//Polling for the NFC data
			nfcObject.StartPolling();
			InsertData (nfcObject);
			break;

		case 2:
			//Polling for the NFC data
			nfcObject.StartPolling();
			AuthenticateData(nfcObject);
			break;

		case 0:
			//finalising the nfc module
			nfcObject.Finalize();
			exit (0);
			
		default:
			printf("Invalid choice....Please Enter Again\n");
		}
	}while (1);

	return 0;
}
