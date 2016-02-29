  // Java class
package org.qtproject.qt5;

//import android.app.Activity;
import com.ftdi.j2xx.D2xxManager;
import com.ftdi.j2xx.FT_Device;

import android.content.Context;

//import android.os.Handler;



import java.io.IOException;


public class QtFtDev  //extends org.qtproject.qt5.android.bindings.QtActivity//extends Activity
{
    // j2xx
     public static D2xxManager ftD2xx = null;
     public Context global_context;

     FT_Device ftDev;
     int DevCount = -1;
     int currentPortIndex = -1;
     int portIndex = -1;

     enum DeviceStatus{
        DEV_NOT_CONNECT,
        DEV_NOT_CONFIG,
        DEV_CONFIG
     }
     boolean uart_configured = false;

     int lastReadLen;

     int baudRate; /* baud rate */
     byte stopBit; /* 1:1stop bits, 2:2 stop bits */
     byte dataBit; /* 8:8bit, 7: 7bit */
     byte parity; /* 0: none, 1: odd, 2: even, 3: mark, 4: space */
     byte flowControl; /* 0:none, 1: CTS/RTS, 2:DTR/DSR, 3:XOFF/XON */

     final byte XON = 0x11;    /* Resume transmission */
     final byte XOFF = 0x13;    /* Pause transmission */
//     ReadThread readThread; // read data from USB

    final int MAX_NUM_BYTES = 65536;
    final int USB_DATA_BUFFER = 8192;

    byte[] writeBuffer;



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    static int fromNumber(int x, int y) {
        return x + y;
    }

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    static String fromNumberOne(int x) {
        return "Hello Qt from Java " + x;
    }

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    String fromNumberTwo(int x){
        return "from number two "  + x;
    }

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    //init lib
     int getInstance(Context parentContext)
     {
        int retVal = 1;
        writeBuffer = new byte[512];
        global_context = parentContext;

        try{
            ftD2xx = D2xxManager.getInstance(parentContext);
            retVal = 2;
        }
        catch (D2xxManager.D2xxException e) {
            retVal = 3;
         }
         return retVal;
    }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    //ftdev list
    int createDeviceList()
    {
        int tempDevCount = ftD2xx.createDeviceInfoList(global_context);
        if (tempDevCount > 0) {
            if( DevCount != tempDevCount )
                DevCount = tempDevCount;
            }else
            DevCount = -1;

        return DevCount;

    }

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    int checkDeviceStatus()
    {

        int retVal = 0;

        switch(checkDevice()){
        case DEV_NOT_CONNECT: retVal = 0;
        case DEV_NOT_CONFIG: retVal = 1;
        case DEV_CONFIG: retVal = 2;
        }
        return retVal;
    }

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    DeviceStatus checkDevice()
    {
        if(ftDev == null || false == ftDev.isOpen()) {
            //"Need to connect to cable"
            return DeviceStatus.DEV_NOT_CONNECT;
        }else{
            if(false == uart_configured){
                    //midToast("CHECK: uart_configured == false", Toast.LENGTH_SHORT);
                    //"Need to configure UART.
                return DeviceStatus.DEV_NOT_CONFIG;
            }
        }
        return DeviceStatus.DEV_CONFIG;
    }

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    int connToOneDevice(int connIndx, int baudR, int dataB, int stopB, int partt, int flowCtrl)
    {
        lastReadLen = 0;
        portIndex = connIndx;
        int connF = 10;
        createDeviceList();
        if(DevCount > 0)
             connF = connectFunction();

        if(DeviceStatus.DEV_NOT_CONNECT  == checkDevice())
            return connF; //dev not connect

        baudRate =   baudR;
        stopBit = (byte)stopB ; /* 0:1stop bits, 1:2 stop bits */
        dataBit = (byte)dataB; /* 8:8bit, 7: 7bit */
        parity = (byte)partt; /* 0: none, 1: odd, 2: even, 3: mark, 4: space */
        flowControl = (byte)flowCtrl; /* 0:none, 1: CTS/RTS, 2:DTR/DSR, 3:XOFF/XON */


        setConfig(baudRate, dataBit, stopBit, parity, flowControl);

        uart_configured = true;

        if(true == ftDev.isOpen())
        return 0;
        else
        return 11;
    }

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    void setConfig(int baud, byte dataBits, byte stopBits, byte parity, byte flowControl)
    {
        // configure port
        // reset to UART mode for 232 devices
        ftDev.setBitMode((byte) 0, D2xxManager.FT_BITMODE_RESET);

        ftDev.setBaudRate(baud);

        switch (dataBits){
        case 7: dataBits = D2xxManager.FT_DATA_BITS_7; break;
        case 8: dataBits = D2xxManager.FT_DATA_BITS_8; break;
        default: dataBits = D2xxManager.FT_DATA_BITS_8; break;
        }

        switch (stopBits){
        case 0: stopBits = D2xxManager.FT_STOP_BITS_1; break;
        case 1: stopBits = D2xxManager.FT_STOP_BITS_2; break;
        default: stopBits = D2xxManager.FT_STOP_BITS_1; break;
        }

        switch (parity){
        case 0: parity = D2xxManager.FT_PARITY_NONE; break;
        case 1: parity = D2xxManager.FT_PARITY_ODD; break;
        case 2: parity = D2xxManager.FT_PARITY_EVEN; break;
        case 3: parity = D2xxManager.FT_PARITY_MARK; break;
        case 4: parity = D2xxManager.FT_PARITY_SPACE; break;
        default: parity = D2xxManager.FT_PARITY_NONE; break;
        }

        ftDev.setDataCharacteristics(dataBits, stopBits, parity);

        short flowCtrlSetting;
        switch (flowControl){
        case 0: flowCtrlSetting = D2xxManager.FT_FLOW_NONE; break;
        case 1: flowCtrlSetting = D2xxManager.FT_FLOW_RTS_CTS; break;
        case 2: flowCtrlSetting = D2xxManager.FT_FLOW_DTR_DSR; break;
        case 3: flowCtrlSetting = D2xxManager.FT_FLOW_XON_XOFF; break;
        default: flowCtrlSetting = D2xxManager.FT_FLOW_NONE; break;
        }

        ftDev.setFlowControl(flowCtrlSetting, XON, XOFF);
        uart_configured = true;
     }

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    int connectFunction()
    {

        if( portIndex + 1 > DevCount)
            portIndex = 0;


        if( currentPortIndex == portIndex && ftDev != null && true == ftDev.isOpen() )
            return 1;


        if(null == ftDev)
            ftDev = ftD2xx.openByIndex(global_context, portIndex);
        else
            ftDev = ftD2xx.openByIndex(global_context, portIndex);

        uart_configured = false;

        if(ftDev == null)
            return 2;

        if (true == ftDev.isOpen()){
            currentPortIndex = portIndex;
            return 0;
        }else
            return 3;

    }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    int sendData(String writeHex/*, byte[] buffer*/)
    {
        int numBytes = -1;
        try {
            String atemp = hexToAscii(writeHex);
            numBytes = atemp.length();
            for (int i = 0; i < numBytes; i++){
                writeBuffer[i] = (byte) atemp.charAt(i);
            }
        }catch(IllegalArgumentException e){
            return -2;
        }

        if (ftDev.isOpen() == false)
            return -3;

        if (numBytes > 0)
            return ftDev.write(writeBuffer, numBytes);
        return 0;

    }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    int readDataArrLen()
    {
        return USB_DATA_BUFFER;
    }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    int readDataLastReadLen()
    {
        return lastReadLen;
    }

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    int bytesAvailable()
    {
        if (ftDev.isOpen() == false){
            lastReadLen = -1;
            return lastReadLen;
        }else{
            return ftDev.getQueueStatus();
        }

    }
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    String readData()
    {
        byte[] usbdata = new byte[USB_DATA_BUFFER];

        if (ftDev.isOpen() == false){
            lastReadLen = -1;
            return new StringBuilder(0).toString();
        }
        int readcount = ftDev.getQueueStatus();

        if (readcount > 0){
            if(readcount > USB_DATA_BUFFER){
                readcount = USB_DATA_BUFFER;
            }
            ftDev.read(usbdata, readcount);
        }
        lastReadLen = readcount;

        return byteArrayToHex(usbdata);
    }

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    int disconnectFunction()
    {
        DevCount = -1;
        currentPortIndex = -1;

        if(ftDev != null){
            if( true == ftDev.isOpen()) {
                ftDev.close();
            }
        }
        return 0;
    }

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

String hexToAscii(String s) throws IllegalArgumentException
{
          int n = s.length();
          StringBuilder sb = new StringBuilder(n / 2);
          for (int i = 0; i < n; i += 2)
          {
            char a = s.charAt(i);
            char b = s.charAt(i + 1);
            sb.append((char) ((hexToInt(a) << 4) | hexToInt(b)));
          }
          return sb.toString();
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 int hexToInt(char ch)
{
          if ('a' <= ch && ch <= 'f') { return ch - 'a' + 10; }
          if ('A' <= ch && ch <= 'F') { return ch - 'A' + 10; }
          if ('0' <= ch && ch <= '9') { return ch - '0'; }
          throw new IllegalArgumentException(String.valueOf(ch));
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

String byteArrayToHex(byte[] a) {
   StringBuilder sb = new StringBuilder(a.length * 2);
   for(byte b: a)
      sb.append(String.format("%02x", b & 0xff));
   return sb.toString();
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}

