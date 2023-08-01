//**************************************************************************
//
//  Copyright © 1999-2008, Axeda Corporation.  All rights reserved
//
//**************************************************************************
//
//  Filename   :  CRYPT_RANGE.H
//  
//
//  Description:  Valid character ranges used by Encrypt and Decrypt ASCII chars
//
//
//**************************************************************************

/////////////////////////////////////////////////////////////////////////////
//
// IS_ASCII_CRYPT_RANGE is used by ASCII encryp/decrypt for password and license.
//
/////////////////////////////////////////////////////////////////////////////
#define IS_ASCII_CRYPT_RANGE(a) ((((a) >= '0' ) && ((a) <= '9')) || (((a) >= 'A' ) && ((a) <= 'Z')) || (((a) >= 'a' ) && ((a) <= 'z')) || (a) == ' ')

