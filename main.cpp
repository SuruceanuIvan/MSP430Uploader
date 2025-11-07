#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h> // Pentru uint16_t
#include <math.h>

#define TIMEOUT_INTERVAL 500    //[msec]
#define DELAY_AFTER_TRANSMIT 2  //[msec]
#define READ_TIMEOUT_MS 2000    //[msec]

#define DELAY_BYTE 1            //[msec]

// Prototipuri functii
unsigned short  calculateChecksum(const unsigned char *data, int length);
bool            sendByteWithDelay(HANDLE hSerial, unsigned char byte);
bool            sendCommandWithDelay(HANDLE hSerial, const unsigned char *command, int length);
int             readResponse(HANDLE hSerial, unsigned char *buffer, int bufferSize);
void            sendMassErase(HANDLE hSerial);
bool            sendRXPassword(HANDLE hSerial,unsigned const char*);
void            sendTXDataBlock(HANDLE hSerial, uint16_t address, uint16_t length);
bool            interpretResponse(unsigned char *response, int length);

//--------------------------------------------------------------
void            sendRXdataBlock(HANDLE,uint16_t,uint16_t,const unsigned char*);
void            sendLoadPC(HANDLE,uint16_t);
//--------------------------------------------------------------

void            sendSync();
void            sendTXreadBSL(HANDLE);


//--------------------------------------------------------------
//  Functie pentru calcularea checksum-ului
//
//
//--------------------------------------------------------------
unsigned short calculateChecksum(const unsigned char *data, int length)
{
    int i;
    unsigned short checksum = 0;

    for (i = 0; i < length; i += 2)
    {
        unsigned short word = (data[i + 1] << 8) | data[i];
        checksum ^= word;
    }//end for
    return ~checksum;
}//end func
//--------------------------------------------------------------



//--------------------------------------------------------------
// Functie pentru trimiterea unui octet (cu delay)
//
//
//--------------------------------------------------------------
bool sendByteWithDelay(HANDLE hSerial, unsigned char byte)
{
    DWORD bytesWritten;
    if (!WriteFile(hSerial, &byte, 1, &bytesWritten, NULL) || bytesWritten != 1)
    {
        printf("Eroare la trimiterea octetului: %02X\n", byte);
        return false;
    }//end if

    Sleep(DELAY_BYTE);  //intarziere DELAY_BYTE[msec]
    return true;
}//end func
//--------------------------------------------------------------



//--------------------------------------------------------------
// Functie pentru trimiterea unei comenzi octet cu octet
//
//
//--------------------------------------------------------------
bool sendCommandWithDelay(HANDLE hSerial, const unsigned char *command, int length)
{
    int i;
    for (i = 0; i < length; i++)
    {
        if (!sendByteWithDelay(hSerial, command[i]))
        {
            //printf("Eroare la trimiterea octetului: %02X\n", byte);
            return false;
        }//end if
    }//end for
    Sleep(DELAY_AFTER_TRANSMIT);    //intarziere DELAY_AFTER_TRANSMIT[msec]
    return true;
}//end function
//--------------------------------------------------------------



//--------------------------------------------------------------
//  Functie pentru citirea raspunsului
//
//--------------------------------------------------------------
int readResponse(HANDLE hSerial, unsigned char *buffer, int bufferSize)
{
    DWORD bytesRead;
    DWORD startTime = GetTickCount();
    while (GetTickCount() - startTime < READ_TIMEOUT_MS)
    {
        if (ReadFile(hSerial, buffer, bufferSize, &bytesRead, NULL) && bytesRead > 0)
        {
            return bytesRead;
        }//end if
    }//end while
    printf("Timeout la citirea raspunsului.\n");
    return -1;
}//end function
//--------------------------------------------------------------



//--------------------------------------------------------------
//  Functie pentru interpretarea raspunsului
//
//--------------------------------------------------------------
bool interpretResponse(unsigned char *response, int length)
{
    if (length == 0)
    {
        printf("No response received!\n");
        return false;
    }//end if

    if (response[0] == 0x90)
    {
        if (response[1] == 0x00)
        {
            printf("Command successful (DATA_ACK: 0x00).\n");
            //return true;
        }
        else if (response[1] == 0xA0)
        {
            printf("Command refused (DATA_NAK: 0xA0).\n");
            //return false;
        }
    }
    printf("Response: ");

    for (int i = 0; i < length; i++)
    {
        printf("%02X ", response[i]);
    }
    printf("\n");
    return false;
}//end function
//--------------------------------------------------------------



//--------------------------------------------------------------
//  Functie pentru LoadPC - salt la o adresa specificata din
//  memoria program
//
//--------------------------------------------------------------
void sendLoadPC(HANDLE hSerial,uint16_t address)
{
    unsigned short chksum;
    unsigned char loadPC[16] = {0x80,0x1A,0x04,0x04,address & 0xFF,
    (address>>8)&0xFF,0xFF,0xFF};

    printf("Sending 'Load PC' command:\n");

    chksum=calculateChecksum(loadPC,8);
    loadPC[8] = chksum & (0xFF);
    loadPC[9] = (chksum>>8) & (0xFF);

    if (!sendCommandWithDelay(hSerial, loadPC, 10))
    {
        printf("Eroare la trimiterea comenzii Load PC.\n");
        return;
    }//end if
    printf("'Load PC' command sent successfully.\n");

    unsigned char response[2];
    int bytesRead = readResponse(hSerial, response, sizeof(response));
    interpretResponse(response, bytesRead);
}//end function
//--------------------------------------------------------------



//--------------------------------------------------------------
//  Functie pentru 'RX Data Block' - SCRIEREA unei sectiuni de
//  memorie
//
//--------------------------------------------------------------
void sendRXdataBlock(HANDLE hSerial,uint16_t address, uint16_t length,const unsigned char *data)
{
    int i;
    unsigned short chksum;
    unsigned char RXdata[256] = {0x80,0x12,(length+4) & 0xFF,(length+4) & 0xFF,address & 0xFF,
    (address>>8)&0xFF,length&0xFF,0x00};

    if(length%2!=0)
    {
        printf("EROARE: lungime bloc de memorie (%d) impara!\n",length);
        return;
    }

    printf("Sending 'RX Data Block' command:\n");

    for(i=8;i<=length+8-1;i++)
    {
        RXdata[i]=data[i-8];
    }//end for

    chksum=calculateChecksum(RXdata,length+8);
    RXdata[length+8] = chksum & (0xFF);
    RXdata[length+9] = (chksum>>8) & (0xFF);

    if (!sendCommandWithDelay(hSerial, RXdata, length+10))
    {
        printf("Eroare la trimiterea comenzii RX Data Block.\n");
        return;
    }
    printf("'RX Data Block' command sent successfully.\n");

    unsigned char response[2];
    int bytesRead = readResponse(hSerial, response, sizeof(response));
    interpretResponse(response, bytesRead);
}//end function
//--------------------------------------------------------------



//--------------------------------------------------------------
//  Functie pentru 'Mass Erase' - stergere completa a memoriei
//
//--------------------------------------------------------------
void sendMassErase(HANDLE hSerial)
{
    unsigned short chksum;
    unsigned char massErase[16] = {0x80, 0x18, 0x04, 0x04, 0xFF, 0xFF, 0x06, 0xA5};
    printf("Sending 'Mass Erase' command:\n");

    chksum=calculateChecksum(massErase,8);
    massErase[8] = chksum & (0xFF);
    massErase[9] = (chksum>>8) & (0xFF);

    if (!sendCommandWithDelay(hSerial, massErase, 10))
    {
        printf("Eroare la trimiterea comenzii 'Mass Erase'!\n");
        return;
    }//end if

    printf("'Mass Erase' command sent successfully!\n");

    unsigned char response[2];
    int bytesRead = readResponse(hSerial, response, sizeof(response));
    interpretResponse(response, bytesRead);
}//end function
//--------------------------------------------------------------



//--------------------------------------------------------------
//  Functie pentru 'RX Password' - trimitere parola
//
//--------------------------------------------------------------
bool sendRXPassword(HANDLE hSerial,unsigned const char* pass)
{
    unsigned char rxPassword[64] = {0x80, 0x10, 0x24, 0x24, 0x00, 0x00, 0x00, 0x00};
    unsigned short chksum;

    for (int i = 8; i < 40; i++)
    {
        rxPassword[i] = pass[i-8];
    }//end for

    printf("Sending RX Password command octet cu octet:\n");

    chksum=calculateChecksum(rxPassword,40);
    rxPassword[40] = chksum & (0xFF);
    rxPassword[41] = (chksum>>8) & (0xFF);

    if (!sendCommandWithDelay(hSerial, rxPassword, 42))
    {
        printf("Eroare la trimiterea comenzii 'RX Password'!\n");
        return false;
    }//end if
    printf("'RX Password' command sent successfully!\n");

    unsigned char response[4];
    int bytesRead = readResponse(hSerial, response, sizeof(response));

    interpretResponse(response, bytesRead);

    return true;
}//end function
//--------------------------------------------------------------



//--------------------------------------------------------------
//  Functie pentru 'TX Data Block' - CITIREA unei sectiuni
//  de memorie
//
//--------------------------------------------------------------
void sendTXDataBlock(HANDLE hSerial, uint16_t address, uint16_t length)
{
    unsigned char txDataBlock[12] = {
        0x80, 0x14, 0x04, 0x04,
        address & 0xFF, (address >> 8) & 0xFF,
        length & 0xFF, (length >> 8) & 0xFF
    };
    unsigned short checksum = calculateChecksum(txDataBlock, 8);
    txDataBlock[8] = checksum & 0xFF;
    txDataBlock[9] = (checksum >> 8) & 0xFF;

    printf("Sending TX Data Block command to read 0x%04X:\n", address);
    printf("Packet (inclusiv checksum): ");

    for (int i = 0; i < 10; i++)
    {
        printf("%02X ", txDataBlock[i]);
    }
    printf("\nChecksum calculat: %04X\n", checksum);

    if (!sendCommandWithDelay(hSerial, txDataBlock, 10))
    {
        printf("Eroare la trimiterea comenzii TX Data Block.\n");
        return;
    }
    printf("'TX Data Block' command sent successfully!\n");

    unsigned char memoryResponse[256];
    int bytesRead = readResponse(hSerial, memoryResponse, sizeof(memoryResponse));
    if (bytesRead > 0)
    {
        printf("Memory response from 0x%04X: ", address);
        for (int i = 0; i < bytesRead; i++)
        {
            printf("%02X ", memoryResponse[i]);
        }
        printf("\n");
    }
    else
    {
        printf("No response received for 'TX Data Block'!\n");
    }
}//end function
//--------------------------------------------------------------



//--------------------------------------------------------------
//  Functie pentru citirea unui bloc de memorie
//
//--------------------------------------------------------------
void sendTXreadBSL(HANDLE hSerial)
{
    unsigned char txDataBlock[6] = {
        0x80, 0x1E, 0x04, 0x04,
    };
    unsigned short checksum = calculateChecksum(txDataBlock, 4);
    txDataBlock[4] = checksum & 0xFF;
    txDataBlock[5] = (checksum >> 8) & 0xFF;

    printf("Sending TX Read BSL Version command\n");
    printf("Packet (inclusiv checksum): ");

    for (int i = 0; i < 6; i++)
    {
        printf("%02X ", txDataBlock[i]);
    }
    printf("\nChecksum calculat: %04X\n", checksum);

    if (!sendCommandWithDelay(hSerial, txDataBlock, 6))
    {
        printf("Eroare la trimiterea comenzii TX Read BSL.\n");
        return;
    }
    printf("TX Read BSL command sent successfully.\n");

    Sleep(10);

    unsigned char memoryResponse[32];
    int bytesRead = readResponse(hSerial, memoryResponse, sizeof(memoryResponse));
    if (bytesRead > 0)
    {
        printf("Memory response:\n");
        for (int i = 0; i < bytesRead; i++)
        {
            printf("%02X ", memoryResponse[i]);
        }
        printf("\n");
    }
    else
    {
        printf("No response received for TX Read BSL.\n");
    }
}//end function
//--------------------------------------------------------------



//--------------------------------------------------------------
//  Functie pentru trimiterea codului de sincronizare: 0x80
//
//--------------------------------------------------------------
void sendSync(HANDLE hSerial)
{
    unsigned char sync[1]={0x80};
    sendCommandWithDelay(hSerial,sync,1);
}//end function
//--------------------------------------------------------------

char linie[256];

int len(char *str, char del){
    int i=0;

    for(i; str[i] != del ;i++);

    return i;
}

int is_hex(char c){
    if((int)c - '0' > 9) return 1;
    return 0;
}

int invertBits(int n)
{
    int x = log2(n) ;

    int m = 1 << x;

    m = m | m - 1;

    n = n ^ m;

    return n;
}

int make_zecimal(char c1, char c2){
    int nodb = 0;

    if(!is_hex(c1)){
        nodb += (c1-'0')*16;
    } else {
        nodb += (c1-'A'+10)*16;
    }

    if(!is_hex(c2)){
        nodb += c2-'0';
    } else {
        nodb += c2-'A'+10;
    }

    return nodb;
}

void set_address_field(char *af, char *l){
    af[0] = l[3];
    af[1] = l[4];
    af[2] = l[5];
    af[3] = l[6];
    af[4] = '\0';
}

void set_record_type(int *record_type, char c){
    *record_type = 0;

    *record_type += c-'0';
}

void set_data_bytes(char *l, unsigned char *db, int nodb){
    for(int i=9, k=0; i<9+nodb*2; i+=2){
        db[k++] = make_zecimal(l[i], l[i+1]);
    }
}

int power(int a, int b){
    int nr=a;

    for(int i=0; i<b-1; i++){
        nr *= a;
    }

    return nr;
}

int make_adress(char *adress){
    int nodb = 0;

    if(!is_hex(adress[0])){
        nodb += (adress[0]-'0')*power(16, 3);
    } else {
        nodb += (adress[0]-'A'+10)*power(16, 3);
    }

    if(!is_hex(adress[1])){
        nodb += (adress[1]-'0')*power(16, 2);
    } else {
        nodb += (adress[1]-'A'+10)*power(16, 2);
    }

    if(!is_hex(adress[2])){
        nodb += (adress[2]-'0')*16;
    } else {
        nodb += (adress[2]-'A'+10)*16;
    }

    if(!is_hex(adress[3])){
        nodb += adress[3]-'0';
    } else {
        nodb += adress[3]-'A'+10;
    }

    return nodb;
}

//--------------------------------------------------------------
//  Functia principala a programului
//
//--------------------------------------------------------------
int main()
{
    //date de test
    unsigned char databuf[32]={
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    };


    unsigned char pass[32]={
        0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
        0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
        0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
        0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
    };

    //variabila asociata portului serial
    HANDLE hSerial;

    //deschidere port serial specificat
    hSerial = CreateFile("\\\\.\\COM11", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    //analiza deschidere port serial
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        printf("Nu s-a putut deschide portul serial.\n");
        return 1;
    }//end if

    printf("COM port opened successfully.\n");

    // Configurare port serial
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.Parity = EVENPARITY;
    dcbSerialParams.StopBits = ONESTOPBIT;
    SetCommState(hSerial, &dcbSerialParams);

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = TIMEOUT_INTERVAL;
    SetCommTimeouts(hSerial, &timeouts);

    //==========================================================

    sendSync(hSerial);
    Sleep(1);
    sendMassErase(hSerial);
    Sleep(100);


    //trimitere parola
    sendSync(hSerial);
    Sleep(1);
    if (!sendRXPassword(hSerial,pass))
    {
        printf("RX Password failed. Nu putem continua cu alte comenzi.\n");
        CloseHandle(hSerial);
        return 1;
    }//end if

    /***
        Efectiv uploader
    ***/

    FILE *fptr;

    fptr = fopen("nice.hex", "r");

    if(fptr == NULL){
        printf("Fisierul nu a fost deschis!");

        return 1;
    }

    int nodb=0;
    int sum=0;
    char address_field[5] = "";
    int record_type=0;
    unsigned char data_bytes[256];

    while(fgets(linie, 256, fptr)){
        //printf("%s", linie);

        /// number of data bytes
        nodb = make_zecimal(linie[1], linie[2]);

        /// adress field
        set_address_field(address_field, linie);

        /// set record type
        set_record_type(&record_type, linie[8]);

        set_data_bytes(linie, data_bytes, nodb);

        Sleep(10);
        sendSync(hSerial);
        Sleep(10);

        sendRXdataBlock(hSerial,make_adress(address_field),nodb,data_bytes);
    }

    //inchidere port serial
    CloseHandle(hSerial);

    return 0;
}//end function

//--------------------------------------------------------------
//  End of file
//--------------------------------------------------------------
