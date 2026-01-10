# MQTT-Auto-Dicovery-Home-Assistant-ESP32
**Installation and Configuration Guide: Home Assistant & ESP32 (MQTT)**

This guide describes the steps required to deploy Home Assistant on a
virtual machine and configure communication with an ESP32
microcontroller using the MQTT protocol.

**1. Installing Home Assistant on Oracle VirtualBox**

The installation is based on using a preconfigured virtual disk image
for Windows.

1.  **Downloading the image:** Download the official Home Assistant
    image (.vdi format) for Windows from the website.

<https://www.home-assistant.io/installation/windows/>

<img src="./media/image1.png" style="width:6.5in;height:3.04861in" />

2.  **Creating the Virtual Machine (VM):**

- Launch Oracle VirtualBox and create a new machine.

<img src="./media/image2.png" style="width:6.5in;height:3.66597in" />

- **System type :** Select "**Linux**".

<img src="./media/image3.png"
style="width:5.21742in;height:1.90126in" />

- **Firmware configuration:** In the system settings, make sure to check
  the "**Use EFI**" option.

<img src="./media/image4.png"
style="width:5.59516in;height:2.19084in" />

- **Hard Drive:** Select the "**Use an existing virtual hard disk
  file**" option and import the previously downloaded **.vdi** file.

<img src="./media/image5.png"
style="width:5.79891in;height:3.29878in" />

3.  **Network Configuration:** Enable network access in "**Bridged**"
    mode to allow the VM to communicate with your local network.

<img src="./media/image6.png"
style="width:5.91697in;height:1.86121in" />

4.  **Initialization:** Start the virtual machine. The system will
    automatically proceed with its initial configuration.

<img src="./media/image7.png" style="width:6.5in;height:5.60764in" />

**2. Initial Home Assistant Configuration**

Once the system has started, the interface is accessible via your web
browser.

1.  **Accessing the interface:** Enter the IP address displayed on the
    virtual machine console into your browser.

<img src="./media/image8.png"
style="width:5.46913in;height:4.82639in" />

2.  **Account setup:** Follow the instructions to create your account.

3.  **Login:** Authenticate with your username and password to access
    the dashboard.

<img src="./media/image9.png"
style="width:3.20601in;height:3.35219in" />

**3. Setting up the MQTT Broker (Mosquitto)**

To allow your ESP32 to communicate with Home Assistant, you must install
an MQTT broker.

1.  **Advanced Mode:** Enable "**Advanced mode**" in your user profile
    settings.

<img src="./media/image10.png" style="width:6.5in;height:2.04167in" />

2.  **Add-on Installation :**

- Go to **Settings \> Add-ons**.

- Search and install **Mosquitto broker**.

<img src="./media/image11.png"
style="width:5.58287in;height:3.91755in" />

3.  **Add-on Configuration:** Enable the "Start on boot," "Watchdog,"
    and "Auto update" options, then click "**Start**."

<img src="./media/image12.png" style="width:6.5in;height:2.07847in" />

4.  **Security:** Create a specific user for MQTT in Home Assistant
    (username and password) to secure the connection of your devices.

<img src="./media/image13.png"
style="width:5.59028in;height:4.21528in" />

5.  **Finalization:** Save the configuration and restart the Mosquitto
    add-on.

<img src="./media/image14.png"
style="width:5.86487in;height:4.1405in" />

**4. Integration and Programming of the ESP32**

The goal is to use the "MQTT Auto-discovery" function for seamless
integration.

1.  **Board Programming:** Program your ESP32 with the Wi-Fi credentials
    and the MQTT authentication information created previously.

<img src="./media/image15.png"
style="width:5.83143in;height:3.27083in" />

<img src="./media/image16.png"
style="width:3.13524in;height:1.04341in" />

2.  **Activation of the integration :**

- In Home Assistant, go to **Settings \> Devices & Services**.

> <img src="./media/image17.png"
> style="width:5.72823in;height:2.31944in" />

- Add the **MQTT integration**, validate the settings, and submit.

<img src="./media/image18.png"
style="width:6.82959in;height:3.02151in" />

3.  **Verification:** Once connected to Wi-Fi, the ESP32 board will be
    automatically detected by Home Assistant.

<img src="./media/image19.png"
style="width:2.9407in;height:3.26462in" />

**5. Exploitation and Results**

- Control: You can now visualize the data from the ESP32 and control its
  peripherals (e.g., driving an LED) directly from the dashboard.

<img src="./media/image20.png" style="width:6.5in;height:2.40278in" />

- **Serial Monitor:** Message reception can be confirmed via the board's
  serial port.

> <img src="./media/image21.png"
> style="width:3.09889in;height:2.69774in" />

- **Scalability:** To add new devices, simply add the new MQTT
  credentials. They will be automatically added to your interface
  without any additional configuration.

> <img src="./media/image22.png" style="width:6.5in;height:5.33333in" />
>
> Refresh Dashboard
>
> <img src="./media/image23.png" style="width:6.5in;height:1.46806in" />
>
> <img src="./media/image24.png" style="width:6.5in;height:3.08681in" />
>
> Verify that all units are connected via MQTT.
>
> <img src="./media/image25.png" style="width:6.5in;height:2.27083in" />
