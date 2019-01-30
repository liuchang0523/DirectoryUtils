/*****************************************************************************
*  Orbbec DispIP 3.0
*  Copyright (C) 2018 by ORBBEC Technology., Inc.
*
*  This file is part of Orbbec DispIP.
*
*  This file belongs to ORBBEC Technology., Inc.
*  It is considered a trade secret, and is not to be divulged or used by
* parties who have NOT received written authorization from the owner.
*
*  Description:
*
****************************************************************************/
#ifndef ___VERSION_H__
#define ___VERSION_H__

#define DirectoryUtils_VERSION_MAJOR 0
#define DirectoryUtils_VERSION_MINOR 0
#define DirectoryUtils_VERSION_PATCH 1

#define DirectoryUtils_STR_EXP(__A)    #__A
#define DirectoryUtils_STR(__A)        DirectoryUtils_STR_EXP(__A)

#define DirectoryUtils_STRW_EXP(__A)   L#__A
#define DirectoryUtils_STRW(__A)       DirectoryUtils_STRW_EXP(__A)

#define VERSION     DirectoryUtils_STR(0) "." DirectoryUtils_STR(0) "." DirectoryUtils_STR(1)


#endif // ___VERSION_H__
