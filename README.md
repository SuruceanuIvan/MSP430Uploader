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
<h3>BSL</h3>
<p>The <b>MSP430™ Bootloader</b> (BSL), formerly known as the “bootstrap loader,” <i>allows users to communicate with the integrated memory of the MSP430 microcontroller</i> (MCU) during prototyping, final production, and usage phases. Both the programmable memory (flash memory) and data memory (RAM) can be modified as needed. The bootloader should not be confused with bootstrap loader programs found in some digital signal processors (DSPs), which automatically load program code (and data) from external memory into the internal memory of the DSP.</p>
	<p>To use the bootloader, a specific <b>BSL entry sequence</b> must be applied. An additional sequence of commands initiates the desired function. A bootloading session can be ended either by continuing execution at a user-defined program address or by a reset condition.</p>
<h3>Commands</h3>
<p>There are two categories of commands: commands that require a password and commands that do not require a password. Password protection ensures security for every command that allows direct or indirect access to data.
</p>
<h3>Unprotected commands</h3>
<ul>
<li>Password reception</li>
<li>Mass Erase (erases the entire flash memory, including both main memory and information memory)</li>
<li>Transmit BSL version</li>
</ul>
<h3>Protected commands</h3>
<ul>
	<li>Receiving a data block for programming flash memory, RAM, or peripherals</li>
	<li>Transmitting a data block</li>
	<li>Segment erase</li>
	<li>Erase verification</li>
	<li>Setting memory offset</li>
	<li>Loading the program counter (PC) and starting user program</li>
</ul>
<h3>Data Frame</h3>
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
