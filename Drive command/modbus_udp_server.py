import asyncio
from pymodbus.datastore import ModbusSlaveContext, ModbusServerContext
from pymodbus.datastore import ModbusSequentialDataBlock
from pymodbus.device import ModbusDeviceIdentification
from pymodbus.server import StartAsyncUdpServer


# ---------------------------------------------------------
# Create context with proper data blocks
# ---------------------------------------------------------
def create_context():
    # Create memory blocks
    hr_block = ModbusSequentialDataBlock(0, [0] * 2000)  # Holding registers
    ir_block = ModbusSequentialDataBlock(0, [0] * 2000)  # Input registers

    slave = ModbusSlaveContext(
        di=None,
        co=None,
        hr=hr_block,   # Holding Regs
        ir=ir_block    # Input Regs
    )

    context = ModbusServerContext(slaves={1: slave}, single=False)
    return context


# ---------------------------------------------------------
# Initialize registers
# ---------------------------------------------------------
def initialize_registers(context):
    slave = context[1]  # Access unit ID = 1

    # Holding registers (0x03)
    slave.setValues(3, 282, [1234])   # position
    slave.setValues(3, 284, [50])     # velocity
    slave.setValues(3, 286, [80])     # accel
    slave.setValues(3, 288, [90])     # decel

    # Input registers (0x04)
    slave.setValues(4, 412, [2500])   # pos_deg *100
    slave.setValues(4, 414, [120])    # speed
    slave.setValues(4, 420, [15])     # current
    slave.setValues(4, 426, [540])    # dc bus

    print("✅ Registers initialized successfully")


# ---------------------------------------------------------
# Run async UDP server
# ---------------------------------------------------------
async def run_udp_server():
    context = create_context()
    initialize_registers(context)

    identity = ModbusDeviceIdentification()
    identity.VendorName = "TestVendor"
    identity.ProductName = "UDP Modbus Simulator"
    identity.MajorMinorRevision = "1.0"

    print("===================================================")
    print("  ✅ Modbus UDP Simulator running on 127.0.0.1:502  ")
    print("  Ready for your C program                          ")
    print("===================================================")

    await StartAsyncUdpServer(
        context=context,
        identity=identity,
        address=("127.0.0.1", 502)
    )


if __name__ == "__main__":
    asyncio.run(run_udp_server())







# """
# Modbus UDP Simulator for your C program (pymodbus ≥3.6)
# Author: ChatGPT
# """

# import asyncio
# import logging
# from pymodbus.datastore import ModbusSlaveContext, ModbusServerContext
# from pymodbus.datastore import ModbusSequentialDataBlock
# from pymodbus.device import ModbusDeviceIdentification
# from pymodbus.server.async_io import StartAsyncUdpServer

# # ---------------------------------------------------------------------
# # 1️⃣ Configure logging (optional)
# # ---------------------------------------------------------------------
# logging.basicConfig(level=logging.INFO)
# log = logging.getLogger()

# # ---------------------------------------------------------------------
# # 2️⃣ Create Modbus data store (HR, IR, Coils, DI)
# # ---------------------------------------------------------------------
# store = ModbusSlaveContext(
#     di=ModbusSequentialDataBlock(0, [0] * 1000),
#     co=ModbusSequentialDataBlock(0, [0] * 1000),
#     hr=ModbusSequentialDataBlock(0, [0] * 2000),
#     ir=ModbusSequentialDataBlock(0, [0] * 2000),
#     zero_mode=True,
# )
# context = ModbusServerContext(slaves=store, single=True)

# # ---------------------------------------------------------------------
# # 3️⃣ Initialize register values (matching your config.h)
# # ---------------------------------------------------------------------
# def initialize_registers(ctx):
#     slave_id = 0x00  # internal index 0
#     # Holding Registers (0x03 / 0x10)
#     ctx[slave_id].setValues(3, 282, [4500, 1500, 500, 500])  # pos, vel, accel, decel
#     ctx[slave_id].setValues(3, 310, [0])
#     ctx[slave_id].setValues(3, 312, [0])
#     ctx[slave_id].setValues(3, 314, [9000])

#     # Input Registers (0x04)
#     ctx[slave_id].setValues(4, 412, [4500])  # position deg
#     ctx[slave_id].setValues(4, 414, [1500])  # velocity
#     ctx[slave_id].setValues(4, 418, [1200])  # rpm
#     ctx[slave_id].setValues(4, 420, [180])   # current
#     ctx[slave_id].setValues(4, 426, [3100])  # DC bus voltage
#     ctx[slave_id].setValues(4, 428, [385])   # temperature
#     ctx[slave_id].setValues(4, 430, [0])     # fault code

# initialize_registers(context)

# # ---------------------------------------------------------------------
# # 4️⃣ Create Modbus device identity
# # ---------------------------------------------------------------------
# identity = ModbusDeviceIdentification()
# identity.VendorName = "Python UDP Simulator"
# identity.ProductCode = "PYMODBUS"
# identity.ProductName = "UDP Drive Simulator"
# identity.ModelName = "VirtualDrive"
# identity.MajorMinorRevision = "3.6.8"

# # ---------------------------------------------------------------------
# # 5️⃣ Async UDP Server
# # ---------------------------------------------------------------------
# async def run_udp_server():
#     print("✅ Modbus UDP Simulator started at 127.0.0.1:502 ...")
#     print("   Ready to receive Modbus requests from your C program.")
#     await StartAsyncUdpServer(context=context, identity=identity, address=("127.0.0.1", 502))

# # ---------------------------------------------------------------------
# # 6️⃣ Entry point
# # ---------------------------------------------------------------------
# if __name__ == "__main__":
#     try:
#         asyncio.run(run_udp_server())
#     except KeyboardInterrupt:
#         print("\n❌ Server stopped manually.")
