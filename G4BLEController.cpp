/*

In Bluetooth 4.0, BLE was introduced with a maximum payload of 33 bytes (not including Access Address and CRC fields).
Each layer in the protocol stack takes its cut:

    2 bytes for packet header (type and length),
    4 bytes for MIC (when encryption is enabled),
    4 bytes for L2CAP header (channel ID and packet length),
    ATT protocol is left with 23 bytes, which is the default and minimal MTU for ATT protocol.

With an ATT write request (or notification), 3 bytes are used by command type and attribute ID, 20 bytes are left for the attribute data.

At the ATT level, this limit may be enlarged in two ways:
Using fragmentation at the L2CAP level:

L2CAP will split ATT PDUs in 27 bytes fragments (23 for the first one).

*/


#include "G4BLEController.h"
#include <QTimer>


const QString TARGET_DEVICE_NAME = "HT_CA";

const QString SERVICE_UUID = "00001800-0000-1000-8000-00805f9b34fb";        /* Generic Access Profile */
// const QString SERVICE_UUID = "00001801-0000-1000-8000-00805f9b34fb";     /* Generic Attribute Profile */
// const QString SERVICE_UUID = "00001809-0000-1000-8000-00805f9b34fb";     /* Health Thermometer */
// const QString SERVICE_UUID = "0000180a-0000-1000-8000-00805f9b34fb";     /* Device Information */

const QString CHARACTERISTIC_UUID = "00002a05-0000-1000-8000-00805f9b34fb";



//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
BLEController::BLEController(QObject *parent)
    : QObject{parent}
{

}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void BLEController::startBLE()
{
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &BLEController::deviceDiscovered);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &BLEController::scanFinished);

    discoveryAgent->start();
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void BLEController::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    qDebug() << "Discovered device:" << device.name() << device.address().toString();


    /* We are only interesting in BLE device */
    if (device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        if (device.name() == TARGET_DEVICE_NAME) {
            qDebug() << "Target device found.";
            discoveryAgent->stop();

            controller = QLowEnergyController::createCentral(device, this);
            connect(controller, &QLowEnergyController::connected, this, &BLEController::deviceConnected);
            connect(controller, &QLowEnergyController::disconnected, this, &BLEController::deviceDisconnected);
            connect(controller, &QLowEnergyController::serviceDiscovered, this, &BLEController::serviceDiscovered);
            connect(controller, &QLowEnergyController::discoveryFinished, this, &BLEController::serviceScanDone);

            controller->connectToDevice();
        }
    }
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void BLEController::scanFinished()
{
    qDebug() << "Device scan finished.";
    if (!controller) {
        qDebug() << "Target device not found.";
    }
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void BLEController::deviceConnected()
{
    qDebug() << "Connected to device.";
    controller->discoverServices();
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void BLEController::deviceDisconnected()
{
    qDebug() << "Disconnected from device.";
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void BLEController::serviceDiscovered(const QBluetoothUuid &newService)
{
    qDebug() << "Service discovered:" << newService.toString();
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void BLEController::serviceScanDone()
{
    qDebug() << "Service scan done.";
    service = controller->createServiceObject(QBluetoothUuid(SERVICE_UUID), this);
    if (!service) {
        qDebug() << "Service not found.";
        return;
    }

    qDebug() << "Creating service object: " << service->serviceUuid();

    connect(service, &QLowEnergyService::stateChanged, this, [this](const QLowEnergyService::ServiceState &newState){
        qDebug() << "State changed to: " << newState;
        if (newState == QLowEnergyService::ServiceDiscovered) {


            QList<QLowEnergyCharacteristic> characteristics = service->characteristics();
            for(auto it=characteristics.begin(); it!=characteristics.end(); it++) {
                QLowEnergyCharacteristic characteristics = *it;
                qDebug() << characteristics.uuid();
            }


            /****
            characteristic = service->characteristic(QBluetoothUuid(CHARACTERISTIC_UUID));
            qDebug() << "Characteristic: " << characteristic.uuid();
            if (!characteristic.isValid()) {
                qDebug() << "Characteristic not found.";
                return;
            }

            QLowEnergyDescriptor descriptor = characteristic.descriptor(
              QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration
            );
            // Enable notification for value changes
            service->writeDescriptor(descriptor, QByteArray::fromHex("0100"));
            ****/

            // service->readCharacteristic(characteristic);



            QLowEnergyCharacteristic deviceNameCharacteristic = service->characteristic(QBluetoothUuid(QBluetoothUuid::CharacteristicType::DeviceName));
            if (deviceNameCharacteristic.isValid()) {
                // Read the value
                service->readCharacteristic(deviceNameCharacteristic);
            } else {
                qDebug() << "deviceName Characteristic not found.";
            }
        }
    });
    connect(service, &QLowEnergyService::characteristicRead, this, &BLEController::characteristicChanged);
    connect(service, &QLowEnergyService::characteristicChanged, this, &BLEController::characteristicChanged);

    service->discoverDetails();
}
//-----------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void BLEController::characteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &value)
{
    qDebug() << "Characteristic value changed:" << value;

    QLowEnergyCharacteristic deviceNameCharacteristic = service->characteristic(QBluetoothUuid(QBluetoothUuid::CharacteristicType::DeviceName));
    if (characteristic.uuid() == deviceNameCharacteristic.uuid()) {
        service->readCharacteristic(characteristic);
        return;
    }

    if (characteristic.uuid() == QBluetoothUuid(CHARACTERISTIC_UUID)) {
        qDebug() << "Characteristic value changed:" << value;
        m_distance = value.toFloat();
        emit distanceChanged();
        service->readCharacteristic(characteristic);
    }
}
