/*
   This file is part of ArduinoIoTCloud.

   Copyright 2019 ARDUINO SA (http://www.arduino.cc/)

   This software is released under the GNU General Public License version 3,
   which covers the main part of arduino-cli.
   The terms of this license can be found at:
   https://www.gnu.org/licenses/gpl-3.0.en.html

   You can be released from the requirements of the above licenses by purchasing
   a commercial license. Buying such a license is mandatory if you want to modify or
   otherwise use the software for commercial activities involving the Arduino
   software without disclosing the source code of your own applications. To purchase
   a commercial license, send an email to license@arduino.cc.
*/

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include <AIoTC_Config.h>

#if OTA_STORAGE_PORTENTA
#include <ArduinoOTAPortenta.h>
//#include "utility/ota/FlashSHA256.h"
#include "OTAStorage_Internal_Flash_Raw.h"
#include "OTAStorage_Portenta_Qspi_Flash.h"
#include "OTAStorage_Portenta_SD.h"


/******************************************************************************
   GLOBAL VARIABLES
 ******************************************************************************/

  static OTAStorage_Internal_Flash_Raw ota_storage_internal_flash_raw;
  //static OTAStorage_Internal_Flash_offset ota_storage_internal_flash_offset;
  static OTAStorage_Portenta_Qspi_Flash ota_storage_qspi_flash;
  static OTAStorage_Portenta_SD ota_storage_sd;

/******************************************************************************
   GLOBAL CONSTANTS
 ******************************************************************************/

static const int TIMEOUT_FOR_LASTVALUES_SYNC = 10000;

/******************************************************************************
   LOCAL MODULE FUNCTIONS
 ******************************************************************************/
/*
extern "C" unsigned long getTime()
{
  return ArduinoCloud.getInternalTime();
}
*/
/******************************************************************************
   CTOR/DTOR
 ******************************************************************************/

ArduinoOTAPortenta::ArduinoOTAPortenta() :
#if OTA_ENABLED
  _ota_error{static_cast<int>(OTAError::None)}
//, _ota_img_sha256{"Inv."}
, _storagePortenta{SD}
#endif /* OTA_ENABLED */
{

}

/******************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

void ArduinoOTAPortenta::begin(storageTypePortenta storage)  //PortentaStorageType PortentaStorage
{

  Serial1.begin(115200);
  Serial1.print("ArduinoOTAPortenta begin. Storage = ");
  Serial1.println(storage);

  //_storagePortenta = storage;

  //_storagePortenta = PortentaStorage;
  if (storage==INTERNAL_FLASH_OFFSET || storage==INTERNAL_FLASH_FATFS || storage==INTERNAL_FLASH_LITTLEFS) {
    Serial1.println("Internal falsh storage");
    setOTAStorage(ota_storage_internal_flash_raw, storage);
  } else if (storage==QSPI_FLASH_OFFSET || storage==QSPI_FLASH_FATFS || storage==QSPI_FLASH_LITTLEFS || storage==QSPI_FLASH_FATFS_MBR || storage==QSPI_FLASH_LITTLEFS_MBR) {
    Serial1.println("QSPI falsh storage");
    setOTAStorage(ota_storage_qspi_flash, storage);
  } else if (storage==SD_OFFSET || storage==SD_FATFS || storage==SD_LITTLEFS || storage==SD_FATFS_MBR || storage==SD_LITTLEFS_MBR) {
    Serial1.println("SD storage");
    setOTAStorage(ota_storage_sd, storage);
  } else {
    Serial1.println("Invalid storage type");
    delay(200);
  }
  /*
  switch (storage) {
    case InternalFlashRaw: //InternalFlashRaw:
      setOTAStorage(ota_storage_internal_flash_raw, INTERNAL_FLASH_OFFSET);
    break;

    case InternalFlashOffset: //InternalFlashOffset:
      //setOTAStorage(ota_storage_internal_flash_offset);
    break;

    case QSPIFlash: //QSPIFlash:
      setOTAStorage(ota_storage_qspi_flash);
    break;

    case SD: //SD:
      setOTAStorage(ota_storage_sd);
    break;

    default:
    break;
  }
  */
}

void ArduinoOTAPortenta::update()
{
#if OTA_ENABLED
    /* If a _ota_logic object has been instantiated then we are spinning its
     * 'update' method here in order to process incoming data and generally
     * to transition to the OTA logic update states.
     */
    OTAError const err = _ota_logic_portenta.update();
    _ota_error = static_cast<int>(err);
    //Serial1.print("ArduinoOTAPortenta::update    _ota_error = ");
    //Serial1.println(_ota_error);
#endif /* OTA_ENABLED */
}

int ArduinoOTAPortenta::connected()
{
  //return _mqttClient.connected();
  return 1;
}
/*
void ArduinoOTAPortenta::printDebugInfo()
{
  DBG_INFO("***** Arduino IoT Cloud for Portenta - configuration info *****");
  //DBG_INFO("Device ID: %s", getDeviceId().c_str());
  //DBG_INFO("Thing ID: %s", getThingId().c_str());
  //DBG_INFO("MQTT Broker: %s:%d", _brokerAddress.c_str(), _brokerPort);
}
*/
//#if OTA_ENABLED
void ArduinoOTAPortenta::setOTAStorage(OTAStoragePortenta & ota_storage, storageTypePortenta storageType)
{
  Serial1.println("ArduinoOTAPortenta::setOTAStorage     storageType = ");
  Serial1.println(storageType);
  _ota_logic_portenta.setOTAStorage(ota_storage, storageType);
}
//#endif /* OTA_ENABLED */

/******************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 ******************************************************************************/

/******************************************************************************
 * EXTERN DEFINITION
 ******************************************************************************/

ArduinoOTAPortenta OTAPortenta;

#endif  /*OTA_STORAGE_PORTENTA*/
