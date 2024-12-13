// Header predefiniti per il protocollo seriale
#define SERIAL_HEADER            0xAA          // Header predefinito
#define SERIAL_END_PACKET        0x0A          // '\n' - Byte per la fine del pacchetto

// Definizione degli header per i vari payload

// DMX Layout
#define SERIAL_HEADER_DMXLEYOUT               0x0A
#define SERIAL_HEADER_GETDMXLEYOUT_ASCII      0x1A
#define SERIAL_HEADER_GETDMXLEYOUT_BYTE       0x2A

// DMX Profiles
#define SERIAL_HEADER_DMXPROFILES            0x0B
#define SERIAL_HEADER_GETDMXPROFILES_ASCII   0x1B
#define SERIAL_HEADER_GETDMXPROFILES_BYTE    0x2B

// Firmware and Filesystem
#define SERIAL_HEADER_GETFWVERS              0x0C
#define SERIAL_HEADER_GETFWVERS_ASCII        0x3C
#define SERIAL_HEADER_GETSPIFFS_LIST_ASCII   0x1C
#define SERIAL_HEADER_GETSPIFFSUSAGE_ASCII   0x2C

// SPIFFS commands
#define SERIAL_HEADER_CLEARSPIFFS            0x0F
#define SERIAL_HEADER_REFRESH_SPIFFS         0x1F

// Discovery command
#define SERIAL_HEADER_DISCOVERY              0xEF
