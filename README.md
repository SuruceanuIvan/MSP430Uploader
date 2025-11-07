# MSP430Uploader
![eu_si_msp](https://github.com/user-attachments/assets/6457aefe-1f5e-49ac-ad1e-27fb848eb23c)
<p>This project involved developing a custom bootloader and programming tool for the <b>TI MSP430 microcontroller</b>. I began by studying the MSP430 documentation and understanding the <b>BSL</b> (Bootstrap Loader) protocol, then <b>built a firmware uploader</b> using BSL communication and <b>DataFrame structures</b>. To support testing, I designed and assembled a <b>custom PCB</b> equipped with basic debug and peripheral components. I also implemented an <b>Intel-Hex file parser</b> to enable reliable firmware transfer to the microcontroller via serial communication. The system was successfully verified through display output, confirming correct operation. Through this project, I gained valuable experience in <b>low-level embedded programming</b>, hardware–software integration, and <b>embedded communication protocols</b>.</p>
<h2><u>Electronic schematic</u></h2>
<img width="1280" height="419" alt="electronic_schematic" src="https://github.com/user-attachments/assets/6d0f21f8-5026-45ce-8f0d-9b60adaee07a" />
<h3>Circuit Overview</h3>
The schematic illustrates the key hardware components designed for the custom MSP430 development board. The board integrates several basic peripherals and interfaces to support testing and firmware development:
<ul>
<li><b>LED Indicator</b> (<i>Fig. 3</i>): The LEDon circuit is connected to the 5V supply through a current-limiting resistor, providing visual feedback for power or system status.</li>
<li><b>Supercapacitor Power Source</b> (<i>Fig. 4</i>): The SCAP component (3300 µF, 6.3V) acts as an energy storage element, supplying backup power to the MSP430 microcontroller through a protection diode.</li>
<li><b>User Buttons</b> (<i>Fig. 11</i>): Two push buttons are connected to GPIO inputs via pull-down resistors and decoupling capacitors, allowing for stable input signal detection.</li>
<li><b>Potentiometer Input</b> (<i>Fig. 5</i>): A potentiometer (POT) connected to pin P1.0 enables analog input testing for ADC calibration and user-adjustable control.</li>
<li><b>Quartz Crystal</b> (<i>Fig. 8</i>): A 32.768 kHz crystal oscillator provides an accurate clock reference for the microcontroller’s timing functions.</li>
<li><b>7-Segment Display Interface</b>: A common-cathode 7-segment display is connected to the microcontroller, allowing visual output for debugging or numeric display.</li>
</ul>
This circuit was designed to provide a modular and experimental platform for embedded systems development with the TI MSP430 microcontroller, combining both digital and analog interfaces to facilitate firmware development and hardware testing.
<h2><u>Peripherals</u></h2>
<img width="984" height="188" alt="schema_microcontroller_din sus" src="https://github.com/user-attachments/assets/8bf247e1-758d-48eb-aa35-e93404c2b463" />
<table>
  <tr>
    <th>Pin Number</th>
    <th>Pin Name</th>
    <th>Potential Functions</th>
    <th>Defined Functions</th>
  </tr>
  <tr>
    <td>1</td>
    <td>DVCC</td>
    <td>Supply voltage (+)</td>
    <td>-</td>
  </tr>
  <tr>
    <td>2</td>
    <td>P1.0</td>
    <td>TA0CLK / ACLK / A0 / CA0 </td>
    <td>ADC potentiometer inputs</td>
  </tr>

  <tr>
    <td>3</td>
    <td>P1.1</td>
    <td><del>TA0.0</del> / <b> UCA0RXD </b> / <del> UCA0SOMI / A1 / CA1 </del></td>
    <td>Serial interface – RX (TX-BSL)</td>
  </tr>

  <tr>
    <td>4</td>
    <td>P1.2</td>
    <td><del> TA0.1 </del>/ <b>UCA0TXD</b> / <del> UCA0SIMO / A2 / CA2</del></td>
    <td>Serial interface – TX</td>
  </tr>

  <tr>
    <td>5</td>
    <td>P1.3</td>
    <td> ADC10CLK / CAOUT / VREF- / VEREF- / A3 / CA3</td>
    <td>Input BUT1</td>
  </tr>

  <tr>
    <td>6</td>
    <td>P1.4</td>
    <td>SMCLK / UCB0STE / UCA0CLK / VREF+ / VEREF+ / A4 / CA4 / TCK</td>
    <td>Input BUT2</td>
  </tr>

  <tr>
    <td>7</td>
    <td>P1.5</td>
    <td>TA0.0 / UCB0CLK / UCA0STE / A5 / CA5 / TMS</td>
    <td>Serial interface (RX-BSL) / LED output – segment DP (Decimal Point)</td>
  </tr>

  <tr>
    <td>8</td>
    <td>P2.0</td>
    <td>TA1.0</td>
    <td>Out LED – segm.C</td>
  </tr>

  <tr>
    <td>9</td>
    <td>P2.1</td>
    <td>TA1.1</td>
    <td>Out LED – segm.D</td>
  </tr>

  <tr>
    <td>10</td>
    <td>P2.2</td>
    <td>TA1.1</td>
    <td>Out LED – segm.E</td>
  </tr>

  <tr>
    <td>11</td>
    <td>P2.3</td>
    <td>TA1.0</td>
    <td>Out LED – segm.B</td>
  </tr>

  <tr>
    <td>12</td>
    <td>P2.4</td>
    <td>TA1.2</td>
    <td>Out LED – segm.A</td>
  </tr>

  <tr>
    <td>13</td>
    <td>P2.5</td>
    <td>TA1.2</td>
    <td>Out LED – segm.F</td>
  </tr>

  <tr>
    <td>14</td>
    <td>P1.6</td>
    <td>TA0.1 / UCB0SOMI / UCB0SCL / A6 / CA6 / TDITCLK</td>
    <td>Out LED – segm.G</td>
  </tr>

  <tr>
    <td>15</td>
    <td>P1.7</td>
    <td>CAOUT / UCB0SIMO / UCB0SDA / A7 / CA7 / TDO / TDI</td>
    <td>Unused</td>
  </tr>

  <tr>
    <td>16</td>
    <td>RST</td>
    <td>Reset, NMI, SBWTDIO</td>
    <td>BRST – Momentary button</td>
  </tr>

  <tr>
    <td>17</td>
    <td>TEST</td>
    <td>Test, SBWTCK</td>
    <td>BTEST - Momentary button</td>
  </tr>

  <tr>
    <td>18</td>
    <td>P2.7</td>
    <td>XOUT</td>
    <td>32768 Hz real-time clock crystal</td>
  </tr>

  <tr>
    <td>19</td>
    <td>P2.6</td>
    <td>XIN/TA0.1</td>
    <td>32768Hz crystal ceas timp real</td>
  </tr>

  <tr>
    <td>20</td>
    <td>DVSS</td>
    <td>Supply voltage (-)</td>
    <td>-</td>
  </tr>

</table>

<img width="992" height="127" alt="reset_schema" src="https://github.com/user-attachments/assets/1c2260dd-d35d-4345-a7c4-4dbc915125f4" />

<p>The microcontroller features a Bootloader (BSL – BootStrap Loader) that allows firmware updates via a defined signal sequence on the <b>RST/NMI</b> and <b>TEST</b> inputs. This sequence can be generated using the <b>BRST</b> and <b>BTEST</b> buttons: <i>by pressing and holding BRST while pressing BTEST twice, and ensuring BRST is released before BTEST on the second press, the microcontroller enters Bootloader mode.</i></p>

<p>At any moment, a single press of BRST resets the microcontroller and executes the firmware (FW) code. The Bootloader is <i>protected in ROM</i>, preventing accidental erasure. This functionality ensures reliable control of firmware operations while maintaining a secure, user-accessible interface for system updates.</p>

<img width="1777" height="461" alt="placa_cu_elemente_notate" src="https://github.com/user-attachments/assets/6f030d53-bced-44a5-a342-6d0d062e3f17" />
<table>
  <tr>
    <th>Name</th>
    <th>Meaning</th>
  </tr>

  <tr>
    <td>LEDon</td>
    <td>Green LED – indicates the presence of correct supply voltage from the USB port</td>
  </tr>
  
  <tr>
    <td>SCAP</td>
    <td>High-capacity capacitor (Supercapacitor) – 3300 μF</td>
  </tr>

  <tr>
    <td>POT</td>
    <td>Potentiometer connected to the supply voltage (DVCC ~ 3.0 V), varying from 0 to 3.0 V</td>
  </tr>

  <tr>
    <td>BRST</td>
    <td>RESET button</td>
  </tr>

  <tr>
    <td>BTEST</td>
    <td>Button dedicated to the TEST input for generating the sequence required by the BSL</td>
  </tr>

  <tr>
    <td>SW</td>
    <td>Switch for selecting Firmware/BSL communication</td>
  </tr>

  <tr>
    <td>IC1</td>
    <td>MSP430G2553 microcontroller in a 20-pin DIP package</td>
  </tr>

  <tr>
    <td>DIG</td>
    <td>7-segment AC display (Common Anode)</td>
  </tr>

  <tr>
    <td>BUT1</td>
    <td>Momentary button</td>
  </tr>

  <tr>
    <td>BUT2</td>
    <td>Momentary button</td>
  </tr>

  <tr>
    <td>USB-to-Serial Converter</td>
    <td>USB-to-Serial converter module based on the FTDI® FT232RL chip, which also provides a 3.3 V supply for the entire circuit</td>
  </tr>
</table>

<h2><u>Uploader</u></h2>
<h3>Development stages of the uploader</h3>
<img width="1169" height="364" alt="stages_of_development" src="https://github.com/user-attachments/assets/b4474192-665a-48f7-9727-f63bb96ad6d8" />
<h2>BSL</h2>
<p>The <b>MSP430™ Bootloader</b> (BSL), formerly known as the “bootstrap loader,” <i>allows users to communicate with the integrated memory of the MSP430 microcontroller</i> (MCU) during prototyping, final production, and usage phases. Both the programmable memory (flash memory) and data memory (RAM) can be modified as needed. The bootloader should not be confused with bootstrap loader programs found in some digital signal processors (DSPs), which automatically load program code (and data) from external memory into the internal memory of the DSP.</p>
	<p>To use the bootloader, a specific <b>BSL entry sequence</b> must be applied. An additional sequence of commands initiates the desired function. A bootloading session can be ended either by continuing execution at a user-defined program address or by a reset condition.</p>
<h2>Commands</h2>
<p>There are two categories of commands: commands that require a password and commands that do not require a password. Password protection ensures security for every command that allows direct or indirect access to data.
</p>
<h2>Unprotected commands</h2>
<ul>
<li>Password reception</li>
<li>Mass Erase (erases the entire flash memory, including both main memory and information memory)</li>
<li>Transmit BSL version</li>
</ul>
<h2>Protected commands</h2>
<ul>
	<li>Receiving a data block for programming flash memory, RAM, or peripherals</li>
	<li>Transmitting a data block</li>
	<li>Segment erase</li>
	<li>Erase verification</li>
	<li>Setting memory offset</li>
	<li>Loading the program counter (PC) and starting user program</li>
</ul>
<h2>Data Frame</h2>
<img width="997" height="434" alt="dataframe" src="https://github.com/user-attachments/assets/3abbbe1b-7b29-4c76-ad71-e0d1f5c8f42a" />

<h2>Implementation of commands for efficient communication</h2>

```c++
bool sendCommandWithDelay(HANDLE hSerial, const unsigned char *command, int length) {
    int i;
    for (i = 0; i < length; i++)
	{
        if (!sendByteWithDelay(hSerial, command[i]))
		{
            return false;
        }
    }
    Sleep(DELAY_AFTER_TRANSMIT); 
    return true;
}
```
One of the key functions in my project addresses communication with the MSP430 Bootloader. Initially, I encountered <b>timing issues</b> because the PC transmits data significantly faster than the microcontroller can process. To prevent data loss, this function implements a <b>controlled delay</b> between transmitted bytes, with the optimal delay determined experimentally. Additionally, the function sends data <b>byte by byte</b>, ensuring reliable delivery and synchronization with the microcontroller’s processing speed.

<hr/>

```cpp
void sendSync(HANDLE hSerial)
{
    unsigned char sync[1]={0x80};
    sendCommandWithDelay(hSerial,sync,1);
}
```
This code transmits a <b>synchronization byte</b> to the Bootloader (BSL) to ensure proper alignment between the host PC and the microcontroller. Sending this byte establishes a known starting point for communication, preventing data corruption and enabling reliable command execution.

<hr/>

```cpp
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
}
```
This function is responsible for <b>transmitting the password</b> to the microcontroller. It leverages a structured data frame implementation for the <b>Rx Password command</b>, ensuring proper formatting and integrity. The program also calculates a checksum to comply with the data frame structure. Command transmission is handled using the previously described sendCommandWithDelay() function. Additionally, the readResponse() and interpretResponse() functions provide real-time feedback in the console, allowing the user to monitor success or error messages following password transmission.

<hr/>

```cpp
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
}
```
This function handles the erasure of the programmed code in the MSP430 microcontroller. It adheres to the data frame protocol to ensure proper command formatting and reliability. The function utilizes the previously described sendCommandWithDelay() for controlled transmission, while readResponse() and interpretResponse() manage real-time feedback, confirming successful execution or reporting errors

<hr />

```cpp
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
}
```

This function transmits an <b>RX data block</b> to the MSP430 microcontroller, enabling the writing of data into its memory. The RX data block is required to have an even length to comply with the data frame protocol. Additionally, the function uses <b>bitwise masks</b> to properly configure the values within the RXdata array, ensuring accurate and reliable transmission.

<hr />

```cpp
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
}
```

This function <b>reads responses</b> from the Bootloader (BSL). It stores the incoming data in a dedicated buffer and implements a <b>timeout</b> mechanism to prevent indefinite blocking, ensuring reliable and controlled communication with the microcontroller.

<hr />

```cpp
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
        }
    }
    printf("Unexpected response: ");

    for (int i = 0; i < length; i++)
    {
        printf("%02X ", response[i]);
    }
    printf("\n");
    return false;
}
```

This function <b>interprets the responses</b> received from the MSP430 microcontroller according to the official MSP430 Bootloader documentation, translating raw data into meaningful status messages for error handling and verification.

<hr />

```cpp
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
}
```

This function <b>calculates the checksum</b> for data frame commands, implementing the algorithm specified in the official MSP430 documentation. It ensures data integrity and compliance with the Bootloader communication protocol.

<hr />

## Intel-Hex Format
The <b>Intel HEX file</b> is an <i>ASCII text file</i> that contains lines of text following the <b>specific format</b> of <i>Intel HEX files</i>. Each line in an Intel HEX file contains a <b>HEX record</b>. These records are composed of hexadecimal numbers representing machine code and/or constant data. Intel HEX files are often used to transfer programs and data intended to be stored in <b>ROM</b> or <b>EPROM memory</b>. <i>Most EPROM programmers or emulators can use Intel HEX files.</i>

## Record Format
Each group of characters corresponds to a different field, and each character represents a <b>hexadecimal digit</b>. Each field consists of at <i>least two hexadecimal digits</i> — forming one byte — as described below:
<ul>
<li><b>:</b> is the colon character that marks the beginning of each Intel HEX record.</li>
<li><b>ll</b> is the record length field, indicating the number of data bytes (dd) in the record.</li>
<li><b>aaaa</b> is the address field, specifying the starting address for the data in the record.</li>
<li><b>tt</b> is the record type field, which can be one of the following:
	<ul>
		<li><b>00</b> – data record</li> 
		<li><b>01</b> – end-of-file record</li> 
		<li><b>02</b> – extended segment address record</li>
		<li><b>04</b> – extended linear address record</li>
		<li><b>05</b> – start linear address record (used only in MDK-ARM)</li>
	</ul>
</li>
<li><b>dd</b> is the data field, representing one data byte. A record can contain multiple data bytes, and their number must match the value in the ll field.</li>
<li><b>cc</b> is the checksum field, which ensures data integrity. It is calculated so that the sum of all bytes in the record (including ll, aaaa, tt, dd plus cc equals 0 (modulo 256).</li>
</ul>

## Intel-Hex & Memory Map
<img width="587" height="456" alt="memory_organization" src="https://github.com/user-attachments/assets/6ff2c48c-7161-40f0-8409-fd85c560d38d" />


The Intel HEX format provides a structured way to navigate the memory of the MSP430 microcontroller. By leveraging this format, we can precisely specify the start address for writing data and determine the exact endpoint, ensuring accurate memory programming and preventing overwrites.

## Intel-Hex Implementations

```cpp
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
```

This function <b>converts memory addresses into hexadecimal format</b>, facilitating precise manipulation and referencing of memory locations within the MSP430 microcontroller.

<hr />

```cpp
int len(char *str, char del){
    int i=0;

    for(i; str[i] != del ;i++);

    return i;
}
```
This function <b>calculates the length</b> of a character string up to a specified delimiter, enabling precise parsing and handling of command or data segments for reliable communication with the MSP430 microcontroller.

<hr />

```cpp
int is_hex(char c){
    if((int)c - '0' > 9) return 1;
    return 0;
}
```
This function <b>determines whether a character is a letter</b>, supporting the conversion of identified letters into their corresponding hexadecimal representation for accurate data processing and memory addressing on the MSP430 microcontroller.

<hr />

```cpp
int invertBits(int n)
{
    int x = log2(n) ;

    int m = 1 << x;

    m = m | m - 1;

    n = n ^ m;

    return n;
}
```

This function <b>inverts bits</b>, enabling precise manipulation of data at the bit level. This operation is essential for tasks such as preparing data frames, applying masks, or aligning information according to the MSP430 Bootloader protocol.

<hr />

```cpp
void set_address_field(char *af, char *l){
    af[0] = l[3];
    af[1] = l[4];
    af[2] = l[5];
    af[3] = l[6];
    af[4] = '\0';
}
```

This function <b>sets the address field</b> within a data frame, aligning with the Intel HEX format. By specifying the correct start address, it ensures that data is written to the intended memory locations on the MSP430 microcontroller, maintaining accurate memory mapping and preventing data corruption.

<hr />

```cpp
void set_record_type(int *record_type, char c){
    *record_type = 0;

    *record_type += c-'0';
}
```

This function <b>sets the record type</b> within an Intel HEX data frame, which is essential for defining the purpose of each record (e.g., data, end-of-file, extended address). The implementation <i>converts a character representation of the type into its integer value</i>. By accurately setting the record type, the function ensures proper interpretation and handling of each segment of the HEX file during memory programming on the MSP430 microcontroller.

<hr />

```cpp
void set_data_bytes(char *l, unsigned char *db, int nodb){
    for(int i=9, k=0; i<9+nodb*2; i+=2){
        db[k++] = make_zecimal(l[i], l[i+1]);
    }
}
```

This function <b>sets the data bytes</b> within an Intel HEX record, which is critical for accurately writing payload data to the MSP430 memory. It converts character pairs from the HEX string into actual byte values using the make_zecimal function, ensuring correct interpretation of the HEX format. <i>By correctly setting the data bytes, this function guarantees that the microcontroller receives the intended values at the right memory locations, maintaining integrity during the programming process</i>.

<hr />

## Intel HEX Record Identification – Example
The example below demonstrates <b>Intel HEX record identification</b>, <i>validating the correct functionality of the implemented parsing and data handling functions</i>. This confirms that the system accurately interprets and processes memory records for reliable programming of the MSP430 microcontroller:

<img width="1183" height="219" alt="intel_Hex_example" src="https://github.com/user-attachments/assets/a6c9796d-14d7-408f-aeca-2ddb8e0d0245" />

## Code loading
### How is the code loaded?
To convert a file into the Intel HEX format, the following steps are followed:
<ul>
<li>The project is compiled using the CSS Theia development environment (a project build is executed).The resulting file, with the extension .out (e.g., project_name.out), is taken from the project's BUILD directory. </li>
<li>This file is then copied to the compiler’s installation directory, usually C:/ti/msp430-gcc/bin.Inside this directory, a console is opened, and the following command is executed to convert the file to Intel HEX format: msp430-elf-objcopy -O ihex project_name.out program.hex</li>
<li>This command uses the MSP430-specific objcopy utility to convert the binary .out file into a .hex file, which is compatible with most programmers and emulators.</li>
</ul>

## Bibliography
 1. Datasheet (Datasheet MSP430G2x53/MSP430G2x13 MIXED SIGNAL MICROCONTROLLER) for TI MSP430G2553 - <a href="https://www.ti.com/lit/ds/symlink/msp430g2553.pdf" target="_blank">https://www.ti.com/lit/ds/symlink/msp430g2553.pdf</a>
 2. Tutorial for working in BSL mode (User’s Guide MSP430™ Flash Devices Bootloader (BSL)) - <a href="https://www.ti.com/lit/ug/slau319af/slau319af.pdf">https://www.ti.com/lit/ug/slau319af/slau319af.pdf</a>
 3. General documentation of the family MSP430 (MSP430F2xx, MSP430G2xx Family User’s Guide) - <a href="https://www.ti.com/lit/ug/slau144k/slau144k.pdf">https://www.ti.com/lit/ug/slau144k/slau144k.pdf</a>
 4. Intel-Hex format (GENERAL: Intel HEX File Format) - <a href="https://developer.arm.com/documentation/ka003292/latest/">https://developer.arm.com/documentation/ka003292/latest/</a>

# Thanks!
