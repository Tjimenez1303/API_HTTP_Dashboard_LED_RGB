# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/ASUS/esp-idf/components/bootloader/subproject"
  "C:/Users/ASUS/OneDrive/Escritorio/Proyectos_ESP_32/16.Leds_Threshold_API_WEB_V2/ESP32-SMB-API/build/bootloader"
  "C:/Users/ASUS/OneDrive/Escritorio/Proyectos_ESP_32/16.Leds_Threshold_API_WEB_V2/ESP32-SMB-API/build/bootloader-prefix"
  "C:/Users/ASUS/OneDrive/Escritorio/Proyectos_ESP_32/16.Leds_Threshold_API_WEB_V2/ESP32-SMB-API/build/bootloader-prefix/tmp"
  "C:/Users/ASUS/OneDrive/Escritorio/Proyectos_ESP_32/16.Leds_Threshold_API_WEB_V2/ESP32-SMB-API/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/ASUS/OneDrive/Escritorio/Proyectos_ESP_32/16.Leds_Threshold_API_WEB_V2/ESP32-SMB-API/build/bootloader-prefix/src"
  "C:/Users/ASUS/OneDrive/Escritorio/Proyectos_ESP_32/16.Leds_Threshold_API_WEB_V2/ESP32-SMB-API/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/ASUS/OneDrive/Escritorio/Proyectos_ESP_32/16.Leds_Threshold_API_WEB_V2/ESP32-SMB-API/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
