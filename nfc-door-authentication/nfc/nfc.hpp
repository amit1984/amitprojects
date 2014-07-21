/**
 * @file	nfc.hpp 
 *  
 * @bri		This file contains the definition of the nfc 
 *      	class
 *  
 * @author	Arpit Jindal 
 */

#ifndef NFC_HPP
#define NFC_HPP


class NFC 
{
public:

    /**
     * @brief	Constructor and Destructor
     */
    NFC ();
    //~NFC ();

	/**
	 * @brief	Getter function
	 */
	nfc_target* GetNFCTarget ();

    bool Initialize ();
	void DisplayInformation ();
	void StartPolling ();
	void Finalize ();

	void print_hex(const uint8_t *pbtData, const size_t szBytes);

private:

    nfc_context *Context;		///> Pointer to nfc_context
	nfc_device * NFCDevice;		///> Reference pointing to the NFC device
	nfc_target NFCTarget;

};

#endif	//NFC_HPP
