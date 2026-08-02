
project (Printer LANGUAGES CXX)


set (Printer_sources
	Backend.cpp
	Factory.cpp
	QtPrinterBackend.cpp
	PhomemoBackend.cpp
	transport/BluetoothSerial.cpp
	protocol/PhomemoProtocol.cpp
)

set (Printer_qobject_headers
	Factory.hpp
)

qt6_wrap_cpp (Printer_moc_sources ${Printer_qobject_headers})

add_library (Printer STATIC
	${Printer_sources}
	${Printer_moc_sources}
)

target_compile_features (Printer PUBLIC cxx_std_20)

target_include_directories (Printer
	PUBLIC .. ${CMAKE_CURRENT_BINARY_DIR}/.. ${CMAKE_SOURCE_DIR}
)

target_link_libraries (Printer
	Qt6::Core
	Qt6::Gui
	Qt6::PrintSupport
	Model
)

if (HAVE_BLUETOOTH_SUPPORT)
	target_link_libraries (Printer
		Qt6::Bluetooth
		Qt6::SerialPort
	)
endif ()
