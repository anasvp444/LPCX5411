/*
 * GENERATED FILE - DO NOT EDIT
 * (c) Code Red Technologies Ltd, 2008-2013
 * (c) NXP Semiconductors 2013-2017
 * Generated linker script file for LPC54114J256
 * Created from memory.ldt by FMCreateLinkMemory
 * Using Freemarker v2.3.23
 * LPCXpresso v8.2.2 [Build 650] [2016-09-09]  on Dec 30, 2017 3:50:37 PM
 */

MEMORY
{
  /* Define each memory region */
  MFlash256 (rx) : ORIGIN = 0x0, LENGTH = 0x40000 /* 256K bytes (alias Flash) */  
  Ram0_64 (rwx) : ORIGIN = 0x20000000, LENGTH = 0x10000 /* 64K bytes (alias RAM) */  
  Ram1_64 (rwx) : ORIGIN = 0x20010000, LENGTH = 0x10000 /* 64K bytes (alias RAM2) */  
  Ram3_32 (rwx) : ORIGIN = 0x20020000, LENGTH = 0x8000 /* 32K bytes (alias RAM3) */  
  RamX_32 (rwx) : ORIGIN = 0x4000000, LENGTH = 0x8000 /* 32K bytes (alias RAM4) */  
}

  /* Define a symbol for the top of each memory region */
  __base_MFlash256 = 0x0  ; /* MFlash256 */  
  __base_Flash = 0x0 ; /* Flash */  
  __top_MFlash256 = 0x0 + 0x40000 ; /* 256K bytes */  
  __top_Flash = 0x0 + 0x40000 ; /* 256K bytes */  
  __base_Ram0_64 = 0x20000000  ; /* Ram0_64 */  
  __base_RAM = 0x20000000 ; /* RAM */  
  __top_Ram0_64 = 0x20000000 + 0x10000 ; /* 64K bytes */  
  __top_RAM = 0x20000000 + 0x10000 ; /* 64K bytes */  
  __base_Ram1_64 = 0x20010000  ; /* Ram1_64 */  
  __base_RAM2 = 0x20010000 ; /* RAM2 */  
  __top_Ram1_64 = 0x20010000 + 0x10000 ; /* 64K bytes */  
  __top_RAM2 = 0x20010000 + 0x10000 ; /* 64K bytes */  
  __base_Ram3_32 = 0x20020000  ; /* Ram3_32 */  
  __base_RAM3 = 0x20020000 ; /* RAM3 */  
  __top_Ram3_32 = 0x20020000 + 0x8000 ; /* 32K bytes */  
  __top_RAM3 = 0x20020000 + 0x8000 ; /* 32K bytes */  
  __base_RamX_32 = 0x4000000  ; /* RamX_32 */  
  __base_RAM4 = 0x4000000 ; /* RAM4 */  
  __top_RamX_32 = 0x4000000 + 0x8000 ; /* 32K bytes */  
  __top_RAM4 = 0x4000000 + 0x8000 ; /* 32K bytes */  
