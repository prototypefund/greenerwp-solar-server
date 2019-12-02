package main

import "fmt"
import "github.com/tarm/serial"

const voltageCorrectionFactor = 1.08436

func readControllerStatus() EnergyStatus {
	c := &serial.Config{Name: "/dev/ttyS1", Baud: 9600}
	s, err := serial.OpenPort(c)
	checkError(err)
	var voltage float32
	fmt.Fscanf(s, "%f\n", &voltage)
	checkError(err)
	return EnergyStatus{Voltage: voltage * voltageCorrectionFactor}
}

// func writeController() {
// 	c := &serial.Config{Name: "/dev/ttyS1", Baud: 9600}
// 	s, err := serial.OpenPort(c)
// 	checkError(err)
// 	var voltage float32
// 	fmt.Fscanf(s, "%f\n", &voltage)
// 	checkError(err)
// 	return EnergyStatus{Voltage: voltage}
// }
